/*
 * sd_spi.c
 *
 *  Created on: 13 июля 2016 г.
 *      Author: snork
 */

#include <string.h>
#include <errno.h>

#include <stm32f4xx_hal.h>

#include "FreeRTOS.h"
#include "task.h"

#include "diag/Trace.h"

#include "sd.h"
#include "crc.h"
#include "state.h"

// =========================================================
// Настроечки
// =========================================================
#define SD_SPI	SPI1 	// настройки пинов и порта сюда пока не вынесены
#define SD_DMA	DMA2

#define SD_PORT	GPIOC
#define SD_PIN	3

#define SD_DMA_CHANNEL		DMA_CHANNEL_3
#define SD_DMA_RX_STREAM	DMA2_Stream0
#define SD_DMA_TX_STREAM	DMA2_Stream3

#define SD_BAUD_RATE_PRESCALER_SLOW SPI_BAUDRATEPRESCALER_256	//!< для первичной прокачки и инициализации SD карты
#define SD_BAUD_RATE_PRESCALER_FAST SPI_BAUDRATEPRESCALER_16	//!< для основной работы. Быстрее не может :c

// =========================================================
// Вспомогательные сущности
// =========================================================
#define SD_R1_IDLE (1 << 0)
#define SD_R1_ILLEGAL_CMD (1 << 2)
#define SD_R1_CRC_ERR (1 << 3)

#define SD_DATA_RESP_MASK           0x1F  // маска ответа SD карты о приеме данных
#define SD_DATA_RESP_DATA_ACCEPTED  0x05  // ответ SD карты - данные получены
#define SD_DATA_RESP_CRC_ERROR      0x0B  // ответ SD карты - ошибка CRC
#define SD_DATA_RESP_WRITE_ERROR    0x0D  // ответ SD карты - ошибка записи

#define SD_TOKEN_SINGLE_DATA 0xFE   // Токен заголовка пакета для CMD17/18/24
#define SD_TOKEN_MULTI_DATA 0xFC    // Токен заголовка пакета для CMD25
#define SD_TOKEN_NO_MORE_DATA 0xFD  // Токен заверешения передачи данных для CMD25

#define GOTO_END_IF_ERROR(X) if ((error = (X)) != SD_ERROR_NONE) goto end;

#define SD_CMD_TEAK_TIMEOUT (100)

// =========================================================
// Статики
// =========================================================

// Тип запроса к SD карте
typedef enum
{
   SD_REQUEST_WRITE = 0x10,
   SD_REQUEST_READ = 0x20
} sd_request_t;

// настройки - чтобы не заполнять полностью каждый раз
DMA_HandleTypeDef _sd_dma_spi_tx;
DMA_HandleTypeDef _sd_dma_spi_rx;


uint8_t _dummy_[512];

uint8_t _SD_DMA_READY_ = 1;


// Ожидание окончания работы SPI модуля
inline static void _spi_wait(void)
{
   bool notYet = true;
   while (notYet)
   {
      //__disable_irq();
      if (SD_SPI->SR & SPI_FLAG_RXNE)
         notYet = false;
      //__enable_irq();
   }
}


/*
// Запуск DMA для обмена блоками данных с SD картой по SPI (считай асинхронная операция)
// Блоки фиксированного размера 512 байт. Буффер должен быть жив до окончания операции
inline static void _sd_dma_transfer(void * target, sd_request_t direction)
{
   // Чистим настройки DMA с прошлого раза FIXME: можно чистить не все а только конкретный флаг,
   // который мешает перезапустить
//   DMA_DeInit(DMA1_Channel4);
//	HAL_DMA_DeInit(SD_DMA_CHANNEL);
	if (SD_REQUEST_WRITE ==  direction)
	{
	  // настраиваем DMA канал на RX. Принятые данные скидываем в фиксированную переменную
	  // без перемещения каретки
	  static uint16_t dummy_rx_buff;
	  _sd_dma_params.DMA_MemoryBaseAddr = (uint32_t)&dummy_rx_buff;
	  _sd_dma_params.DMA_DIR = DMA_DIR_PeripheralSRC;
	  _sd_dma_params.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_Init(DMA1_Channel4, &_sd_dma_params);
	  // Теперь на TX. Пишем из данного буфера с перемещением каретки
	  _sd_dma_params.DMA_MemoryBaseAddr = (uint32_t)target;
	  _sd_dma_params.DMA_DIR = DMA_DIR_PeripheralDST;
	  _sd_dma_params.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_Init(DMA1_Channel5, &_sd_dma_params);
	}
	else // чтение
	{
	  // настраиваем DMA канал на RX. Собираем данные в буффер
	  _sd_dma_params.DMA_MemoryBaseAddr = (uint32_t)target;
	  _sd_dma_params.DMA_DIR = DMA_DIR_PeripheralSRC;
	  _sd_dma_params.DMA_MemoryInc = DMA_MemoryInc_Enable;
	  DMA_Init(DMA1_Channel4, &_sd_dma_params);
	  // Теперь на TX. Кормим муодуль 0xFFками
	  static const uint16_t dummy_tx_buff = 0xFFFF;
	  _sd_dma_params.DMA_MemoryBaseAddr = (uint32_t)&dummy_tx_buff;
	  _sd_dma_params.DMA_DIR = DMA_DIR_PeripheralDST;
	  _sd_dma_params.DMA_MemoryInc = DMA_MemoryInc_Disable;
	  DMA_Init(DMA1_Channel5, &_sd_dma_params);
	}
	// настройка завершена, но DMA еще не запущено
	// активируем само DMA
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	// разрешаем SPI к нему обращаться
	SPI_I2S_DMACmd(SD_SPI, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);
	// ждем завершения
	while (DMA_GetFlagStatus(DMA1_IT_TC4) == RESET) {}
	if (DMA_GetITStatus(DMA1_IT_TC5) != SET)
	  abort();
	// отключаем DMA. FIXME: Впринципе можно этого и не делать, но на всякий - пускай будет
	SPI_I2S_DMACmd(SD_SPI, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, DISABLE);
}
*/



void sd_cs(bool state)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, state ? RESET : SET);
}


// =================================================================

uint8_t sd_init(SPI_HandleTypeDef* hspi)
{
	memset(_dummy_, 0xFF, 512);


	uint8_t error = 0;
	//Иницилизация SPI
	hspi->Instance = SPI1;
	hspi->Init.Mode = SPI_MODE_MASTER;
	hspi->Init.Direction = SPI_DIRECTION_2LINES;
	hspi->Init.DataSize = SPI_DATASIZE_8BIT;
	hspi->Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi->Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi->Init.NSS = SPI_NSS_SOFT;
	hspi->Init.BaudRatePrescaler = SD_BAUD_RATE_PRESCALER_FAST;
	hspi->Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi->Init.TIMode = SPI_TIMODE_DISABLE;
	hspi->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

	PROCESS_ERROR(HAL_SPI_Init(hspi));

	for (int i = 0; i < 600000; i++) {
		volatile uint8_t x = 0;
	}

	sd_cs(false);


	/*HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI1_IRQn);
	__HAL_RCC_DMA2_CLK_ENABLE();
	//	 настраиваем DMA для работы с SPI_TX
	_sd_dma_spi_tx.Instance = SD_DMA_TX_STREAM;
	_sd_dma_spi_tx.Init.Channel = SD_DMA_CHANNEL;
	_sd_dma_spi_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
	_sd_dma_spi_tx.Init.PeriphInc = DMA_PINC_DISABLE;
	_sd_dma_spi_tx.Init.MemInc = DMA_MINC_ENABLE;
	_sd_dma_spi_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	_sd_dma_spi_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	_sd_dma_spi_tx.Init.Mode = DMA_NORMAL;
	_sd_dma_spi_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
	_sd_dma_spi_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	_sd_dma_spi_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	_sd_dma_spi_tx.Init.MemBurst = DMA_MBURST_SINGLE;
	_sd_dma_spi_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	PROCESS_ERROR(HAL_DMA_Init(&_sd_dma_spi_tx));
	__HAL_LINKDMA(&spi_nRF24L01, hdmatx, _sd_dma_spi_tx);
	 DMA interrupt init
	HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
	//	 настраиваем DMA для работы с SPI_RX
	_sd_dma_spi_rx.Instance = SD_DMA_RX_STREAM;
	_sd_dma_spi_rx.Init.Channel = SD_DMA_CHANNEL;
	_sd_dma_spi_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	_sd_dma_spi_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	_sd_dma_spi_rx.Init.MemInc = DMA_MINC_ENABLE;
	_sd_dma_spi_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	_sd_dma_spi_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	_sd_dma_spi_rx.Init.Mode = DMA_NORMAL;
	_sd_dma_spi_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
	_sd_dma_spi_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	_sd_dma_spi_rx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	_sd_dma_spi_rx.Init.MemBurst = DMA_MBURST_SINGLE;
	_sd_dma_spi_rx.Init.PeriphBurst = DMA_PBURST_SINGLE;
	PROCESS_ERROR(HAL_DMA_Init(&_sd_dma_spi_rx));
	__HAL_LINKDMA(&spi_nRF24L01, hdmarx, _sd_dma_spi_rx);
	 DMA interrupt init
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);*/

end:
	sd_cs(false);
	return error;
}

void sd_exchange(void * to_send, void * to_recieve, size_t amount)
{
	HAL_SPI_TransmitReceive(&spi_nRF24L01, to_send, to_recieve, amount, 0xFF);
}


void sd_write(void * to_send, size_t amount)
{
	HAL_SPI_Transmit(&spi_nRF24L01, to_send, amount, 0xFF);
}


void sd_read(void * to_recieve, size_t amount)
{
	memset(to_recieve, 0xFF, amount);
	HAL_SPI_Receive(&spi_nRF24L01, to_recieve, amount, 0xFF);
}


sd_resp_t sd_response_type(sd_cmd_t cmd)
{
   switch (cmd)
   {
   case SD_CMD0:
   case SD_CMD1:
   case SD_CMD12:
   case SD_CMD17:
   case SD_CMD18:
   case SD_CMD24:
   case SD_CMD55:
   case SD_CMD25:
      return SD_R1;
   case SD_CMD41:
	   return SD_R3;
   case SD_CMD8:
      return SD_R7;
   default:
      abort();
      return 0; // не дойдем до сюда
   }
}


size_t sd_response_length(sd_resp_t resp)
{
   switch (resp)
   {
   case SD_R1:
      return 1;
   case SD_R3:
      return 5;
   case SD_R7:
      return 5;
   default:
      abort();
      return 0; // не дойдем до сюда, но компилятор ругается
   }
}


sd_error_t sd_cmd(sd_cmd_t cmd, uint32_t argument, void * response)
{
   const uint8_t * arg = (const uint8_t *)&argument;

   uint8_t data[] = { (uint8_t)((cmd & 0x3F) | 0x40), arg[3], arg[2], arg[1], arg[0], 0x95/*CRC*/};
   // 0x95 - фиксированное значение CRC для CMD0, для которой без CRC никак. Для остальных CRC мы не испольуем
   data[5] = crc7(data, 5);

//   uint8_t dummy = 0xFF;
//   sd_write(&dummy, 1);

   sd_cs(true);
   sd_write(data, sizeof(data));
   sd_cs(false);

   sd_resp_t resp_type = sd_response_type(cmd);
   size_t response_length = sd_response_length(resp_type);

   uint8_t * volatile first_response_byte_ptr = (uint8_t*)response;
   size_t timeleft = SD_CMD_TEAK_TIMEOUT;
   sd_cs(true);
   for (; timeleft != 0; timeleft--)
   {
		sd_read(first_response_byte_ptr, 1);
		// пока ответа нет - шина держится в единице (0xFF)
		// первый бит любого ответа должен быть равен 0
		if ((*first_response_byte_ptr & 0x80) == 0)
			break;
   }

   if (0 == timeleft)
   {
	  sd_cs(false);
      return SD_ERROR_TIMEOUT;
   }

   // дочитываем остальное
   if (response_length != 1) {
		sd_read(first_response_byte_ptr+1, response_length-1);
   }
   sd_cs(false);

   return SD_ERROR_NONE;
}


sd_error_t sd_wait_busy(void)
{
   //GPIOC->BRR |= (1 << 13);
   uint8_t buffer;
   do {
	   sd_cs(true);
      sd_read(&buffer, 1);
      sd_cs(false);
   } while(0x00 == buffer);

   //GPIOC->BSRR |= (1 << 13);
   return SD_ERROR_NONE;
}


sd_error_t sd_startup(void)
{
   sd_error_t error = SD_ERROR_NONE;
   sd_card_type_t card_type;

   for (int i = 0; i < 10000; i++) {
   		volatile uint8_t x = 0;
   	}

   // Переключаем SPI на медленный режим
   spi_nRF24L01.Init.BaudRatePrescaler = SD_BAUD_RATE_PRESCALER_SLOW; // пока такой, можно будет и побыстрее
   HAL_SPI_Init(&spi_nRF24L01);

   // прокачиваем 20 SPI тактов на SD карту при выключенном CS и удерживаю на линии MOSI единицу (0xFF)
   sd_cs(false);
   uint8_t dummy[10];
   memset(dummy, 0xFF, sizeof(dummy));
   sd_write(dummy, sizeof(dummy));
//   sd_cs(true);
   // отправляем карте команду CMDO уже при включенном CS
   uint8_t r1_resp;
   GOTO_END_IF_ERROR(sd_cmd(SD_CMD0, 0x00, &r1_resp));
   if (r1_resp & ~SD_R1_IDLE) // должен быть только IDLE бит, без других ошибок
   {
      error = SD_ERROR_WRONG_RESPONSE;
      goto end;
   }

   // отлично - карта в состоние IDLE

   // заставим её проверить напряжение питания
   // ошибки игнорируем, тк эту команду понимают только некоторые SD карты
   // заодно и выясним - какой тип карточки у нас
   uint8_t r7_resp[5] = {0, 0, 0, 0, 0};
   GOTO_END_IF_ERROR(sd_cmd(SD_CMD8, 0x000001AA, r7_resp)); // 0xAA - спец обязательный паттерн. 0x01 - команда на проверку питания
   if (r7_resp[0] & SD_R1_ILLEGAL_CMD) // карта не поняла команду - это SD1
      card_type = SD_TYPE_SD1;
   else if (0xAA == r7_resp[4]) // карта поняла нашу команду - это SD2 карта
      card_type = SD_TYPE_SD2;
   else // карта ведет себя неадекватно
   {
      error = SD_ERROR_WRONG_RESPONSE;
      goto end;
   }

   // долбим карту командами не включение, пока ей не надоест
   // если это SDHC, она ждет от нас ACMD41


   // для SD2 аргумент команды 41 должен быть 0X40000000, для остальных 0
   uint32_t cmd41_arg = (card_type == SD_TYPE_SD2) ? 0X40020000 : 0;
   do
   {
      GOTO_END_IF_ERROR(sd_cmd(SD_CMD55, 0x00, &r1_resp));
      if (r1_resp & ~SD_R1_IDLE) // должен быть только IDLE бит, без других ошибок
      {
         error = SD_ERROR_WRONG_RESPONSE;
         goto end;
      }

      GOTO_END_IF_ERROR(sd_cmd(SD_CMD41, cmd41_arg, &r1_resp));
      if (r1_resp & ~SD_R1_IDLE) // есть биты ошибки
      {
         if (SD_TYPE_SD2 == card_type) // SD V2 не позволительно тут давать ошибку - с ней что-то не то
         {
            error = SD_ERROR_WRONG_RESPONSE;
            goto end;
         }
         else
         {
            break; // остальные карты могут ошибаться - мы будем пробовать на них CMD1
         }
      }
   } while (r1_resp != 0x00);


   // если это карта, которая не понимает ACMD41 и idle бит все еще стоит - пробуем запустить её через CMD1
   while (r1_resp != 0x00)
   {
      GOTO_END_IF_ERROR(sd_cmd(SD_CMD1, 0x00, &r1_resp));
      if (r1_resp & ~SD_R1_IDLE) // есть биты ошибки
      {
         error = SD_ERROR_WRONG_RESPONSE;
         goto end;
      }
   }

   // все, карта готова
end:
	sd_cs(false);
	// Переключаем SPI на высокую скорость
	sd_init(&spi_nRF24L01);

   return error;
}




sd_error_t sd_block_write_multi(size_t offset, void * block_start, size_t block_count)
{
   sd_error_t error = SD_ERROR_NONE;
   static uint16_t dummy = 0xFFFF;
   // определяемся с командой и токенами - запись нескольких блоков или одного
   uint8_t packet_token = block_count > 1 ? SD_TOKEN_MULTI_DATA : SD_TOKEN_SINGLE_DATA;
   sd_cmd_t write_cmd = block_count > 1 ? SD_CMD25 : SD_CMD24;

   sd_wait_busy();

   // отправляем команду на запись
   uint8_t resp = 0x00;
   GOTO_END_IF_ERROR(sd_cmd(write_cmd, (uint32_t)offset, &resp));
   if (resp != 0x00)
   {
      error = SD_ERROR_WRONG_RESPONSE;
      goto end;
   }
   // теперь отправляем специальный "пропускающий" байт
   sd_cs(true);
   sd_write(&dummy, 1);
   sd_cs(false);

   for (size_t i = 0; i < block_count; i++)
   {
      if (i != 0)
      sd_wait_busy(); // ждем пока карта освободится

      // заголовок пакета данных
      sd_cs(true);
      sd_write(&packet_token, 1);
      // сами данные
      HAL_SPI_Transmit(&spi_nRF24L01, (uint8_t*)block_start + 512*i, 512, 0xFF);
      // контрольная сумма (она выключена)
      sd_write(&dummy, 2);
      sd_cs(false);

      // sd карта отвечает на пакет - слушаем
      sd_cs(true);
      sd_read(&resp, 1);
      sd_cs(false);
      resp &= SD_DATA_RESP_MASK;
      if (resp != SD_DATA_RESP_DATA_ACCEPTED)
      {
         error = SD_ERROR_WRONG_RESPONSE;
         break;
      }
   }

   if (block_count > 1)
   {
      sd_wait_busy();
      // заканчиваем - отправляем соответсвующий токен
      static uint8_t stop_tran_token = SD_TOKEN_NO_MORE_DATA;
      sd_cs(true);
      sd_write(&stop_tran_token, 1);
      // нужно прогнать еще один пропускающий байт, чтобы карта вошла в состояние busy
      // чтобы начать запись на самой sd карте
      // нужно отправить еще хотябы один байт по SPI в состоянии busy
      // (согласно статье http://elm-chan.org/docs/mmc/mmc_e.html)
      // поэтому отправляем сразу два байта
      sd_write(&dummy, 2);
      sd_cs(false);
   }

   //sd_wait_busy();

end:
   sd_cs(false);
   return error;
}


sd_error_t sd_block_read_multi(size_t offset, void * block, size_t block_count)
{
   sd_error_t error = SD_ERROR_NONE;

   // определяемся с командой на чтение в зависимости от количества блоков
   sd_cmd_t read_cmd = block_count > 1 ? SD_CMD18 : SD_CMD17;

   sd_wait_busy();

   // Подготовка к чтению блока - отправка команды и заголовков
   uint8_t resp_r1 = 0;
   GOTO_END_IF_ERROR(sd_cmd(read_cmd, (uint32_t)offset, &resp_r1));
   if (resp_r1 != 0x00)
   {
      error = SD_ERROR_WRONG_RESPONSE;
      goto end;
   }

   for (size_t i = 0; i < block_count; i++)
   {
      // ждем токена о начале передачи
      uint8_t token;
      do
      {
    	  sd_cs(true);
         sd_read(&token, 1);
//         sd_cs(false);
      } while (token == 0xFF);

      // проверяем токен
      if (token != SD_TOKEN_SINGLE_DATA)
      {
         error = SD_ERROR_INVALID_DATA_TOKEN;
         break;
      }

      // читаем сам блок
//      sd_cs(true);
      HAL_SPI_TransmitReceive(&spi_nRF24L01, _dummy_, (uint8_t*)block + 512*i, 512, 0xFF);
      for (int i = 0; i < 10000; i++) {volatile int x = 0;}
      // читаем CRC
      uint16_t crc = 0;
      sd_read(&crc, 2);
      sd_cs(false);

   }

   if (block_count > 1)
   {
      // горшочек - не вари. Засылаем спеиальную команду на остановку
      sd_cmd(SD_CMD12, 0x00, &resp_r1);
      if (resp_r1 != 0x00)
      {
         error = SD_ERROR_WRONG_RESPONSE;
         goto end;
      }
   }

   // карта вошла в состояние busy.
   // когда блоки пишуются, нужно отправить хотябы один байт, чтобы запись реально пошла
   // может быть с чтением такая же ерунда?
   uint8_t dummy;
   sd_cs(true);
   sd_read(&dummy, 1);
   sd_cs(false);
   //sd_wait_busy();

end:
   sd_cs(false);
   return error;
}

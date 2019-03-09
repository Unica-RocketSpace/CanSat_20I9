/*
 * exchange_task.c
 *
 *  Created on: 8 дек. 2018 г.
 *      Author: developer
 */

#include <stdint.h>

#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"

#include "exchange_task.h"

#include "state.h"

uint8_t internal_cmd;
portBASE_TYPE internal_queue_status;

uint8_t uplink_command;

/*
static volatile size_t _dma_carret;
static char _dma_buffer[EXCHANGE_DMA_BUFFER_SIZE] = {0};

static volatile size_t _msg_carret;
static char _msg_buffer[EXCHANGE_MSG_BUFFER_SIZE];



static UART_HandleTypeDef uartExchangeData;
static DMA_HandleTypeDef dmaExchangeData;


inline static char _read_dma_buffer(void)
{
	// ждем, пока ДМА чего-нибудь скачает
	while(_dma_carret == (size_t)(EXCHANGE_DMA_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&dmaExchangeData))) {}

	char retval = _dma_buffer[_dma_carret];

	_dma_carret++;
	if (_dma_carret >= EXCHANGE_DMA_BUFFER_SIZE)
		_dma_carret = 0;

	return retval;
}


// ИНИЦИАЛИЗАЦИЯ USART //

void initExUsartDma() {
	uint8_t error;

	uartExchangeData.Instance = USART1;
	uartExchangeData.Init.BaudRate = 9600;
	uartExchangeData.Init.Woruplink_commanddLength = UART_WORDLENGTH_8B;
	uartExchangeData.Init.StopBits = UART_STOPBITS_1;
	uartExchangeData.Init.Parity = UART_PARITY_NONE;
	uartExchangeData.Init.Mode = UART_MODE_TX_RX;
	uartExchangeData.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeData.Init.OverSampling = UART_OVERSAMPLING_16;

	PROCESS_ERROR(HAL_UART_Init(&uartExchangeData));


	__HAL_RCC_DMA2_CLK_ENABLE();
	//	Инициализация DMA1_Stream5 для работы c FC через USART
	dmaExchangeData.Instance = DMA2_Stream5;
	dmaExchangeData.Init.Channel = DMA_CHANNEL_4;						// 4 канал - на USART2_RX
	dmaExchangeData.Init.Direction = DMA_PERIPH_TO_MEMORY;				// направление - из периферии в память
	dmaExchangeData.Init.PeriphInc = DMA_PINC_DISABLE;					// инкрементация периферии выключена
	dmaExchangeData.Init.MemInc = DMA_MINC_ENABLE;						// инкрементация памяти включена
	dmaExchangeData.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;		// длина слова в периферии - байт
	dmaExchangeData.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;		// длина слова в памяти - байт
	dmaExchangeData.Init.Mode = DMA_CIRCULAR;							// режим - обычный
	dmaExchangeData.Init.Priority = DMA_PRIORITY_HIGH;					// приоритет - средний
	dmaExchangeData.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	dmaExchangeData.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	dmaExchangeData.Init.MemBurst = DMA_MBURST_SINGLE;
	dmaExchangeData.Init.PeriphBurst = DMA_PBURST_SINGLE;
	PROCESS_ERROR(HAL_DMA_Init(&dmaExchangeData));

	// запускаем ДМА на трансфер данных
	PROCESS_ERROR(HAL_DMA_Start(
			&dmaExchangeData, (uint32_t)&uartExchangeData.Instance->DR,
			(uint32_t)&_dma_buffer, sizeof(_dma_buffer)
	));

     Enable the DMA transfer for the receiver request by setting the DMAR bit
    in the UART CR3 register
    SET_BIT(uartExchangeData.Instance->CR3, USART_CR3_DMAR);

	end:
		state_system.GPS_state = error;
}
*/
/*void EXCHANGE_task (){

	for(;;){
		// Ждать команду в очереди
		if (uxQueueMessagesWaiting(handleInternalCmdQueue) != 0){	//Проверка очереди на наличие в ней элементов
			internal_queue_status = xQueueReceive(handleInternalCmdQueue, &internal_cmd, 0);
		}

		//Данные пришли
		if (internal_queue_status == pdPASS){
			 //отправка данных на FC

		}

//		while(){
//			if (command == )

//		}
		//записать ответ в sensors_data





	}
}*/

static UART_HandleTypeDef uartExchangeData;
void initAll(){

	uartExchangeData.Instance = USART1;
	uartExchangeData.Init.BaudRate = 9600;
	uartExchangeData.Init.WordLength = UART_WORDLENGTH_8B;
	uartExchangeData.Init.StopBits = UART_STOPBITS_1;
	uartExchangeData.Init.Parity = UART_PARITY_NONE;
	uartExchangeData.Init.Mode = UART_MODE_TX_RX;
	uartExchangeData.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeData.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&uartExchangeData);
}



void ReciveCommand(UART_HandleTypeDef * uart, uint8_t * command){
	HAL_UART_Receive(&uart, &command, sizeof(command), 0);
}

void TransmitData(UART_HandleTypeDef * uart, state_master_t * stateToSend){
	HAL_UART_Transmit(&uart, stateToSend, sizeof(stateToSend), 0);
}

void ParseCommand(uint8_t * uplink_command){
	if (uplink_command == COMMAND_DATA){
		taskENTER_CRITICAL();
		for (int i = 0; i < 3; i++){
			state_master.coordinates[i] = stateIMU_isc.coordinates[i];
			state_master.velosities[i] = stateIMU_isc.velocities[i];
			state_master.quaternion[i] = stateIMU_isc.quaternion[i];
		}
		state_master.quaternion[4] = stateIMU_isc.quaternion[4];

		taskEXIT_CRITICAL();
		//copy_data for FC
	}
	else if (uplink_command == COMMAND_OK){
		//write master state register
	}


}







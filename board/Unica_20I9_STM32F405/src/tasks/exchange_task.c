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

#include "state.h"

#define COMMAND_TEST 2
#define COMMAND_SLEEP_MODE 0
#define COMMAND_START 1


uint8_t internal_cmd;
portBASE_TYPE internal_queue_status;

// ИНИЦИАЛИЗАЦИЯ USART //

static UART_HandleTypeDef uartExchangeData;
static DMA_HandleTypeDef dmaExchangeData;

void initExchange() {
	uint8_t error;


	uartExchangeData.Instance = USART1;
	uartExchangeData.Init.BaudRate = 9600;
	uartExchangeData.Init.WordLength = UART_WORDLENGTH_8B;
	uartExchangeData.Init.StopBits = UART_STOPBITS_1;
	uartExchangeData.Init.Parity = UART_PARITY_NONE;
	uartExchangeData.Init.Mode = UART_MODE_TX_RX;
	uartExchangeData.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeData.Init.OverSampling = UART_OVERSAMPLING_16;

	PROCESS_ERROR(HAL_UART_Init(&uartExchangeData));


	__HAL_RCC_DMA1_CLK_ENABLE();
	//	Инициализация DMA1_Stream5 для работы c GPS через USART
	dmaExchangeData.Instance = DMA1_Stream5;
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

	end:
		state_system.GPS_state = error;
}

void EXCHANGE_task (){

	for(;;){
		// Ждать команду в очереди

		if (uxQueueMessagesWaiting(handleInternalCmdQueue) != 0){	//Проверка очереди на наличие в ней элементов
			internal_queue_status = xQueueReceive(handleInternalCmdQueue, &internal_cmd, 0);
		}

		//Данные пришли
		if (internal_queue_status == pdPASS){
			 //отправка данных на FC

		}
		uint32_t count = 0;
		while(count < 100000){
			//ждем команду от FC
			count++;
		}
		//записать ответ в sensors_data





	}
}





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





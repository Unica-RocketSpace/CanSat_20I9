/*
 * exchange_task.c
 *
 *  Created on: 4 июн. 2019 г.
 *      Author: developer
 */
#include "stm32f4xx_hal.h"

#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"

#include <diag/Trace.h>

#include "state.h"

UART_HandleTypeDef uartExchangeData;
UART_HandleTypeDef uartExchangeCommand;

uint8_t internal_cmd;
BaseType_t internal_queue_status;

// ИНИЦИАЛИЗАЦИЯ UART //
void init_exchange_data_UART(void){
	uint8_t error = 0;

	uartExchangeData.Instance = USART1;
	uartExchangeData.Init.BaudRate = 9600;
	uartExchangeData.Init.WordLength = UART_WORDLENGTH_8B;
	uartExchangeData.Init.StopBits = UART_STOPBITS_1;
	uartExchangeData.Init.Parity = UART_PARITY_NONE;
	uartExchangeData.Init.Mode = UART_MODE_TX_RX;
	uartExchangeData.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeData.Init.OverSampling = UART_OVERSAMPLING_16;

	PROCESS_ERROR(HAL_UART_Init(&uartExchangeData));

	end:
		state_system.EX_logs_UART_state = error;
}


void init_exchange_command_UART(void){
	uint8_t error = 0;

	__HAL_RCC_USART3_CLK_ENABLE();
	uartExchangeCommand.Instance = USART3;
	uartExchangeCommand.Init.BaudRate = 9600;
	uartExchangeCommand.Init.WordLength = UART_WORDLENGTH_8B;
	uartExchangeCommand.Init.StopBits = UART_STOPBITS_1;
	uartExchangeCommand.Init.Parity = UART_PARITY_NONE;
	uartExchangeCommand.Init.Mode = UART_MODE_TX_RX;
	uartExchangeCommand.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeCommand.Init.OverSampling = UART_OVERSAMPLING_16;

	PROCESS_ERROR(HAL_UART_Init(&uartExchangeCommand));

	end:
		state_system.EX_cmd_UART_state = error;
}


void USART3_IRQHandler(void){
	uint8_t tmp;
	//Проверка флага о приеме байтика по USART
	if ((USART3->SR & USART_SR_RXNE) != 0){
		//Сохранение принятого байтика
		tmp = USART3->DR;
		BaseType_t p = pdFALSE;
		xQueueSendToBackFromISR(handleInternalCmdQueue, &tmp, &p);
	}
}


void parse_command(uint8_t uplink_command){
	switch (uplink_command){
		case COMMAND_TEST:
			HAL_UART_Transmit(&uartExchangeCommand, (uint8_t*)COMMAND_OK, sizeof(COMMAND_OK), 10);
			break;

		case COMMAND_DATA:
			HAL_UART_Transmit(&uartExchangeCommand, (uint8_t*)COMMAND_DATA, sizeof(COMMAND_DATA), 10);
			HAL_UART_Receive(&uartExchangeData, (uint8_t*)&state_master, sizeof(state_master), HAL_MAX_DELAY);
			break;

		case COMMAND_LOGS:
			HAL_UART_Transmit(&uartExchangeData, (uint8_t*)&FC_logs, sizeof(FC_logs), 10);
			break;

		case COMMAND_START:
			parse_command(COMMAND_DATA);
//			start predictor task
//			vTaskResume();
			break;

		case COMMAND_SLEEP:
//			stop all tasks
//			vTaskSuspend();
			break;
	}
}

void EXCHANGE_task(void){

	for(;;){
		//Проверка очереди на наличие в ней элементов
		internal_queue_status = xQueueReceive(handleInternalCmdQueue, &internal_cmd, portMAX_DELAY);
		//Данные пришли
		if (internal_queue_status == pdPASS){
			trace_printf("E");
			parse_command(internal_cmd);
		}
	}
}

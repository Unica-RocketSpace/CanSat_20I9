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
	uartExchangeData.Init.BaudRate = 115200;
	uartExchangeData.Init.WordLength = UART_WORDLENGTH_8B;
	uartExchangeData.Init.StopBits = UART_STOPBITS_1;
	uartExchangeData.Init.Parity = UART_PARITY_NONE;
	uartExchangeData.Init.Mode = UART_MODE_TX_RX;
	uartExchangeData.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeData.Init.OverSampling = UART_OVERSAMPLING_16;

	PROCESS_ERROR(HAL_UART_Init(&uartExchangeData));

	end:
		state_system.EX_logs_UART_state = error;
		trace_printf("EX_data_UART %d\n", error);
}


void init_exchange_command_UART(void){
	uint8_t error = 0;

	__HAL_RCC_USART3_CLK_ENABLE();
	uartExchangeCommand.Instance = USART3;
	uartExchangeCommand.Init.BaudRate = 115200;
	uartExchangeCommand.Init.WordLength = UART_WORDLENGTH_8B;
	uartExchangeCommand.Init.StopBits = UART_STOPBITS_1;
	uartExchangeCommand.Init.Parity = UART_PARITY_NONE;
	uartExchangeCommand.Init.Mode = UART_MODE_TX_RX;
	uartExchangeCommand.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeCommand.Init.OverSampling = UART_OVERSAMPLING_16;

	PROCESS_ERROR(HAL_UART_Init(&uartExchangeCommand));

	end:
		state_system.EX_cmd_UART_state = error;
		trace_printf("EX_cmd_UART %d\n", error);
}


void USART3_IRQHandler(void){
	uint8_t tmp;
	//Проверка флага о приеме байтика по USART
	if ((USART3->SR & USART_SR_RXNE) != 0){
		//Сохранение принятого байтика
		tmp = USART3->DR;
		BaseType_t p = pdTRUE;
		xQueueSendToBackFromISR(handleInternalCmdQueue, &tmp, &p);
	}
}


void parse_command(uint8_t uplink_command){
	trace_printf("command %d\n", uplink_command);
	uint8_t tmp;
	switch (uplink_command){
		case COMMAND_TEST:
			tmp = COMMAND_OK;
			HAL_UART_Transmit(&uartExchangeCommand, &tmp, sizeof(tmp), 10);
			break;

		case COMMAND_DATA:
			tmp = COMMAND_DATA;
			do HAL_UART_Transmit(&uartExchangeCommand, &tmp, sizeof(tmp), 10);
			while(HAL_UART_Receive(&uartExchangeData, (uint8_t*)&state_master, sizeof(state_master), 10 / portTICK_RATE_MS) == HAL_TIMEOUT);
			led();
			break;

		case COMMAND_LOGS:
			tmp = COMMAND_LOGS;
			HAL_UART_Transmit(&uartExchangeCommand, &tmp, sizeof(tmp), 20);
			HAL_UART_Transmit(&uartExchangeData, (uint8_t*)&FC_logs, sizeof(FC_logs), 3 / portTICK_RATE_MS);
			break;

		case COMMAND_START:
//			start predictor task
			tmp = COMMAND_OK;
			HAL_UART_Transmit(&uartExchangeCommand, &tmp, sizeof(tmp), 10);
			led();
			//vTaskResume(handleControlTask);
			vTaskResume(handleSoARTask);
			break;

		case COMMAND_SLEEP:
//			stop all tasks
			tmp = COMMAND_OK;
			HAL_UART_Transmit(&uartExchangeCommand, &tmp, sizeof(tmp), 10);
//			vTaskSuspend();
			break;
	}
}


void init_EX(void){
	init_exchange_command_UART();
	init_exchange_data_UART();

	//Включение прерывания USART: RXNE
	USART3->CR1 |= USART_CR1_RXNEIE;
	HAL_NVIC_SetPriority(USART3_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

//	//Включение прерывания USART: RXNE
//	USART1->CR1 |= USART_CR1_RXNEIE;
//	HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
//	HAL_NVIC_EnableIRQ(USART1_IRQn);
}


void EXCHANGE_task(void){

//	vTaskSuspend(handleControlTask);
	vTaskSuspend(handleSoARTask);
	trace_printf("START");

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

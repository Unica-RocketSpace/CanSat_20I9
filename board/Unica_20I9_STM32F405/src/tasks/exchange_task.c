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

#include "sd/sd.h"
#include "exchange_task.h"

#include "state.h"

#include <diag/Trace.h>

UART_HandleTypeDef uartExchangeData;
UART_HandleTypeDef uartExchangeCommand;
DMA_HandleTypeDef dmaExchangeLogs;

uint8_t Exchange_DMA_Buffer[EXCHANGE_BUFFER_SIZE];

uint8_t internal_cmd;
portBASE_TYPE internal_queue_status, answer_queue;

uint8_t uplink_command;


// ИНИЦИАЛИЗАЦИЯ USART //
void init_exchange_data_UART(){

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

void init_exchange_command_UART(){
	__HAL_RCC_USART3_CLK_ENABLE();
	uartExchangeCommand.Instance = USART3;
	uartExchangeCommand.Init.BaudRate = 9600;
	uartExchangeCommand.Init.WordLength = UART_WORDLENGTH_8B;
	uartExchangeCommand.Init.StopBits = UART_STOPBITS_1;
	uartExchangeCommand.Init.Parity = UART_PARITY_NONE;
	uartExchangeCommand.Init.Mode = UART_MODE_TX_RX;
	uartExchangeCommand.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	uartExchangeCommand.Init.OverSampling = UART_OVERSAMPLING_16;

	HAL_UART_Init(&uartExchangeCommand);
}


void init_exchange_DMA_logs(){
	uint8_t error;

	__HAL_RCC_DMA2_CLK_ENABLE();
	//	Инициализация DMA2_Stream5 для работы c FC через USART
	dmaExchangeLogs.Instance = DMA2_Stream5;
	dmaExchangeLogs.Init.Channel = DMA_CHANNEL_4;						// 4 канал - на USART1_RX
	dmaExchangeLogs.Init.Direction = DMA_PERIPH_TO_MEMORY;				// направление - из периферии в память
	dmaExchangeLogs.Init.PeriphInc = DMA_PINC_DISABLE;					// инкрементация периферии выключена
	dmaExchangeLogs.Init.MemInc = DMA_MINC_ENABLE;						// инкрементация памяти включена
	dmaExchangeLogs.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;		// длина слова в периферии - байт
	dmaExchangeLogs.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;		// длина слова в памяти - байт
	dmaExchangeLogs.Init.Mode = DMA_CIRCULAR;							// режим - обычный
	dmaExchangeLogs.Init.Priority = DMA_PRIORITY_HIGH;					// приоритет - средний
	dmaExchangeLogs.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	dmaExchangeLogs.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
	dmaExchangeLogs.Init.MemBurst = DMA_MBURST_SINGLE;
	dmaExchangeLogs.Init.PeriphBurst = DMA_PBURST_SINGLE;
	PROCESS_ERROR(HAL_DMA_Init(&dmaExchangeLogs));

	// запускаем ДМА на трансфер данных
	PROCESS_ERROR(HAL_DMA_Start_IT(
			&dmaExchangeLogs, (uint32_t)&uartExchangeData.Instance->DR,
			(uint32_t)&Exchange_DMA_Buffer, EXCHANGE_BUFFER_SIZE
	));

	/*	Enable the DMA transfer for the receiver request by setting the DMAR bit
	in the UART CR3 register		*/
	SET_BIT(uartExchangeData.Instance->CR3, USART_CR3_DMAR);

	end:
	state_system.master_state = error;
}


void parse_command(uint8_t uplink_command){
	if (uplink_command == COMMAND_DATA){
		//copy_data for FC
		taskENTER_CRITICAL();
		for (int i = 0; i < 2; i++){
			state_master.coordinates[i] = stateIMU_isc.coordinates[i];
			state_master.accel_isc[i] = stateIMU_isc.accel[i];
			state_master.quaternion[i] = stateIMU_isc.quaternion[i];
		}
		state_master.accel_isc[2] = stateIMU_isc.accel[2];
		state_master.quaternion[2] = stateIMU_isc.quaternion[2];
		state_master.quaternion[3] = stateIMU_isc.quaternion[3];

		state_master.speed_GPS = stateGPS.speed;
		state_master.course = stateGPS.course;
		taskEXIT_CRITICAL();

		//отправка данных на FC
		HAL_UART_Transmit(&uartExchangeData, (uint8_t*)&state_master, sizeof(state_master), 10);
	}

	else if (uplink_command == COMMAND_OK){
		taskENTER_CRITICAL();
		state_system.master_state = 1;
		taskEXIT_CRITICAL();
	}
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


void USART1_IRQHandler(void){
	//Проверка флага о приеме байтика по USART
	if ((USART3->SR & USART_SR_RXNE) != 0){
		HAL_UART_Receive_IT(&uartExchangeData, (uint8_t *)&FCLogs, sizeof(FCLogs));
	}
}
/*
void DMA2_Stream5_IRQHandler(void){
	//Проверка флага о заполнении половины буффера
	if (__HAL_DMA_GET_HT_FLAG_INDEX(&dmaExchangeLogs) != 0){
		__HAL_DMA_CLEAR_FLAG(&dmaExchangeLogs, DMA_FLAG_HTIF1_5);
		//Запись на SD
		trace_printf("HTIE\n");
	}
	//Проверка флага о заполнении всего буффера
	if (__HAL_DMA_GET_TC_FLAG_INDEX(&dmaExchangeLogs) != 0){
		__HAL_DMA_CLEAR_FLAG(&dmaExchangeLogs, DMA_FLAG_TCIF1_5);
		//Запись на SD
		trace_printf("TCIE\n");
	}
}
*/


void EXCHANGE_task(void){
	init_exchange_data_UART();
	init_exchange_command_UART();
//	init_exchange_DMA_logs();

	//Включение прерывания USART: RXNE
	USART3->CR1 |= USART_CR1_RXNEIE;
	HAL_NVIC_SetPriority(USART3_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	//Включение прерывания USART: RXNE
	USART1->CR1 |= USART_CR1_RXNEIE;
	HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);


/*	//Включение прерывания DMA: TC, HT
	DMA2_Stream5->CR |= DMA_SxCR_HTIE;
	DMA2_Stream5->CR |= DMA_SxCR_TCIE;
	HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
*/

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







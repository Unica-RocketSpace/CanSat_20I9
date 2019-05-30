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

#include <diag/Trace.h>

static UART_HandleTypeDef uartExchangeData;
static UART_HandleTypeDef uartExchangeCommand;
static DMA_HandleTypeDef dmaExchangeData;
static DMA_HandleTypeDef dmaExchangeLogs;


uint8_t internal_cmd;
portBASE_TYPE internal_queue_status, answer_queue;

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

void init_exchange_DMA_data(){
	__HAL_RCC_DMA2_CLK_ENABLE();
	//	Инициализация DMA2_Stream7 для работы c FC через USART
	dmaExchangeData.Instance = DMA2_Stream7;
	dmaExchangeData.Init.Channel = DMA_CHANNEL_4;						// 4 канал - на USART1_TX
	dmaExchangeData.Init.Direction = DMA_MEMORY_TO_PERIPH;				// направление - из памяти в периферию
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
	HAL_DMA_Init(&dmaExchangeData);

}
	//FIXME:
	// запускаем ДМА на трансфер данных
	/*PROCESS_ERROR(HAL_DMA_Start(
			&dmaExchangeData, (uint32_t)&uartExchangeData.Instance->DR,
			(uint32_t)&DMA_UARTBuffer, sizeof(EXCHANGE_DMA_BUFFER_SIZE)
	));
*/
	/*	Enable the DMA transfer for the receiver request by setting the DMAR bit
	in the UART CR3 register		*/
/*	SET_BIT(uartExchangeData.Instance->CR3, USART_CR3_DMAR);
*/



void init_exchange_DMA_logs(){
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
	HAL_DMA_Init(&dmaExchangeLogs);


}
	// запускаем ДМА на трансфер данных
/*	PROCESS_ERROR(HAL_DMA_Start(
			&dmaExchangeLogs, (uint32_t)&uartExchangeData.Instance->DR,
			(uint32_t)&DMA_UARTBuffer, sizeof(EXCHANGE_DMA_BUFFER_SIZE)
	));
*/
	/*Enable the DMA transfer for the receiver request by setting the DMAR bit
	in the UART CR3 register		*/
/*	SET_BIT(uartExchangeData.Instance->CR3, USART_CR3_DMAR);
*/


//FIXME: возможно не нужна
/*
void recive_command(UART_HandleTypeDef *uart, uint8_t * command){
	HAL_UART_Receive(uart, command, sizeof(command), 0);
}
*/

// FIXME: stateToSend - state_master_t, а должен быть uint8_t
/*
void TransmitData(UART_HandleTypeDef *uart, state_master_t * stateToSend){
	uint8_t buffer[100];

	HAL_UART_Transmit(uart, stateToSend, sizeof(stateToSend), 0);
}
*/

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
		HAL_UART_Transmit_DMA(&uartExchangeData, (uint8_t*)&state_master, sizeof(state_master));
//		trace_printf("addres %d\n", state_master.accel_isc[0]);
	}

	else if (uplink_command == COMMAND_OK){
		taskENTER_CRITICAL();
		state_system.master_state = 1;
		taskEXIT_CRITICAL();
	}
}



void EXCHANGE_task(){
	init_exchange_data_UART();
	init_exchange_command_UART();
	init_exchange_DMA_data();
	init_exchange_DMA_logs();

	HAL_UART_MspInit(&uartExchangeData);
	HAL_UART_MspInit(&uartExchangeCommand);

	for(;;){
		//Проверка очереди на наличие в ней элементов
		internal_queue_status = xQueueReceive(handleInternalCmdQueue, &internal_cmd, portMAX_DELAY);
		//Данные пришли
		if (internal_queue_status == pdPASS){
			parse_command(internal_cmd);

		}
	}
}







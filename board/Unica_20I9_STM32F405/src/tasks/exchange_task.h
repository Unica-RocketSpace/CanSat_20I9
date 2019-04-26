/*
 * exchange_task.h
 *
 *  Created on: 8 дек. 2018 г.
 *      Author: developer
 */

#ifndef EXCHANGE_TASK_H_
#define EXCHANGE_TASK_H_

#include "stm32f4xx_hal.h"

#include "state.h"

#define EXCHANGE_USART 				(USART1)
#define EXCHANGE_DMA_BUFFER_SIZE 	(400)
#define EXCHANGE_MSG_BUFFER_SIZE 	(200)
#define EXCHANGE_DMA_USART_STREAM 	(DMA2_Stream5)


#define EXCHANGE_BUFFER_SIZE			(100)


#define COMMAND_TEST 		2
#define COMMAND_SLEEP_MODE 	0
#define COMMAND_START 		1
#define COMMAND_DATA 		3
#define COMMAND_OK			4


extern uint8_t dma_usartBuffer[100];

//инициализация uart для приема/передачи команд
void init_exchange_command_uart();

//инициализация uart для приема/передачи данных
void init_exchange_data_uart();

/*
 * Функция для разбора команд
 * uplink_command - команда которую надо разобрать
 */
void parse_command(uint8_t uplink_command);


void EXCHANGE_task();


#endif /* EXCHANGE_TASK_H_ */

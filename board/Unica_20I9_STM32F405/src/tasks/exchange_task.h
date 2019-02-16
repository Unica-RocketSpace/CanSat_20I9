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


extern uint8_t dma_usartBuffer[100];

void initExUsartDma();

void EXCHANGE_task();


#endif /* EXCHANGE_TASK_H_ */

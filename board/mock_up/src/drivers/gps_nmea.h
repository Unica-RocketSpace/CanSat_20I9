/*
 * gps_nmea.h
 *
 *  Created on: 16 дек. 2017 г.
 *      Author: developer
 */

#ifndef GPS_NMEA_H_
#define GPS_NMEA_H_

#include <stm32f4xx_hal.h>

#include "state.h"

#define GPS_USART 				(USART2)
#define GPS_DMA_BUFFER_SIZE 	(400)
#define GPS_MSG_BUFFER_SIZE 	(200)
#define GPS_DMA_USART_STREAM 	(DMA1_Stream5)


#define GPS_BUFFER_SIZE			(100)


extern uint8_t dma_usartBuffer[100];

void GPS_Init();
void GPS_task();


#endif /* GPS_NMEA_H_ */

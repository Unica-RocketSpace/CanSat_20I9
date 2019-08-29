/*
 * exchange_task.h
 *
 *  Created on: 8 дек. 2018 г.
 *      Author: developer
 */

#ifndef EXCHANGE_TASK_H_
#define EXCHANGE_TASK_H_

#include <stm32f4xx_hal.h>
#include "state.h"

//#define EXCHANGE_BUFFER_SIZE	100


//инициализация uart для приема/передачи команд
void init_exchange_command_UART();

//инициализация uart для приема/передачи данных
void init_exchange_data_UART();

//void init_exchange_DMA_data();

void init_exchange_DMA_logs();

/*
 * Функция для разбора команд
 * uplink_command - команда которую надо разобрать
 */
void parse_command(uint8_t uplink_command);

void init_EX();

void EXCHANGE_task();


#endif /* EXCHANGE_TASK_H_ */

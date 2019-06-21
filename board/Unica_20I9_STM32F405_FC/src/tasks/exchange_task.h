/*
 * exchange_task.h
 *
 *  Created on: 4 июн. 2019 г.
 *      Author: developer
 */

#ifndef EXCHANGE_TASK_H_
#define EXCHANGE_TASK_H_

#include <stm32f4xx_hal.h>
#include "state.h"

extern UART_HandleTypeDef uartExchangeData;
extern UART_HandleTypeDef uartExchangeCommand;


void init_exchange_command_UART(void);
void init_exchange_data_UART(void);

void init_EX(void);
void EXCHANGE_task(void);

#endif /* EXCHANGE_TASK_H_ */

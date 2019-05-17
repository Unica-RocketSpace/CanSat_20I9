/*
 * control_task.h
 *
 *  Created on: 24 нояб. 2018 г.
 *      Author: developer
 */

#ifndef CONTROL_TASK_H_
#define CONTROL_TASK_H_

#include "state.h"
#include <stdint.h>

void init_pins();
void CONTROL_task();

extern GPIO_InitTypeDef d;

#endif /* CONTROL_TASK_H_ */

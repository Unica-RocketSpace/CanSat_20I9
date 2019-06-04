/*
 * Servo.h
 *
 *  Created on: 9 мар. 2019 г.
 *      Author: developer
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <stm32f4xx_hal.h>
#include "state.h"


void _timerPWMInit(TIM_HandleTypeDef *htim);
void _timerPWMChanelInit(TIM_HandleTypeDef *htim, uint32_t Channel);
void _timerPWMChangePulse(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t pulse);
void allServosInit();
void servoRotate(servo_id_t servo_id, float angle_deg);


extern TIM_OC_InitTypeDef CH1;
extern TIM_OC_InitTypeDef CH2;
extern TIM_OC_InitTypeDef CH3;
extern TIM_HandleTypeDef htimServo;

#endif /* SERVO_H_ */

/*
 * servo.h
 *
 *  Created on: 13 апр. 2019 г.
 *      Author: developer
 */

#ifndef SERVO_H_
#define SERVO_H_

//FIXME: DELETE this!!!

#include "state.h"


#define fl float
#define u32 uint32_t


void _timerPWMInit(TIM_HandleTypeDef *htim);
void _timerPWMChanelInit(TIM_HandleTypeDef *htim, uint32_t Channel);
void _timerPWMChangePulse(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t pulse);
void allServosInit();
void servoRotate(servo_id_t servo_id, float angle_deg);
void speedRot(fl speed, servo_id_t servo, fl alpha_start, fl alpha_finish);

void SCHEDULE_SERVO_task();

//обновляет данные в структуре servo_task_param_t
void update_struct(servo_task_param_t * str, float speed, float start_angle, float finish_angle);

//запускает серво таски
void resume_servos(bool left, bool right, bool keel);

//переводит положение серв в angle
void to_angle(bool right, bool left, bool keel, float speed, float angle_right, float angle_left, float angle_keel);


#endif /* SERVO_H_ */

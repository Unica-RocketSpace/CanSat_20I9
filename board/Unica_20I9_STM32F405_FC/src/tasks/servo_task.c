/*
 * servo.c
 *
 *  Created on: 13 апр. 2019 г.
 *      Author: developer
 */

//FIXME: DELETE this!!


#include "state.h"
#include "stdint.h"
#include "stdbool.h"
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>
#include <math.h>

#include "diag/Trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SoAR_task.h"
#include "servo_task.h"


//TODO:Написать ошибки к функциям

//Внутренние глобалные переменные
TIM_OC_InitTypeDef CH1;
TIM_OC_InitTypeDef CH2;
TIM_OC_InitTypeDef CH3;
TIM_HandleTypeDef htimServo;

const fl pulseMin = 3000;
const fl pulseMax = 6000;

const fl speed = 0.1;
const fl fast_speed = 1;

//some constants
const fl angleMin = -20;
const fl angleMax = 10;
const fl angleKeelMax = 45;
const fl angleKeelMin = -45;


void speedRot(servo_task_param_t * str) {

	for(;;){
		if (str->finish_angle > str->start_angle) {
			for (fl i = str->start_angle; i <= str->finish_angle; i += str->speed) {
				servoRotate(str->id, i);
				vTaskDelay(10);
			}
		} else {
			for (fl i = str->start_angle; i >= str->finish_angle; i -= str->speed) {
				servoRotate(str->id, i);
				vTaskDelay(10);
			}
		}
		vTaskSuspend(str->handle);
	}

}

void _timerPWMInit(TIM_HandleTypeDef *htim) {
	htim->Instance = TIM1;
	htim->Init.Prescaler = 52;
	htim->Channel = HAL_TIM_ACTIVE_CHANNEL_1 | HAL_TIM_ACTIVE_CHANNEL_2 | HAL_TIM_ACTIVE_CHANNEL_3;
	htim->Init.CounterMode = TIM_COUNTERMODE_UP;
	htim->Init.Period = 63400;
	htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim->Init.RepetitionCounter = 0;
	HAL_TIM_PWM_Init(htim);
}

void _timerPWMChanelInit(TIM_HandleTypeDef *htim, u32 Channel) {
	TIM_OC_InitTypeDef timOC;
	timOC.OCMode = TIM_OCMODE_PWM1;
	timOC.Pulse = 4000;
	timOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	timOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	timOC.OCFastMode = TIM_OCFAST_ENABLE;
	timOC.OCIdleState = TIM_OCIDLESTATE_SET;	//	??
	timOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
	HAL_TIM_PWM_ConfigChannel(htim, &timOC, Channel);

	HAL_TIMEx_PWMN_Start(htim, Channel);
	//HAL_TIM_PWM_Start(htim, Channel);
}

void _timerPWMChangePulse(TIM_HandleTypeDef *htim, u32 Channel, u32 pulse) {
	TIM_OC_InitTypeDef timOC;
	timOC.Pulse = pulse;
	timOC.OCMode = TIM_OCMODE_PWM1;
	timOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	timOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	timOC.OCFastMode = TIM_OCFAST_ENABLE;
	timOC.OCIdleState = TIM_OCIDLESTATE_SET;	//	??
	timOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
	HAL_TIM_PWM_ConfigChannel(htim, &timOC, Channel);

	HAL_TIMEx_PWMN_Start(htim, Channel);
	//HAL_TIM_PWM_Start(htim, Channel);
	//trace_printf("%d pulse = %d\n", Channel, pulse);
}

void allServosInit() {
	//Включение тактирования
	__GPIOB_CLK_ENABLE();
	__TIM1_CLK_ENABLE();

	GPIO_InitTypeDef gpiob;
	//Настройки пина для ШИМ таймера 1 на канале 1
	gpiob.Alternate = GPIO_AF1_TIM1;
	gpiob.Mode = GPIO_MODE_AF_PP;
	gpiob.Pin = GPIO_PIN_13;
	gpiob.Pull = GPIO_NOPULL;
	gpiob.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpiob);

	//Настройки пина для ШИМ таймера 1 на канале 2
	gpiob.Alternate = GPIO_AF1_TIM1;
	gpiob.Mode = GPIO_MODE_AF_PP;
	gpiob.Pin = GPIO_PIN_14;
	gpiob.Pull = GPIO_NOPULL;
	gpiob.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpiob);

	//Настройки пина для ШИМ таймера 1 на канале 3
	gpiob.Alternate = GPIO_AF1_TIM1;
	gpiob.Mode = GPIO_MODE_AF_PP;
	gpiob.Pin = GPIO_PIN_15;
	gpiob.Pull = GPIO_NOPULL;
	gpiob.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &gpiob);

	//Инициализация таймера TIM1
	_timerPWMInit(&htimServo);
	trace_printf(":this");

	//Инициализация каналов CH1, CH2 и CH3
//
//	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_1);
//	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_2);
//	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_3);

	_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, b1);
	_timerPWMChangePulse(&htimServo, TIM_CHANNEL_2, b2);
	_timerPWMChangePulse(&htimServo, TIM_CHANNEL_3, b3);
}

void update_struct(servo_task_param_t * str, float speed, float start_angle, float finish_angle){
	str->speed = speed;
	str->start_angle = start_angle;
	str->finish_angle = finish_angle;

}

void resume_servos(bool left, bool right, bool keel){
	if (keel)
		vTaskResume(servo_param_keel.handle);
	if (right)
		vTaskResume(servo_param_right.handle);
	if (left)
		vTaskResume(servo_param_left.handle);
}


void to_angle(bool right, bool left, bool keel, float speed, float angle_right, float angle_left, float angle_keel){
	if (right)
		update_struct(&servo_param_right, speed, servo_param_right.finish_angle, angle_right);
	if (left)
		update_struct(&servo_param_left, speed, servo_param_left.finish_angle, angle_left);
	if (keel)
		update_struct(&servo_param_keel, speed, servo_param_keel.finish_angle, angle_keel);

}




void SCHEDULE_SERVO_task(){

	vTaskSuspend(handleLeft);
	vTaskSuspend(handleRight);
	vTaskSuspend(handleKeel);
	vTaskDelay(1000);

	trace_printf("Hi!\n");

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
	servoRotate(servo_go_left, 0);
	servoRotate(servo_go_right, 0);
	servoRotate(servo_keel, 0);


	vTaskDelay(10000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);

	float angle_go_left;
	float angle_go_right;
	float angle_keel;

	for(;;) {
		//начало техасской резни

		//0 to min

		angle_go_left = FC_logs.angle_left;
		angle_go_right = FC_logs.angle_right;
		angle_keel = FC_logs.angle_keel;
		to_angle(1, 1, 1, fast_speed, angle_go_left, angle_go_right, angle_keel);
	}
}

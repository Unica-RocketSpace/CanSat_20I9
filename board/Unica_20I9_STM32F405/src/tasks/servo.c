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
#include <math.h>

#include "diag/Trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "servo.h"


//TODO:Написать ошибки к функциям

//Внутренние глобалные переменные
TIM_OC_InitTypeDef CH1;
TIM_OC_InitTypeDef CH2;
TIM_OC_InitTypeDef CH3;
TIM_HandleTypeDef htimServo;

const fl pulseMin = 3000;
const fl pulseMax = 6000;
const fl k1 = 36.69725;
const fl b1 = 5270;
const fl k2 = -38.2429;
const fl b2 = 5130;
const fl k3 = 40.0;
const fl b3 = 5720;

const fl speed = 0.1;
const fl fast_speed = 1;

//some constants
const fl angleMin = -40;
const fl angleMax = 40;
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
	htim->Channel = 1;
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
	trace_printf("pulse = %d\n", pulse);
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

	//Инициализация каналов CH1, CH2 и CH3
	_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, b1);
	_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, b2);
	_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, b3);
//	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_1);
//	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_2);
//	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_3);
}
void servoRotate(servo_id_t servo_id, fl angle_deg) {
	u32 pulse;
	//trace_printf("angle_deg = %d\t pulse1 = %lf\n pulse = %d\n", (uint16_t)angle_deg, pulse1, pulse);
	switch (servo_id) {
		case servo_left:
			taskENTER_CRITICAL();
			stateServo.angle_left = angle_deg;
			taskEXIT_CRITICAL();

			pulse = (u32)round(k1 * angle_deg + b1);
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, pulse);
			break;
		case servo_right:
			taskENTER_CRITICAL();
			stateServo.angle_right = angle_deg;
			taskEXIT_CRITICAL();

			pulse = (u32)round(k2 * angle_deg + b2);
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_2, pulse);
			break;
		case servo_keel:
			taskENTER_CRITICAL();
			stateServo.angle_keel = angle_deg;
			taskEXIT_CRITICAL();

			pulse = (u32)round(k3 * angle_deg + b3);
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_3, pulse);
			break;
		default:
			break;
	}
	//автогенерация программы которая будет отклонять наши сервы туда-сюда, и быстро пожалуйста
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

	for(;;){

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
		servoRotate(servo_left, 0);
		servoRotate(servo_right, 0);
		servoRotate(servo_keel, 0);


		vTaskDelay(10000);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
		//начало техасской резни


		//поворот левой сервы
/*		update_struct(&servo_param_left, fast_speed, 0, angleMax);
		vTaskResume(servo_param_left.handle);
		vTaskDelay(10000);
		update_struct(&servo_param_left, speed, angleMax, angleMin);
		vTaskResume(servo_param_left.handle);
		vTaskDelay(10000);
		update_struct(&servo_param_left, fast_speed, angleMin, 0);
		vTaskResume(servo_param_left.handle);
		vTaskDelay(1000);


		//поворот правой сервы
		update_struct(&servo_param_right, fast_speed, 0, angleMax);
		vTaskResume(servo_param_right.handle);
		vTaskDelay(10000);
		update_struct(&servo_param_right, speed, angleMax, angleMin);
		vTaskResume(servo_param_right.handle);
		vTaskDelay(10000);
		update_struct(&servo_param_right, fast_speed, angleMin, 0);
		vTaskResume(servo_param_right.handle);
		vTaskDelay(1000);


		//поворот киля
		update_struct(&servo_param_keel, fast_speed, 0, angleKeelMax);
		vTaskResume(servo_param_keel.handle);
		vTaskDelay(10000);
		update_struct(&servo_param_keel, speed, angleKeelMax, angleKeelMin);
		vTaskResume(servo_param_keel.handle);
		vTaskDelay(10000);
		update_struct(&servo_param_keel, fast_speed, angleKeelMin, 0);
		vTaskResume(servo_param_keel.handle);
		vTaskDelay(1000);


		//to -9
		to_angle(1, 1, 0, fast_speed, -9, -9, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);

		//крен го
		to_angle(1, 1, 0, speed, 21, -39, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);

		to_angle(1, 1, 0, speed, -39, 21, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);

		//to 0
		to_angle(1, 1, 0, fast_speed, 0, 0, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);
*/

		to_angle(1, 1, 0, 0.05, angleMin, angleMin, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(30000);

		to_angle(1, 1, 0, fast_speed, 0, 0, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(20000);

		to_angle(1, 1, 0, 0.05, angleMax, angleMax, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(30000);

		to_angle(1, 1, 0, fast_speed, 0, 0, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(20000);

/*
		//to -9
		to_angle(1, 1, 0, fast_speed, -9, -9, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);

		//all
		to_angle(1, 1, 1, speed, 21, -39, -30);
		resume_servos(1, 1, 1);
		vTaskDelay(10000);


		//to -9
		to_angle(1, 1, 1, fast_speed, -9, -9, 0);
		resume_servos(1, 1, 1);
		vTaskDelay(10000);

		//all
		to_angle(1, 1, 1, speed, -39, 21, -30);
		resume_servos(1, 1, 1);
		vTaskDelay(10000);


		//to -9
		to_angle(1, 1, 1, fast_speed, -9, -9, 0);
		resume_servos(1, 1, 1);
		vTaskDelay(10000);

		//all
		to_angle(1, 1, 1, speed, 21, -39, 30);
		resume_servos(1, 1, 1);
		vTaskDelay(10000);


		//to -9
		to_angle(1, 1, 1, fast_speed, -9, -9, 0);
		resume_servos(1, 1, 1);
		vTaskDelay(10000);

		//all
		to_angle(1, 1, 1, speed, -39, 21, 30);
		resume_servos(1, 1, 1);
		vTaskDelay(10000);
*/
/*
		update_struct(&servo_param_left, fast_speed, 0, angleMax);
		update_struct(&servo_param_right, fast_speed, 0, angleMin);
		update_struct(&servo_param_keel, fast_speed, 0, angleKeelMax);
		resume_servos();
		vTaskDelay(10000);

		update_struct(&servo_param_left, speed, angleMax, angleMin);
		update_struct(&servo_param_right, speed, angleMin, angleMax);
		update_struct(&servo_param_keel, speed, angleKeelMax, angleKeelMin);
		resume_servos();
		vTaskDelay(10000);

		update_struct(&servo_param_left, fast_speed, angleMin, 0);
		update_struct(&servo_param_right, fast_speed, angleMax, 0);
		update_struct(&servo_param_keel, fast_speed, angleKeelMin, 0);
		resume_servos();
		vTaskDelay(10000);



		update_struct(&servo_param_left, fast_speed, 0, angleMin);
		update_struct(&servo_param_right, fast_speed, 0, angleMax);
		update_struct(&servo_param_keel, fast_speed, 0, angleKeelMin);
		resume_servos();
		vTaskDelay(10000);

		update_struct(&servo_param_left, speed, angleMin, angleMax);
		update_struct(&servo_param_right, speed, angleMax, angleMin);
		update_struct(&servo_param_keel, speed, angleKeelMin, angleKeelMax);
		resume_servos();
		vTaskDelay(10000);

		update_struct(&servo_param_left, fast_speed, angleMax, 0);
		update_struct(&servo_param_right, fast_speed, angleMin, 0);
		update_struct(&servo_param_keel, fast_speed, angleKeelMax, 0);
		resume_servos();
		vTaskDelay(10000);







		update_struct(&servo_param_left, fast_speed, 0, angleMax);
		update_struct(&servo_param_right, fast_speed, 0, angleMax);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);

		update_struct(&servo_param_left, speed, angleMin, angleMin);
		update_struct(&servo_param_right, speed, angleMin, angleMin);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);

		update_struct(&servo_param_left, fast_speed, angleMin, 0);
		update_struct(&servo_param_right, fast_speed, angleMin, 0);
		resume_servos(1, 1, 0);
		vTaskDelay(10000);
*/
	}
}






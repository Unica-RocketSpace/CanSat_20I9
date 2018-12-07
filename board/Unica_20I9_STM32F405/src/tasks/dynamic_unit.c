///*
// * dynamic_unit.c
// *
// *  Created on: 11 нояб. 2017 г.
// *      Author: developer
// */
//#include "stdbool.h"
//#include <math.h>
//#include <string.h>
//#include <stdlib.h>
//
//#include <diag/Trace.h>
//
//#include <stm32f4xx_hal.h>
//#include <tasks/dynamic_unit.h>
//#include "FreeRTOS.h"
//#include "task.h"
//
//#include "state.h"
//#include "quaternion.h"
//
//#define STEP_DIVIDER		2
//
//#define DRV8855_STEP_PIN	GPIO_PIN_13	//C3
//#define DRV8855_STEP_PORT	GPIOB
//#define DRV8855_DIR_PIN		GPIO_PIN_14	//A4
//#define DRV8855_DIR_PORT	GPIOB
//#define DRV8855_MODE0_PIN	GPIO_PIN_6	//D2
//#define DRV8855_MODE0_PORT	GPIOC
//#define DRV8855_MODE1_PIN	GPIO_PIN_8	//C11
//#define DRV8855_MODE1_PORT	GPIOC
//#define DRV8855_MODE2_PIN	GPIO_PIN_9	//A10
//#define DRV8855_MODE2_PORT	GPIOC
//#define DRV8855_nFAULT_PIN	GPIO_PIN_6	//B11
//#define DRV8855_nFAULT_PORT	GPIOC
//
//#define TARGET_X			10
//#define TARGET_Y			0
//
//
//USART_HandleTypeDef usart_HC05;
//TIM_HandleTypeDef htimSE;
//
//void HC05_Init() {
//
//	usart_HC05.Instance = USART1;
//	usart_HC05.Init.BaudRate = 9600;
//	usart_HC05.Init.WordLength = UART_WORDLENGTH_8B;
//	usart_HC05.Init.StopBits = UART_STOPBITS_1;
//	usart_HC05.Init.Parity = UART_PARITY_NONE;
//	usart_HC05.Init.Mode = UART_MODE_TX_RX;
//
//	HAL_USART_Init(&usart_HC05);
//	HAL_Delay(200);
//}
//
//void TIM1_UP_TIM10_IRQHandler(void) {
//	HAL_TIM_IRQHandler(&htimSE);
//}
//
//
//void timerSEPWMStart(int step_counter) {
//	htimSE.Instance = TIM1;
//	htimSE.Init.Prescaler = 79;
//	htimSE.Init.CounterMode = TIM_COUNTERMODE_UP;
//	htimSE.Init.Period = 800;
//	htimSE.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//	htimSE.Init.RepetitionCounter = (uint32_t)(step_counter - 1);
//	HAL_TIM_PWM_Init(&htimSE);
//
//	TIM_OC_InitTypeDef timOC;
//	timOC.OCMode = TIM_OCMODE_PWM1;
//	timOC.Pulse = 400;
//	timOC.OCPolarity = TIM_OCPOLARITY_HIGH;
//	timOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
//	timOC.OCFastMode = TIM_OCFAST_ENABLE;
//	timOC.OCIdleState = TIM_OCIDLESTATE_SET;	//	??
//	timOC.OCNIdleState = TIM_OCNIDLESTATE_SET;
//	HAL_TIM_PWM_ConfigChannel(&htimSE, &timOC, TIM_CHANNEL_1);
//
//
//	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
//	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0, 0);
//	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
//
//	HAL_TIMEx_PWMN_Start_IT(&htimSE, TIM_CHANNEL_1);
//
//}
//
//
//void step_engine_init () {
//
//	__GPIOA_CLK_ENABLE();
//	__GPIOB_CLK_ENABLE();
//	__GPIOC_CLK_ENABLE();
//	__GPIOD_CLK_ENABLE();
//
//	GPIO_InitTypeDef DRV8825_pins;
//
////	/*Pin STEP*/
////	DRV8825_pins.Pin = DRV8855_STEP_PIN;
////	DRV8825_pins.Mode = GPIO_MODE_OUTPUT_PP;
////	DRV8825_pins.Pull = GPIO_NOPULL;
////	DRV8825_pins.Speed = GPIO_SPEED_FREQ_MEDIUM;
////	HAL_GPIO_Init(DRV8855_STEP_PORT, &DRV8825_pins);
//
//	__TIM1_CLK_ENABLE();
//	__GPIOB_CLK_ENABLE();
//
//	GPIO_InitTypeDef gpiob;
//	gpiob.Alternate = GPIO_AF1_TIM1;
//	gpiob.Mode = GPIO_MODE_AF_PP;
//	gpiob.Pin = GPIO_PIN_13;
//	gpiob.Pull = GPIO_NOPULL;
//	gpiob.Speed = GPIO_SPEED_FREQ_HIGH;
//	HAL_GPIO_Init(GPIOB, &gpiob);
//
//	/*Pin DIR*/
//	DRV8825_pins.Pin = DRV8855_DIR_PIN;
//	DRV8825_pins.Mode = GPIO_MODE_OUTPUT_PP;
//	DRV8825_pins.Pull = GPIO_NOPULL;
//	DRV8825_pins.Speed = GPIO_SPEED_FREQ_MEDIUM;
//	HAL_GPIO_Init(DRV8855_DIR_PORT, &DRV8825_pins);
//
//	/*Pin MODE0*/
//	DRV8825_pins.Pin = DRV8855_MODE0_PIN;
//	DRV8825_pins.Mode = GPIO_MODE_OUTPUT_PP;
//	DRV8825_pins.Pull = GPIO_NOPULL;
//	DRV8825_pins.Speed = GPIO_SPEED_FREQ_MEDIUM;
//	HAL_GPIO_Init(DRV8855_MODE0_PORT, &DRV8825_pins);
//	/*Pin MODE1*/
//	DRV8825_pins.Pin = DRV8855_MODE1_PIN;
//	DRV8825_pins.Mode = GPIO_MODE_OUTPUT_PP;
//	DRV8825_pins.Pull = GPIO_NOPULL;
//	DRV8825_pins.Speed = GPIO_SPEED_FREQ_MEDIUM;
//	HAL_GPIO_Init(DRV8855_MODE1_PORT, &DRV8825_pins);
//	/*Pin MODE2*/
//	DRV8825_pins.Pin = DRV8855_MODE2_PIN;
//	DRV8825_pins.Mode = GPIO_MODE_OUTPUT_PP;
//	DRV8825_pins.Pull = GPIO_NOPULL;
//	DRV8825_pins.Speed = GPIO_SPEED_FREQ_MEDIUM;
//	HAL_GPIO_Init(DRV8855_MODE2_PORT, &DRV8825_pins);
//
//	/*Pin nFAULT*/
//	DRV8825_pins.Pin = DRV8855_nFAULT_PIN;
//	DRV8825_pins.Mode = GPIO_MODE_OUTPUT_PP;
//	DRV8825_pins.Pull = GPIO_NOPULL;
//	DRV8825_pins.Speed = GPIO_SPEED_FREQ_MEDIUM;
//	HAL_GPIO_Init(DRV8855_nFAULT_PORT, &DRV8825_pins);
//
//	HAL_Delay(200);
//
//	//TODO:Написать определение MODE-ов с учетом STEP_DIVIDER
//	//DRV8855_MODE0_PORT->BSRR = 0xFF;
//	HAL_GPIO_WritePin(DRV8855_MODE0_PORT, DRV8855_MODE0_PIN, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(DRV8855_MODE1_PORT, DRV8855_MODE1_PIN, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(DRV8855_MODE2_PORT, DRV8855_MODE2_PIN, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(DRV8855_nFAULT_PORT, DRV8855_nFAULT_PIN, SET);
//
//}
//
//
//void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim) {
//	if (htim->Instance == TIM1) {
//		__TIM1_CLK_ENABLE();
//		__GPIOB_CLK_ENABLE();
//
//		GPIO_InitTypeDef gpiob;
//		gpiob.Alternate = GPIO_AF1_TIM1;
//		gpiob.Mode = GPIO_MODE_AF_PP;
//		gpiob.Pin = GPIO_PIN_12;
//		gpiob.Pull = GPIO_NOPULL;
//		gpiob.Speed = GPIO_SPEED_FREQ_HIGH;
//		HAL_GPIO_Init(GPIOB, &gpiob);
//	}
//}
//
//
//static void get_servo_angle(float* vect, float* angle) {
//	float proj_xy = sqrt(pow(vect[0], 2) + pow(vect[1], 2));
//
//	float angle_current = 0;
//	if (proj_xy != 0) {angle_current = (float)atan(vect[2]/proj_xy);}
//	else angle_current = M_PI/2;
//
//	*angle = angle_current;
//}
//
//static void get_step_engine_angle(float* vect, float* angle) {
//	float angle_current = 0;
//
//	if (vect[0] != 0) {angle_current = (float)atan(vect[0]/vect[1]);}
//	else angle_current = M_PI/2;
//
// 	*angle = angle_current;
//}
//
//
//void calculate_angles (float* step_engine_pos, float* servo_pos) {
//
//	float quat_ISC_RSC[4] = {0.0, 0.0, 0.0, 0.0};
//	float target[3] = {0.0, 0.0, 0.0};				//	направляющий вектор цели
//	float target_RSC[3] = {0.0, 0.0, 0.0};			//	направляющий вектор цели в ССК
//	float local_quaternion[4] = {0.0, 0.0, 0.0, 0.0};	//	локальная переменная для нахождения кватерниона
//
//	//	находим координаты цели
//taskENTER_CRITICAL();
//	memcpy(local_quaternion, stateIMU_isc.quaternion, sizeof(stateIMU_isc.quaternion));
//	//TODO: CHOOSE WHAT DATA TO USE: IMU OR GPS
//	target[0] = TARGET_X - stateIMU_isc.coordinates[0];
//	target[1] = TARGET_Y - stateIMU_isc.coordinates[1];
//	target[2] = - stateIMU_isc.coordinates[2];
//taskEXIT_CRITICAL();
//
//	vect_normalise(target, target);
//	quat_invert(local_quaternion, quat_ISC_RSC);			//	получаем кватернион ИСК->ССК (invert)
//	vect_rotate(target, quat_ISC_RSC, target_RSC);		//	получаем вектор цели в ССК
//
//taskENTER_CRITICAL();
//	memcpy(stateCamera_orient.target, target_RSC, sizeof(target_RSC));
////	for (int i = 0; i < 3; i++) {target_RSC_prev[i] = stateCamera_orient_prev.target[i];}
//taskEXIT_CRITICAL();
//
////TODO: добавить составляющие углов по оринтации
//	get_servo_angle(target_RSC, servo_pos);
//	get_step_engine_angle(target_RSC, step_engine_pos);
//}
//
//
//void send_stepEngine_angle (float* SE_pos, float* SE_pos_prev) {
//
//	float STEP_DEGREES = *SE_pos - *SE_pos_prev;
//	rotate_step_engine_by_angles(&STEP_DEGREES);
//
//}
//
//
//void rotate_step_engine_by_angles (float* angles) {
//
//	float STEP_DEGREES = *angles;
//	bool direction = (STEP_DEGREES >= 0) ? 1 : 0;
//
//	if (direction) HAL_GPIO_WritePin(DRV8855_DIR_PORT, DRV8855_DIR_PIN, GPIO_PIN_SET);
//	else HAL_GPIO_WritePin(DRV8855_DIR_PORT, DRV8855_DIR_PIN, GPIO_PIN_RESET);
//
//	int STEP_TERNS = (int)roundf(STEP_DEGREES / (M_PI *2) * 200)/* * pow(2, STEP_DIVIDER)*/;
//	trace_printf("turnes: %d\n", STEP_TERNS);
////	for(int i = 0; i < (int)round(STEP_TERNS); i++) {
////		HAL_GPIO_WritePin(DRV8855_STEP_PORT, DRV8855_STEP_PIN, GPIO_PIN_SET);
////		//HAL_Delay(1);
////		for(int j = 0; j < 4000; j++){volatile int x = 0;}			//Таймер
////		HAL_GPIO_WritePin(DRV8855_STEP_PORT, DRV8855_STEP_PIN, GPIO_PIN_RESET);
////		//HAL_Delay(1);
////		for(int j = 0; j < 4000; j++){volatile int x = 0;}			//Таймер
////	}
//	timerSEPWMStart(STEP_TERNS);
////	timerSEPWMStart(2000);
//
//}
//
//
//void send_servo_pos(float* servo_pos) {
//
//	float data = *servo_pos;
//	char msg[sizeof(data) + 2];
//	sprintf(msg, "%f\n", data);
//
//	HAL_USART_Transmit(&usart_HC05, (uint8_t*)msg, strlen(msg), 0xFF);
//}
//
//
//static void motors_rotateAll() {
//	//	рассчитываем углы
//	float step_engine_pos = 0;
//	float servo_pos = 0;
//taskENTER_CRITICAL();
//	float step_engine_pos_prev = stateCamera_orient_prev.step_engine_pos;
//taskEXIT_CRITICAL();
//	calculate_angles(&step_engine_pos, &servo_pos);
//
//	//	передаем углы исполняющим органам
//	send_servo_pos(&servo_pos);
//	send_stepEngine_angle(&step_engine_pos, &step_engine_pos_prev);
//
//	//	записываем углы в state
//taskENTER_CRITICAL();
//	stateCamera_orient.step_engine_pos = step_engine_pos;
//	stateCamera_orient.servo_pos = servo_pos;
//taskEXIT_CRITICAL();
//}
//
//
//void MOTORS_Init() {
//	//	Инициализация USART1 для Bluetooth (HC-05)
//	HC05_Init();
//	//	Инициализация драйвера ШД
//	step_engine_init();
//}
//
//
//void MOTORS_task() {
//
//	for (;;) {
////		// Этап 0. Подтверждение инициализации отправкой пакета состояния и ожидание ответа от НС
////		if (state_system.globalStage == 0) {
////		}
////		// Этап 1. Определение начального состояния
////		if (state_system.globalStage == 1) {
////		}
////
////		// Этап 2. Полет в ракете
////		if (state_system.globalStage == 2) {
////		}
//
//		// Этап 3. Свободное падение
//		if (state_system.globalStage == 3) {
//			static int counter = 0;
//
//			if (counter == 0) {
//				float step_engine_pos = 14;
//				float step_engine_pos_prev = 0;
//				send_stepEngine_angle(&step_engine_pos, &step_engine_pos_prev);
//				counter = 1;
//			}
//
//			motors_rotateAll();
//		}
//
//		// Этап 4. Спуск
//		if (state_system.globalStage == 4) {
//			//TODO:	РАСКРЫВАЕМ ПАРАШЮТ
//			static int counter = 0;
//
//			if (counter == 0) {
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, SET);
//				counter = 1;
//			}
//
//			motors_rotateAll();
//		}
//
//		// Этап 5. Окончание полета
//		if (state_system.globalStage == 5) {
//			static int counter = 0;
//
//			if (counter == 0) {
//				float servo_pos = M_PI / 2;
//				send_servo_pos(&servo_pos);
//				counter = 1;
//			}
//
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, RESET);
//		}
//	}
//
//
//}

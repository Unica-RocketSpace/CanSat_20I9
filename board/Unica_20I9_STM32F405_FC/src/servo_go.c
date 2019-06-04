#include "config.h"
#include <stm32f4xx_hal.h>
#include <math.h>

#include "diag/Trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Servo.h"

#define fl float
#define u32 uint32_t
//TODO:Написать ошибки к функциям

//Внутренние глобалные переменные
TIM_OC_InitTypeDef CH1;
TIM_OC_InitTypeDef CH2;
TIM_OC_InitTypeDef CH3;
TIM_HandleTypeDef htimServo;

const fl pulseMin = 3000;
const fl pulseMax = 6000;
const fl k1 = 37.8250591;
const fl b1 = 3500;
const fl k2 = 37.8250591;
const fl b2 = 3500;
const fl k3 = 37.8250591;
const fl b3 = 3500;


void _timerPWMInit(TIM_HandleTypeDef *htim) {
	htim->Instance = TIM1;
	htim->Init.Prescaler = 52;
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
	//trace_printf("pulse = %d\n", pulse);
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
	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_1);
	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_2);
	_timerPWMChanelInit(&htimServo, TIM_CHANNEL_3);
}
void servoRotate(servo_id_t servo_id, fl angle_deg) {
	u32 pulse;
	//trace_printf("angle_deg = %d\t pulse1 = %lf\n pulse = %d\n", (uint16_t)angle_deg, pulse1, pulse);
	switch (servo_id) {
		case servo_go_left:
			pulse = (u32)round(k1 * angle_deg + b1);
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, pulse);
			break;
		case servo_go_right:
			pulse = (u32)round(k2 * angle_deg + b2);
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_2, pulse);
			break;
		case servo_keel:
			pulse = (u32)round(k3 * angle_deg + b3);
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_3, pulse);
			break;
		default:
			break;
	}
	//автогенерация программы которая будет отклонять наши сервы туда-сюда, и быстро пожалуйста
}


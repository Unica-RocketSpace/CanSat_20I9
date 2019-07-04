

//1. НЕ НАЖИМАТЬ CTRL + B НИКОГДА.
//2. НИКОГДА НЕ НАЖИМАТЬ CTRL + B.
//3. ЕСЛИ ВЫ ВДРУГ ПОДУМАЛИ НАЖАТЬ CTRL + B - ТО СМОТРИТЕ ПЕРВЫЕ ДВА ПУНКТА
//4. ЕСЛИ ВЫ (ТУПОЙ) ВСЕ-ТАКИ НАЖАЛИ CTRL + B - ТО РАЗБЕЙТЕ КОМПЬЮТЕР, СОЖГИТЕ ОСТАНКИ, НЕМЕДЛЕННО УЛЕТАЙТЕ С ЭТОЙ ПЛАНЕТЫ И УБЕЙТЕСЬ ОБ СТЕНУ, ПОТОМУ ЧТО УВИДЕННОЕ ВЫ НИКОГДА НЕ ЗАБУДЕТЕ.

#include "state.h"
#include "queue.h"
#include <FreeRTOS.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include <task.h>
#include "SoAR_task.h"
#include "servo_task.h"

#include <diag/Trace.h>


#define ui8 uint8_t
#define deg * M_PI / 180
#define rad / M_PI * 180
#define gfa_len	58
#define kfy_len 21
#define fl float

void servoRotate(servo_id_t servo_id, fl angle_rad) {
	u32 pulse;
	fl angle_deg = angle_rad rad;
//	trace_printf("servo %d angle = %f\n", servo_id, angle_deg);
	switch (servo_id) {
		case servo_go_left:
			taskENTER_CRITICAL();
			stateServo.angle_left = angle_deg;
			taskEXIT_CRITICAL();

			pulse = (u32)round(k1 * angle_deg + b1);
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, pulse);
			break;
		case servo_go_right:
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

servo_id_t serv;

void multiRot(fl angle_go_left, fl angle_go_right, fl angle_keel) {
	serv = 0;
	servoRotate(serv, angle_go_left);
	serv = 1;
	servoRotate(serv, angle_go_right);
	serv = 2;
	servoRotate(serv, angle_keel);
	return;
}

void SoAR_task() {
	const float EPS_alpha = 3 deg; 			//Погрешность даблов в сравнении
	const float EPS_beta = 3 deg;			//Погрешность в beta
	const float EPS_yaw = 5 deg;			//Погрешность рысканья
	const float EPS_roll_straight = 5 deg;	//Погрешность крена для прямолинейного движения
	const float EPS_roll_turn = 10 deg;		//Погрешность крена для повороте
	const float EPS_omega = 5 deg;			//Погрешность омеги
	const float EPS_pitch = 5 deg;
	const float EPS_speed = 0.5;

	const ui8 SHIFT_PITCH = 9;				//СМЕЩЕНИЕ ИНДЕКСАЦИИ МАССИВА go_from_alpha
	const ui8 SHIFT_YAW = 10;				//СМЕЩЕНИЕ ИНДЕКСАЦИИ МАССИВА keel_from_yaw

	const float PITCH_MIN = -90;
	const float PITCH_MAX = 0;

	const float DELTA_SPEED = 2;

	const float alpha_nominal = 5 deg;

	float go_from_alpha[gfa_len] = {-0.15, -1.03, -2.6, -3.75, -4.15, -4.6, -5.05, -5.73, -6.4, -6.6, -6.8, -7.25, -7.48, -7.7, -7.93, -8.15, -8.38, -8.55, -8.78, -9, -9.23, -9.45, -9.65, -9.85, -10.1, -10.35, -10.58, -10.8, -11.2, -11.65, -12.33, -13.18, -14.05, -14.7, -15.57, -16.7, -17.35, -17.8, -18.25, -18.65, -19.1, -19.325, -19.75, -20.175, -20.85, -21.525, -24.1849, -27.03, -27.9, -28.35, -28.9748, -29.65, -30.3248, -31.4331, -33.4081, -35.3997, -37.3664, -38.8997};
	float keel_from_yaw[kfy_len] = {0};

	for (int i = 0; i < kfy_len; ++i) {
		keel_from_yaw[i] = (i - SHIFT_YAW) * 2 deg;
	}

	float alpha_memory = 1e8;
	float yaw_memory = 1e8;

	float alpha;
	float beta;
	float speed_gps;
	float speed_bmp;
	float pitch;
	float course;

	float roll;
	float yaw;

	float time;
	float go_pitch;
	float keel_yaw;

	const float roll_slow = 5 deg;
	const float roll_turn = 30 deg;

	const float delta_nominal = 5 deg;
	const float delta_roll_turn = 5 deg;
	const float delta_roll_rot = 8 deg;

	float pitch_last;
	float speed;
	float speed_last;

	//ВЫХОД ИЗ БОЧКИ

	Predictor_Angles.alpha = 10 deg;
	Predictor_Angles.beta = 0.0 deg;

	//запрашиваем данные с SC
	ui8 tmp = COMMAND_DATA;
	xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);

	taskENTER_CRITICAL();
	pitch_last = state_master.pitch;
	speed_last = state_master.speed_BMP;
	taskEXIT_CRITICAL();

	float omega_x;
	vTaskDelay(100 / portTICK_RATE_MS);

	//запрашиваем данные с SC
	tmp = COMMAND_DATA;
	xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);

	taskENTER_CRITICAL();
	omega_x = state_master.omega;
	speed = state_master.speed_BMP;
	pitch = state_master.pitch;
	taskEXIT_CRITICAL();

	int8_t sign;
	sign = 1;
	if (omega_x < 0) {
		sign = -1;
	}
	//присваиваем значения углов тангажа, рысканья, крена в структуру
	FC_logs.angle_left =  (go_from_alpha[SHIFT_PITCH] deg) - (float)sign * delta_roll_rot;
	FC_logs.angle_right = (go_from_alpha[SHIFT_PITCH] deg) + (float)sign * delta_roll_rot;
	FC_logs.angle_keel =  0 deg;
//	trace_printf("Омега sign: %d \n", sign);
	multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
	float omega_start = omega_x;
//	trace_printf("Omega_Start: %f \n", omega_start rad);
	while (omega_start / omega_x > 0) {
		vTaskDelay(200 / portTICK_RATE_MS);
		//запрашиваем данные с SC
		tmp = COMMAND_DATA;
		xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);
		pitch_last = pitch;
		speed_last = speed;
		taskENTER_CRITICAL();
		omega_x = state_master.omega;
		speed = state_master.speed_BMP;
		pitch = state_master.pitch;
		taskEXIT_CRITICAL();
		//trace_printf("Omega_X: %f \n", omega_x rad);
		//trace_printf("БОЧКА!!!");
	}
	//trace_printf("ВЫШЛИ ИЗ БОЧКИ!");
	FC_logs.angle_left =  go_from_alpha[SHIFT_PITCH] deg;
	FC_logs.angle_right = go_from_alpha[SHIFT_PITCH] deg;
	multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);

	while (abs(pitch - pitch_last) >= EPS_pitch && abs(speed - speed_last) >= EPS_speed) {
		if (speed_last - speed > DELTA_SPEED) {
			FC_logs.angle_left =  go_from_alpha[0] deg;
			FC_logs.angle_right = go_from_alpha[0] deg;
//			trace_printf("trash");
			multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
		} else {
			float k = ((- SHIFT_PITCH / 2) - (gfa_len - SHIFT_PITCH) / 2) / (PITCH_MAX - PITCH_MIN);
			float b = (gfa_len - SHIFT_PITCH) / 2 - k * PITCH_MIN;
			alpha = k * pitch + b;
			FC_logs.angle_left =  go_from_alpha[(ui8)round(alpha rad * 2) + SHIFT_PITCH] deg;
			FC_logs.angle_right = go_from_alpha[(ui8)round(alpha rad * 2) + SHIFT_PITCH] deg;
			multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
		}
		vTaskDelay(500 / portTICK_RATE_MS);
		//запрашиваем данные с SC
		tmp = COMMAND_DATA;
		xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);
		speed_last = speed;
		pitch_last = pitch;

		taskENTER_CRITICAL();
		speed = state_master.speed_BMP;
		pitch = state_master.pitch;
		taskEXIT_CRITICAL();

		//trace_printf("ПИКЕ!!");
	}
	//trace_printf("ВЫШЛИ ИЗ ПИКЕ!");
	vTaskDelay(500 / portTICK_RATE_MS);
	FC_logs.angle_left =  go_from_alpha[(ui8)round(alpha_nominal rad * 2) + SHIFT_PITCH] deg;
	FC_logs.angle_right = go_from_alpha[(ui8)round(alpha_nominal rad * 2) + SHIFT_PITCH] deg;
	multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);

	while (true) {
		//запрашиваем данные с SC
		tmp = COMMAND_DATA;
		xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);

		taskENTER_CRITICAL();
		alpha = Predictor_Angles.alpha;
		beta = Predictor_Angles.beta;
		speed_gps = state_master.speed_GPS;
		speed_bmp = state_master.speed_BMP;
		pitch = state_master.pitch;
		course = state_master.course;
		roll = state_master.roll;
		yaw = state_master.yaw;

		taskEXIT_CRITICAL();
		//наверно

		//trace_printf("Параметры--------\n");
		//trace_printf("ALPHA \t \t BETA \t \t speed_GPS \t speed_BMP \t PITCH \t YAW \t \t ROLL \t \t COURSE \n");
		//trace_printf("%f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n", alpha rad, beta rad, speed_gps, speed_bmp, pitch rad, yaw rad, roll rad, course rad);

		if (fabs(beta) <= EPS_beta) {
		//trace_printf("<---Прямолинейный полет--->\n");
			//РАСЧЕТ КАНАЛА ТАНГАЖА
			/*
			 *
			 */

			if (alpha > (gfa_len - SHIFT_PITCH) / 2) {
				alpha = (gfa_len - SHIFT_PITCH) / 2;
			}
			if (alpha < (-SHIFT_PITCH) / 2) {
				alpha = (-SHIFT_PITCH) / 2;
			}
			if (abs(alpha - alpha_memory) >= EPS_alpha) {
				alpha_memory = alpha;
				//мы должны поставить на движках go_from_alpha[alpha_memory]
				//trace_printf("Alpha: %f \n", alpha);
				go_pitch = go_from_alpha[(ui8)round(alpha rad * 2) + SHIFT_PITCH] deg;
			} else {
				//alpha_now - now-time angle of attack
				double temp = (double)speed_gps / (double)speed_bmp;
				temp -= cos((double)pitch);
				float alpha_now = - (float)atan(temp / sin((double)pitch));
				//trace_printf("sin(pitch) = %f deg\n", sin((double)pitch));
				//trace_printf("cos(pitch) = %f deg\n", cos((double)pitch));
				//trace_printf("v/v = %f deg\n", (double)(speed_gps / speed_bmp));
				//trace_printf("pitch = %f deg\n", pitch rad);
				//trace_printf("Alpha_now: %f \n", alpha_now rad);
				//dif от difference - разница между углом атаки предиктора и углом аттаки аппарата
				float dif = alpha_now - alpha;
				//мы должны поставить на движках go_from_alpha[alpha_memory - (dif / 2)]
				if ((ui8)round((alpha rad - (dif rad / 2)) * 2) + SHIFT_PITCH > gfa_len) {
					go_pitch = go_from_alpha[gfa_len] deg;
					FC_logs.critical_angles |= (1 << 0);
					//предельный угол атаки
				} else if ((ui8)round((alpha rad - (dif rad / 2)) * 2) + SHIFT_PITCH < 0) {
					go_pitch = go_from_alpha[0] deg;
					FC_logs.critical_angles |= (1 << 0);
					//предельный угол атаки
				} else {
					go_pitch = go_from_alpha[(ui8)round((alpha rad - (dif rad / 2)) * 2) + SHIFT_PITCH] deg;
					FC_logs.critical_angles &= ~(1 << 0);
				}
//				trace_printf("Go_pitch: %f \n", go_pitch);
			}


			FC_logs.angle_left = go_pitch;
			FC_logs.angle_right = go_pitch;

			//РАСЧЕТ КАНАЛА РЫСКАНИЯ
			/*
			 *
			 */

			keel_yaw = 0 deg;

			taskENTER_CRITICAL();
			yaw = state_master.yaw;
			taskEXIT_CRITICAL();

			if (abs(yaw - course) >= EPS_yaw) {
				keel_yaw = keel_from_yaw[(ui8)round(yaw rad - course rad) + SHIFT_YAW];
			}

			//РАСЧЕТ КАНАЛА КРЕНА
			/*
			 *
			 */

			taskENTER_CRITICAL();
			roll = state_master.roll;
			taskEXIT_CRITICAL();

			while (fabs(roll) >= EPS_roll_straight) {
//				trace_printf("O, we have some kren %f { \n", roll);
				if (roll > 0) {
					//присваиваем значения углов тангажа, рысканья, крена в структуру
					time = HAL_GetTick() / 1000;
					FC_logs.time = time;
					FC_logs.angle_left =  go_pitch - roll_slow;
					FC_logs.angle_right = go_pitch + roll_slow;
					FC_logs.angle_keel = keel_yaw;
					multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
				} else {
					//присваиваем значения углов тангажа, рысканья, крена в структуру
					time = HAL_GetTick() / 1000;
					FC_logs.time = time;
					FC_logs.angle_left =  go_pitch + roll_slow;
					FC_logs.angle_right = go_pitch - roll_slow;
					FC_logs.angle_keel = keel_yaw;
					multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
				}
				vTaskDelay(50 / portTICK_RATE_MS);
				//присваиваем значения углов тангажа, рысканья, крена в структуру
				/*time = HAL_GetTick() / 1000;
				FC_logs.time = time;
				FC_logs.angle_left =  go_pitch;
				FC_logs.angle_right = go_pitch;
				FC_logs.angle_keel = keel_yaw;
				multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
				vTaskDelay(200 / portTICK_RATE_MS);
				//запрашиваем данные с SC
				*/tmp = COMMAND_DATA;
				xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);

				taskENTER_CRITICAL();
				roll = state_master.roll;
				taskEXIT_CRITICAL();
			}
			time = HAL_GetTick() / 1000;
			FC_logs.time = time;
			FC_logs.angle_left =  go_pitch;
			FC_logs.angle_right = go_pitch;
			FC_logs.angle_keel = keel_yaw;
			multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
			vTaskDelay(200 / portTICK_RATE_MS);
			multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);

		} else {

			taskENTER_CRITICAL();
			beta = Predictor_Angles.beta;
			taskEXIT_CRITICAL();

			yaw_memory = course + beta;

			//РАСЧЕТ КАНАЛА КРЕНА
			/*
			 *
			 */

			taskENTER_CRITICAL();
			roll = state_master.roll;
			taskEXIT_CRITICAL();

			sign = 1.0;
			if (beta < 0) {
				sign = -1.0;
			}
			while (abs(roll - sign * roll_turn) >= EPS_roll_turn) {
				//присваиваем значения углов тангажа, рысканья, крена в структуру
				FC_logs.angle_left = delta_nominal - sign * delta_roll_turn;
				FC_logs.angle_right = delta_nominal + sign * delta_roll_turn;
				multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
				vTaskDelay(50 / portTICK_RATE_MS);
				//присваиваем значения углов тангажа, рысканья, крена в структуру
				FC_logs.angle_left = delta_nominal;
				FC_logs.angle_right = delta_nominal;
				multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
				vTaskDelay(200 / portTICK_RATE_MS);
				//запрашиваем данные с SC
				tmp = COMMAND_DATA;
				xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);

				taskENTER_CRITICAL();
				roll = state_master.roll;
				taskEXIT_CRITICAL();

//				trace_printf("Крен: %d \n", roll);
			}

			//РАСЧЕТ КАНАЛА РЫСКАНИЯ
			/*
			 *
			 */

			taskENTER_CRITICAL();
			yaw = state_master.yaw;
			course = state_master.course;
			taskEXIT_CRITICAL();

			while (fabs(yaw_memory - course) >= EPS_beta) {
				//Управление килем
				if (fabs(yaw - course) >= EPS_yaw) {
					keel_yaw = keel_from_yaw[((- (ui8)round(yaw rad - course rad) / 2) * 2) + SHIFT_YAW];
				}

				vTaskDelay(500 / portTICK_RATE_MS);
				//запрашиваем данные с SC
				tmp = COMMAND_DATA;
				xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);

				taskENTER_CRITICAL();
				course = state_master.course;
				yaw = state_master.yaw;
				taskEXIT_CRITICAL();

				//trace_printf("Курс: %d \n Рыскание: %d \n", course, yaw);

				taskENTER_CRITICAL();
				if (Predictor_Angles.beta / sign < 0) {
					break;
				}
				if (fabs(beta - Predictor_Angles.beta) >= EPS_beta) {
					beta = Predictor_Angles.beta;
					yaw_memory = course + beta;
				}
				taskEXIT_CRITICAL();

				time = HAL_GetTick() / 1000;
				FC_logs.time = time;
//				trace_printf("KEK2");
				tmp = COMMAND_LOGS;
				xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);
			}

			//КРЕН НА ДНО

			taskENTER_CRITICAL();
			roll = state_master.roll;
			taskEXIT_CRITICAL();

			while (fabs(roll) >= EPS_roll_turn) {
				//присваиваем значения углов тангажа, рысканья, крена в структуру
				time = HAL_GetTick() / 1000;
				FC_logs.time = time;
				FC_logs.angle_left = delta_nominal + sign * delta_roll_turn;
				FC_logs.angle_right = delta_nominal - sign * delta_roll_turn;
				multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
				vTaskDelay(50 / portTICK_RATE_MS);
				//присваиваем значения углов тангажа, рысканья, крена в структуру
				time = HAL_GetTick() / 1000;
				FC_logs.time = time;
				FC_logs.angle_left = delta_nominal;
				FC_logs.angle_right = delta_nominal;
				multiRot(FC_logs.angle_left, FC_logs.angle_right, FC_logs.angle_keel);
				vTaskDelay(200 / portTICK_RATE_MS);
				//запрашиваем данные с SC
				tmp = COMMAND_DATA;
				xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);

				taskENTER_CRITICAL();
				roll = state_master.roll;
				taskEXIT_CRITICAL();

//				trace_printf("Крен: %d \n", roll);
			}
		}
		//присваиваем значения углов тангажа, рысканья, крена в структуру
		time = HAL_GetTick() / 1000;
		FC_logs.time = time;
//		trace_printf("KEK1");
		tmp = COMMAND_LOGS;
		xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
}

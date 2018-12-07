/*
 * sensors_task.c
 *
 *  Created on: 24 нояб. 2018 г.
 *      Author: developer
 */


#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sofa.h>

#include "FreeRTOS.h"
#include "task.h"

#include "diag/Trace.h"
#include "state.h"
#include "MadgwickAHRS.h"
#include "quaternion.h"
#include "MPU9255.h"

#define BETA_0	sqrt(3/4) * M_PI * (0.2f / 180.0f)
#define BETA_1	0.033
#define BETA_2	0.3
#define BETA_3	0.25



I2C_HandleTypeDef 	i2c_mpu9255;
USART_HandleTypeDef usart_dbg;

rscs_bmp280_descriptor_t * bmp280;
rscs_bmp280_descriptor_t * IMUbmp280;
const rscs_bmp280_calibration_values_t * bmp280_calibration_values;


uint8_t get_gyro_staticShift(float* gyro_staticShift) {
	uint8_t error = 0;
	uint16_t zero_orientCnt = 200;

	//	находим статическое смещение гироскопа
	for (int i = 0; i < zero_orientCnt; i++) {
		int16_t accelData[3] = {0, 0, 0};
		int16_t gyroData[3] = {0, 0, 0};
		float gyro[3] = {0, 0, 0};

		//	собираем данные
		PROCESS_ERROR(mpu9255_readIMU(accelData, gyroData));
		mpu9255_recalcGyro(gyroData, gyro);

		for (int m = 0; m < 3; m++) {
			gyro_staticShift[m] += gyro[m];
		}
//		vTaskDelay(10/portTICK_RATE_MS);
	}
	for (int m = 0; m < 3; m++) {
		gyro_staticShift[m] /= zero_orientCnt;
	}
end:
	return error;
}

uint8_t get_accel_staticShift(float* gyro_staticShift, float* accel_staticShift) {
	uint8_t error = 0;
	uint16_t zero_orientCnt = 100;
	float time = 0, time_prev = (float)HAL_GetTick() / 1000;

	for (int i = 0; i < zero_orientCnt; i++) {
		int16_t accelData[3] = {0, 0, 0};
		int16_t gyroData[3] = {0, 0, 0};
		float accel[3] = {0, 0, 0};
		float accel_ISC[3] = {0, 0, 0};
		float gyro[3] = {0, 0, 0};

		//	собираем данные
		PROCESS_ERROR(mpu9255_readIMU(accelData, gyroData));
		mpu9255_recalcGyro(gyroData, gyro);
		mpu9255_recalcAccel(accelData, accel);

		time = (float)HAL_GetTick() / 1000;
		for (int k = 0; k < 3; k++) {
			gyro[k] -= gyro_staticShift[k];
		}

		float quaternion[4] = {0, 0, 0, 0};
		MadgwickAHRSupdateIMU(quaternion,
				gyro[0], gyro[1], gyro[2],
				accel[0], accel[1], accel[2], time - time_prev, 0.033);
		vect_rotate(accel, quaternion, accel_ISC);

		for (int m = 0; m < 3; m++) {
			accel_staticShift[m] += accel_ISC[m];
		}

		time_prev = time;
	}
	for (int m = 0; m < 3; m++) {
		accel_staticShift[m] /= zero_orientCnt;
	}
end:
	return error;
}


static int IMU_updateDataAll() {
//////	СОБИРАЕМ ДАННЫЕ IMU	//////////////////////
	//	массивы для хранения
	int error = 0;
	int16_t accelData[3] = {0, 0, 0};
	int16_t gyroData[3] = {0, 0, 0};
	int16_t compassData[3] = {0, 0, 0};
	float accel[3] = {0, 0, 0}; float gyro[3] = {0, 0, 0}; float compass[3] = {0, 0, 0};

	//	собираем данные
	PROCESS_ERROR(mpu9255_readIMU(accelData, gyroData));
	PROCESS_ERROR(mpu9255_readCompass(compassData));
	mpu9255_recalcAccel(accelData, accel);
	mpu9255_recalcGyro(gyroData, gyro);
	mpu9255_recalcCompass(compassData, compass);

taskENTER_CRITICAL();
	float _time = (float)HAL_GetTick() / 1000;
	state_system.time = _time;
	//	пересчитываем их и записываем в структуры
	for (int k = 0; k < 3; k++) {
		stateIMU_rsc.accel[k] = accel[k];
		gyro[k] -= state_zero.gyro_staticShift[k];
		stateIMU_rsc.gyro[k] = gyro[k];
		stateIMU_rsc.compass[k] = compass[k];
	}
taskEXIT_CRITICAL();
////////////////////////////////////////////////////


/////////	ОБНОВЛЯЕМ КВАТЕРНИОН  //////////////////
	float quaternion[4] = {0, 0, 0, 0};
taskENTER_CRITICAL();
	float dt = _time - state_system_prev.time;
taskEXIT_CRITICAL();

//	if (state_system.globalStage <=2)
//		MadgwickAHRSupdateIMU(quaternion, gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], dt, 0.033);
//	if (state_system.globalStage >= 3)
		MadgwickAHRSupdate(quaternion, gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], compass[0], compass[1], compass[2], dt, 1);

	//	копируем кватернион в глобальную структуру
taskENTER_CRITICAL();
	stateIMU_isc.quaternion[0] = quaternion[0];
	stateIMU_isc.quaternion[1] = quaternion[1];
	stateIMU_isc.quaternion[2] = quaternion[2];
	stateIMU_isc.quaternion[3] = quaternion[3];
taskEXIT_CRITICAL();
////////////////////////////////////////////////////


/////////  ПЕРЕВОДИМ ВЕКТОРЫ в ISC  ////////////////
	float accel_ISC[3] = {0, 0, 0};
	float compass_ISC[3] = {0, 0, 0};
	//	ускорение
	vect_rotate(accel, quaternion, accel_ISC);
	//	вектор магнитного поля
	vect_rotate(compass, quaternion, compass_ISC);

	//	копируем векторы в глобальную структуру
taskENTER_CRITICAL();
	for (int i = 0; i < 3; i++)
		accel_ISC[i] -= state_zero.accel_staticShift[i];

	stateIMU_isc.accel[0] = accel_ISC[0];
	stateIMU_isc.accel[1] = accel_ISC[1];
	stateIMU_isc.accel[2] = accel_ISC[2];
	stateIMU_isc.compass[0] = compass_ISC[0];
	stateIMU_isc.compass[1] = compass_ISC[1];
	stateIMU_isc.compass[2] = compass_ISC[2];
taskEXIT_CRITICAL();
////////////////////////////////////////////////////


///////  ОБНОВЛЯЕМ КООРДИНАТЫ И СКОРОСТИ  //////////
	if (state_system.globalStage >= 2) { //FIXME: Посмотреть в каком этапе это нужно?

		float delta_velo[3] = {0, 0, 0};
		float delta_coord[3] = {0, 0, 0};
		float accel_ISC_prev[3] = {0, 0, 0};
		float velo[3] = {0, 0, 0};
		float velo_prev[3] = {0, 0, 0};
	taskENTER_CRITICAL();
		for (int i = 0; i < 3; i++) {
			accel_ISC_prev[i] = stateIMU_isc_prev.accel[i];
			velo[i] = stateIMU_isc.velocities[i];
			velo_prev[i] = stateIMU_isc_prev.velocities[i];
		}
	taskEXIT_CRITICAL();

		for (int i = 0; i < 3; i++) {
			delta_velo[i] = (accel_ISC[i] + accel_ISC_prev[i]) * dt / 2;
			delta_coord[i] = (velo[i] + velo_prev[i]) * dt / 2;
		}

	taskENTER_CRITICAL();
		for (int i = 0; i < 3; i++) {
			stateIMU_isc.velocities[i] += delta_velo[i];
			stateIMU_isc.coordinates[i] += delta_coord[i];
		}
	taskEXIT_CRITICAL();
}
////////////////////////////////////////////////////

end:
	return error;
}


void bmp280_update() {
	int32_t pressure = 0;
	int32_t temp = 0;
	float pressure_f = 0;
	float temp_f = 0;
	float height = 0;
	rscs_bmp280_read(IMUbmp280, &pressure, &temp);
	rscs_bmp280_calculate(bmp280_calibration_values, pressure, temp, &pressure_f, &temp_f);

taskENTER_CRITICAL();
	float zero_pressure = state_zero.zero_pressure;
taskEXIT_CRITICAL();
	height = 18.4 * log(zero_pressure / pressure_f);

taskENTER_CRITICAL();
	stateIMUSensors_raw.pressure = pressure;
	stateIMUSensors_raw.temp = temp;
	stateIMUSensors.pressure = pressure_f;
	stateIMUSensors.temp = temp_f;
	stateIMUSensors.height = height;
taskEXIT_CRITICAL();

	pressure = 0; temp = 0; pressure_f = 0;	temp_f = 0; height = 0;

	rscs_bmp280_read(bmp280, &pressure, &temp);
	rscs_bmp280_calculate(bmp280_calibration_values, pressure, temp, &pressure_f, &temp_f);

	height = 18.4 * log(zero_pressure / pressure_f);

taskENTER_CRITICAL();
	stateSensors_raw.pressure = pressure;
	stateSensors_raw.temp = temp;
	stateSensors.pressure = pressure_f;
	stateSensors.temp = temp_f;
	stateSensors.height = height;
taskEXIT_CRITICAL();
}


static void get_staticShifts() {
	float gyro_staticShift[3] = {0, 0, 0};
	float accel_staticShift[3] = {0, 0, 0};
	get_gyro_staticShift(gyro_staticShift);
	get_accel_staticShift(gyro_staticShift, accel_staticShift);
taskENTER_CRITICAL();
	for (int i = 0; i < 3; i++) {
		state_zero.gyro_staticShift[i] = gyro_staticShift[i];
		state_zero.accel_staticShift[i] = accel_staticShift[i];
	}
taskEXIT_CRITICAL();
}


void _IMUtask_updateData() {
taskENTER_CRITICAL();
	memcpy(&stateIMU_isc_prev, 			&stateIMU_isc,			sizeof(stateIMU_isc));
	memcpy(&stateSensors_prev,			&stateSensors, 			sizeof(stateSensors));
	memcpy(&state_system_prev, 			&state_system,		 	sizeof(state_system));
	memcpy(&stateCamera_orient_prev, 	&stateCamera_orient, 	sizeof(stateCamera_orient));
taskEXIT_CRITICAL();
}


void IMU_Init() {
	//---ИНИЦИАЛИЗАЦИЯ MPU9255---//
	uint8_t mpu9255_initError = mpu9255_init(&i2c_mpu9255);
	trace_printf("mpu: %d\n", mpu9255_initError);

	//---ИНИЦИАЛИЗАЦИЯ IMUBMP280---//
	IMUbmp280 = rscs_bmp280_initi2c(&i2c_mpu9255, RSCS_BMP280_I2C_ADDR_HIGH);					//создание дескриптора
	rscs_bmp280_parameters_t IMUbmp280_parameters;
	IMUbmp280_parameters.pressure_oversampling = RSCS_BMP280_OVERSAMPLING_X4;		//4		16		измерения на один результат
	IMUbmp280_parameters.temperature_oversampling = RSCS_BMP280_OVERSAMPLING_X2;	//1		2		измерение на один результат
	IMUbmp280_parameters.standbytyme = RSCS_BMP280_STANDBYTIME_500US;				//0.5ms	62.5ms	время между 2 измерениями
	IMUbmp280_parameters.filter = RSCS_BMP280_FILTER_X16;							//x16	x16		фильтр

	int8_t IMUbmp280_initError = rscs_bmp280_setup(bmp280, &IMUbmp280_parameters);								//запись параметров
	rscs_bmp280_changemode(bmp280, RSCS_BMP280_MODE_NORMAL);					//установка режима NORMAL, постоянные измерения
	bmp280_calibration_values = rscs_bmp280_get_calibration_values(IMUbmp280);

	state_system.MPU_state = mpu9255_initError;
	state_system.IMU_BMP_state = IMUbmp280_initError;

	//---ИНИЦИАЛИЗАЦИЯ BMP280---//
	bmp280 = rscs_bmp280_initi2c(&i2c_mpu9255, RSCS_BMP280_I2C_ADDR_LOW);					//создание дескриптора
	rscs_bmp280_parameters_t bmp280_parameters;
	bmp280_parameters.pressure_oversampling = RSCS_BMP280_OVERSAMPLING_X4;		//4		16		измерения на один результат
	bmp280_parameters.temperature_oversampling = RSCS_BMP280_OVERSAMPLING_X2;	//1		2		измерение на один результат
	bmp280_parameters.standbytyme = RSCS_BMP280_STANDBYTIME_500US;				//0.5ms	62.5ms	время между 2 измерениями
	bmp280_parameters.filter = RSCS_BMP280_FILTER_X16;							//x16	x16		фильтр

	int8_t bmp280_initError = rscs_bmp280_setup(bmp280, &bmp280_parameters);								//запись параметров
	rscs_bmp280_changemode(bmp280, RSCS_BMP280_MODE_NORMAL);					//установка режима NORMAL, постоянные измерения
	bmp280_calibration_values = rscs_bmp280_get_calibration_values(bmp280);

	state_system.BMP_state = bmp280_initError;
}

void zero_data(){
	taskENTER_CRITICAL();
	state_zero.zero_pressure = stateSensors.pressure;
	for (int i = 0; i < 3; i++)
		state_zero.zero_GPS[i] = stateGPS.coordinates[i];
	for (int i = 0; i < 4; i++)
		state_zero.zero_quaternion[i] = stateIMU_isc.quaternion[i];
	taskEXIT_CRITICAL();
}


void SENSORS_task() {

	for (;;) {

		if (state_system.globalStage == 1)
		// start all

//		get_staticShifts();
//
//
//
////		workig
		bmp280_update();
		IMU_updateDataAll();
		_IMUtask_updateData();
//
//
//		//zero_data
//		zero_data();
		trace_printf("SENSORS_task");

		while(1){
		vTaskDelay(1000);
		}

		xTaskNotifyGive(handleControl);
		xTaskNotifyGive(handleRF);

		vTaskDelay(10/portTICK_RATE_MS);

	}

/*
	//	usart_dbg init
	usart_dbg.Instance = USART3;
	usart_dbg.Init.BaudRate = 256000;
	usart_dbg.Init.WordLength = UART_WORDLENGTH_8B;
	usart_dbg.Init.StopBits = UART_STOPBITS_1;
	usart_dbg.Init.Parity = UART_PARITY_NONE;
	usart_dbg.Init.Mode = UART_MODE_TX_RX;

	HAL_USART_Init(&usart_dbg);
*/
}








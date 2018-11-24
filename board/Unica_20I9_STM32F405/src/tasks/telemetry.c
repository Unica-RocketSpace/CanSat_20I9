/*
 * TELEMETRY_UNIT
 *
 * 		This unit will be used for transmitting and receiving data
 * 	with device and earth using radio transmitter.
 *
 * 	Authors: Korr237i, RaKetov
 */

#include <tasks/telemetry.h>
#include "stdint.h"

#include "diag/Trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mavlink/UNISAT/mavlink.h"

#include "state.h"
#include "nRF24L01.h"
#include "drivers/sd/sd.h"
#include "drivers/sd/dump.h"

#include "mavlink/UNISAT/mavlink.h"

uint8_t UNISAT_ID = 0x01;
uint8_t UNISAT_NoComp = 0xFF;
uint8_t UNISAT_IMU = 0x02;
uint8_t UNISAT_SENSORS = 0x03;
uint8_t UNISAT_GPS = 0x04;
uint8_t UNISAT_RPI = 0x05;
uint8_t UNISAT_CAM = 0x06;

static dump_channel_state_t stream_file;

// FIXME: ЗАМЕНИТЬ ВРЕМЯ В ПАКЕТЕ НА ВРЕМЯ ПОЛУЧЕНИЯ ДАННЫХ

static uint8_t mavlink_msg_state_send() {

	mavlink_state_t msg_state;
	msg_state.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_state.MPU_state		= state_system.MPU_state;
	msg_state.BMP_state 	= state_system.BMP_state;
	msg_state.SD_state 		= state_system.SD_state;
//	msg_state.NRF_state		= state_system.NRF_state;
//	msg_state.MOTOR_state	= state_system.MOTOR_state;
//	msg_state.GPS_state		= state_system.GPS_state;

//	msg_state.globalStage	= state_system.globalStage;
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_state_encode(UNISAT_ID, UNISAT_NoComp, &msg, &msg_state);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);
	uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len, 1);

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	dump(&stream_file, buffer, len);

	return error;
}

static uint8_t mavlink_msg_imu_rsc_send() {

	mavlink_imu_rsc_t msg_imu_rsc;
	msg_imu_rsc.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	for (int i = 0; i < 3; i++) {
		msg_imu_rsc.accel[i] = stateIMU_rsc.accel[i];
		msg_imu_rsc.gyro[i] = stateIMU_rsc.gyro[i];
		msg_imu_rsc.compass[i] = stateIMU_rsc.compass[i];
	}
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_imu_rsc_encode(UNISAT_GPS, UNISAT_IMU, &msg, &msg_imu_rsc);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);
	uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len, 1);

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	dump(&stream_file, buffer, len);

	return error;
}

static uint8_t mavlink_msg_imu_isc_send() {

	mavlink_imu_isc_t msg_imu_isc;
	msg_imu_isc.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	for (int i = 0; i < 3; i++) {
		msg_imu_isc.accel[i] = stateIMU_isc.accel[i];
		msg_imu_isc.compass[i] = stateIMU_isc.compass[i];
//		msg_imu_isc.velocities[i] = stateIMU_isc.velocities[i];
//		msg_imu_isc.coordinates[i] = stateIMU_isc.coordinates[i];
	}
	for (int j = 0; j < 4; j++) {
		msg_imu_isc.quaternion[j] = stateIMU_isc.quaternion[j];
	}
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_imu_isc_encode(UNISAT_ID, UNISAT_IMU, &msg, &msg_imu_isc);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);
	uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len, 1);

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	dump(&stream_file, buffer, len);

	return error;
}

static uint8_t mavlink_msg_sensors_send() {

	mavlink_sensors_t msg_sensors;
	msg_sensors.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_sensors.temp = stateSensors.temp;
	msg_sensors.pressure = stateSensors.pressure;
	msg_sensors.height = stateSensors.height;
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_sensors_encode(UNISAT_ID, UNISAT_SENSORS, &msg, &msg_sensors);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);
	uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len, 1);

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	dump(&stream_file, buffer, len);

	return error;
}

//static uint8_t mavlink_msg_gps_send() {
//
//	mavlink_gps_t msg_gps;
//	msg_gps.time = (float)HAL_GetTick() / 1000;
//taskENTER_CRITICAL();
//	for (int i = 0; i < 3; i++)
//		msg_gps.coordinates[i] = stateGPS.coordinates[i];
//taskEXIT_CRITICAL();
//
//	mavlink_message_t msg;
//	uint16_t len = mavlink_msg_gps_encode(UNISAT_ID, UNISAT_GPS, &msg, &msg_gps);
//	uint8_t buffer[100];
//	len = mavlink_msg_to_send_buffer(buffer, &msg);
//	uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len, 1);
//
//	taskENTER_CRITICAL();
//	state_system.SD_state = stream_file.res;
//	taskEXIT_CRITICAL();
//	dump(&stream_file, buffer, len);
//
//	return error;
//}

static uint8_t mavlink_msg_state_zero_send() {

	mavlink_state_zero_t msg_state_zero;
	msg_state_zero.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_state_zero.zero_pressure = state_zero.zero_pressure;
	for (int i = 0; i < 4; i++)
		msg_state_zero.zero_quaternion[i] = state_zero.zero_quaternion[i];
	for (int i = 0; i < 3; i++) {
//		msg_state_zero.zero_GPS[i] = state_zero.zero_GPS[i];
		msg_state_zero.gyro_staticShift[i] = state_zero.gyro_staticShift[i];
		msg_state_zero.accel_staticShift[i] = state_zero.accel_staticShift[i];
	}
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_state_zero_encode(UNISAT_ID, UNISAT_GPS, &msg, &msg_state_zero);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);
	uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len, 1);

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	dump(&stream_file, buffer, len);

	return error;
}




void IO_RF_Init() {

	uint8_t nRF24L01_initError = nRF24L01_init(&spi_nRF24L01);
	state_system.NRF_state = nRF24L01_initError;
	HAL_Delay(100);

	stream_file.res = 1;
	//	запуск SD
	stream_file.file_opened = false;
	dump_init(&stream_file);
	state_system.SD_state = (uint8_t)stream_file.res;
	HAL_Delay(200);
}


void IO_RF_task() {

	for (;;) {

		ulTaskNotifyTake(1);


//		vTaskDelay(100/portTICK_RATE_MS);
//
		mavlink_msg_state_send();
		mavlink_msg_imu_isc_send();
		mavlink_msg_imu_rsc_send();
		mavlink_msg_sensors_send();
////		mavlink_msg_gps_send();
////		mavlink_msg_camera_orientation_send();
//
//
//		// Этап 0. Подтверждение инициализации отправкой пакета состояния и ожидание ответа от НС
//		if (state_system.globalStage == 0) {
//		taskENTER_CRITICAL();
//			if (state_system.globalCommand == 1)
//				state_system.globalStage = 1;
//		taskEXIT_CRITICAL();
//		}
//
//		// Этап 1. Определение начального состояния
//		if (state_system.globalStage == 1) {
//			mavlink_msg_state_zero_send();
//
//
//		taskENTER_CRITICAL();
//			if (state_system.globalCommand == 2)
//				state_system.globalStage = 2;
//		taskEXIT_CRITICAL();
//
//		}
//		// Этап 2. Полет в ракете
//		if (state_system.globalStage == 2) {
//		taskENTER_CRITICAL();
//			if (state_system.globalCommand == 3)
//				state_system.globalStage = 3;
//		taskEXIT_CRITICAL();
//		}
//		// Этап 3. Свободное падение
//		if (state_system.globalStage == 3) {
//		taskENTER_CRITICAL();
//			if (state_system.globalCommand == 4)
//				state_system.globalStage = 4;
//		taskEXIT_CRITICAL();
//		}
//		// Этап 4. Спуск
//		if (state_system.globalStage == 4) {
//		}
//		// Этап 5. Окончание полета
//		if (state_system.globalStage == 5) {
//		}
	}
}


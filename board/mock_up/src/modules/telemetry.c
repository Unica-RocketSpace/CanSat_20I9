/*
 * TELEMETRY_UNIT
 *
 * 		This unit will be used for transmitting and receiving data
 * 	with device and earth using radio transmitter.
 *
 * 	Authors: Korr237i, RaKetov
 */

#include "stdint.h"

#include "diag/Trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mavlink/UNISAT/mavlink.h"

#include "state.h"
#include "telemetry.h"
#include "drivers/sd/sd.h"
#include "drivers/sd/dump.h"

uint8_t first;


SPI_HandleTypeDef	spi_nRF24L01;


uint8_t UNISAT_ID = 0x01;
uint8_t UNISAT_NoComp = 0xFF;
uint8_t UNISAT_IMU = 0x02;
uint8_t UNISAT_SENSORS = 0x03;
uint8_t UNISAT_GPS = 0x04;
uint8_t UNISAT_RPI = 0x05;
uint8_t UNISAT_CAM = 0x06;

static dump_channel_state_t stream_file;


static uint16_t mavlink_msg_state_send() {

	mavlink_state_t msg_state;
	msg_state.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_state.MPU_state		= state_system.MPU_state;
	msg_state.IMU_BMP_state = state_system.IMU_BMP_state;
	msg_state.SD_state 		= state_system.SD_state;
	msg_state.BMP_state		= state_system.BMP_state;
//	msg_state.MOTOR_state	= state_system.MOTOR_state;
//	msg_state.GPS_state		= state_system.GPS_state;

//	msg_state.globalStage	= state_system.globalStage;
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len_state = mavlink_msg_state_encode(UNISAT_ID, UNISAT_NoComp, &msg, &msg_state);
	uint8_t buffer[100];
	len_state = mavlink_msg_to_send_buffer(buffer, &msg);
	if(USE_RF){
		uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len_state, 1);
	}

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	if(USE_SD){
		//trace_printf("sd used\n");
		dump(&stream_file, buffer, len_state);
	}
	return len_state;
//	return error;
}

static uint16_t mavlink_msg_imu_rsc_send() {

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
	uint16_t len_imu_rsc = mavlink_msg_imu_rsc_encode(UNISAT_GPS, UNISAT_IMU, &msg, &msg_imu_rsc);
	uint8_t buffer[100];
	len_imu_rsc = mavlink_msg_to_send_buffer(buffer, &msg);
	if(USE_RF){
		uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len_imu_rsc, 1);
	}

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	if(USE_SD){
		//trace_printf("sd used\n");
		dump(&stream_file, buffer, len_imu_rsc);
	}
	return len_imu_rsc;
//	return error;
}

static uint16_t mavlink_msg_imu_isc_send() {

	mavlink_imu_isc_t msg_imu_isc;
	msg_imu_isc.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	for (int i = 0; i < 3; i++) {
		msg_imu_isc.accel[i] = stateIMU_isc.accel[i];
		msg_imu_isc.compass[i] = stateIMU_isc.compass[i];
	}
	for (int j = 0; j < 4; j++) {
		msg_imu_isc.quaternion[j] = stateIMU_isc.quaternion[j];
	}
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len_imu_isc = mavlink_msg_imu_isc_encode(UNISAT_ID, UNISAT_IMU, &msg, &msg_imu_isc);
	uint8_t buffer[100];
	len_imu_isc = mavlink_msg_to_send_buffer(buffer, &msg);
	if(USE_RF){
		uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len_imu_isc, 1);
	}

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();

	if(USE_SD){
		//trace_printf("sd used\n");
		dump(&stream_file, buffer, len_imu_isc);
	}
	return len_imu_isc;
//	return error;
}

static uint16_t mavlink_msg_sensors_send() {

	mavlink_sensors_t msg_sensors;
	msg_sensors.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_sensors.temp = stateIMUSensors.temp;
	msg_sensors.pressure = stateIMUSensors.pressure;
	msg_sensors.height = stateIMUSensors.height;
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len_sensors = mavlink_msg_sensors_encode(UNISAT_ID, UNISAT_SENSORS, &msg, &msg_sensors);
	uint8_t buffer[100];
	len_sensors = mavlink_msg_to_send_buffer(buffer, &msg);
	if(USE_RF){
		uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len_sensors, 1);
	}

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	if(USE_SD){
		//trace_printf("sd used\n");
		dump(&stream_file, buffer, len_sensors);
	}
	return len_sensors;
//	return error;
}

static uint16_t mavlink_msg_BMP_send() {

	mavlink_bmp280_t msg_BMP;
		msg_BMP.time = (float)HAL_GetTick() / 1000;
	taskENTER_CRITICAL();
		msg_BMP.temp = stateSensors.temp;
		msg_BMP.pressure = stateSensors.pressure;

	taskEXIT_CRITICAL();

		mavlink_message_t msg;
		uint16_t len_sensors = mavlink_msg_bmp280_encode(UNISAT_ID, UNISAT_SENSORS, &msg, &msg_BMP);
		uint8_t buffer[100];
		len_sensors = mavlink_msg_to_send_buffer(buffer, &msg);
		if(USE_RF){
			uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len_sensors, 1);
		}

		taskENTER_CRITICAL();
		state_system.SD_state = stream_file.res;
		taskEXIT_CRITICAL();
		if(USE_SD){
			//trace_printf("sd used\n");
			dump(&stream_file, buffer, len_sensors);
		}
		return len_sensors;
	//	return error;
	}
//
static void mavlink_msg_state_zero_send() {

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
//	uint8_t error = nRF24L01_send(&spi_nRF24L01, buffer, len, 1);

	taskENTER_CRITICAL();
	state_system.SD_state = stream_file.res;
	taskEXIT_CRITICAL();
	dump(&stream_file, buffer, len);

//	return error;
}

void IO_RF_Init() {

	stream_file.res = 1;
	//	запуск SD
	stream_file.file_opened = false;
	dump_init(&stream_file);
	state_system.SD_state = (uint8_t)stream_file.res;
	HAL_Delay(200);
}


void IO_RF_task() {
	GPIO_InitTypeDef gpioc;
	gpioc.Mode = GPIO_MODE_OUTPUT_PP;
	gpioc.Pin = GPIO_PIN_12;
	gpioc.Pull = GPIO_NOPULL;
	gpioc.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &gpioc);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, RESET);

	first = 1;

	for (;;) {

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, SET);
		vTaskDelay(20/portTICK_RATE_MS);
//
		if (first == 1){
			mavlink_msg_state_zero_send();
			first = 0;
		}
			$(mavlink_msg_state_send());
			$(mavlink_msg_imu_isc_send());
			$(mavlink_msg_imu_rsc_send());
			$(mavlink_msg_sensors_send());
			$(mavlink_msg_BMP_send());
		}

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, RESET);
	}



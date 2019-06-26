#include "stm32f4xx_hal.h"

#include "tasks/ground.h"
#include "tasks/telemetry.h"

#include "stdint.h"
#include "stdbool.h"

#include "diag/Trace.h"
#include "FreeRTOS.h"
#include "task.h"
#include "mavlink/UNISAT/mavlink.h"

#include "state.h"
#include "nRF24L01.h"

#include "stm32f4xx_hal_uart.h"

/*
uint8_t UNISAT_ID = 0x01;
uint8_t UNISAT_NoComp = 0xFF;
uint8_t UNISAT_IMU = 0x02;
uint8_t UNISAT_SENSORS = 0x03;
uint8_t UNISAT_GPS = 0x04;
uint8_t UNISAT_RPI = 0x05;
uint8_t UNISAT_CAM = 0x06;
*/

static uint16_t mavlink_msg_state() {

	mavlink_state_t msg_state;
	msg_state.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_state.MPU_state		= state_system.MPU_state;
	msg_state.IMU_BMP_state = state_system.IMU_BMP_state;
	msg_state.SD_state 		= state_system.SD_state;
	msg_state.NRF_state		= state_system.NRF_state;
	msg_state.GPS_state		= state_system.GPS_state;
	msg_state.BMP_state		= state_system.BMP_state;
	msg_state.master_state	= state_system.master_state;
	msg_state.buttons		= state_system.buttons;

	msg_state.globalStage	= state_system.globalStage;
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_state_encode(UNISAT_ID, UNISAT_NoComp, &msg, &msg_state);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);

	return len;
}

static uint16_t mavlink_msg_imu_rsc() {

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

	return len;
}

static uint16_t mavlink_msg_imu_isc() {

	mavlink_imu_isc_t msg_imu_isc;
	msg_imu_isc.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	for (int i = 0; i < 3; i++) {
		msg_imu_isc.accel[i] = stateIMU_isc.accel[i];
		msg_imu_isc.compass[i] = stateIMU_isc.compass[i];
		msg_imu_isc.velocities[i] = stateIMU_isc.velocities[i];
		msg_imu_isc.coordinates[i] = stateIMU_isc.coordinates[i];
	}
	for (int j = 0; j < 4; j++) {
		msg_imu_isc.quaternion[j] = stateIMU_isc.quaternion[j];
	}
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_imu_isc_encode(UNISAT_ID, UNISAT_IMU, &msg, &msg_imu_isc);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);

	return len;
}

static uint16_t mavlink_msg_sensors(){

	mavlink_sensors_t msg_sensors;
	msg_sensors.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_sensors.temp = stateIMUSensors.temp;
	msg_sensors.pressure = stateIMUSensors.pressure;
	msg_sensors.height = stateIMUSensors.height;
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_sensors_encode(UNISAT_ID, UNISAT_SENSORS, &msg, &msg_sensors);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);

	return len;
}


static uint16_t mavlink_msg_BMP(){

	mavlink_sensors_t msg_BMP;
	msg_BMP.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_BMP.temp = stateSensors.temp;
	msg_BMP.pressure = stateSensors.pressure;
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_sensors_encode(UNISAT_ID, UNISAT_SENSORS, &msg, &msg_BMP);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);

	return len;
}


// FIXME:::::::::::::::::::::::::::::::
static uint16_t mavlink_msg_gps(){

	mavlink_gps_t msg_gps;
	msg_gps.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	for (int i = 0; i < 2; i++){
		msg_gps.coordinates[i] = stateGPS.coordinates[i];
	}
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_gps_encode(UNISAT_ID, UNISAT_GPS, &msg, &msg_gps);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);

	return len;
}


static uint16_t mavlink_msg_state_zero(){

	mavlink_state_zero_t msg_state_zero;
	msg_state_zero.time = (float)HAL_GetTick() / 1000;
taskENTER_CRITICAL();
	msg_state_zero.zero_pressure = state_zero.zero_pressure;
	for (int i = 0; i < 4; i++)
		msg_state_zero.zero_quaternion[i] = state_zero.zero_quaternion[i];
	for (int i = 0; i < 2; i++) {
		msg_state_zero.zero_GPS[i] = state_zero.zero_GPS[i];
		msg_state_zero.gyro_staticShift[i] = state_zero.gyro_staticShift[i];
		msg_state_zero.accel_staticShift[i] = state_zero.accel_staticShift[i];
	}
taskEXIT_CRITICAL();

	mavlink_message_t msg;
	uint16_t len = mavlink_msg_state_zero_encode(UNISAT_ID, UNISAT_GPS, &msg, &msg_state_zero);
	uint8_t buffer[100];
	len = mavlink_msg_to_send_buffer(buffer, &msg);

	return len;
}

USART_HandleTypeDef usart_ground;


uint8_t buffer[32];
uint8_t _status;
uint8_t __error;
uint16_t len;
uint8_t read_error;

uint16_t sensors_len;
uint16_t BMP_len;
uint16_t imu_isc_len;
uint16_t imu_rsc_len;
uint16_t gps_len;
uint16_t state_len;
uint16_t state_zero_len;


void get_lenght_msg(){
	sensors_len = mavlink_msg_sensors();
	BMP_len = mavlink_msg_BMP();
	imu_isc_len = mavlink_msg_imu_isc();
	imu_rsc_len = mavlink_msg_imu_rsc();
	gps_len = mavlink_msg_gps();
	state_len = mavlink_msg_state();
	state_zero_len = mavlink_msg_state_zero();
}


uint16_t check_len_mav_packet(uint32_t ID){
	//uint8_t ID = buffer[5];
//	trace_printf("ID\t%d\n", ID);
	if (ID == MAVLINK_MSG_ID_BMP280) return BMP_len;
	if (ID == MAVLINK_MSG_ID_GPS) return gps_len;
	if (ID == MAVLINK_MSG_ID_IMU_isc) return imu_isc_len;
	if (ID == MAVLINK_MSG_ID_IMU_rsc) return imu_rsc_len;
	if (ID == MAVLINK_MSG_ID_Sensors) return sensors_len;
	if (ID == MAVLINK_MSG_ID_STATE) return state_len;
	if (ID == MAVLINK_MSG_ID_STATE_ZERO) return state_zero_len;
	return -2;
}

void GROUND_Init(){

	uint8_t error = 255;

	uint8_t nRF24L01_initError = -1;

	do {
		nRF24L01_initError =  nRF24L01_init(&spi_nRF24L01);
		trace_printf("init_error\t%d\n", nRF24L01_initError);
//		led();
	}
	while (nRF24L01_initError != 0);

	trace_printf("nRF init error  %d\n", nRF24L01_initError);
	state_system.NRF_state = nRF24L01_initError;

	get_lenght_msg();

	HAL_Delay(100);

		//	usart_dbg init
//		usart_ground.Instance = USART3;
//		usart_ground.Init.BaudRate = 115200;
//		usart_ground.Init.WordLength = UART_WORDLENGTH_8B;
//		usart_ground.Init.StopBits = UART_STOPBITS_1;
//		usart_ground.Init.Parity = UART_PARITY_NONE;
//		usart_ground.Init.Mode = UART_MODE_TX_RX;
//
//		PROCESS_ERROR(HAL_USART_Init(&usart_ground));
//
//	HAL_Delay(200);
//
//end:
//	trace_printf("%d\n", error);
}


void GROUND_task() {


	for (;;) {
		uint32_t start_read = HAL_GetTick();

		read_error = nRF24L01_read(&spi_nRF24L01, buffer, sizeof(buffer), &data);


//		uint32_t ID = (uint32_t)buffer[7];
//		uint16_t len_msg = check_len_mav_packet(ID);

//		if (len_msg != 65534){
//			uint8_t foo = (len_msg - 32) / 32;
//			trace_printf("foo   %d\n", foo);
//			for (int i = 0; i < foo; i++){
//				read_error = nRF24L01_read(&spi_nRF24L01, buffer + (i+1)*32, (len_msg - 32*(i+1)),&data);
//			}
//		}

//		if (data && (len_msg != 65534) && !read_error){
//			nRF24L01_clear_status(&spi_nRF24L01, 1, 1, 1);
		if (data) {
			read_error = HAL_USART_Transmit(&usart_ground, buffer, sizeof(buffer), 10);
			led();
		}

//		}
			uint32_t end_read = HAL_GetTick();

		trace_printf("task start:\t%d\tend read\t%d\ttask end\t%d\n", start_read, (tick_ - tickstart_), (end_read - start_read));
		memset(buffer, 0, sizeof(buffer));

//		vTaskDelay(5/portTICK_RATE_MS);





//			read_error = nRF24L01_read(&spi_nRF24L01, buffer, sizeof(buffer), &data);
//
//			uint32_t ID = (uint32_t)buffer[7];
//			uint16_t len = check_len_mav_packet(ID);
//
//			trace_printf("len\t%d\n", len);
//
//			nRF24L01_read_status(&spi_nRF24L01, &_status);
//			trace_printf("RE\t%d\tSTAT\t%d\n---------------------------\n", read_error, _status);
////			trace_printf("data\t%d\n", data);
//			if (data /*&& (len != 65534)*/){
//				trace_printf("DR\t");
//				nRF24L01_clear_status(&spi_nRF24L01, 1, 1, 1);
//				read_error = HAL_USART_Transmit(&usart_ground, buffer, len, 10);
//				memset(buffer, 0, 100);
//
//				led();
//			}
		}

//			vTaskDelay(5/portTICK_RATE_MS);
			//trace_printf();

}


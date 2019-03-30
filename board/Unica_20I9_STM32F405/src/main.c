#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_cortex.h>
#include <stm32f4xx_hal_i2c.h>
#include <stm32f4xx_hal_usart.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_gpio.h>


#include "diag/Trace.h"
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <tasks/control_task.h>
#include <tasks/sensors_task.h>
#include <tasks/telemetry.h>
#include <tasks/ground.h>
#include "task.h"
#include "mavlink/UNISAT/mavlink.h"


#include "state.h"
#include "gps_nmea.h"
#include "MPU9255.h"
#include "UNICS_bmp280.h"
#include "nRF24L01.h"

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


// глобальные структуры
stateSensors_raw_t 	stateIMUSensors_raw;
stateSensors_raw_t	stateSensors_raw;
stateGPS_t 			stateGPS;
stateIMU_rsc_t 		stateIMU_rsc;
stateIMU_isc_t 		stateIMU_isc;
stateSensors_t 		stateIMUSensors;
stateBMPSensors_t	stateSensors;
state_system_t 		state_system;
state_master_t		state_master;
state_zero_t		state_zero;

stateIMU_isc_t		stateIMU_isc_prev;
stateSensors_t		stateIMUSensors_prev;
stateSensors_t		stateSensors_prev;
state_system_t		state_system_prev;

TaskHandle_t 		handleControl;
TaskHandle_t		handleRF;

QueueHandle_t		handleInternalCmdQueue;


//	параметры IO_RF_task
#define IO_RF_TASK_STACK_SIZE (50*configMINIMAL_STACK_SIZE)
static StackType_t	_iorfTaskStack[IO_RF_TASK_STACK_SIZE];
static StaticTask_t	_iorfTaskObj;

//	параметры GPS_task
#define GPS_TASK_STACK_SIZE (80*configMINIMAL_STACK_SIZE)
static StackType_t _gpsTaskStack[GPS_TASK_STACK_SIZE];
static StaticTask_t _gpsTaskObj;

//	параметры IMU_task
#define IMU_TASK_STACK_SIZE (60*configMINIMAL_STACK_SIZE)
static StackType_t	_IMUTaskStack[IMU_TASK_STACK_SIZE];
static StaticTask_t	_IMUTaskObj;

#define CONTROL_TASK_STACK_SIZE (20*configMINIMAL_STACK_SIZE)
static StackType_t _CONTROLTaskStack[CONTROL_TASK_STACK_SIZE];
static StaticTask_t _CONTROLTaskObj;

#define CALIBRATION_TASK_STACK_SIZE (20*configMINIMAL_STACK_SIZE)
static StackType_t	_CALIBRATIONTaskStack[CALIBRATION_TASK_STACK_SIZE];
static StaticTask_t	_CALIBRATIONTaskObj;

//	параметры GROUND_task
#define GROUND_TASK_STACK_SIZE (50*configMINIMAL_STACK_SIZE)
static StackType_t	_groundTaskStack[GROUND_TASK_STACK_SIZE];
static StaticTask_t	_groundTaskObj;

#define LED_TASK_STACK_SIZE (10*configMINIMAL_STACK_SIZE)
static StackType_t	_ledTaskStack[LED_TASK_STACK_SIZE];
static StaticTask_t	_ledfTaskObj;


#define INTERNAL_QUEUE_LENGHT  sizeof( uint8_t )
#define INTERNAL_QUEUE_ITEM_SIZE  5
uint8_t internal_queue_storage_area[INTERNAL_QUEUE_LENGHT * INTERNAL_QUEUE_ITEM_SIZE];
static StaticQueue_t internal_queue_static;



void CALIBRATION_task() {
	GPIO_InitTypeDef gpioc;
	gpioc.Mode = GPIO_MODE_OUTPUT_PP;
	gpioc.Pin = GPIO_PIN_12;
	gpioc.Pull = GPIO_NOPULL;
	gpioc.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &gpioc);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, RESET);

	uint8_t error = 0;

	for (;;) {

		int16_t accelData[3] = {0, 0, 0};
		int16_t gyroData[3] = {0, 0, 0};
		int16_t compassData[3] = {0, 0, 0};
		float accel[3] = {0, 0, 0};
		float gyro[3] = {0, 0, 0};
		float compass[3] = {0, 0, 0};

		//	geting data
		PROCESS_ERROR(mpu9255_readIMU(accelData, gyroData));
		PROCESS_ERROR(mpu9255_readCompass(compassData))
		mpu9255_recalcAccel(accelData, accel);
		mpu9255_recalcGyro(gyroData, gyro);
		mpu9255_recalcCompass(compassData, compass);


		//	transmitting raw values
		mavlink_imu_rsc_t msg_imu_rsc;
		msg_imu_rsc.time = (float)HAL_GetTick() / 1000;
	taskENTER_CRITICAL();
		for (int i = 0; i < 3; i++) {
			msg_imu_rsc.accel[i] = accel[i];
			msg_imu_rsc.gyro[i] = gyro[i];
			msg_imu_rsc.compass[i] = compass[i];
		}
	taskEXIT_CRITICAL();
		mavlink_message_t msg;
		uint16_t len = mavlink_msg_imu_rsc_encode(0, 0, &msg, &msg_imu_rsc);
		uint8_t buffer[100];
		len = mavlink_msg_to_send_buffer(buffer, &msg);
		nRF24L01_send(&spi_nRF24L01, buffer, len, 1);


		//	flashing the led
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, SET);

		vTaskDelay(2000);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, RESET);
//		float tickend = HAL_GetTick();
//		trace_printf("start: %f\nend %f\n", tickstart, tickend);

	end:
		error = 0;
		continue;
	}

}

void Init_led(){
	GPIO_InitTypeDef gpioc;
	gpioc.Mode = GPIO_MODE_OUTPUT_PP;
	gpioc.Pin = GPIO_PIN_12;
	gpioc.Pull = GPIO_NOPULL;
	gpioc.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC, &gpioc);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, SET);
}


void LED_task(){

	for(;;){
		taskENTER_CRITICAL();
		if ((state_system.BMP_state == 0) & (state_system.GPS_state == 0) & (state_system.IMU_BMP_state == 0) & (state_system.MPU_state == 0) & (state_system.SD_state == 0)){
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, RESET);
			//volatile int x = 0;
			vTaskDelay(10);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, SET);
		}
		taskEXIT_CRITICAL();
	}
}




int main(int argc, char* argv[])
{
	// Инициализация структур глобального состояния (в нашем случае просто заполняем их нулями)
	memset(&stateIMUSensors_raw, 	0x00, sizeof(stateIMUSensors_raw));
	memset(&stateSensors_raw,		0x00, sizeof(stateSensors_raw));
	memset(&stateGPS, 				0x00, sizeof(stateGPS));
	memset(&stateIMU_rsc, 			0x00, sizeof(stateIMU_rsc));
	memset(&stateIMU_isc, 			0x00, sizeof(stateIMU_isc));
	memset(&stateIMUSensors, 		0x00, sizeof(stateIMUSensors));
	memset(&stateSensors,			0x00, sizeof(stateSensors));
	memset(&state_system, 			0x00, sizeof(state_system));
	memset(&state_master,			0x00, sizeof(state_master));
	memset(&state_zero, 	 		0x00, sizeof(state_zero));

	memset(&stateIMU_isc_prev, 			0x00, sizeof(stateIMU_isc_prev));
	memset(&stateIMUSensors_prev,		0x00, sizeof(stateIMUSensors_prev));
	memset(&stateSensors_prev,			0x00, sizeof(stateSensors_prev));
	memset(&state_system_prev, 			0x00, sizeof(state_system_prev));

	state_system.BMP_state 	= 255;
	state_system.GPS_state 	= 255;
	state_system.buttons 	= 255;
	state_system.MPU_state 	= 255;
	state_system.NRF_state 	= 255;
	state_system.SD_state 	= 255;

//	xTaskCreateStatic(SENSORS_task, 	"SENSORS", 		IMU_TASK_STACK_SIZE, 	NULL, 1, _IMUTaskStack, 	&_IMUTaskObj);

//	handleRF = xTaskCreateStatic(IO_RF_task, 	"IO_RF", 	IO_RF_TASK_STACK_SIZE,	NULL, 1, _iorfTaskStack, 	&_iorfTaskObj);

	xTaskCreateStatic(GROUND_task, 	"GROUND", 	GROUND_TASK_STACK_SIZE,	NULL, 1, _groundTaskStack, 	&_groundTaskObj);

//	xTaskCreateStatic(LED_task, "LED", LED_TASK_STACK_SIZE, NULL, 1, _ledTaskStack, &_ledfTaskObj);

//	handleControl = xTaskCreateStatic(CONTROL_task, "CONTROL", CONTROL_TASK_STACK_SIZE, NULL, 2, _CONTROLTaskStack, &_CONTROLTaskObj);

//	xTaskCreateStatic(GPS_task, 	"GPS", 		GPS_TASK_STACK_SIZE, 	NULL, 2, _gpsTaskStack, 	&_gpsTaskObj);



//	handleInternalCmdQueue = xQueueCreateStatic(INTERNAL_QUEUE_LENGHT, INTERNAL_QUEUE_ITEM_SIZE, internal_queue_storage_area, &internal_queue_static);

//	xTaskCreateStatic(CALIBRATION_task, "CALIBRATION", CALIBRATION_TASK_STACK_SIZE, NULL, 1, _CALIBRATIONTaskStack, &_CALIBRATIONTaskObj);


	__GPIOA_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOE_CLK_ENABLE();
	__GPIOF_CLK_ENABLE();
	__GPIOG_CLK_ENABLE();
	__GPIOH_CLK_ENABLE();

//	IMU_Init();
//	IO_RF_Init();
	Init_led();
	GROUND_Init();
//	GPS_Init();

	HAL_InitTick(15);


	vTaskStartScheduler();


	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

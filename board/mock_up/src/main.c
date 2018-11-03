#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_cortex.h>
#include <stm32f4xx_hal_i2c.h>
#include <stm32f4xx_hal_usart.h>
#include <stm32f4xx_hal_dma.h>


#include "diag/Trace.h"
#include <FreeRTOS.h>
#include "task.h"

#include "state.h"
#include "kinematic_unit.h"
#include "MPU9255.h"
#include "UNICS_bmp280.h"
#include "telemetry.h"



// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


// глобальные структуры
stateSensors_raw_t 	stateIMUSensors_raw;
stateSensors_raw_t 	stateSensors_raw;
stateIMU_rsc_t 		stateIMU_rsc;
stateIMU_isc_t 		stateIMU_isc;
stateSensors_t 		stateIMUSensors;
stateBMPSensors_t 	stateSensors;
state_system_t 		state_system;
state_zero_t		state_zero;

stateIMU_isc_t		stateIMU_isc_prev;
stateSensors_t		stateIMUSensors_prev;
stateBMPSensors_t		stateSensors_prev;
state_system_t		state_system_prev;



//	параметры IO_RF_task
#define IO_RF_TASK_STACK_SIZE (50*configMINIMAL_STACK_SIZE)
static StackType_t	_iorfTaskStack[IO_RF_TASK_STACK_SIZE];
static StaticTask_t	_iorfTaskObj;

//	параметры IMU_task
#define IMU_TASK_STACK_SIZE (60*configMINIMAL_STACK_SIZE)
static StackType_t	_IMUTaskStack[IMU_TASK_STACK_SIZE];
static StaticTask_t	_IMUTaskObj;


void CALIBRATION_task() {

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

		vTaskDelay(2000);

	end:
		error = 0;
		continue;
	}
}


int main(int argc, char* argv[])
{
	// Инициализация структур глобального состояния (в нашем случае просто заполняем их нулями)
	memset(&stateSensors_raw, 		0x00, sizeof(stateSensors_raw));
	memset(&stateIMU_rsc, 			0x00, sizeof(stateIMU_rsc));
	memset(&stateIMU_isc, 			0x00, sizeof(stateIMU_isc));
	memset(&stateIMUSensors, 		0x00, sizeof(stateIMUSensors));
	memset(&stateSensors, 			0x00, sizeof(stateSensors));
	memset(&state_system, 			0x00, sizeof(state_system));
	memset(&state_zero, 			0x00, sizeof(state_zero));

	memset(&stateIMU_isc_prev, 				0x00, sizeof(stateIMU_isc_prev));
	memset(&stateIMUSensors_prev,			0x00, sizeof(stateIMUSensors_prev));
	memset(&stateSensors_prev,				0x00, sizeof(stateSensors_prev));
	memset(&state_system_prev, 				0x00, sizeof(state_system_prev));


	state_system.IMU_BMP_state = 255;
	state_system.BMP_state = 255;
	state_system.MPU_state = 255;
	state_system.SD_state = 255;

	xTaskCreateStatic(IMU_task, 	"IMU", 		IMU_TASK_STACK_SIZE, 	NULL, 1, _IMUTaskStack, 	&_IMUTaskObj);

	xTaskCreateStatic(IO_RF_task, 	"IO_RF", 	IO_RF_TASK_STACK_SIZE,	NULL, 1, _iorfTaskStack, 	&_iorfTaskObj);


//	xTaskCreateStatic(CALIBRATION_task, "CALIBRATION", CALIBRATION_TASK_STACK_SIZE, NULL, 1, _CALIBRATIONTaskStack, &_CALIBRATIONTaskObj);

	IO_RF_Init();
	IMU_Init();
	HAL_Delay(300);

//	HAL_InitTick(15);


	vTaskStartScheduler();

	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

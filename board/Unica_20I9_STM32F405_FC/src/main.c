#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_cortex.h>
#include <stm32f4xx_hal_i2c.h>
#include <stm32f4xx_hal_usart.h>
#include <stm32f4xx_hal_dma.h>
#include <stm32f4xx_hal_gpio.h>


#include "diag/Trace.h"
#include <FreeRTOS.h>
#include "task.h"


#include "state.h"
#include "Servo.h"

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

#define fl float
#define u32 uint32_t

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC optimize("Ofast,unroll-all-loops")
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

//some constants
const fl angleMin = -30;
const fl angleMax = 60;

void speedRot(fl speed, servo_id_t servo, int flag) {
	if (!flag) {
		for (fl i = 0; i >= angleMin; i -= speed) {
			servoRotate(servo, i);
			HAL_delay(10);
		}
		for (fl i = angleMin; i <= angleMax; i += speed) {
			servoRotate(servo, i);
			HAL_delay(10);
		}
	} else {
		for (fl i = 0; i <= angleMax; i += speed) {
			servoRotate(servo, i);
			HAL_delay(10);
		}
		for (fl i = angleMax; i >= angleMin; i -= speed) {
			servoRotate(servo, i);
			HAL_delay(10);
		}
	}
	servoRotate(servo, 0);
	return;
}

// глобальные структуры
stateSensors_raw_t 	stateSensors_raw;
stateGPS_t 			stateGPS;
stateIMU_rsc_t 		stateIMU_rsc;
stateIMU_isc_t 		stateIMU_isc;
stateSensors_t 		stateSensors;
stateCamera_orient_t stateCamera_orient;
state_system_t 		state_system;
state_zero_t		state_zero;

stateIMU_isc_t		stateIMU_isc_prev;
stateSensors_t		stateSensors_prev;
state_system_t		state_system_prev;
stateCamera_orient_t stateCamera_orient_prev;


//	параметры IO_RF_task
/*#define IO_RF_TASK_STACK_SIZE (50*configMINIMAL_STACK_SIZE)
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

//	параметры MOTORS_task
#define MOTORS_TASK_STACK_SIZE (40*configMINIMAL_STACK_SIZE)
static StackType_t	_MOTORSTaskStack[MOTORS_TASK_STACK_SIZE];
static StaticTask_t	_MOTORSTaskObj;


#define CALIBRATION_TASK_STACK_SIZE (20*configMINIMAL_STACK_SIZE)
static StackType_t	_CALIBRATIONTaskStack[MOTORS_TASK_STACK_SIZE];
static StaticTask_t	_CALIBRATIONTaskObj;*/


int main(int argc, char* argv[])
{
	// Инициализация структур глобального состояния (в нашем случае просто заполняем их нулями)
	/*memset(&stateSensors_raw, 	0x00, sizeof(stateSensors_raw));
	memset(&stateGPS, 			0x00, sizeof(stateGPS));
	memset(&stateIMU_rsc, 		0x00, sizeof(stateIMU_rsc));
	memset(&stateIMU_isc, 		0x00, sizeof(stateIMU_isc));
	memset(&stateSensors, 		0x00, sizeof(stateSensors));
	memset(&stateCamera_orient, 0x00, sizeof(stateCamera_orient));
	memset(&state_system, 		0x00, sizeof(state_system));
	memset(&state_zero, 		0x00, sizeof(state_zero));

	memset(&stateIMU_isc_prev, 			0x00, sizeof(stateIMU_isc_prev));
	memset(&stateSensors_prev,			0x00, sizeof(stateSensors_prev));
	memset(&state_system_prev, 			0x00, sizeof(state_system_prev));
	memset(&stateCamera_orient_prev, 	0x00, sizeof(stateCamera_orient_prev));

	state_system.BMP_state = 255;
	state_system.GPS_state = 255;
	state_system.MOTOR_state = 255;
	state_system.MPU_state = 255;
	state_system.NRF_state = 255;
	state_system.SD_state = 255;

	xTaskCreateStatic(IMU_task, 	"IMU", 		IMU_TASK_STACK_SIZE, 	NULL, 1, _IMUTaskStack, 	&_IMUTaskObj);

	xTaskCreateStatic(IO_RF_task, 	"IO_RF", 	IO_RF_TASK_STACK_SIZE,	NULL, 1, _iorfTaskStack, 	&_iorfTaskObj);

	xTaskCreateStatic(MOTORS_task,	"MOTORS", 	MOTORS_TASK_STACK_SIZE, NULL, 1, _MOTORSTaskStack, 	&_MOTORSTaskObj);

	xTaskCreateStatic(GPS_task, 	"GPS", 		GPS_TASK_STACK_SIZE, 	NULL, 1, _gpsTaskStack, 	&_gpsTaskObj);


//	xTaskCreateStatic(CALIBRATION_task, "CALIBRATION", CALIBRATION_TASK_STACK_SIZE, NULL, 1, _CALIBRATIONTaskStack, &_CALIBRATIONTaskObj);

	IO_RF_Init();
	IMU_Init();
	MOTORS_Init();
	GPS_Init();
	HAL_Delay(300);

//	HAL_InitTick(15);


	vTaskStartScheduler();
	*/

	//Инициализация сервоприводов
	allServosInit();
	uint32_t pulse = 0;
	int i = 0;
	while(1)
	{
		//_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, 5200);
		//trace_printf("pulse = %d\n", pulse);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_Delay(100);

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_Delay(100);
		for (pulse = 2500; pulse < 7000; pulse += 100)
		{
			_timerPWMChangePulse(&htimServo, TIM_CHANNEL_1, pulse);
			trace_printf("pulse = %d\n", pulse);
			HAL_Delay(100);
		}
		//write tasks on wednesday
    }

}





#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

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
#include "task.h"
#include "queue.h"

#include "state.h"
#include "tasks/servo_task.h"
#include "tasks/exchange_task.h"
#include "tasks/control_task.h"
#include "tasks/SoAR_task.h"

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
state_system_t 		state_system;
state_zero_t		state_zero;
state_master_t		state_master;
state_system_t		state_system_prev;
FC_logs_t			FC_logs;
Predictor_Angles_t	Predictor_Angles;

QueueHandle_t handleInternalCmdQueue;
TaskHandle_t handleControlTask;
TaskHandle_t handleSoARTask;

//servo
servo_task_param_t servo_param_left, servo_param_right, servo_param_keel;
TaskHandle_t handleLeft, handleRight, handleKeel;
state_servo_t		stateServo;

#define EXCHANGE_TASK_STACK_SIZE (50*configMINIMAL_STACK_SIZE)
static StackType_t	_exchangeTaskStack[EXCHANGE_TASK_STACK_SIZE];
static StaticTask_t	_exchangeTaskObj;


#define CONTROL_TASK_STACK_SIZE (10*configMINIMAL_STACK_SIZE)
static StackType_t	_controlTaskStack[CONTROL_TASK_STACK_SIZE];
static StaticTask_t	_controlTaskObj;

#define SoAR_TASK_STACK_SIZE (100*configMINIMAL_STACK_SIZE)
static StackType_t	_SoARTaskStack[SoAR_TASK_STACK_SIZE];
static StaticTask_t	_SoARTaskObj;

//	параметры SERVO_task
#define SERVO_TASK_STACK_SIZE (10*configMINIMAL_STACK_SIZE)
static StackType_t	_servoTaskStack[SERVO_TASK_STACK_SIZE];
static StaticTask_t	_servoTaskObj;

static StackType_t	_servoTaskStackLeft[SERVO_TASK_STACK_SIZE];
static StaticTask_t	_servoTaskObjLeft;

static StackType_t	_servoTaskStackRight[SERVO_TASK_STACK_SIZE];
static StaticTask_t	_servoTaskObjRight;

static StackType_t	_servoTaskStackKeel[SERVO_TASK_STACK_SIZE];
static StaticTask_t	_servoTaskObjKeel;

#define INTERNAL_QUEUE_LENGHT  sizeof( uint8_t )
#define INTERNAL_QUEUE_ITEM_SIZE  5
uint8_t internal_queue_storage_area[INTERNAL_QUEUE_LENGHT * INTERNAL_QUEUE_ITEM_SIZE];
static StaticQueue_t internal_queue_static;


// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)


void Init_led(){
	GPIO_InitTypeDef gpioc;
	gpioc.Mode = GPIO_MODE_OUTPUT_PP;
	gpioc.Pin = GPIO_PIN_12;
	gpioc.Pull = GPIO_NOPULL;
	gpioc.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOC, &gpioc);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, SET);
}

void led(){
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_12))
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, RESET);
	else
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, SET);
}


signed main(int argc, char* argv[]) {
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// Инициализация структур глобального состояния (в нашем случае просто заполняем их нулями)
	memset(&state_system, 			0x00, sizeof(state_system));
	memset(&state_zero, 			0x00, sizeof(state_zero));
	memset(&state_master, 	        0x00, sizeof(state_master));
	memset(&FC_logs, 				0x00, sizeof(FC_logs));
	memset(&state_system_prev, 		0x00, sizeof(state_system_prev));


	xTaskCreateStatic(EXCHANGE_task, 	"EXCHANGE", 	EXCHANGE_TASK_STACK_SIZE, 	NULL, 3, _exchangeTaskStack, 	&_exchangeTaskObj);
//	handleControlTask = xTaskCreateStatic(CONTROL_task, "CONTROL", CONTROL_TASK_STACK_SIZE, NULL, 2, _controlTaskStack, &_controlTaskObj);
	handleSoARTask = xTaskCreateStatic(SoAR_task, "SoAR", SoAR_TASK_STACK_SIZE, NULL, 1, _SoARTaskStack, &_SoARTaskObj);

	handleInternalCmdQueue = xQueueCreateStatic(INTERNAL_QUEUE_LENGHT, INTERNAL_QUEUE_ITEM_SIZE, internal_queue_storage_area, &internal_queue_static);

	/*xTaskCreateStatic(SCHEDULE_SERVO_task, "SERVO", SERVO_TASK_STACK_SIZE, NULL, 1, _servoTaskStack, &_servoTaskObj);
	handleLeft = xTaskCreateStatic(speedRot, "left", SERVO_TASK_STACK_SIZE, &servo_param_left, 2, _servoTaskStackLeft, &_servoTaskObjLeft);
	handleRight = xTaskCreateStatic(speedRot, "right", SERVO_TASK_STACK_SIZE, &servo_param_right, 2, _servoTaskStackRight, &_servoTaskObjRight);
	handleKeel = xTaskCreateStatic(speedRot, "keel", SERVO_TASK_STACK_SIZE, &servo_param_keel, 2, _servoTaskStackKeel, &_servoTaskObjKeel);
	servo_param_left.handle = handleLeft;
	servo_param_right.handle = handleRight;
	servo_param_keel.handle = handleKeel;
	servo_param_left.id = 0;
	servo_param_right.id = 1;
	servo_param_keel.id = 2;
	 */

	allServosInit();
	init_EX();
	Init_led();


	HAL_Delay(300);

	__enable_irq();

	HAL_InitTick(15);
	vTaskStartScheduler();

	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

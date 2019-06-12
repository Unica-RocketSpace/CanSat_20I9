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
#include "Servo.h"
#include "tasks/exchange_task.h"

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

QueueHandle_t handleInternalCmdQueue;


//	параметры IO_RF_task
#define EXCHANGE_TASK_STACK_SIZE (50*configMINIMAL_STACK_SIZE)
static StackType_t	_exchangeTaskStack[EXCHANGE_TASK_STACK_SIZE];
static StaticTask_t	_exchangeTaskObj;

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


int main(int argc, char* argv[]) {
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// Инициализация структур глобального состояния (в нашем случае просто заполняем их нулями)
	memset(&state_system, 			0x00, sizeof(state_system));
	memset(&state_zero, 			0x00, sizeof(state_zero));
	memset(&state_master, 			0x00, sizeof(state_master));
	memset(&FC_logs, 				0x00, sizeof(FC_logs));
	memset(&state_system_prev, 		0x00, sizeof(state_system_prev));


	xTaskCreateStatic(EXCHANGE_task, 	"EXCHANGE", 	EXCHANGE_TASK_STACK_SIZE, 	NULL, 3, _exchangeTaskStack, 	&_exchangeTaskObj);

	handleInternalCmdQueue = xQueueCreateStatic(INTERNAL_QUEUE_LENGHT, INTERNAL_QUEUE_ITEM_SIZE, internal_queue_storage_area, &internal_queue_static);


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

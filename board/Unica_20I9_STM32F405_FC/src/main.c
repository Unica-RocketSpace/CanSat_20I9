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



//	параметры IO_RF_task
#define EXCHANGE_TASK_STACK_SIZE (50*configMINIMAL_STACK_SIZE)
static StackType_t	_exchangeTaskStack[EXCHANGE_TASK_STACK_SIZE];
static StaticTask_t	_exchangeTaskObj;




int main(int argc, char* argv[]) {
	// Инициализация структур глобального состояния (в нашем случае просто заполняем их нулями)
	memset(&state_system, 		0x00, sizeof(state_system));
	memset(&state_zero, 		0x00, sizeof(state_zero));

	memset(&state_system_prev, 			0x00, sizeof(state_system_prev));



	xTaskCreateStatic(EXCHANGE_task, 	"EXCHANGE", 	EXCHANGE_TASK_STACK_SIZE, 	NULL, 3, _exchangeTaskStack, 	&_exchangeTaskObj);


	init_exchange_command_UART();
	init_exchange_data_UART();

	//Включение прерывания USART: RXNE
	USART3->CR1 |= USART_CR1_RXNEIE;
	HAL_NVIC_SetPriority(USART3_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);

	//Включение прерывания USART: RXNE
	USART1->CR1 |= USART_CR1_RXNEIE;
	HAL_NVIC_SetPriority(USART1_IRQn, 6, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	HAL_Delay(300);

	__enable_irq();


	HAL_InitTick(15);

	vTaskStartScheduler();

	return 0;
}





#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

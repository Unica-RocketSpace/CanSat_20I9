/*
 * control_task.c
 *
 *  Created on: 24 нояб. 2018 г.
 *      Author: developer
 */
#include <tasks/control_task.h>
#include <stdint.h>
#include <diag/Trace.h>
#include <stm32f4xx_hal_gpio.h>
#include "stm32f4xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "state.h"

GPIO_InitTypeDef engine_pin;


int8_t global_command;
uint8_t global_stage;
float height;
uint8_t button;
uint8_t command;


void init_pins(){
	engine_pin.Mode = GPIO_MODE_OUTPUT_PP;
	engine_pin.Pin = ENGINE_PIN_GOL | ENGINE_PIN_GOR | ENGINE_PIN_KEEL | ENGINE_PIN_WL | ENGINE_PIN_WR | DEPLOY_PARACHUTE_PIN;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(ENGINE_PORT, &engine_pin);

	engine_pin.Mode = GPIO_MODE_INPUT;
	engine_pin.Pin = PHOTORES_PIN;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(PHOTORES_PORT, &engine_pin);

	engine_pin.Mode = GPIO_MODE_INPUT;
	engine_pin.Pin = BUTTON_PIN_GOR | BUTTON_PIN_GOL;
	engine_pin.Pull = GPIO_PULLUP;
	engine_pin.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(BUTTON_PORT, &engine_pin);

	engine_pin.Mode = GPIO_MODE_INPUT;
	engine_pin.Pin = BUTTON_PIN_KEEL | BUTTON_PIN_WL | BUTTON_PIN_WR;
	engine_pin.Pull = GPIO_PULLUP;
	engine_pin.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(BUTTON_PORT_2, &engine_pin);

/*
	__USART1_CLK_ENABLE();
	engine_pin.Alternate = GPIO_AF7_USART1;
	engine_pin.Mode = GPIO_MODE_AF_OD;
	engine_pin.Pin = GPIO_PIN_10;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &engine_pin);

	engine_pin.Alternate = GPIO_AF7_USART1;
	engine_pin.Mode = GPIO_MODE_AF_PP;
	engine_pin.Pin = GPIO_PIN_9;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &engine_pin);

	__USART3_CLK_ENABLE();
	engine_pin.Alternate = GPIO_AF7_USART3;
	engine_pin.Mode = GPIO_MODE_AF_OD;
	engine_pin.Pin = GPIO_PIN_11;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &engine_pin);

	engine_pin.Alternate = GPIO_AF7_USART3;
	engine_pin.Mode = GPIO_MODE_AF_PP;
	engine_pin.Pin = GPIO_PIN_10;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOC, &engine_pin);
*/
}


void deploy_parachute(){
	HAL_GPIO_WritePin(DEPLOY_PARACHUTE_PORT, DEPLOY_PARACHUTE_PIN, GPIO_PIN_SET);
	vTaskDelay(3000/portTICK_RATE_MS);
	HAL_GPIO_WritePin(DEPLOY_PARACHUTE_PORT, DEPLOY_PARACHUTE_PIN, GPIO_PIN_RESET);
}

void set_engines(){
	HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_SET);
	HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_SET);
}

void reset_engines(){
	HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_RESET);
}


void CONTROL_task() {

	for(;;){

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		trace_printf("C");

		taskENTER_CRITICAL();
		global_stage = state_system.globalStage;
		global_command = state_system.globalCommand;
		height = stateIMUSensors.height;
		button = state_system.buttons;
		taskEXIT_CRITICAL();

		trace_printf("stage %d\n", global_stage);



//		if (global_command != -1){}


//		FIXME: нужен ли он здесь?????
//		if (global_stage == 0){

//		}
		switch (global_stage){
//			case 0:
//				xQueueSendToBack(handleInternalCmdQueue, &command, 0);
//				break;

			case 1:
				command = 3;
				xQueueSendToBack(handleInternalCmdQueue, &command, 0);
				break;

			case 3:
				if (HAL_GPIO_ReadPin(PHOTORES_PORT, PHOTORES_PIN)){
					taskENTER_CRITICAL();
					state_system.globalStage = 4;
					taskEXIT_CRITICAL();
				}
				break;

			case 4:
				if (height <= HEIGHT_TO_DEPLOY_PARACHUTE){
					taskENTER_CRITICAL();
					deploy_parachute();
					state_system.globalStage = 5;
					taskEXIT_CRITICAL();
				}
				break;

			case 5:
//				trace_printf("buttons %d\n", button);
				switch (button){
				//ждем прерывания от крыльев и стабилизаторов
					/*case 4:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WL, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_WL)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WL, GPIO_PIN_RESET);
						}
						break;

					case 5:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WR, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_WR)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WR, GPIO_PIN_RESET);
						}
						break;*/

					case 0:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_GOL)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_RESET);
						}
						break;

					case 1:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_GOR)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_RESET);
						}
						break;

					case 2:
						vTaskDelay(100/portTICK_RATE_MS);
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_KEEL)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_RESET);
						}
						break;

					case 3:
						taskENTER_CRITICAL();
						state_system.globalStage = 6;
						taskEXIT_CRITICAL();
						break;
				}
				break;

			case 6:
				//unhook_parachute
				//тук-тук к мастеру

	//			Определяем неизменность высоты
	/*			if (){
					task_ENTER_CRITICAL();
					state_system.globalStage = 6;
					task_EXIT_CRITICAL();
					global_stage = 6;
				}
	*/			break;

			case 7:
				//Переходим в режим ожидания
				break;
		}

		vTaskDelay(150 / portTICK_RATE_MS);

	}
}




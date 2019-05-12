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
uint8_t global_stage = 0;
float height;
uint8_t buttons = 0;

uint8_t command;


void init_pins(){
	engine_pin.Mode = GPIO_MODE_OUTPUT_PP;
	engine_pin.Pin = ENGINE_PIN_GOL | ENGINE_PIN_GOR | ENGINE_PIN_KEEL | ENGINE_PIN_WL | ENGINE_PIN_WR | DEPLOY_PARACHUTE_PIN;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(ENGINE_PORT, &engine_pin);

	engine_pin.Mode = GPIO_MODE_OUTPUT_PP;
	engine_pin.Pin = PHOTORES_PIN;
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(PHOTORES_PORT, &engine_pin);

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

		init_pins();
		taskENTER_CRITICAL();
		global_stage = state_system.globalStage;
		global_command = state_system.globalCommand;
		height = stateIMUSensors.height;
//		buttons = state_system.buttons;
		taskEXIT_CRITICAL();



		if (global_command != -1){
		}


//		FIXME: нужен ли он здесь?????
//		if (global_stage == 0){
//			xQueueSendToBack(handleInternalCmdQueue, &command, 0);
//		}

		switch (global_stage){
			case 0:
				vTaskDelay(1000/portTICK_RATE_MS);
				set_engines();
				vTaskDelay(1000/portTICK_RATE_MS);
				reset_engines();
				break;

			case 2:
//				FIXME: инициализировать фотосенсор(а надо ли?)
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
					state_system.globalStage = 4;
					taskEXIT_CRITICAL();
				}
				break;

			case 5:



				//ждем прерывания от крыльев и стабилизаторов
				if (buttons == ALL_BUTTONS_WORKED){
					taskENTER_CRITICAL();
					state_system.globalStage = 5;
					taskEXIT_CRITICAL();
					global_stage = 5;
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




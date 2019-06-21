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
float height_now, height_prev;
uint8_t button;
uint8_t command;
uint8_t master;


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
//		trace_printf("C");

		taskENTER_CRITICAL();
		global_stage = state_system.globalStage;
		global_command = state_system.globalCommand;
		height_now = stateIMUSensors.height;
		height_prev = stateIMUSensors_prev.height;
		button = state_system.buttons;
		master = state_system.master_state;
		taskEXIT_CRITICAL();

//		trace_printf("mas_state %d\n", master);

//		trace_printf("stage %d\n", global_stage);


		switch (global_stage){
			case 0:
				if (master != 4)
					xQueueSendToBack(handleInternalCmdQueue, (uint8_t*)2, 10);
				//				xQueueSendToBack(handleInternalCmdQueue, &global_command, 0);
				break;

			case 3:
				if (HAL_GPIO_ReadPin(PHOTORES_PORT, PHOTORES_PIN)){
					taskENTER_CRITICAL();
					state_system.globalStage = 4;
					taskEXIT_CRITICAL();
//				trace_printf("PHOTORES %d\n", (HAL_GPIO_ReadPin(PHOTORES_PORT, PHOTORES_PIN)));
				}
				break;

			case 4:
				if (height_now <= HEIGHT_TO_DEPLOY_PARACHUTE){
					taskENTER_CRITICAL();
					deploy_parachute();
					state_system.globalStage = 5;
					taskEXIT_CRITICAL();
				}
				break;

			case 5:
				trace_printf("buttons %d\n", button);
				switch (button){
				//ждем прерывания от крыльев и стабилизаторов
					case 0:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WL, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_WL)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WL, GPIO_PIN_RESET);
						}
						break;

					case 1:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WR, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_WR)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WR, GPIO_PIN_RESET);
						}
						break;

					case 2:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_GOL)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_RESET);
						}
						break;

					case 3:
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_GOR)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_RESET);
						}
						break;

					case 4:
						vTaskDelay(100/portTICK_RATE_MS);
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_KEEL)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_RESET);
						}
						break;

					case 5:
						taskENTER_CRITICAL();
						state_system.globalStage = 6;
						taskEXIT_CRITICAL();
						break;
				}
				break;

			case 6:
				//тук-тук к мастеру
				xQueueSendToBack(&handleInternalCmdQueue, (uint8_t*)COMMAND_START, 0);
				vTaskDelay(30);

				//Определяем неизменность высоты
				if ((height_now + DELTA_HEIGHT <= height_prev) || (height_now - DELTA_HEIGHT <= height_prev)){
					taskENTER_CRITICAL();
					state_system.globalStage = 7;
					taskEXIT_CRITICAL();
				}
				break;

			case 7:
				//Переходим в режим ожидания
				xQueueSendToBack(&handleInternalCmdQueue, (uint8_t*)COMMAND_SLEEP_MODE, 0);
				break;
		}

		vTaskDelay(150 / portTICK_RATE_MS);

	}
}




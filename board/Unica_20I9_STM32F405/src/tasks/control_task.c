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
float start_tick = 0;
float delta = 5000.0;
uint8_t count = 1;


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
	engine_pin.Pull = GPIO_NOPULL;
	engine_pin.Speed = GPIO_SPEED_MEDIUM;
	HAL_GPIO_Init(BUTTON_PORT, &engine_pin);

	engine_pin.Mode = GPIO_MODE_INPUT;
	engine_pin.Pin = BUTTON_PIN_KEEL | BUTTON_PIN_WL | BUTTON_PIN_WR;
	engine_pin.Pull = GPIO_NOPULL;
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
		trace_printf("C");

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
				if (master != 4){
					uint8_t tmp = COMMAND_TEST;
					xQueueSendToBack(handleInternalCmdQueue, &tmp, 10);
				}
				//				xQueueSendToBack(handleInternalCmdQueue, &global_command, 0);
				break;

			case 3:
//				if (HAL_GPIO_ReadPin(PHOTORES_PORT, PHOTORES_PIN)){
					taskENTER_CRITICAL();
					state_system.globalStage = 4;
					taskEXIT_CRITICAL();
//				}
				break;

			case 4:
				if (height_now <= HEIGHT_TO_DEPLOY_PARACHUTE){
					HAL_GPIO_WritePin(DEPLOY_PARACHUTE_PORT, DEPLOY_PARACHUTE_PIN, GPIO_PIN_SET);
					vTaskDelay(6000/portTICK_RATE_MS);
					HAL_GPIO_WritePin(DEPLOY_PARACHUTE_PORT, DEPLOY_PARACHUTE_PIN, GPIO_PIN_RESET);

					taskENTER_CRITICAL();
					state_system.globalStage = 5;
					taskEXIT_CRITICAL();
				}
				break;

			case 5:
				trace_printf("buttons %d\n", button);
				switch (button){
				//ждем прерывания от крыльев и стабилизаторов
					case 0:
						if (count){
							vTaskDelay(2000 / portTICK_RATE_MS);
							start_tick = (float)HAL_GetTick() + 2000.0;
							count = 0;
						}

						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WL, GPIO_PIN_SET);
//						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_WL)){
//							taskENTER_CRITICAL();
//							state_system.buttons = (++button);
//							taskEXIT_CRITICAL();
//							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WL, GPIO_PIN_RESET);
//							count = 1;
//						}
						if (start_tick <= (float)HAL_GetTick()){
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WL, GPIO_PIN_RESET);
							count = 1;
							taskENTER_CRITICAL();
//							state_system.globalStage = 1;
							state_system.buttons = 1;
							taskEXIT_CRITICAL();
						}
						break;

					case 1:
						if (count){
							start_tick = (float)HAL_GetTick() + delta;
							count = 0;
						}

						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WR, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_WR)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WR, GPIO_PIN_RESET);
							count = 1;
						}
						else if (start_tick <= (float)HAL_GetTick()){
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_WR, GPIO_PIN_RESET);
							count = 1;
							taskENTER_CRITICAL();
							state_system.globalStage = 1;
							taskEXIT_CRITICAL();
						}
						break;

					case 2:
						if (count){
							start_tick = (float)HAL_GetTick() + 400.0;
							count = 0;
						}

						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_SET);
//						if (!HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_GOL)){
//							taskENTER_CRITICAL();
//							state_system.buttons = (++button);
//							taskEXIT_CRITICAL();
//							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_RESET);
//							vTaskDelay(20 / portTICK_RATE_MS);
//							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_SET);
//							vTaskDelay(7 / portTICK_RATE_MS);
//							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_RESET);
//							count = 1;
//						}
						if (start_tick <= (float)HAL_GetTick()){
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOL, GPIO_PIN_RESET);
							count = 1;
							taskENTER_CRITICAL();
//							state_system.globalStage = 1;
							state_system.buttons++;
							taskEXIT_CRITICAL();
						}
						break;

					case 3:
						if (count){
							start_tick = (float)HAL_GetTick() + delta;
							count = 0;
						}

						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN_GOR)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							vTaskDelay(400 / portTICK_RATE_MS);
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_RESET);
							count = 1;
						}
						else if (start_tick <= (float)HAL_GetTick()){
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_GOR, GPIO_PIN_RESET);
							count = 1;
							taskENTER_CRITICAL();
							state_system.globalStage = 1;
//							state_system.buttons++;
							taskEXIT_CRITICAL();
						}
						break;

					case 4:
						if (count){
							start_tick = (float)HAL_GetTick() + delta;
							count = 0;
						}

//						vTaskDelay(100/portTICK_RATE_MS);
						HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_SET);
						if (!HAL_GPIO_ReadPin(BUTTON_PORT_2, BUTTON_PIN_KEEL)){
							taskENTER_CRITICAL();
							state_system.buttons = (++button);
							taskEXIT_CRITICAL();
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_RESET);
							count = 1;
						}
						else if (start_tick <= (float)HAL_GetTick()){
							HAL_GPIO_WritePin(ENGINE_PORT, ENGINE_PIN_KEEL, GPIO_PIN_RESET);
							count = 1;
							taskENTER_CRITICAL();
							state_system.globalStage = 1;
//							state_system.buttons++;
							taskEXIT_CRITICAL();
						}
						break;

					case 5:
						taskENTER_CRITICAL();
						state_system.globalStage = 6;
						state_system.master_state = 0;
						taskEXIT_CRITICAL();
						break;
				}
				break;

			case 6:
				//тук-тук к мастеру
				if (master != 4){
					uint8_t tmp = COMMAND_START;
					xQueueSendToBack(handleInternalCmdQueue, &tmp, 10);
//					vTaskDelay(10);
				}
//				vTaskDelay(30);

				//Определяем неизменность высоты
//				if ((height_now + DELTA_HEIGHT >= height_prev) & (height_now - DELTA_HEIGHT <= height_prev)){
//					taskENTER_CRITICAL();
//					state_system.globalStage = 7;
//					taskEXIT_CRITICAL();
//				}
				break;

			case 7:
				//Переходим в режим ожидания
				while(master != 4){
					uint8_t tmp = COMMAND_SLEEP_MODE;
					xQueueSendToBack(handleInternalCmdQueue, &tmp, 10);
				}
				break;
		}

		vTaskDelay(50 / portTICK_RATE_MS);

	}
}




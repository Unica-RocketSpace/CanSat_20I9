/*
 * control_task.c
 *
 *  Created on: 24 нояб. 2018 г.
 *      Author: developer
 */
#include <tasks/control_task.h>
#include <stdint.h>
#include <diag/Trace.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "state.h"


#define HEIGHT_TO_DEPLOY_PARACHUTE 270
#define ALL_BUTTONS_WORKED 255
#define DELTA_HEIGHT 0.2

int8_t global_command;
uint8_t global_stage = 0;
float height;
uint8_t buttons = 0;

uint8_t command;


void CONTROL_task() {


	for(;;){

		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		trace_printf("CONTROL_task");

		taskENTER_CRITICAL();
		global_stage = state_system.globalStage;
		global_command = state_system.globalCommand;
		height = stateIMUSensors.height;
		buttons = state_system.buttons;
		taskEXIT_CRITICAL();



		if (global_command != -1){
			if (global_command == 0){
				taskENTER_CRITICAL();
				state_system.globalCommand = -1;
				state_system.globalStage = 0;
				taskEXIT_CRITICAL();
				global_stage = 0;
			}
			if (global_command == 1){
				taskENTER_CRITICAL();
				state_system.globalCommand = -1;
				state_system.globalStage = 1;
				taskEXIT_CRITICAL();
				global_stage = 1;
			}
			if (global_command == 2){
				taskENTER_CRITICAL();
				state_system.globalCommand = -1;
				state_system.globalStage = 2;
				taskEXIT_CRITICAL();
				global_stage = 2;
			}
			if (global_command == 3){
				taskENTER_CRITICAL();
				state_system.globalCommand = -1;
				state_system.globalStage = 3;
				taskEXIT_CRITICAL();
				global_stage = 3;
			}
			if (global_command == 4){
				taskENTER_CRITICAL();
				state_system.globalCommand = -1;
				state_system.globalStage = 4;
				taskEXIT_CRITICAL();
				global_stage = 4;
			}
		}



		if (global_stage == 0){
			xQueueSendToBack(handleInternalCmdQueue, &command, 0);

		}
		if (global_stage == 1){
			//проверка на затемнение фотосенсора
//			if (/* фотосенсор затемнился*/){
//				vTaskDelay(1000);
//				task_ENTER_CRITICAL();
//				state_system.globalStage = 2;
//				task_EXIT_CRITICAL();
//				global_stage = 2;
//			}

		}
		if (global_stage == 2){

			//начальное состояние

		}
		if (global_stage == 3){
			if (height <= HEIGHT_TO_DEPLOY_PARACHUTE){
				taskENTER_CRITICAL();
				state_system.globalStage = 4;
				taskEXIT_CRITICAL();
				global_stage = 4;

			}
		}
		if (global_stage == 4){
			//deploy parachute
			//ждем прерывания от крыльев и стабилизаторов
			if (buttons == ALL_BUTTONS_WORKED){
				taskENTER_CRITICAL();
				state_system.globalStage = 5;
				taskEXIT_CRITICAL();
				global_stage = 5;
			}
		}
		if (global_stage == 5){
			//unhook_parachute
			//тук-тук к мастеру

			//Определяем неизменность высоты
//			if (){
//				task_ENTER_CRITICAL();
//				state_system.globalStage = 6;
//				task_EXIT_CRITICAL();
//				global_stage = 6;
//			}
		}
		if (global_stage == 6){
			//Переходим в режим ожидания

		}


	}

}




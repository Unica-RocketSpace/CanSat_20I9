/*
 * control_task.c
 *
 *  Created on: 24 нояб. 2018 г.
 *      Author: developer
 */

uint8_t global_stage;


void CONTROL_task() {

	for(;;){

		ulTaskNotifyTake(1);
		trace_printf("CONTROL_task");
		vTaskDelay(1000);

		taskENTER_CRITICAL();
		global_stage = state_system.globalStage;
		TaskEXIT_CRITICAL();

		if (global_stage == 0){

		}


	}
	vTaskDelay(1000);


}




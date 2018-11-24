/*
 * control_task.c
 *
 *  Created on: 24 нояб. 2018 г.
 *      Author: developer
 */

void CONTROL_task() {

	for(;;){
		ulTaskNotifyTake(1);
		trace_printf("CONTROL_task");
		vTaskDelay(1000);

	}
	vTaskDelay(1000);


}




/*
 * exchange_task.c
 *
 *  Created on: 8 дек. 2018 г.
 *      Author: developer
 */

#include <stdint.h>

#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"

#include "state.h"


uint8_t internal_cmd;
portBASE_TYPE internal_queue_status;

void EXCHANGE_task (){

	for(;;){
//		if (uxQueueMessagesWaiting( handleInternalCmdQueue) != 0){
//		}

		if (uxQueueMessagesWaiting(handleInternalCmdQueue) != 0){
			internal_queue_status = xQueueReceive(handleInternalCmdQueue, &internal_cmd, portMAX_DELAY);
		}



	}
}





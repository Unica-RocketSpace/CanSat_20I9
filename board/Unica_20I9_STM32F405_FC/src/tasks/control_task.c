#include "state.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "predictor_module.h"

#define ui8 uint8_t

void CONTROL_task(void){
	for(;;){
		ui8 tmp = COMMAND_DATA;
		xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);
		direction_predictor();
		vTaskDelay(5000 / portTICK_RATE_MS);
	}
	return;
}

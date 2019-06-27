#include "state.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "predictor_module.h"
#include <math.h>

#define ui8 uint8_t
#define deg * M_PI / 180;

void CONTROL_task(void){
	for(;;){
		ui8 tmp = COMMAND_DATA;
		xQueueSendToBack(handleInternalCmdQueue, &tmp, 5);
		Predictor_Angles.alpha = 10 deg;
		Predictor_Angles.beta = 0.0;
		vTaskDelay(1000000 / portTICK_RATE_MS);
	}
	return;
}

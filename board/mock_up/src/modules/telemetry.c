/*
 * TELEMETRY_UNIT
 *
 * 		This unit will be used for transmitting and receiving data
 * 	with device and earth using radio transmitter.
 *
 * 	Authors: Korr237i, RaKetov
 */

#include "stdint.h"

#include "diag/Trace.h"
#include "FreeRTOS.h"
#include "task.h"

#include "state.h"
#include "telemetry.h"
#include "drivers/sd/sd.h"
#include "drivers/sd/dump.h"


uint8_t UNISAT_ID = 0x01;
uint8_t UNISAT_NoComp = 0xFF;
uint8_t UNISAT_IMU = 0x02;
uint8_t UNISAT_SENSORS = 0x03;
uint8_t UNISAT_GPS = 0x04;
uint8_t UNISAT_RPI = 0x05;
uint8_t UNISAT_CAM = 0x06;

static dump_channel_state_t stream_file;

/*
void IO_RF_Init() {

	uint8_t nRF24L01_initError = nRF24L01_init(&spi_nRF24L01);
	state_system.NRF_state = nRF24L01_initError;
	HAL_Delay(100);

	stream_file.res = 1;
	//	запуск SD
	stream_file.file_opened = false;
	dump_init(&stream_file);
	state_system.SD_state = (uint8_t)stream_file.res;
	HAL_Delay(200);
}


void IO_RF_task() {

	for (;;) {

		vTaskDelay(100/portTICK_RATE_MS);


		// Этап 0. Подтверждение инициализации отправкой пакета состояния и ожидание ответа от НС
		if (state_system.globalStage == 0) {
		taskENTER_CRITICAL();
			if (state_system.globalCommand == 1)
				state_system.globalStage = 1;
		taskEXIT_CRITICAL();
		}

		// Этап 1. Определение начального состояния
		if (state_system.globalStage == 1) {
			mavlink_msg_state_zero_send();


		taskENTER_CRITICAL();
			if (state_system.globalCommand == 2)
				state_system.globalStage = 2;
		taskEXIT_CRITICAL();

		}
		// Этап 2. Полет в ракете
		if (state_system.globalStage == 2) {
		taskENTER_CRITICAL();
			if (state_system.globalCommand == 3)
				state_system.globalStage = 3;
		taskEXIT_CRITICAL();
		}
		// Этап 3. Свободное падение
		if (state_system.globalStage == 3) {
		taskENTER_CRITICAL();
			if (state_system.globalCommand == 4)
				state_system.globalStage = 4;
		taskEXIT_CRITICAL();
		}
		// Этап 4. Спуск
		if (state_system.globalStage == 4) {
		}
		// Этап 5. Окончание полета
		if (state_system.globalStage == 5) {
		}
	}
}
*/


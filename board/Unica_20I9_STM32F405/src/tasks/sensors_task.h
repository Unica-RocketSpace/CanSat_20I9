/*
 * sensors_task.h
 *
 *  Created on: 24 нояб. 2018 г.
 *      Author: developer
 */

#ifndef SENSORS_TASK_H_
#define SENSORS_TASK_H_

#include "state.h"

extern rscs_bmp280_descriptor_t * bmp280;
extern const rscs_bmp280_calibration_values_t * bmp280_calibration_values;

void IMU_Init();

/*
 * 	FreeRTOS задача для работы с IMU
 */
void SENSORS_task();




#endif /* SENSORS_TASK_H_ */

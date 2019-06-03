/*
 * telemetry.h
 *
 *  Created on: 11 нояб. 2017 г.
 *      Author: developer
 */

#ifndef TELEMETRY_H_
#define TELEMETRY_H_

#include <stdint.h>
#include "state.h"


void IO_RF_Init();
void IO_RF_task();

uint8_t UNISAT_ID;
uint8_t UNISAT_NoComp;
uint8_t UNISAT_IMU;
uint8_t UNISAT_SENSORS;
uint8_t UNISAT_GPS;
uint8_t UNISAT_RPI;
uint8_t UNISAT_CAM;

bool data;

#endif /* TELEMETRY_H_ */

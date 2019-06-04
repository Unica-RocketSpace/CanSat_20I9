/*
 * predictor_module.h
 *
 *  Created on: 4 июн. 2019 г.
 *      Author: developer
 */

#ifndef PREDICTOR_MODULE_H_
#define PREDICTOR_MODULE_H_

#include <stm32f4xx_hal.h>
#include "state.h"


void height_predictor(double x, double y, double z);
void straight_flight(double alpha);
bool check_tube_target(double a, double b, double c, double z, double x, bool flag);
void turn_flight(double z, double x, char turn);
void direction_predictor();
void calculating_distance_of_linear_further_motion(double R, double r, double vz, double vx, char turn);

#endif /* PREDICTOR_MODULE_H_ */

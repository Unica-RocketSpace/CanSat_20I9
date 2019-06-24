/*
 * quaternion.c
 *
 *  Created on: 01 нояб. 2017 г.
 *      Author: developer
 */

#include <stdint.h>
#include <string.h>
#include <math.h>
#include <diag/Trace.h>

#include "quaternion.h"

void quat_mult(float* quat, float k, float* res_quat) {
	for (uint8_t i = 0; i < 4; i++) {
		res_quat[i] = k * quat[i];
	}
}

void vect_mult(float* vect, float k, float* res_vect) {
	for (uint8_t i = 0; i < 4; i++) {
		res_vect[i] = k * vect[i];
	}
}

float quat_abs(float* quat) {
	float tmp = 0;
	for (int i = 0; i < 4; i++) {
		tmp += pow(quat[i], 2);
	}
	return sqrt(tmp);
}

float vect_abs(float* vect) {
	float tmp = 0;
	for (int i = 0; i < 3; i++) {
		tmp += pow(vect[i], 2);
	}
	return sqrt(tmp);
}

void quat_normalize(float* quat,  float* res_quat) {
	quat_mult(quat, 1 / quat_abs(quat), res_quat);
}

void vect_normalise(float* vect,  float* res_vect) {
	vect_mult(vect, 1 / vect_abs(vect), res_vect);
}

void quat_invert(float* quat, float* res_quat) {

	res_quat[0] = quat[0];
	for (int i = 1; i < 4; i++) {
		res_quat[i] = - quat[i];
	}

	float res_[4] = {0, 0, 0, 0};
	quat_normalize(res_quat, res_);
	for (int i = 0; i < 4; i++)
		res_quat[i] = res_[i];
}


void quat_mult_by_quat(float * a, float * b, float* res_quat) {

	float res[4] = {0, 0, 0, 0};

	res[0] = a[0] * b[0] - a[1] * b[1] - a[2] * b[2] - a[3] * b[3];
	res[1] = a[0] * b[1] + a[1] * b[0] + a[2] * b[3] - a[3] * b[2];
	res[2] = a[0] * b[2] - a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
	res[3] = a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];

	for (int i = 0; i < 4; i++)
		res_quat[i] = res[i];
}


void quat_mult_by_vect(float* a, float* b, float* res_quat) {

	float vectQuat[4] = {0, b[0], b[1], b[2]};			//	quat from vect
	float res_quat_local[4] = {0, 0, 0, 0};				//	normalised vector

	quat_mult_by_quat(a, vectQuat, res_quat_local);

	for (int i = 0; i < 4; i++)
		res_quat[i] = res_quat_local[i];
}


void vect_rotate(float* vect, float* quat, float* res_vect) {
	float res_vect_local[4] = {0, 0, 0, 0};
	float quat_n[4] = {0, 0, 0, 0};
	quat_normalize(quat, quat_n);

	float tmp[4] = {0, 0, 0, 0};
	quat_mult_by_vect(quat_n, vect, tmp);

	float inverted[4] = {0, 0, 0, 0};
	quat_invert(quat_n, inverted);

	quat_mult_by_quat(tmp, inverted, res_vect_local);
	for (int i = 0; i < 3; i++)
		res_vect[i] = res_vect_local[i+1];
}


Euler_angles_t quat_to_angles(float* quat){
	Euler_angles_t angles;

	angles.roll = (float)atan((double)((2 * (quat[0] * quat[1] + quat[2] * quat[3])) / (1 - 2 * (quat[1] * quat[1] + quat[2] * quat[2]))));

	angles.pitch = (float)asin((double)(2 * (quat[0] * quat[2] - quat[3] * quat[1])));

	angles.yaw = (float)atan((double)((2 * (quat[0] * quat[3] + quat[1] * quat[2])) / (1 - 2 * (quat[2] * quat[2] + quat[3] * quat[3]))));
//	trace_printf("roll 		%f	pitch		%f	yaw   %f\n---------------------------------------------------\n", angles.roll / M_PI * 180, angles.pitch / M_PI * 180, angles.yaw / M_PI * 180);
//	trace_printf("quat_0 %f	quat_1 %f quat_2 %f	quat_3 %f\n--------------------------------------------------\n", quat[0], quat[1], quat[2], quat[3]);
	return angles;
}



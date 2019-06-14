/*
 * quaternion.h
 *
 *  Created on: 01 нояб. 2017 г.
 *      Author: developer
 */

#ifndef MADGWICK_QUATERNION_H_
#define MADGWICK_QUATERNION_H_


typedef struct{
	float roll;
	float pitch;
	float yaw;
}Euler_angles_t;


void quat_mult(float* quat, float k, float* res_quat);

void vect_mult(float* vect, float k, float* res_vect);

float quat_abs(float* quat);

float vect_abs(float* vect);

void quat_normalize(float* quat,  float* res_quat);

void vect_normalise(float* vect,  float* res_vect);

void quat_invert(float* quat, float* res_quat);

void quat_mult_by_quat(float * a, float * b, float* res_quat);

void quat_mult_by_vect(float* a, float* b, float* res_vect);

void vect_mult_by_quat(float* a, float* b, float* res_vect);

void vect_rotate(float* vect, float* quat, float* res_vect);

Euler_angles_t quat_to_angles(float* quat);


#endif /* MADGWICK_QUATERNION_H_ */

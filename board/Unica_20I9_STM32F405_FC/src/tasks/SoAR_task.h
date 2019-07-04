/*
 * SoAR_task.h
 *
 *  Created on: 13 июн. 2019 г.
 *      Author: developer
 */

#ifndef SOAR_TASK_H_
#define SOAR_TASK_H_

#define k1 -34.78261
#define b1 4790.0
#define k2 34.78261
#define b2 4740.0
#define k3 35.2941
#define b3 5680.0

void servoRotate(servo_id_t servo_id, fl angle_rad);
void multiRot(fl angle_go_left, fl angle_go_right, fl angle_keel);
void SoAR_task(void);

#endif /* SOAR_TASK_H_ */

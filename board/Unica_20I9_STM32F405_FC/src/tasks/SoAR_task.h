/*
 * SoAR_task.h
 *
 *  Created on: 13 июн. 2019 г.
 *      Author: developer
 */

#ifndef SOAR_TASK_H_
#define SOAR_TASK_H_

#define k1 -36.69725
#define b1 5070.0
#define k2 38.24384
#define b2 5000.0
#define k3 40.0
#define b3 5820.0

void servoRotate(servo_id_t servo_id, fl angle_rad);
void multiRot(fl angle_go_left, fl angle_go_right, fl angle_keel);
void SoAR_task(void);

#endif /* SOAR_TASK_H_ */

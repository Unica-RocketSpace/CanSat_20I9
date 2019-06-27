/*
 * state.h
 *
 *  Created on: 11 нояб. 2017 г.
 *      Author: Korr237i
 *
 *  This header included main structure to share data from module to module
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_hal.h"

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_usart.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_dma.h"

#include <FreeRTOS.h>
#include "task.h"
#include "queue.h"

#define fl float
#define u32 uint32_t


// if error set value and go to end
#define PROCESS_ERROR(x) if (0 != (error = (x))) { goto end; }


#define COMMAND_SLEEP	 	0
#define COMMAND_START 		1
#define COMMAND_TEST 		2
#define COMMAND_DATA 		3
#define COMMAND_OK			4
#define COMMAND_LOGS		5


/*#################################################*/
/*################## СТРУКТУРЫ ###################*/
/*#################################################*/

/*
 * 	Структуры состояний аппарата
 */

//	system parameters
typedef struct {
	uint8_t EX_cmd_UART_state;
	uint8_t EX_logs_UART_state;

	uint8_t globalStage;	//	number of current global stage
	uint8_t globalCommand;

	float time;				//	current time
} state_system_t;


typedef struct {
	//	zero params; this fields should be filled when device started it`s work
	float zero_pressure;
	float zero_quaternion[4];
	float zero_GPS[3];
	float gyro_staticShift[3];
	float accel_staticShift[3];

} state_zero_t;


typedef struct {
	float coordinates[2];
	float quaternion[4];
	float accel_isc[3];
	float height;

	float speed_GPS;
	float speed_BMP;

	float roll;
	float pitch;
	float yaw;

	float omega;

	float course;

} state_master_t;


typedef enum {
	servo_go_left		= 0,
	servo_go_right		= 1,
	servo_keel			= 2

} servo_id_t;


typedef enum {
	no_error		= 0,
	driver_overheat	= -1

} error;


typedef struct {
	float time;
	uint8_t fc_stage;
	float angle_left;
	float angle_right;
	float angle_keel;
	uint8_t critical_angles;
} FC_logs_t;

typedef struct {
	float alpha;
	float beta; //угол по курсу
} Predictor_Angles_t;

void led();

typedef struct {
	servo_id_t id;
	float speed;
	float start_angle;
	float finish_angle;
	xTaskHandle handle;
} servo_task_param_t;

typedef struct {
	float angle_left;
	float angle_right;
	float angle_keel;
} state_servo_t;


/*##################################################*/
/*################### ПЕРЕМЕННЫЕ ###################*/
/*##################################################*/

extern TaskHandle_t handleControlTask;
extern TaskHandle_t handleSoARTask;
extern QueueHandle_t handleInternalCmdQueue;
extern uint8_t FLAG_SoAR;
extern Predictor_Angles_t Predictor_Angles;
extern TaskHandle_t handleLeft, handleRight, handleKeel;

// глобальные структуры
extern state_system_t 		state_system;
extern state_zero_t			state_zero;
extern state_master_t		state_master;
extern FC_logs_t			FC_logs;

extern state_system_t		state_system_prev;

extern TIM_HandleTypeDef htimServo;
extern 	servo_task_param_t servo_param_left, servo_param_right, servo_param_keel;
extern state_servo_t	stateServo;

#endif /* STATE_H_ */

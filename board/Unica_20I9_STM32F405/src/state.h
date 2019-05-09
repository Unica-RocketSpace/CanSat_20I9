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


#include "drivers/UNICS_bmp280.h"


#define SD 		0
#define RF		1	//Влияет на отправку телеметрии
#define IMU		1
#define IMU_BMP	1
#define BMP		1
#define GPS		1
#define LED		1
#define CONTROL	1

#define GROUND	0


#define SERVO	0


#define HEIGHT_TO_DEPLOY_PARACHUTE 	270
#define DEPLOY_PARACHUTE_PORT		GPIOC
#define DEPLOY_PARACHUTE_PIN		GPIO_PIN_6
#define DELTA_HEIGHT 				10
#define ALL_BUTTONS_WORKED 			255




// if error set value and go to end
#define PROCESS_ERROR(x) if (0 != (error = (x))) { goto end; }


/*#################################################*/
/*################## СТРУКТУРЫ ###################*/
/*#################################################*/

/*
 * 	Структуры состояний аппарата
 */

typedef struct {
	//	temperature and pressure
	int32_t temp;
	int32_t pressure;
} stateSensors_raw_t;


typedef struct {
	//GPS data
	float coordinates[3];
	float speed;
	float course;
	float time;
} stateGPS_t;


//	IMU data in RSC (related system of coordinates)
typedef struct {
	float accel[3];
	float gyro[3];
	float compass[3];
} stateIMU_rsc_t;


//	orientation and position of device in ISC (inertial system of coordinates)
typedef struct {
	//	IMU data
	float accel[3];
	float compass[3];

	//	position
	float velocities[3];
	float coordinates[3];

	//	orientation
	float quaternion[4];

} stateIMU_isc_t;


//	transformed temperature and pressure
typedef struct {
	float temp;
	float pressure;
	float height;
} stateSensors_t;


typedef struct {
	float temp;
	float pressure;
} stateBMPSensors_t;


typedef struct {
	//	"position" of servo and step engine
	float servo_pos;
	float step_engine_pos;
	float target[3];
} stateCamera_orient_t;

//	system parameters
typedef struct {
	uint8_t MPU_state;		//	state of MPU9255
	uint8_t IMU_BMP_state;	//	state of IMU barometer
	uint8_t BMP_state;		//  state of barometer
	uint8_t SD_state;		//	state of SD
	uint8_t NRF_state;		//	state of NRF24L01
	uint8_t GPS_state;		//	state of GPS-module
	uint8_t master_state;	//  state of master

	uint8_t buttons;		//  state of all buttons

	uint8_t globalStage;	//	number of current global stage
	uint8_t globalCommand;	// entering command

	float time;				//	current time
} state_system_t;

typedef struct {
	float coordinates[3];
	float quaternion[4];
	float velosities[3];

} state_master_t;


typedef struct {
	//	zero params; this fields should be filled when device started it`s work
	float zero_pressure;
	float zero_quaternion[4];
	float zero_GPS[3];
	float gyro_staticShift[3];
	float accel_staticShift[3];
} state_zero_t;


typedef enum {
	no_error		= 0,
	driver_overheat	= -1

} error;


//FIXME: DELETE
typedef enum {
	servo_left		= 0,
	servo_right		= 1,
	servo_keel		= 2
} servo_id_t;


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

extern USART_HandleTypeDef	usart_dbg;
//extern UART_HandleTypeDef uartExchangeData;
//extern UART_HandleTypeDef uartExchangeCommand;
//extern DMA_HandleTypeDef dmaExchangeData;

extern SPI_HandleTypeDef	spi_nRF24L01;
extern I2C_HandleTypeDef 	i2c_mpu9255;


//////// Имена тасков и очередей ////////
extern TaskHandle_t 		handleControl;
extern TaskHandle_t			handleRF;
extern TaskHandle_t			handleLeft, handleRight, handleKeel;


extern QueueHandle_t		handleInternalCmdQueue;


// глобальные структуры
extern stateSensors_raw_t 	stateIMUSensors_raw;
extern stateSensors_raw_t	stateSensors_raw;
extern stateGPS_t 			stateGPS;
extern stateIMU_rsc_t 		stateIMU_rsc;
extern stateIMU_isc_t 		stateIMU_isc;
extern stateSensors_t 		stateIMUSensors;
extern stateBMPSensors_t	stateSensors;
extern state_system_t 		state_system;
extern state_master_t		state_master;
extern state_zero_t			state_zero;

extern stateIMU_isc_t		stateIMU_isc_prev;
extern stateSensors_t		stateIMUSensors_prev;
extern stateSensors_t		stateSensors_prev;
extern state_system_t		state_system_prev;

//FIXME: DELETE
extern TIM_HandleTypeDef htimServo;
extern 	servo_task_param_t servo_param_left, servo_param_right, servo_param_keel;
extern state_servo_t		stateServo;

#endif /* STATE_H_ */

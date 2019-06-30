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
#include "quaternion.h"

#define SD 			1
#define RF			1 	//Влияет на отправку телеметрии
#define IMU			1
#define IMU_BMP		1
#define BMP			1
#define GPS			1
#define LED			0
#define CONTROL 	1
#define EXCHANGE	1

#define GROUND		0
#define SERVO		0


#define HEIGHT_TO_DEPLOY_PARACHUTE  3	//FIXME:270
#define DEPLOY_PARACHUTE_PORT		GPIOC
#define DEPLOY_PARACHUTE_PIN		GPIO_PIN_6
#define DELTA_HEIGHT 				5.0

#define PHOTORES_PORT				GPIOA
#define PHOTORES_PIN				GPIO_PIN_1

#define ENGINE_PORT					GPIOC
#define ENGINE_PIN_GOL				GPIO_PIN_0
#define ENGINE_PIN_GOR				GPIO_PIN_2
#define ENGINE_PIN_KEEL				GPIO_PIN_1
#define ENGINE_PIN_WR				GPIO_PIN_5
#define ENGINE_PIN_WL				GPIO_PIN_4

#define BUTTON_PORT					GPIOB
#define BUTTON_PIN_GOL				GPIO_PIN_13
#define BUTTON_PIN_GOR				GPIO_PIN_14
#define BUTTON_PORT_2				GPIOC
#define BUTTON_PIN_KEEL				GPIO_PIN_7
#define BUTTON_PIN_WR				GPIO_PIN_9
#define BUTTON_PIN_WL				GPIO_PIN_8


#define COMMAND_TEST 		2
#define COMMAND_SLEEP_MODE 	0
#define COMMAND_START 		1
#define COMMAND_DATA 		3
#define COMMAND_OK			4
#define COMMAND_LOGS		5


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
	float coordinates[2];
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
	float speed_bmp;
} stateBMPSensors_t;


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
	float magnASA[3];
} state_system_t;


typedef struct {
	float coordinates[2];
	float quaternion[4];
	float accel_isc[3];
	float height;

	float speed_GPS;
	float speed_BMP;

	Euler_angles_t angles;

	float omega;

	float course;
} state_master_t;


typedef struct {
	//	zero params; this fields should be filled when device started it`s work
	double zero_pressure;
	float zero_quaternion[4];
	float zero_GPS[2];
	float gyro_staticShift[3];
	float accel_staticShift[3];
} state_zero_t;


typedef struct {
	float time;
	uint8_t fc_stage;
	float angle_left;
	float angle_right;
	float angle_keel;
	uint8_t critical_angles;
} FC_logs_t;


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
extern UART_HandleTypeDef uartExchangeData;
extern UART_HandleTypeDef uartExchangeCommand;

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
extern FC_logs_t			FCLogs;

extern stateIMU_isc_t		stateIMU_isc_prev;
extern stateSensors_t		stateIMUSensors_prev;
extern stateSensors_t		stateSensors_prev;
extern state_system_t		state_system_prev;

//FIXME: DELETE
extern TIM_HandleTypeDef htimServo;
extern 	servo_task_param_t servo_param_left, servo_param_right, servo_param_keel;
extern state_servo_t		stateServo;

void init_pin(GPIO_InitTypeDef port, uint32_t Alternate, uint32_t Mode, uint32_t Pin, uint32_t Pull, uint32_t Speed, GPIO_TypeDef GPIOT);
void led();

#endif /* STATE_H_ */

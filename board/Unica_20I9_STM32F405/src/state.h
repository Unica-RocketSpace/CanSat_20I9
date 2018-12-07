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


#include "drivers/UNICS_bmp280.h"


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
	//	"position" of servo and step engine
	float servo_pos;
	float step_engine_pos;
	float target[3];
} stateCamera_orient_t;

//	system parameters
typedef struct {
	uint8_t MPU_state;		//	state of MPU9255
	uint8_t IMU_BMP_state;		//	state of IMU barometer
	uint8_t BMP_state;		//  state of barometer
	uint8_t SD_state;		//	state of SD
	uint8_t NRF_state;		//	state of NRF24L01
	uint8_t GPS_state;		//	state of GPS-module
	uint8_t master_state;	//  state of master

	uint8_t buttons;		//  state of all buttons

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


typedef enum {
	no_error		= 0,
	driver_overheat	= -1

} error;



/*##################################################*/
/*################### ПЕРЕМЕННЫЕ ###################*/
/*##################################################*/

extern USART_HandleTypeDef  usart_HC05;
extern USART_HandleTypeDef	usart_dbg;
extern SPI_HandleTypeDef	spi_nRF24L01;
extern I2C_HandleTypeDef 	i2c_mpu9255;

extern TaskHandle_t 		handleControl;
extern TaskHandle_t			handleRF;

// глобальные структуры
extern stateSensors_raw_t 	stateIMUSensors_raw;
extern stateSensors_raw_t	stateSensors_raw;
extern stateGPS_t 			stateGPS;
extern stateIMU_rsc_t 		stateIMU_rsc;
extern stateIMU_isc_t 		stateIMU_isc;
extern stateSensors_t 		stateIMUSensors;
extern stateSensors_t		stateSensors;
extern stateCamera_orient_t	stateCamera_orient;
extern state_system_t 		state_system;
extern state_zero_t			state_zero;

extern stateIMU_isc_t		stateIMU_isc_prev;
extern stateSensors_t		stateIMUSensors_prev;
extern stateSensors_t		stateSensors_prev;
extern state_system_t		state_system_prev;
extern stateCamera_orient_t	stateCamera_orient_prev;





#endif /* STATE_H_ */

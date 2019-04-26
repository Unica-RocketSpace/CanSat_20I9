/*
 * MPU9255.h
 *
 *  Created on: 21 янв. 2017 г.
 *      Author: developer
 */

#ifndef MPU9255_H_
#define MPU9255_H_

#include "state.h"

#define G_VECTOR	9.8155

#define ACCEL_NOISE			0.18623601824029636399941083751228		//	m/s
#define GYRO_NOISE			0.31622776601683793319988935444327		//	grad/s

#define MPU9255_GYRO_SCALE_FACTOR	M_PI / (180 * 131)
//#define MPU9255_ACCEL_SCALE_FACTOR	0.00055217
#define MPU9255_ACCEL_SCALE_FACTOR	9.8155 / 16384

#define X_ACCEL_ADDITIONAL_OFFSET		-0.023
#define Y_ACCEL_ADDITIONAL_OFFSET		-0.023
#define Z_ACCEL_ADDITIONAL_OFFSET		0.01843

/*
#define X_ACCEL_OFFSET		0x003F//( 0x7FC1 )
#define Y_ACCEL_OFFSET		0x003E//( 0x7FC2 )
#define Z_ACCEL_OFFSET		0x0067//( 0x7F99 )
#define X_ACCEL_KOEFF		1.0796414
#define Y_ACCEL_KOEFF		1.0833494
#define Z_ACCEL_KOEFF		1.0717212
*/

#define X_ACCEL_KOEFF		1
#define Y_ACCEL_KOEFF		1
#define Z_ACCEL_KOEFF		1

#define X_ACCEL_OFFSET		0.172550
#define Y_ACCEL_OFFSET		0.041228
#define Z_ACCEL_OFFSET		0.255673
#define XX_ACCEL_TRANSFORM_MATIX	0.996478
#define YY_ACCEL_TRANSFORM_MATIX	0.997762
#define ZZ_ACCEL_TRANSFORM_MATIX	0.989338
#define XY_ACCEL_TRANSFORM_MATIX	-0.000398
#define XZ_ACCEL_TRANSFORM_MATIX	-0.002554
#define YZ_ACCEL_TRANSFORM_MATIX	0.000347

#define X_GYRO_KOEFF		1
#define Y_GYRO_KOEFF		1
#define Z_GYRO_KOEFF		1

#define X_COMPAS_OFFSET		479.1609
#define Y_COMPAS_OFFSET		42.0570
#define Z_COMPAS_OFFSET		191.5415
#define XX_COMPAS_TRANSFORM_MATIX	0.004041
#define YY_COMPAS_TRANSFORM_MATIX	0.004061
#define ZZ_COMPAS_TRANSFORM_MATIX	0.003486
#define XY_COMPAS_TRANSFORM_MATIX	-0.000029
#define XZ_COMPAS_TRANSFORM_MATIX	-0.000166
#define YZ_COMPAS_TRANSFORM_MATIX	-0.000030

#define ACCEL_RANGE			1			//2g - 00, 4g - 01, 8g - 10, 16g - 11
#define GYRO_RANGE			0			//250degps - 00, 500degps - 01, 1000degps - 10, 2000degps - 11


/*#################################################*/
/*################## СТРУКТУРЫ ###################*/
/*#################################################*/

typedef enum {

	RESET_ = (1 << 7)

} mpu9255_params_t;


typedef enum {

	//	full-scale range (FS_SEL_X, +- x = range in g)
	FS_SEL_2G = 0,
	FS_SEL_4G = 1,
	FS_SEL_8G = 2,
	FS_SEL_16G = 3

} accel_params_t;

typedef enum {

	//	full-scale range (FS_SEL_X, +- x = range in dps)
	FS_SEL_250DPS = 0,
	FS_SEL_500DPS = 1,
	FS_SEL_1000DPS = 2,
	FS_SEL_2000DPS = 3

} gyro_params_t;

typedef enum {

	FS_ = 0

} compass_params_t;


// errors
typedef enum {
	MPU9255_ERROR_NONE = 0,
	MPU9255_ERROR_DIV_BY_ZERO = 1
} mpu9255_errors_t;


// addresses of mpu9255 (for getting acsess to accel and gyro)
typedef enum {
	GYRO_AND_ACCEL = 0b11010000,
//	GYRO_AND_ACCEL = 0b11010010,
	COMPASS = 0b00011000
} mpu9255_address_t;


/*##################################################*/
/*################### ПЕРЕМЕННЫЕ ###################*/
/*##################################################*/


/*###############################################*/
/*################### ФУНКЦИИ ###################*/
/*###############################################*/

/*
 *  Чтение регистра
 *	Параметры:
 *		address		- адрес устройства на шине (в нашем случае - магнитометр, гироскоп и акселерометр или барометр)
 *		reg_address	- адрес первого читаемого регистра
 *		dataRead	- массив, в который записываем читаемые значения
 *		count		- количество читаемых регистров
 */
int mpu9255_readRegister(mpu9255_address_t address, uint8_t regAddress, uint8_t *dataRead, uint8_t count);

/*
 *  Запись регистра
 *	Параметры:
 *		address		- адрес устройства на шине (в нашем случае - магнитометр, гироскоп и акселерометр или барометр)
 *		reg_address	- адрес первого записываемого регистра
 *		dataWrite	- массив, из которого берем значения
 */
int mpu9255_writeRegister(mpu9255_address_t address, uint8_t regAddress, uint8_t dataWrite);

/*
 * 	Чтение показаний акселерометра и гироскопа
 * 	Параметры:
 *		raw_accelData	- сырые данные акселерометра
 *		raw_gyroData	- сырые данные гироскопа
 */
int mpu9255_readIMU(int16_t* raw_accelData, int16_t* raw_gyroData);

/*
 * 	Чтение показаний магнитометра
 * 	Параметры:
 * 		raw_compassData	- сырые показания магнитометра
 */
int mpu9255_readCompass(int16_t * raw_compassData);

/*
 * 	Пересчет сырых показаний акселерометра
 * 	Параметры:
 * 		raw_accelData	- сырые показания акселерометра
 * 		accelData		- преобразованные показания акселерометра (в g)
 */
void mpu9255_recalcAccel(const int16_t* raw_accelData, float* accelData);

/*
 * 	Пересчет сырых показаний гироскопа
 * 	Параметры:
 * 		raw_gyroData	- сырые показания гироскопа
 * 		gyroData		- преобразованные показания гироскопа (в 1/c)
 */
void mpu9255_recalcGyro(const int16_t* raw_gyroData, float* gyroData);

/*
 * 	Пересчет сырых показаний магнитометра
 * 	Параметры:
 * 		raw_compassData	- сырые показания магнитометра
 * 		compassData		- преобразованные показания магнитометра (в мТл)
 */
void mpu9255_recalcCompass(const int16_t* raw_compassData, float* compassData);

/*
 * 	Инициализация mpu9255
 * 	Производится инициализация шины I2C и самой платы (установка настроек)
 * 	Параметры:
 *		i2c	- хэндл инициализируемой шины
 */
uint8_t mpu9255_init();		//инициализация mpu9255


#endif /* MPU9255_H_ */

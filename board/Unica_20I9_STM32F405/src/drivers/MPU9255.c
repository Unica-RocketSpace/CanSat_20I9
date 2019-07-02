/*
 * MPU9255.c
 *
 *  Created on: 21 янв. 2017 г.
 *      Author: developer
 */

#include <stdio.h>
#include <math.h>
#include <stm32f4xx_hal.h>
#include "diag/Trace.h"

#include <sofa.h>
#include "FreeRTOS.h"
#include "task.h"

#include "MadgwickAHRS.h"
#include "MPU9255.h"
#include "state.h"
#include "UNICS_bmp280.h"


int mpu9255_readRegister(mpu9255_address_t address, uint8_t reg_address, uint8_t* dataRead, uint8_t count)
{
	return HAL_I2C_Mem_Read(&i2c_mpu9255, address, reg_address, I2C_MEMADD_SIZE_8BIT, dataRead, count, 0xFF);
}


int mpu9255_writeRegister(mpu9255_address_t address, uint8_t reg_address, uint8_t dataWrite)
{
	return HAL_I2C_Mem_Write(&i2c_mpu9255, address, reg_address, I2C_MEMADD_SIZE_8BIT, &dataWrite, 1, 0xFF);
}


int mpu9255_rewriteRegister(mpu9255_address_t address, uint8_t reg_address, uint8_t dataWrite)
{
	int error = 0;
	uint8_t regData = 0x00;
	PROCESS_ERROR(mpu9255_readRegister(address, reg_address, &regData, 1));
	uint8_t regData_new = (regData | dataWrite);
	return HAL_I2C_Mem_Write(&i2c_mpu9255, address, reg_address, I2C_MEMADD_SIZE_8BIT, &regData_new, 1, 0xFF);

end:
	return error;
}


int mpu9255_init(I2C_HandleTypeDef* hi2c)
{
	int error = 0;

	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	107,	0b10000000));	//RESET
	HAL_Delay(200);


//	uint8_t whoami = 0;
//	PROCESS_ERROR(mpu9255_readRegister(GYRO_AND_ACCEL, 117, &whoami, 1));
//	trace_printf("me: %d\n", whoami);


	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	25,		0b00000001));	//Sample Rate Divider
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	26,		0b00000101));	//config (DLPF = 101)
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	28,		(0b00000000 | (ACCEL_RANGE << 3)))); 	//accel config (rate 4g = 01)
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	29,		0b00000000));	//accel config 2 (Fch_b = 0, DLPF = 100)
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	35,		0b00000000));	//FIFO enable (not enabled)
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	56,		0b00000000));	//interrupt enable (int disable = 0)
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	106,	0b00000000));	//user control
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	107,	0b00000001));	//power managment 1
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	108,	0b00000000));	//power managment 2
//	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	27,		(0b00000000 | (GYRO_RANGE << 4)) ));	//gyro config (rate 500dps = 01, Fch_b = 00)
	PROCESS_ERROR(mpu9255_rewriteRegister(GYRO_AND_ACCEL,	27,		0b000000000 | (1 << 4)));

	//  Magnetometer init
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	55,		0b00000010));	//режим bypass on

	PROCESS_ERROR(mpu9255_writeRegister(COMPASS,	0x0A,   AK8963_MODE_POWER_DOWN));	// power down before entering fuse mode
	HAL_Delay(20);

	PROCESS_ERROR(mpu9255_writeRegister(COMPASS,	0x0A,   AK8963_MODE_FUSE_ROM));		// Enter Fuse ROM access mode
	HAL_Delay(10);

	static int8_t ASA[3] = {};
	PROCESS_ERROR(mpu9255_readRegister(COMPASS,		0x10,   (uint8_t*)ASA, 3));   //ASAX
	//  Recalc magnetometer sensitivity adjustment values to floats to store them
	taskENTER_CRITICAL();
	state_system.magnASA[0] = (float)(ASA[0] + 128) / (256);
	state_system.magnASA[1] = (float)(ASA[1] + 128) / (256);
	state_system.magnASA[2] = (float)(ASA[2] + 128) / (256);
	taskEXIT_CRITICAL();

	PROCESS_ERROR(mpu9255_writeRegister(COMPASS,        0x0A,   AK8963_MODE_POWER_DOWN));	// power down after reading
	HAL_Delay(20);

	uint8_t state = 0;
	//	Clear status registers
	PROCESS_ERROR(mpu9255_readRegister(COMPASS, 	0x02, &state, 1));
	PROCESS_ERROR(mpu9255_readRegister(COMPASS, 	0x09, &state, 1));

	PROCESS_ERROR(mpu9255_writeRegister(COMPASS,	0x0A,   AK8963_MODE_100HZ | AK8963_BIT_16_BIT));

end:
	mpu9255_writeRegister(GYRO_AND_ACCEL, 55,     0b00000000);   		//режим bypass off
	return error;
}

static int16_t _swapBytesI16(int16_t value)
{
	uint8_t * value_ptr = (uint8_t*)&value;
	uint8_t tmp = value_ptr[0];
	value_ptr[0] = value_ptr[1];
	value_ptr[1] = tmp;

	return value;
}

int mpu9255_readIMU(int16_t * raw_accelData, int16_t * raw_gyroData)
{
	int error = 0;

	PROCESS_ERROR(mpu9255_readRegister(GYRO_AND_ACCEL, 59, (uint8_t*)raw_accelData, 6));	//чтение данных с акселерометра
	PROCESS_ERROR(mpu9255_readRegister(GYRO_AND_ACCEL, 67, (uint8_t*)raw_gyroData, 6));		//чтение данных с гироскопа

	for (int i = 0; i < 3; i++)
		raw_accelData[i] = _swapBytesI16(raw_accelData[i]);

	for (int i = 0; i < 3; i++)
		raw_gyroData[i] = _swapBytesI16(raw_gyroData[i]);

end:
	return error;
}

int mpu9255_readCompass(int16_t * raw_compassData)
{
	int error = 0;

	//	state of magn (ready to give data or not)
	uint8_t magn_state = 0;

	/*
	 * Bypass mode on, get ST1 register value
	 */
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL, 55, 0b00000010));	//	bypass on
	PROCESS_ERROR(mpu9255_readRegister(COMPASS, 0x02, &magn_state, 1));

	/*
	 * Check if ST1 value bit ready is set
	 */
	if (magn_state & AK8963_DATA_READY)
	{
		magn_state = 0;

		/*
		 * Read data values and read ST2 register to prove that data values is read
		 * I found that I should read 7 bytes together
		 */
		uint8_t bytes[7] = {};
		PROCESS_ERROR(mpu9255_readRegister(COMPASS, 0x03, bytes, 7));

		for (int i = 0; i < 3; i++)
			raw_compassData[i] = (int16_t)((bytes[2*i+1] << 8) | bytes[2*i]);

		magn_state = bytes[6];

		/*
		 * Check HOFL bit for overflow
		 */
		if (magn_state & AK8963_DATA_OVERFLOW)
		{
			for (uint8_t i = 0; i < 3; i++)
				raw_compassData[i] = 0;
		}
	}

end:
	mpu9255_writeRegister(GYRO_AND_ACCEL, 55, 0b00000000);	//режим bypass off
	return error;
}


void mpu9255_recalcAccel(const int16_t * raw_accelData, float * accelData)
{
	float _accelData[3] = {0, 0, 0};

	int accel_range = 1;
	for (int i = 0; i < ACCEL_RANGE; i++)
		accel_range *= 2;
	float factor = accel_range * MPU9255_ACCEL_SCALE_FACTOR;

	_accelData[0] = - (float)(raw_accelData[1]) * factor;
	_accelData[1] = - (float)(raw_accelData[0]) * factor;
	_accelData[2] = - (float)(raw_accelData[2]) * factor;

	float offset_vector[3] = {X_ACCEL_OFFSET, Y_ACCEL_OFFSET, Z_ACCEL_OFFSET};
	float transform_matrix[3][3] =	{{XX_ACCEL_TRANSFORM_MATIX, XY_ACCEL_TRANSFORM_MATIX, XZ_ACCEL_TRANSFORM_MATIX},
									 {XY_ACCEL_TRANSFORM_MATIX, YY_ACCEL_TRANSFORM_MATIX, YZ_ACCEL_TRANSFORM_MATIX},
									 {XZ_ACCEL_TRANSFORM_MATIX, YZ_ACCEL_TRANSFORM_MATIX, ZZ_ACCEL_TRANSFORM_MATIX}};

	iauPmp(_accelData, offset_vector, accelData);
	iauRxp(transform_matrix, accelData, accelData);

	for (int i = 0; i < 3; i++) {
		accelData[i] = _accelData[i];
	}
}

void mpu9255_recalcGyro(const int16_t * raw_gyroData, float * gyroData)
{
	float _gyroData[3] = {0, 0, 0};

	int gyro_range = 1;
	for (int i = 0; i < GYRO_RANGE; i++)
		gyro_range *= 2;
	float factor = gyro_range * MPU9255_GYRO_SCALE_FACTOR;

	_gyroData[0] = - (float)(raw_gyroData[1]) * factor;
	_gyroData[1] = - (float)(raw_gyroData[0]) * factor;
	_gyroData[2] = - (float)(raw_gyroData[2]) * factor;

//	float offset_vector[3] = {X_GYRO_OFFSET, Y_GYRO_OFFSET, Z_GYRO_OFFSET};

//	iauPmp(_gyroData, offset_vector, gyroData);

	for (int i = 0; i < 3; i++) {
		gyroData[i] = _gyroData[i];
	}

}

void mpu9255_recalcCompass(const int16_t * raw_compassData, float * compassData)
{
	//переводим систему координат магнетометра в систему координат MPU
	float raw_data[3] = {	  (float)raw_compassData[0],
							- (float)raw_compassData[1],
							- (float)raw_compassData[2]};

	/*
	 * Adjustment
	 */
	taskENTER_CRITICAL();
	float magnASA[3] = {state_system.magnASA[0], state_system.magnASA[2], - state_system.magnASA[1]};
	taskEXIT_CRITICAL();

	for (uint8_t i = 0; i < 3; i++)
		raw_data[i] *= magnASA[i];


//	float offset_vector[3] = {X_COMPAS_OFFSET, Y_COMPAS_OFFSET, Z_COMPAS_OFFSET};
//	float transform_matrix[3][3] =	{	{XX_COMPAS_TRANSFORM_MATIX, XY_COMPAS_TRANSFORM_MATIX, XZ_COMPAS_TRANSFORM_MATIX},
//										{XY_COMPAS_TRANSFORM_MATIX, YY_COMPAS_TRANSFORM_MATIX, YZ_COMPAS_TRANSFORM_MATIX},
//										{XZ_COMPAS_TRANSFORM_MATIX, YZ_COMPAS_TRANSFORM_MATIX, ZZ_COMPAS_TRANSFORM_MATIX}};
//
//	iauPmp(raw_data, offset_vector, compassData);
//	iauRxp(transform_matrix, compassData, compassData);

	float offset_vector[3] = {0, 0, 0};
	iauPmp(raw_data, offset_vector, compassData);
}


uint8_t check_i2cSlaveConnetion(uint8_t address)
{
	uint8_t error = 0;
	if ((address == GYRO_AND_ACCEL) && (MPU))
	{
		uint8_t whoami = 0;
		PROCESS_ERROR(mpu9255_readRegister(address, 117, &whoami, 1));

		if (whoami != 113)
			return -2;
		else
			return 0;
	}

end:
	return -3;
	return error;
}


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
#include <tasks/dynamic_unit.h>
#include <tasks/kinematic_unit.h>
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
	int error = 0;
	uint8_t regData = 0x00;
	PROCESS_ERROR(mpu9255_readRegister(address, reg_address, &regData, 1));
	uint8_t regData_new = (regData | dataWrite);
	return HAL_I2C_Mem_Write(&i2c_mpu9255, address, reg_address, I2C_MEMADD_SIZE_8BIT, &regData_new, 1, 0xFF);

end:
	return error;
}


uint8_t mpu9255_init(){

	uint8_t error = 0;

	HAL_Delay(400);

	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	107,	0b10000000));	//RESET
	HAL_Delay(400);

	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	25,		0b00000001));	//Sample Rate Divider
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	26,		0b00000101));	//config (DLPF = 101)
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	28,		(0b00000000 | (ACCEL_RANGE << 3)))); 	//accel config (rate 4g = 01)
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	29,		0b00000100));	//accel config 2 (Fch_b = 0, DLPF = 100)
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	35,		0b00000000));	//FIFO enable (not enabled)
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	56,		0b00000000));	//interrupt enable (int disable = 0)
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	106,	0b00000000));	//user control
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	107,	0b00000001));	//power managment 1
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	108,	0b00000000));	//power managment 2
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	27,		(0b00000000 | (GYRO_RANGE << 4)) ));	//gyro config (rate 500dps = 01, Fch_b = 00)

	//compass init
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	55,		0b00000010));	//режим bypass on
	PROCESS_ERROR(mpu9255_writeRegister(COMPASS,		0x0A,	0b00010110));	//control 1
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL,	55,		0b00000000));	//режим bypass off

end:
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

	// state of magn (ready to give data or not)
	uint8_t magn_state;

	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL, 55, 0b00000010));	//режим bypass on
	mpu9255_readRegister(COMPASS, 0x02, &magn_state, 1);
	//printf("read_dummy_error = %d\n", read_dummy_error);
	PROCESS_ERROR(mpu9255_readRegister(COMPASS, 0x02, &magn_state, 1));


	if ((magn_state && 0x01) != 1)
	{
//		state_system.state &= ~(1 << 1);		//магнитометр не готов
		goto end;
	}

//	state_system.state |= (1 << 1);	//магнитометр готов
	PROCESS_ERROR(mpu9255_readRegister(COMPASS, 0x03, (uint8_t*)raw_compassData, 6));
	PROCESS_ERROR(mpu9255_readRegister(COMPASS, 0x09, &magn_state, 1));

	//FIXME: не надо ли свапать здесь байты???

end:
	PROCESS_ERROR(mpu9255_writeRegister(GYRO_AND_ACCEL, 55, 0b00000000));	//режим bypass off
	return error;
}

void mpu9255_recalcAccel(const int16_t * raw_accelData, float * accelData)
{
	float _accelData[3] = {0, 0, 0};

	_accelData[0] = - (float)(raw_accelData[0]) * MPU9255_ACCEL_SCALE_FACTOR * 2;//* pow(2, ACCEL_RANGE);
	_accelData[1] =   (float)(raw_accelData[2]) * MPU9255_ACCEL_SCALE_FACTOR * 2;//* pow(2, ACCEL_RANGE);
	_accelData[2] =   (float)(raw_accelData[1]) * MPU9255_ACCEL_SCALE_FACTOR * 2;//* pow(2, ACCEL_RANGE);

//	float offset_vector[3] = {X_ACCEL_OFFSET, Y_ACCEL_OFFSET, Z_ACCEL_OFFSET};
//	float transform_matrix[3][3] =	{{XX_ACCEL_TRANSFORM_MATIX, XY_ACCEL_TRANSFORM_MATIX, XZ_ACCEL_TRANSFORM_MATIX},
//									 {XY_ACCEL_TRANSFORM_MATIX, YY_ACCEL_TRANSFORM_MATIX, YZ_ACCEL_TRANSFORM_MATIX},
//									 {XZ_ACCEL_TRANSFORM_MATIX, YZ_ACCEL_TRANSFORM_MATIX, ZZ_ACCEL_TRANSFORM_MATIX}};
//
//	iauPmp(_accelData, offset_vector, accelData);
//	iauRxp(transform_matrix, accelData, accelData);

	for (int i = 0; i < 3; i++) {
		accelData[i] = _accelData[i];
	}
}

void mpu9255_recalcGyro(const int16_t * raw_gyroData, float * gyroData)
{
	gyroData[0] = - (float)(raw_gyroData[0]) * MPU9255_GYRO_SCALE_FACTOR * pow(2, GYRO_RANGE);
	gyroData[1] = 	(float)(raw_gyroData[2]) * MPU9255_GYRO_SCALE_FACTOR * pow(2, GYRO_RANGE);
	gyroData[2] = 	(float)(raw_gyroData[1]) * MPU9255_GYRO_SCALE_FACTOR * pow(2, GYRO_RANGE);
}

void mpu9255_recalcCompass(const int16_t * raw_compassData, float * compassData)
{
//	float raw_data[3] = {(float)raw_compassData[0], (float)raw_compassData[1], (float)raw_compassData[2]};
//	float offset_vector[3] = {X_COMPAS_OFFSET, Y_COMPAS_OFFSET, Z_COMPAS_OFFSET};
//	float transform_matrix[3][3] =	{	{XX_COMPAS_TRANSFORM_MATIX, XY_COMPAS_TRANSFORM_MATIX, XZ_COMPAS_TRANSFORM_MATIX},
//										{XY_COMPAS_TRANSFORM_MATIX, YY_COMPAS_TRANSFORM_MATIX, YZ_COMPAS_TRANSFORM_MATIX},
//										{XZ_COMPAS_TRANSFORM_MATIX, YZ_COMPAS_TRANSFORM_MATIX, ZZ_COMPAS_TRANSFORM_MATIX}};
//
//	iauPmp(raw_data, offset_vector, compassData);
//	iauRxp(transform_matrix, compassData, compassData);

	compassData[0] = - (float)raw_compassData[1];
	compassData[1] =   (float)raw_compassData[2];
	compassData[2] = - (float)raw_compassData[0];
}

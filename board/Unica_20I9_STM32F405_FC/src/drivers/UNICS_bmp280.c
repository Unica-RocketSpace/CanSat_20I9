/*
 * UNICS_bmp280.c
 *
 *  Created on: 29 апр. 2018 г.
 *      Author: developer
 */

#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include "FreeRTOS.h"
#include "task.h"

#include "state.h"
#include "UNICS_bmp280.h"

//#include "../i2c.h"
//#include "../spi.h"


/*Дескриптор датчика.
 *Поле mode заполняется rscs_bmp280_changemode()*/
struct rscs_bmp280_descriptor {
	rscs_bmp280_parameters_t parameters;
	rscs_bmp280_calibration_values_t calibration_values;
	// Режим работы - непрерывный, одиночный, ожидания
	rscs_bmp280_mode_t mode;
	int (*read_reg)(rscs_bmp280_descriptor_t * /*descr*/, uint8_t /*reg_addr*/, void * /*buffer*/, size_t /*buffer_size*/);
	int (*write_reg)(rscs_bmp280_descriptor_t * /*descr*/, uint8_t /*reg_addr*/, void * /*buffer*/, size_t /*buffer_size*/);

	union
	{
		struct
		{
			rscs_bmp280_addr_t addr;
			I2C_HandleTypeDef *hi2c;
		} i2c;
		struct
		{
			volatile uint8_t * cs_port;
			uint8_t cs_pin_mask;
		} spi;
	} bus_params;
};


/*static int _read_reg_spi(rscs_bmp280_descriptor_t * descr, uint8_t reg_addr,
		void * buffer, size_t buffer_size)
{
	*descr->bus_params.spi.cs_port &= ~(descr->bus_params.spi.cs_pin_mask);

	reg_addr |= (1<<7);
	rscs_spi_write(&reg_addr,1);
	rscs_spi_read(buffer, buffer_size, 0xFF);

	*descr->bus_params.spi.cs_port |= (descr->bus_params.spi.cs_pin_mask);
	return 0;
}

static int _write_reg_spi(rscs_bmp280_descriptor_t * descr, uint8_t reg_addr,
		const void * buffer, size_t buffer_size)
{
	*descr->bus_params.spi.cs_port &= ~(descr->bus_params.spi.cs_pin_mask);

	reg_addr &= ~(1<<7);
	rscs_spi_write(&reg_addr,1);
	rscs_spi_write(buffer,buffer_size);

	*descr->bus_params.spi.cs_port |= (descr->bus_params.spi.cs_pin_mask);
	return 0;
}*/


static int _read_reg_i2c(rscs_bmp280_descriptor_t * descr, uint8_t reg_addr, void * buffer, size_t buffer_size)
{
	return HAL_I2C_Mem_Read(descr->bus_params.i2c.hi2c, descr->bus_params.i2c.addr, reg_addr, I2C_MEMADD_SIZE_8BIT, buffer, buffer_size, 20000);
}



static int _write_reg_i2c(rscs_bmp280_descriptor_t * descr, uint8_t reg_addr, void * buffer, size_t buffer_size)
{
	return HAL_I2C_Mem_Write(descr->bus_params.i2c.hi2c, descr->bus_params.i2c.addr, reg_addr, I2C_MEMADD_SIZE_8BIT, buffer, buffer_size, 20000);
}



rscs_bmp280_descriptor_t * rscs_bmp280_initi2c(I2C_HandleTypeDef *hi2c, rscs_bmp280_addr_t addr){
	rscs_bmp280_descriptor_t * pointer = (rscs_bmp280_descriptor_t *) malloc(sizeof(rscs_bmp280_descriptor_t));
	if (pointer == NULL)
		return NULL;

	pointer->bus_params.i2c.addr = addr;
	pointer->bus_params.i2c.hi2c = hi2c;
	pointer->read_reg = _read_reg_i2c;
	pointer->write_reg = _write_reg_i2c;
	return pointer;
}


/*rscs_bmp280_descriptor_t * rscs_bmp280_initspi(volatile uint8_t * cs_port,
		volatile uint8_t * cs_ddr, uint8_t pin_n)
{
	rscs_bmp280_descriptor_t * pointer = (rscs_bmp280_descriptor_t *) malloc(sizeof(rscs_bmp280_descriptor_t));
	if (pointer == NULL)
		return NULL;

	pointer->bus_params.spi.cs_port = cs_port;
	pointer->bus_params.spi.cs_pin_mask = (1 << pin_n);
	pointer->read_reg = _read_reg_spi;
	pointer->write_reg = _write_reg_spi;
	*cs_ddr |= pointer->bus_params.spi.cs_pin_mask;

	*pointer->bus_params.spi.cs_port &= ~(pointer->bus_params.spi.cs_pin_mask);
	_delay_ms(10);
	*pointer->bus_params.spi.cs_port |= (pointer->bus_params.spi.cs_pin_mask);

	return pointer;
}*/


void rscs_bmp280_deinit(rscs_bmp280_descriptor_t * descr){
	free(descr);
}


int rscs_bmp280_setup(rscs_bmp280_descriptor_t * descr, const rscs_bmp280_parameters_t * params){
	int error = -1;
	uint8_t tmp[2] = {231, 123};

/*#ifdef RSCS_DEBUGMODE
	for(int i = 0; i < sizeof(descr->calibration_values); i++) {
		*( ( (uint8_t *) &descr->calibration_values) + i ) = 237;
	}
#endif*/

	HAL_Delay(50);

	//RSCS_DEBUG("BMP280: SETUP: trying to read ID reg\n");
	PROCESS_ERROR(descr->read_reg(descr, RSCS_BMP280_REG_ID, tmp, 1));
	//RSCS_DEBUG("BMP280: returned IDCODE 0x%X\n", tmp[0]);
	if(tmp[0] != RSCS_BMP280_IDCODE) {
		return -1;
	}

	//RSCS_DEBUG("BMP280: SETUP: trying to reset\n");
	tmp[0] = 0xb6;  // специальное значение, которое сбрасывает датчик
	PROCESS_ERROR(descr->write_reg(descr, RSCS_BMP280_REG_RESET, tmp, 1));

	HAL_Delay(50);

	PROCESS_ERROR(descr->read_reg(descr, RSCS_BMP280_REG_CALVAL_START, &(descr->calibration_values), sizeof(descr->calibration_values)));
/*#ifdef RSCS_DEBUGMODE
	RSCS_DEBUG("BMP280: calvals: ");
	for(int i = 0; i < sizeof(descr->calibration_values); i++) {
		RSCS_DEBUG("%d ", *( ( (uint8_t *) &descr->calibration_values) + i ) );
	}
	RSCS_DEBUG("\n");
#endif*/

	tmp[0] = 	(params->temperature_oversampling << 5) |
				(params->pressure_oversampling << 2);
	tmp[1] = 	(params->standbytyme << 5) |
				(params->filter << 2);
	//PROCESS_ERROR(descr->write_reg(descr, RSCS_BMP280_REG_CTRL_MEAS, tmp, 2));

	descr->parameters = *params;

end:
	//RSCS_DEBUG("BMP280: SETUP: returning %d\n", error);
	return error;
}


const rscs_bmp280_parameters_t * rscs_bmp280_get_config(rscs_bmp280_descriptor_t * descr){
	return &descr->parameters;
}


int rscs_bmp280_set_config(rscs_bmp280_descriptor_t * descr, const rscs_bmp280_parameters_t * params) {
	int error = 0;

	uint8_t tmp[2];

	tmp[0] = 	(params->temperature_oversampling << 5) |
				(params->pressure_oversampling << 2);
	tmp[1] = 	(params->standbytyme << 5) |
				(params->filter << 2);
	PROCESS_ERROR(descr->write_reg(descr, RSCS_BMP280_REG_CTRL_MEAS, tmp, 2));

	descr->parameters = *params;

end:
	return error;
}


const rscs_bmp280_calibration_values_t * rscs_bmp280_get_calibration_values(rscs_bmp280_descriptor_t * descr){
	return &descr->calibration_values;
}


int rscs_bmp280_changemode(rscs_bmp280_descriptor_t * descr, rscs_bmp280_mode_t mode){
	int error = 0;
	uint8_t tmp = (	descr->parameters.temperature_oversampling << 5) |
			(descr->parameters.pressure_oversampling << 2) |
			mode;

	PROCESS_ERROR(descr->write_reg(descr, RSCS_BMP280_REG_CTRL_MEAS, &tmp, 1));

end:
	//RSCS_DEBUG("BMP280: CHMODE: returning %d\n", error);
	return error;
}


int rscs_bmp280_read(rscs_bmp280_descriptor_t * descr, int32_t * rawpress, int32_t * rawtemp){
	int error = 0;
	uint8_t tmp[6];

	PROCESS_ERROR(descr->read_reg(descr, RSCS_BMP280_REG_PRESS_MSB, tmp, 6));
	*rawpress = ((uint32_t)tmp[0] << 12) | ((uint32_t)tmp[1] << 4) | ((uint32_t)tmp[2] >> 4);
	*rawtemp = ((uint32_t)tmp[3] << 12) | ((uint32_t)tmp[4] << 4) | ((uint32_t)tmp[5] >> 4);

end:
	//RSCS_DEBUG("BMP280: READ: returning %d\n", error);
	return error;
}


uint8_t rscs_bmp280_read_status(rscs_bmp280_descriptor_t * descr) {
	int error = 0;
	uint8_t status = 255;
	PROCESS_ERROR(descr->read_reg(descr, RSCS_BMP280_REG_STATUS, &status, 1));

end:
	//RSCS_DEBUG("BMP280: READ: returning %d\n", error);
	return status;
}


int rscs_bmp280_calculate(const rscs_bmp280_calibration_values_t * calvals , int32_t rawpress, int32_t rawtemp, float * press_p, float * temp_p) {

int32_t t_fine;
	{
		float var1, var2;
		var1 = (((float)rawtemp) / 16384.0 - ((float)calvals->T1) / 1024.0) * ((float)calvals->T2);
		var2 = ((((float)rawtemp) / 131072.0 - ((float)calvals->T1) / 8192.0) * (((float)rawtemp) / 131072.0 - ((float)calvals->T1) / 8192.0)) * ((float)calvals->T3);
		t_fine = (int32_t)(var1 + var2);

		*temp_p = (var1 + var2) / 5120.0;
	}

	{
		float var1_p, var2_p;
		float p;
		var1_p = (((float)t_fine) / 2 - 64000.0);
		var2_p = (var1_p * var1_p * (((float)calvals->P6) / 32768.0));
		var2_p = (var2_p + var1_p * ((float)calvals->P5) * 2.0);
		var2_p = ((var2_p / 4.0) + ((float)calvals->P4) * 65536.0);
		var1_p = ((((float)calvals->P3) * var1_p * var1_p / 524288.0 + ((float)calvals->P2) * var1_p) / 524288.0);
		var1_p = ((1.0 + var1_p / 32768.0) * ((float)calvals->P1));

		if (var1_p == 0)	return -1; // чтобы не делить на ноль

		p = (1048576.0 - ((float)rawpress)) ;
		p = ((p -(var2_p / 4096.0)) * 6250 / var1_p);
		var1_p = (((float)calvals->P9) * p * p / 2147483648.0);
		var2_p = (p * ((float)calvals->P8) / 32768.0);
		p = (p + ((var1_p + var2_p + (float)calvals->P7)) / 16.0);

		*press_p = p;
	}

	return 0;
}





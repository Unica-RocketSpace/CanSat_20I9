/*
 * nRF24L01.h
 *
 *  Created on: 10 марта 2018 г.
 *      Author: developer
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stm32f4xx_hal.h>

#define nRF24L01_WRITE_REGISTER(ADDR)	(ADDR | 0b00100000)
#define nRF24L01_READ_REGISTER(ADDR)	(ADDR | 0b00000000)
#define nRF24L01_WRITE_TX_FIFO			0b10100000
#define nRF24L01_WRITE_TX_FIFO_NO_ACK	0b10110000
#define nRF24L01_READ_RX_FIFO			0b01100001
#define nRF24L01_FLUSH_TX_FIFO			0b11100001
#define nRF24L01_FLUSH_RX_FIFO			0b11100010

#define nRF24L01_RX_ADDR_P0		{0xe7, 0xe7, 0xe7, 0xe7, 0xe7}	//Адрес получателя 0
#define nRF24L01_RX_ADDR_P1		0x0000000000	//Адрес получателя 1
#define nRF24L01_RX_ADDR_P2		0x0000000000	//Адрес получателя 2
#define nRF24L01_RX_ADDR_P3		0x0000000000	//Адрес получателя 3
#define nRF24L01_RX_ADDR_P4		0x0000000000	//Адрес получателя 4
#define nRF24L01_RX_ADDR_P5		0x0000000000	//Адрес получателя 5
#define nRF24L01_TX_ADDR		{0xe7, 0xe7, 0xe7, 0xe7, 0xe7}	//Адрес устройства

#define nRF24L01_RX_PW_P0		nRF24L01_BUFFER_LEN		//Кол-во байт, получаемых по каналу 0
#define nRF24L01_RX_PW_P1		0							//Кол-во байт, получаемых по каналу 1
#define nRF24L01_RX_PW_P2		0							//Кол-во байт, получаемых по каналу 2
#define nRF24L01_RX_PW_P3		0							//Кол-во байт, получаемых по каналу 3
#define nRF24L01_RX_PW_P4		0							//Кол-во байт, получаемых по каналу 4
#define nRF24L01_RX_PW_P5		0							//Кол-во байт, получаемых по каналу 5

#define nRF24L01_BUFFER_LEN			32

#define nRF24L01_CS_PORT GPIOA
//#define nRF24L01_CS_DDR  DDRB
#define nRF24L01_CS_PIN  GPIO_PIN_4

#define nRF24L01_CE_PORT GPIOA
//#define nRF24L01_CE_DDR  DDRB
#define nRF24L01_CE_PIN  GPIO_PIN_8

typedef enum {
	nRF24L01_CONFIG_ADDR			= 0x00,
	nRF24L01_EN_AA_ADDR				= 0x01,
	nRF24L01_EN_RXADDR_ADDR 		= 0x02,
	nRF24L01_SETUP_AW_ADDR			= 0x03,
	nRF24L01_ARC_CNT_ADDR			= 0x04,
	nRF24L01_RF_CH_ADDR				= 0x05,
	nRF24L01_RF_SETUP_ADDR			= 0x06,
	nRF24L01_STATUS_ADDR			= 0x07,
	nRF24L01_OBSERVE_TX_ADDR		= 0x08,
	nRF24L01_CD_ADDR				= 0x09,

	nRF24L01_RX_ADDR_P0_ADDR		= 0x0A,
	nRF24L01_RX_ADDR_P1_ADDR		= 0x0B,
	nRF24L01_RX_ADDR_P2_ADDR		= 0x0C,
	nRF24L01_RX_ADDR_P3_ADDR		= 0x0D,
	nRF24L01_RX_ADDR_P4_ADDR		= 0x0E,
	nRF24L01_RX_ADDR_P5_ADDR		= 0x0F,

	nRF24L01_TX_ADDR_ADDR			= 0x10,
	nRF24L01_RX_PW_P0_ADDR			= 0x11,
	nRF24L01_RX_PW_P1_ADDR			= 0x12,
	nRF24L01_RX_PW_P2_ADDR			= 0x13,
	nRF24L01_RX_PW_P3_ADDR			= 0x14,
	nRF24L01_RX_PW_P4_ADDR			= 0x15,
	nRF24L01_RX_PW_P5_ADDR			= 0x16,
	nRF24L01_FIFO_STATUS_ADDR		= 0x17,

	nRF24L01_DYNPD_ADDR				= 0x1C,
	nRF24L01_FEATURE_ADDR			= 0x1D


}nRF24L01_registr_addr_t;

typedef enum {
	MASK_RX_DR	= 6,	//Включение прерывания для входных данных
	MASK_TX_DS	= 5,	//Включение прерывания для выходных данных
	MASK_MAX_RT	= 4,	//Включение прерывания по превышению лимита повтора передачи данных
	EN_CRC		= 3,	//Включить контрольную сумму
	CRCO		= 2,	//Размер контрольной суммы (0 - 1 байт; 1 - 2 байта)
	PWR_UP		= 1,	//Включение устройства
	PRIM_RX		= 0		//Режим работы (0 - режим передачи данных; 1 - режим приема данных)

} nRF24L01_CONFIG_t;

typedef enum {			//Разрешение отправки/передачи ACK для каждого из каналов
	ENAA_P5     = 5,
	ENAA_P4     = 4,
	ENAA_P3     = 3,
	ENAA_P2     = 2,
	ENAA_P1     = 1,
	ENAA_P0     = 0

} nRF24L01_EN_AA_t;

typedef enum {			//Включнеие канала для приема/передачи
	ERX_P5 = 5,
	ERX_P4 = 4,
	ERX_P3 = 3,
	ERX_P2 = 2,
	ERX_P1 = 1,
	ERX_P0 = 0

}nRF24L01_EN_RXADDR_t;

typedef enum {			//Длины адресов других радиомодулей
	AW   = 0		//[1:0] (01 - 3 байта, 10 - 4 байта, 11 - 5 байт,

}nRF24L01_SETUP_AW_t;

typedef enum {
	ARD  = 4,			//Время на повторную передачу данных
						/*  ‘0000’ – Wait 250μS
							‘0000’ – Wait 250μS
							‘0001’ – Wait 500μS
							‘0010’ – Wait 750μS
							……..
							‘1111’ – Wait 4000μS  */
	ARC  = 0				/*Кол-во повторных передач
							‘0000’ –Re-Transmit disabled
							‘0001’ – Up to 1 Re-Transmit on fail of AA
							……
							‘1111’ – Up to 15 Re-Transmit on fail of AA
												  */

}nRF24L01_SETUP_RETR_t;

typedef enum {			//Частота канала
	RF_CH = 0			//[6:0]

}nRF24L01_RF_CH_t;

typedef enum {
	RF_DR     = 3,		//Скорость передачи данных (0 - 1 Mbps, 1 - 2 Mbps)
	RF_PWR    = 1,		//[2:1] Усиление сигнала
						//(00 – -18dBm, 01 – -12dBm, 10 – -6dBm, 11 – 0dBm)
	LNA_HCURR = 0		//Установка LNA модуля

}nRF24L01_RF_SETUP_t;

typedef enum {
	RX_DR    = 6,		//Флаг получения входных данных
	TX_DS    = 5,		//Флаг отправки выходных данных
	MAX_RT   = 4,		//Флаг превышения лимита повторов передачи данных
						//(выставить 1 для очистки списка)
	RX_P_NO  = 1,		//000-101 - номер канала, по которому пришли данные
						//110 - не используется, 111 - буфер пустой
	TX_FULL  = 0		//Флаг переполнения буфера

}nRF24L01_STATUS_t;

typedef enum {
	PLOS_CNT = 4,		//Счетчик потерянных пакетов
	ARC_CNT  = 0		//Счетчик переотправленных пакетов

}nRF24L01_OBSERVE_TX_t;

typedef enum {
	CD = 0				//Смотри datasheet
}CD_t;

typedef enum {
	TX_REUSE = 6,		//Исползует последний пакет данных при 1
	TX_FIFO_FULL  = 5,	//Флаг переполнения буфера отправки
	TX_EMPTY = 4,		//Флаг пустоты буфера отправки
	RX_FULL  = 1,		//Флаг переполнения буфера приема
	RX_EMPTY = 0		//Флаг пустоты буфера приема

}nRF24L01_FIFO_STATUS_t;

typedef enum {
	DPL_P5 = 5,			//Разрешение на динамическую длину пакеров для 5-го канала
	DPL_P4 = 4,			//Разрешение на динамическую длину пакеров для 4-го канала
	DPL_P3 = 3,			//Разрешение на динамическую длину пакеров для 3-го канала
	DPL_P2 = 2,			//Разрешение на динамическую длину пакеров для 2-го канала
	DPL_P1 = 1,			//Разрешение на динамическую длину пакеров для 1-го канала
	DPL_P0 = 0			//Разрешение на динамическую длину пакеров для 0-го канала

}nRF24L01_DYNPD_t;

typedef enum {
	EN_DPL     = 2,		//Разрешение на динамическую длину пакетов
	EN_ACK_PAY = 1,		//Разрешение на отпрвку ответов о приеме пакетов
	EN_DYN_ACK = 0		//Разрешение на команду W_TX_PAYLOAD_NOACK

}nRF24L01_FEATURE_t;

typedef enum {
	interrupt_error = 0,	//Флаг когда что-то пошло не так
	data_transmited = 1,	//Флаг отправки данных
	data_receved = 2,		//Флаг получения данных
	TX_data_overflow = 3	//Флаг по превышению лимита повтора передачи данных
}nRF24L01_INTERRUPTS_t;


//extern const uint8_t nRF24L01_RX_BUFFER_LEN;
extern uint8_t nRF24L01_RX_BUFFER[nRF24L01_BUFFER_LEN];

/*
 * Инициализация радиомодуля.
 */
uint8_t nRF24L01_init(SPI_HandleTypeDef* hspi);

/*
 * Функция для чтения данных.
 * Параметры: read_buffer - буффер, в которые приходят данные, buffer_size - размер буффера.
 * Возвращает "1" если данные пришли, "0" если данных нет.
 */
uint8_t nRF24L01_read (SPI_HandleTypeDef* hspi, uint8_t * read_buffer, size_t buffer_size, bool* isData);

/*
 * Функция для записи данных.
 * Параметры: write_buffer - буффер для записи данных, buffer_size - размер буффера, ACK - ответ (1 если нужен, 0 если нет).
 */
uint8_t nRF24L01_write (SPI_HandleTypeDef* hspi, void * write_buffer, size_t buffer_size, bool ACK, uint8_t* cmd);

/*
 * Позволяет записать адрес регистра (размер до 5 байт)
 * Параметры: address - адрес регистра, data_register - данные регистра, data_len - длина регистра.
 */
uint8_t nRF24L01_write_register_address (SPI_HandleTypeDef* hspi, nRF24L01_registr_addr_t address, uint8_t * data_register, size_t data_len);

/*
 * Позволяет записать данные в регистер
 * Параметры: address - адрес регистра, data_register - данные, которые надо записать.
 */
uint8_t nRF24L01_write_register (SPI_HandleTypeDef* hspi, nRF24L01_registr_addr_t address, uint8_t data_register);

/*
 * Позволяет прочитать адрес регистра
 * Параметры: address - адрес регистра, data_register - данные регистра, data_len - длина регистра.
 */
uint8_t nRF24L01_read_register_address (SPI_HandleTypeDef* hspi, nRF24L01_registr_addr_t address, uint8_t * data_register, size_t data_len);

/*
 * Позволяет прочитать регистр
 * Параметры: address - адрес регистра.
 * Возвращает данные регистра.
 */
uint8_t nRF24L01_read_register (SPI_HandleTypeDef* hspi, nRF24L01_registr_addr_t address, uint8_t* data);

/*
 * Позволяет прочитать статус регистр
 * Возвращает данные статус регстра
 */
uint8_t nRF24L01_read_status(SPI_HandleTypeDef* hspi, uint8_t* status);

/*
 * Позволяет поменять режим работы (RX or TX)
 * Параметры: mode - 1 - режим приема данных, 0 - режим передачи данных.
 */
uint8_t nRF24L01_RX_mode_on (SPI_HandleTypeDef* hspi, bool mode);

/*
 * Позволяет очистить статус регистр (погасить флаги: RX_DR, TX_DS, MAX_RT)
 * Параметры: flag_RX_DR - флаг получения входных данных, flag_TX_DS - флаг отправки выходных данных, flag_MAX_RT - флаг превышения лимита повторов передачи данных.
 * Гасит флаги если записана 1.
 */
uint8_t nRF24L01_clear_status (SPI_HandleTypeDef* hspi, bool flag_RX_DR, bool flag_TX_DS,bool flag_MAX_RT);

/*
 * Позволяет очистить TX_FIFO
 * Использует команду FLUSH_TX_FIFO
 */
uint8_t nRF24L01_clear_TX_FIFO (SPI_HandleTypeDef* hspi);

/*
 * Позволяет очистить RX_FIFO
 * Использует команду FLUSH_RX_FIFO
 */
uint8_t nRF24L01_clear_RX_FIFO (SPI_HandleTypeDef* hspi);


/*
 * Отправляем буфер через nRF24L01
 * Функция производит отправку буфера, учитывая максимальную длину пакета и ожидая отправки предыдущего
 */
uint8_t nRF24L01_send(SPI_HandleTypeDef* hspi, uint8_t* write_buffer, uint16_t buffer_size, bool ACK);

#endif /* NRF24L01_H_ */

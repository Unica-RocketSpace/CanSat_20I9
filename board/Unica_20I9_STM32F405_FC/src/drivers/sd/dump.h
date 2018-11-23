/*
 * dump.h
 *
 *  Created on: 05 окт. 2016 г.
 *      Author: snork
 */

#ifndef DUMP_H_
#define DUMP_H_

#include <stddef.h>
#include <stdbool.h>
#include <drivers/sd/fatfs/ff.h>


typedef struct
{
	FIL file;
	FRESULT res;
	int file_opened;
	const char * file_prefix;
	size_t sync_counter;
} dump_channel_state_t;



extern dump_channel_state_t _state;	// DUMP


bool dump_init(dump_channel_state_t* state);
bool dump(dump_channel_state_t* state, const void * data, size_t datasize);


#endif /* DUMP_H_ */

/*
 * dump.c
 *
 *  Created on: 05 окт. 2016 г.
 *      Author: snork
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <diag/Trace.h>

#include "FreeRTOS.h"
#include "task.h"

#include "drivers/sd/dump.h"
#include "state.h"


const char* file_extension = "bin";

//static FATFS fs;
//static bool fs_mounted = false;
//
//void dump_init(dump_state_t * state, const char * filename)
//{
//	FRESULT res;
//	size_t i = 0;
//
//	// монтируем диск
//	if (!fs_mounted )
//		do {
//			res = f_mount(&fs, "0", 1);
//			trace_printf("mount res = %d\n", res);
//			i++;
//			state->res = res;
//			if (i >= 3)
//			{
//				trace_printf("mount fails\n");
//				abort();
//			}
//		} while (res != FR_OK);
//	fs_mounted = true;
//
//	// выбираем имя файлу
//	char fname[15];
//	for (i = 0; i < 100; i++)
//	{
//		fname[0] ='\0';
//		sprintf(fname, "0:/%s%u.txt", filename, i);
//		FILINFO info;
//		if ((res = f_stat(fname, &info)) == FR_NO_FILE)
//		{
//			// отлично - такого файла нет!
//			trace_printf("dump file name '%s' ok\n", fname);
//			break;
//		}
//		else if (res == FR_OK)
//		{
//			//trace_printf("dump file name '%s' already exists\n", fname);
//		}
//		else
//		{
//			trace_printf("dump fname error '%s': err: %d\n", fname, res);
//			abort();
//		}
//	}
//
//	// предполагается что с именем файла мы всетаки определились раз мы тут
//	if (FR_OK != (res = f_open(&state->file, fname, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND)))
//	{
//		trace_printf("open error = %d\n", res);
//		abort();
//	}
//}
//
//void dump(dump_state_t * state, const void * data, size_t datasize)
//{
//	UINT dummy;
//	FRESULT res;
//	// пишем на флешку
//	if ((res = f_write(&state->file, data, datasize, &dummy)) != FR_OK) {
//		state->res = res;
//		fs_mounted = false;
//
//	taskENTER_CRITICAL();
//		state_system.SD_state = res;
//	taskEXIT_CRITICAL();
//	}
//
//	for (int i = 0; i < 5000; i++) {}
////		trace_printf("write error %d\n");
//
//	// было int_fast32_t
//	static int32_t sync_counter = 0;
//	if (sync_counter >= 10)
//	{
//		if ((res = f_sync(&state->file)) != FR_OK) {
//			state->res = res;
//		taskENTER_CRITICAL();
//			state_system.SD_state = res;
//		taskEXIT_CRITICAL();
//		}
////			trace_printf("sync error %d\n");
//	   sync_counter = 0;
//	   for (int i = 0; i < 5000; i++) {}
//	}
//	sync_counter++;
//}


static FATFS _fs;
static bool _fs_mounted = false;


static void _reset(dump_channel_state_t* state)
{
	_fs_mounted = false;
	state->file_opened = false;
}


static void _generate_fname(char * target, size_t number, const char* extension)
{
	sprintf(target, "0:/U%u.%s", number, extension);
}


bool dump_init(dump_channel_state_t* state)
{
	FRESULT res;
	size_t i = 0;

	if (_fs_mounted)
	{
		bool files_is_fine = true;
		if (!state->file_opened)
			files_is_fine = false;

		if (files_is_fine)
			return true;
	}


	// монтируем диск
	if (!_fs_mounted )
		do {
			res = f_mount(&_fs, "0", 1);
			trace_printf("mount res = %d\n", res);
			i++;
			state->res = res;
			if (i >= 3)
			{
				trace_printf("mount fails\n");
				_reset(state);
				return false;
			}
		} while (res != FR_OK);
	_fs_mounted = true;

	for (int i = 0; i < 50000; i++) {volatile int x = 0;}

	// выбираем имена файлам
	char fname[15];
	for (i = 0; i < SIZE_MAX; i++)
	{
		fname[0] ='\0';
		_generate_fname(fname, i, file_extension);
		FIL fp;
		res = f_open(&fp, fname, FA_OPEN_EXISTING);
		if (res == FR_NO_FILE)
		{
			state->file_opened = true;
			state->file_prefix = "U";
			state->sync_counter = 0;
			// отлично - такого файла нет!
			trace_printf("dump file name '%s' ok\n", fname);
		}
		else if (res == FR_OK)
		{
			state->file_opened = false;
			//trace_printf("dump file name '%s' already exists\n", fname);
			//f_close(&fp); пока попробуем без этого
			goto again;
		}
		else
		{
			state->file_opened = false;
			trace_printf("dump fname error '%s': err: %d\n", fname, res);
			// сдаемся на этой итерации
			_reset(state);
			return false;
		}

		// если все прошло ок, разрываем цикл
		break;

		again:
			continue;
	}


	// предполагается что с именем файла мы всетаки определились раз мы тут
	_generate_fname(fname, i, file_extension);
	if (FR_OK != (res = f_open(&state->file, fname, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND)))
	{
		trace_printf("open error = %d\n", res);
		// снова сдаемся
		_reset(state);
		return false;
	}

	// в противном случае у нас все ок
	state->file_opened = true;

	// раз мы тут, то все ок
	return true;
}


bool dump(dump_channel_state_t* state, const void * data, size_t datasize)
{
//	dump_channel_state_t * state = &_state;

	//	FIXME:
	// на случай, если что-то не так
	if (!dump_init(state))
		return false;

	UINT dummy;
	FRESULT res;

	// пишем на флешку
	for (int i = 0; i < 50000; i++) {volatile int x = 0;}
	if ((res = f_write(&state->file, data, datasize, &dummy)) != FR_OK)
	{
		state->res = res;
		trace_printf("write error %d\n", res);
		_reset(state);
		return false;
	}

	if (state->sync_counter >= 10)
	{
		if ((res = f_sync(&state->file)) != FR_OK)
		{
			state->res = res;
			trace_printf("sync error %d\n", res);
			_reset(state);
			return false;
		}

		state->sync_counter = 0;
	}
	state->sync_counter++;
	return true;
}


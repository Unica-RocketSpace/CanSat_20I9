/*
 * FreeRTOS-openocd.c
 *
 *  Created on: 2 июл. 2019 г.
 *      Author: developer
 */


#include "FreeRTOS.h"

#ifdef __GNUC__
#define USED __attribute__((used))
#else
#define USED
#endif

const int USED uxTopUsedPriority = configMAX_PRIORITIES - 1;

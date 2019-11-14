/*
 * timer.c
 *
 *  Created on: 2018Äê10ÔÂ23ÈÕ
 *      Author: Administrator
 */

#include <assert.h>

#include "timer.h"

#if defined S32K14x
#include "timer_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#include "timer_stm32fxxx.inc"
#else
#error Mcu type not defined!!!
#endif

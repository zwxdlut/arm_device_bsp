/*
 * timer.c
 *
 *  Created on: 2018��10��23��
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

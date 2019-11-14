/*
 * flexcan.c
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#include <string.h>
#include <assert.h>

#include "can.h"

#if defined S32K14x
#include "can_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#include "can_stm32fxxx.inc"
#else
#error Mcu type not defined!!!
#endif

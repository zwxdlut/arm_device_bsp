/*
 * i2c.c
 *
 *  Created on: 2019Äê1ÔÂ9ÈÕ
 *      Author: Administrator
 */

#include <string.h>
#include <assert.h>

#include "i2c.h"

#if defined S32K14x
#include "i2c_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#include "i2c_stm32fxxx.inc"
#else
#error Mcu type not defined!!!
#endif

/*
 * flash_ctrl.c
 *
 *  Created on: 2018Äê10ÔÂ17ÈÕ
 *      Author: Administrator
 */

#include <assert.h>

#include "flash_ctrl.h"

#if defined S32K14x
#include "flash_ctrl_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#include "flash_ctrl_stm32fxxx.inc"
#else
#error Mcu type not defined!!!
#endif

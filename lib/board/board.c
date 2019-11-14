/*
 * board.c
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#include "board.h"

#if defined S32K14x
#include "board_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#include "board_stm32fxxx.inc"
#else
#error Mcu type not defined!!!
#endif

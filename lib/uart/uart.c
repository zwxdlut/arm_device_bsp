/*
 * uart.c
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#include <string.h>
#include <assert.h>

#include "uart.h"

#if defined S32K14x
#include "uart_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#include "uart_stm32fxxx.inc"
#else
#error Mcu type not defined!!!
#endif

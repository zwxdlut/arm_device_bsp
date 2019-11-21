/*
 * board.c
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
#if defined S32K14x
#include "board_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#if defined USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
extern void xPortSysTickHandler(void);
#endif
#if defined USE_STDPERIPH_DRIVER
#include "board_stm32fxxx_stdperiph.inc"
#elif defined USE_HAL_DRIVER
#include "board_stm32fxxx_hal.inc"
#else
#error SDK type not defined!!!
#endif
#else
#error Mcu type not defined!!!
#endif

/*******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 * Local Functions
 ******************************************************************************/

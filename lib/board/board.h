/*
 * board.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdint.h>

#if defined S32K14x
#include "board_s32k1xx.h"
#elif defined STM32F10X_CL || defined STM32F205xx
#if defined STM32F10X_CL
#include "stm32f10x.h"
#elif defined STM32F205xx
#include "stm32f2xx.h"
#else
#endif
#if defined USE_STDPERIPH_DRIVER
#include "board_stm32fxxx_stdperiph.h"
#elif defined USE_HAL_DRIVER
#include "board_stm32fxxx_hal.h"
#else
#error SDK type not defined!!!
#endif
#else
#error Mcu type not defined!!!
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup Power modes.
 * @{
 */
#define PWR_MODE_SLEEP                          0
#define PWR_MODE_DEEPSLEEP                      1
/** @} */ /* End of group Power modes. */

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * @brief  Initialize clocks, etc...
 *
 * @return Success(0) or failure(other values).
 */
int32_t sys_init(void);

/**
 * @brief Initialize GPIO.
 */
void gpio_init(void);

/**
 * @brief Deinitialize GPIO.
 */
void gpio_deinit(void);

/**
 * @brief  Get current system time since startup.
 *
 * @return Time in milliseconds.
 */
uint32_t sys_time_ms(void);

/**
 * @brief Delay milliseconds function.
 *
 * @param [in] _ms Time in milliseconds.
 */
void delay_ms(const uint32_t _ms);

/**
 * @brief Reset system.
 */
void sys_reset(void);

/**
 * @brief Transfer power mode.
 *
 * @param [in] _mode Power mode:
 *             @arg PWR_MODE_SLEEP Sleep.
 *             @arg PWR_MODE_DEEPSLEEP Deep sleep.
 */
void pwr_mode_trans(const uint8_t _mode);

/**
 * @brief Enable watch dog.
 *
 * @return Success(0) or failure(other values).
 */
int32_t wdog_enable(void);

/**
 * @brief Refresh watch dog.
 *
 * @return Success(0) or failure(other values).
 */
int32_t wdog_refresh(void);

/**
 * @brief Disable watch dog.
 *
 * @return Success(0) or failure(other values).
 */
int32_t wdog_disable(void);

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H__ */

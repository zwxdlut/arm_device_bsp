/*
 * timer.h
 *
 *  Created on: 2018Äê10ÔÂ23ÈÕ
 *      Author: Administrator
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include <stdint.h>

/******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup Timer configuration.
 * @{
 */
#if defined S32K14x
#include "Cpu.h"
#define TIMER0_IRQ                              LPTMR0_IRQn
#elif defined STM32F10X_CL || STM32F205xx
#if defined STM32F10X_CL
#include "stm32f10x.h"
#elif defined STM32F205xx
#include "stm32f2xx.h"
#endif
#define TIMER0_INST                             TIM3
#define TIMER0_CLK                              10000
#define TIMER0_IRQ                              TIM3_IRQn
#define TIMER0_IRQ_HANDLER                      TIM3_IRQHandler
#if defined USE_STDPERIPH_DRIVER
#define TIMER_CLK_ENABLE(INDEX)                 do { if(0 == (INDEX)) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);} else\
                                                                      {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);}} while(0)
#define TIMER_CLK_DISABLE(INDEX)                do { if(0 == (INDEX)) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);} else\
                                                                      {RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);}} while(0)
#elif defined USE_HAL_DRIVER
#define TIMER_CLK_ENABLE(INDEX)                 do { if(0 == (INDEX)) {__HAL_RCC_TIM3_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_TIM3_CLK_ENABLE();}} while(0)
#define TIMER_CLK_DISABLE(INDEX)                do { if(0 == (INDEX)) {__HAL_RCC_TIM3_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_TIM3_CLK_DISABLE();}} while(0)
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#else
#error Mcu type not defined!!!
#endif
/** @} */ /* End of group Timer configuration. */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup Timer module index.
 * @{
 */
#define TIMER0_INDEX                            0
/** @} */ /* End of group Timer module index. */
																	  
/******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * @brief  Initialize timer.
 *
 * @param  [in] _index  Timer index.
 * @param  [in] _period Timer period in milliseconds.
 * @return Success(0) or failure(other values).
 */

int32_t timer_init(const uint8_t _index, const uint32_t _period);

/**
 * @brief  Deinitialize timer.
 *
 * @param  [in] _index Timer index.
 * @return Success(0) or failure(other values).
 */
int32_t timer_deinit(const uint8_t _index);

/**
 * @brief  Start timer.
 *
 * @param  [in] _index Timer index.
 * @return Success(0) or failure(other values).
 */
int32_t timer_start(const uint8_t _index);

/**
 * @brief  Stop timer.
 *
 * @param  [in] _index Timer index.
 * @return Success(0) or failure(other values).
 */
int32_t timer_stop(const uint8_t _index);

/**
 * @brief   Timer IRQ callback.
 *
 * @details This weak function can be implemented by user.
 *
 * @param   [in] _index Timer index.
 */
void timer_irq_callback(const uint8_t _index);

#ifdef __cplusplus
}
#endif

#endif /* __TIMER_H__ */

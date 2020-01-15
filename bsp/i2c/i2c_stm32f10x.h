/*
 * i2c_stm32f10x.h
 *
 *  Created on: 2019Äê1ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef __I2C_STM32F10X_H__
#define __I2C_STM32F10X_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Definitions
 ******************************************************************************/
/** 
 * @defgroup I2C configuration.
 * @{
 */
#define I2C0_INST     		                    I2C1
#define I2C0_EV_IRQ                             I2C1_EV_IRQn
#define I2C0_ER_IRQ                             I2C1_ER_IRQn
#define I2C0_EV_IRQ_HANDLER                     I2C1_EV_IRQHandler
#define I2C0_ER_IRQ_HANDLER                     I2C1_ER_IRQHandler
/** @} */ /* End of I2C configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_STM32F10X_H__ */

/*
 * i2c_s32k1xx.h
 *
 *  Created on: 2019Äê1ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef __I2C_S32K1XX_H__
#define __I2C_S32K1XX_H__

#include "Cpu.h"

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
#if defined MX_TB || defined EVB || defined FRDM
#define I2C0_PORT                               PORTA
#define I2C0_SCL_PIN                            3
#define I2C0_SDA_PIN                            2
#define I2C0_GPIO_AF                            PORT_MUX_ALT3
#define I2C0_IRQ                                LPI2C0_Master_IRQn
/** @} */ /* End of I2C configuration. */
#else
#error Board type not defined!!!
#endif

#ifdef __cplusplus
}
#endif

#endif /* __I2C_S32K1XX_H__ */

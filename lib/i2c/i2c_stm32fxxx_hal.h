/*
 * i2c_stm32fxxx.h
 *
 *  Created on: 2019Äê1ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef __I2C_STM32FXXX_HAL_H__
#define __I2C_STM32FXXX_HAL_H__

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
#define I2C0_GPIO                               GPIOB
#define I2C0_SCL_PIN                            GPIO_PIN_6
#define I2C0_SDA_PIN                            GPIO_PIN_7
#define I2C0_GPIO_AF                            GPIO_AF4_I2C1
#define I2C0_INST     		                    I2C1
#define I2C0_EV_IRQ                             I2C1_EV_IRQn
#define I2C0_ER_IRQ                             I2C1_ER_IRQn
#define I2C_GPIO_CLK_ENABLE(INDEX)              do { if(0 == (INDEX)) {__HAL_RCC_GPIOB_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_ENABLE();}} while(0)
#define I2C_GPIO_CLK_DISABLE(INDEX)             do { if(0 == (INDEX)) {__HAL_RCC_GPIOB_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_DISABLE();}} while(0)
#define I2C_CLK_ENABLE(INDEX)                   do { if(0 == (INDEX)) {__HAL_RCC_I2C1_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_I2C1_CLK_ENABLE();}} while(0)
#define I2C_CLK_DISABLE(INDEX)                  do { if(0 == (INDEX)) {__HAL_RCC_I2C1_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_I2C1_CLK_DISABLE();}} while(0)
#define I2C_FORCE_RESET(INDEX)                  do { if(0 == (INDEX)) {__HAL_RCC_I2C1_FORCE_RESET();} else\
                                                                      {__HAL_RCC_I2C1_FORCE_RESET();}} while(0)
#define I2C_RELEASE_RESET(INDEX)                do { if(0 == (INDEX)) {__HAL_RCC_I2C1_RELEASE_RESET();} else\
                                                                      {__HAL_RCC_I2C1_RELEASE_RESET();}} while(0)
#define I2C0_EV_IRQ_HANDLER                     I2C1_EV_IRQHandler
#define I2C0_ER_IRQ_HANDLER                     I2C1_ER_IRQHandler
/** @} */ /* End of I2C configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_STM32FXXX_HAL_H__ */

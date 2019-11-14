/*
 * can_stm32fxxx.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __CAN_STM32FXXX_H__
#define __CAN_STM32FXXX_H__

#if defined STM32F10X_CL
#include "stm32f10x.h"
#elif defined STM32F205xx
#include "stm32f2xx.h"
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
 * @defgroup CAN configuration.
 * @{
 */
#if defined MX_TB
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
#define CAN0_GPIO                               GPIOB
#define CAN0_RX_PIN                             GPIO_PIN_8
#define CAN0_TX_PIN                             GPIO_PIN_9
#define CAN0_GPIO_AF                            GPIO_AF9_CAN1
#define CAN0_INST     		                    CAN1
#define CAN0_RX_IRQ                             CAN1_RX0_IRQn
#define CAN0_TRANS_STB_N_GPIO                   GPIOC
#define CAN0_TRANS_STB_N_PIN                    GPIO_PIN_7
#define CAN0_TRANS_EN_GPIO                      GPIOC
#define CAN0_TRANS_EN_PIN                       GPIO_PIN_9
#define CAN0_TRANS_INH_GPIO                     GPIOC
#define CAN0_TRANS_INH_PIN                      GPIO_PIN_8
#define CAN0_TRANS_INH_IRQ                      EXTI9_5_IRQn
#define CAN1_GPIO                               GPIOB
#define CAN1_RX_PIN                             GPIO_PIN_12
#define CAN1_TX_PIN                             GPIO_PIN_13
#define CAN1_GPIO_AF                            GPIO_AF9_CAN2
#define CAN1_INST     		                    CAN2
#define CAN1_RX_IRQ                             CAN2_RX0_IRQn
#define CAN1_TRANS_STB_N_GPIO                   GPIOB
#define CAN1_TRANS_STB_N_PIN                    GPIO_PIN_10
#define CAN1_TRANS_EN_GPIO                      GPIOB
#define CAN1_TRANS_EN_PIN                       GPIO_PIN_14
#define CAN1_TRANS_INH_GPIO                     GPIOB
#define CAN1_TRANS_INH_PIN                      GPIO_PIN_15
#define CAN1_TRANS_INH_IRQ                      EXTI15_10_IRQn
#define CAN_GPIO_CLK_ENABLE(INDEX)              do { if(0 == (INDEX)) {__HAL_RCC_GPIOB_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_ENABLE();}} while(0)
#define CAN_GPIO_CLK_DISABLE(INDEX)             do { if(0 == (INDEX)) {__HAL_RCC_GPIOB_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_DISABLE();}} while(0)
#define CAN_CLK_ENABLE(INDEX)                   do { if(0 == (INDEX)) {__HAL_RCC_CAN1_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_CAN2_CLK_ENABLE();}} while(0)
#define CAN_CLK_DISABLE(INDEX)                  do { if(0 == (INDEX)) {__HAL_RCC_CAN1_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_CAN2_CLK_DISABLE();}} while(0)
#define CAN_FORCE_RESET(INDEX)                  do { if(0 == (INDEX)) {__HAL_RCC_CAN1_FORCE_RESET();} else\
                                                                      {__HAL_RCC_CAN2_FORCE_RESET();}} while(0)
#define CAN_RELEASE_RESET(INDEX)                do { if(0 == (INDEX)) {__HAL_RCC_CAN1_RELEASE_RESET();} else\
                                                                      {__HAL_RCC_CAN2_RELEASE_RESET();}} while(0)
#define CAN_TRANS_STB_N_GPIO_CLK_ENABLE(INDEX)  do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_ENABLE();}} while(0)
#define CAN_TRANS_STB_N_GPIO_CLK_DISABLE(INDEX) do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_DISABLE();}} while(0)
#define CAN_TRANS_EN_GPIO_CLK_ENABLE(INDEX)     do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_ENABLE();}} while(0)
#define CAN_TRANS_EN_GPIO_CLK_DISABLE(INDEX)    do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_DISABLE();}} while(0)
#define CAN_TRANS_INH_GPIO_CLK_ENABLE(INDEX)    do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_ENABLE();}} while(0)
#define CAN_TRANS_INH_GPIO_CLK_DISABLE(INDEX)   do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOB_CLK_DISABLE();}} while(0)
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#define CAN0_TRANS_INH_IRQ_HANDLER              EXTI9_5_IRQHandler
#define CAN1_TRANS_INH_IRQ_HANDLER              EXTI15_10_IRQHandler
#else
#if defined USE_STDPERIPH_DRIVER
#define CAN0_GPIO                               GPIOB
#define CAN0_RX_PIN                             GPIO_Pin_8
#define CAN0_TX_PIN                             GPIO_Pin_9
#define CAN0_INST     		                    CAN1
#define CAN0_RX_IRQ                             CAN1_RX0_IRQn
#define CAN1_GPIO                               GPIOB
#define CAN1_RX_PIN                             GPIO_Pin_12
#define CAN1_TX_PIN                             GPIO_Pin_13
#define CAN1_INST     		                    CAN2
#define CAN1_RX_IRQ                             CAN2_RX0_IRQn
#define CAN_GPIO_CLK_ENABLE(INDEX)              do { if(0 == (INDEX)) {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);} else\
                                                                      {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}} while(0)
#define CAN_GPIO_CLK_DISABLE(INDEX)             do { if(0 == (INDEX)) {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE)();} else\
                                                                      {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE)();}} while(0)
#define CAN_CLK_ENABLE(INDEX)                   do { if(0 == (INDEX)) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);} else\
                                                                      {RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);}} while(0)
#define CAN_CLK_DISABLE(INDEX)                  do { if(0 == (INDEX)) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE);} else\
                                                                      {RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, DISABLE);}} while(0)
#define CAN_FORCE_RESET(INDEX)                  do { if(0 == (INDEX)) {RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, ENABLE);} else\
                                                                      {RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);}} while(0)
#define CAN_RELEASE_RESET(INDEX)                do { if(0 == (INDEX)) {RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);} else\
                                                                      {RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);}} while(0)
#elif defined USE_HAL_DRIVER
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#endif

#define CAN0_RX_IRQ_HANDLER                     CAN1_RX0_IRQHandler
#define CAN1_RX_IRQ_HANDLER                     CAN2_RX0_IRQHandler
#define CAN_SLAVE_START_FILTER_BANK_NUM         21 /**< Slave CAN start filter bank number */
/** @} */ /* End of group CAN configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_STM32FXXX_H__ */

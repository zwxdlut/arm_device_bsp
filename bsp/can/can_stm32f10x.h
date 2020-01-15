/*
 * can_stm32f10x.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __CAN_STM32F10X_H__
#define __CAN_STM32F10X_H__

#include "stm32f10x.h"

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
#define CAN0_TRANS_INH_IRQ_HANDLER              EXTI9_5_IRQHandler
#define CAN1_TRANS_INH_IRQ_HANDLER              EXTI15_10_IRQHandler
#else
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
#define CAN0_RX_IRQ_HANDLER                     CAN1_RX0_IRQHandler
#define CAN1_RX_IRQ_HANDLER                     CAN2_RX0_IRQHandler
#endif
/** @} */ /* End of group CAN configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_STM32F10X_H__ */

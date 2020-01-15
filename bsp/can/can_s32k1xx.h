/*
 * can_s32k1xx.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __CAN_S32K1XX_H__
#define __CAN_S32K1XX_H__

#include "Cpu.h"

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
#define CAN0_PORT                               PORTE
#define CAN0_RX_PIN                             4
#define CAN0_TX_PIN                             5
#define CAN0_GPIO_AF                            PORT_MUX_ALT5
#define CAN0_ORED_IRQ                           CAN0_ORed_IRQn
#define CAN0_ERROR_IRQ                          CAN0_Error_IRQn
#define CAN0_WAKEUP_IRQ                         CAN0_Wake_Up_IRQn
#define CAN0_ORED_0_15_IRQ                      CAN0_ORed_0_15_MB_IRQn
#define CAN0_ORED_16_31_IRQ                     CAN0_ORed_16_31_MB_IRQn
#define CAN0_TRANS_STB_N_PORT				    PORTC
#define CAN0_TRANS_STB_N_GPIO    			    PTC
#define CAN0_TRANS_STB_N_PIN				    8
#define CAN0_TRANS_EN_PORT				        PORTC
#define CAN0_TRANS_EN_GPIO    			        PTC
#define CAN0_TRANS_EN_PIN 					    9
#define CAN0_TRANS_INH_PORT				        PORTC
#define CAN0_TRANS_INH_GPIO    			        PTC
#define CAN0_TRANS_INH_PIN					    17
#define CAN0_TRANS_INH_IRQ                      PORTC_IRQn
#define CAN1_PORT                               PORTC
#define CAN1_RX_PIN                             6
#define CAN1_TX_PIN                             7
#define CAN1_GPIO_AF                            PORT_MUX_ALT3
#define CAN1_ORED_IRQ                           CAN1_ORed_IRQn
#define CAN1_ERROR_IRQ                          CAN1_Error_IRQn
#define CAN1_ORED_0_15_IRQ                      CAN1_ORed_0_15_MB_IRQn
#define CAN1_TRANS_STB_N_PORT				    PORTC
#define CAN1_TRANS_STB_N_GPIO				    PTC
#define CAN1_TRANS_STB_N_PIN				    14
#define CAN1_TRANS_EN_PORT    				    PORTC
#define CAN1_TRANS_EN_GPIO    				    PTC
#define CAN1_TRANS_EN_PIN 					    15
#define CAN1_TRANS_INH_PORT				        PORTB
#define CAN1_TRANS_INH_GPIO    			        PTB
#define CAN1_TRANS_INH_PIN					    2
#define CAN1_TRANS_INH_IRQ  		            PORTB_IRQn
#else
#define CAN0_PORT                               PORTE
#define CAN0_RX_PIN                             4
#define CAN0_TX_PIN                             5
#define CAN0_GPIO_AF                            PORT_MUX_ALT5
#define CAN0_ORED_IRQ                           CAN0_ORed_IRQn
#define CAN0_ERROR_IRQ                          CAN0_Error_IRQn
#define CAN0_WAKEUP_IRQ                         CAN0_Wake_Up_IRQn
#define CAN0_ORED_0_15_IRQ                      CAN0_ORed_0_15_MB_IRQn
#define CAN0_ORED_16_31_IRQ                     CAN0_ORed_16_31_MB_IRQn
#define CAN1_PORT                               PORTC
#define CAN1_RX_PIN                             16
#define CAN1_TX_PIN                             17
#define CAN1_GPIO_AF                            PORT_MUX_ALT3
#define CAN1_ORED_IRQ                           CAN1_ORed_IRQn
#define CAN1_ERROR_IRQ                          CAN1_Error_IRQn
#define CAN1_ORED_0_15_IRQ                      CAN1_ORed_0_15_MB_IRQn
#endif
/** @} */ /* End of group CAN configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __CAN_S32K1XX_H__ */

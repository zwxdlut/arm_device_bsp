/*
 * board_s32k1xx.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __BOARD_S32K1XX_H__
#define __BOARD_S32K1XX_H__

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined MX_TB
/** 
 * @defgroup LEDs configuration.
 * @{
 */
#define LED0_PORT       					    PORTE
#define LED0_GPIO       					    PTE
#define LED0_PIN           					    8
#define LED1_PORT       					    PORTB
#define LED1_GPIO       					    PTB
#define LED1_PIN           					    5
#define LED2_PORT       					    PORTB
#define LED2_GPIO       					    PTB
#define LED2_PIN           					    4
#define LED_ON          					    1
#define LED_OFF         					    0
/** @} */ /* End of group LEDs configuration. */
 
/** 
 * @defgroup Button configuration.
 * @{
 */                                       
#define BTN_PORT        					    PORTA
#define BTN_GPIO        					    PTA
#define BTN_PIN         					    0
#define BTN_IRQ                                 PORTA_IRQn
/** @} */ /* End of group Button configuration. */

/**
 * @defgroup Upper computer configuration.
 * @{
 */
#define UC_POWER_PORT						    PORTE
#define UC_POWER_GPIO 					        PTE
#define UC_POWER_PIN						    2
#define UC_WAKEUP_PORT						    PORTA
#define UC_WAKEUP_GPIO 					        PTA
#define UC_WAKEUP_PIN						    12
#define UC_RESET_PORT						    PORTA
#define UC_RESET_GPIO 					        PTA
#define UC_RESET_PIN						    13
/** @} */ /* End of group Upper computer configuration. */

/**
 * @defgroup Ignition configuration.
 * @{
 */
#define IGN_PORT        					    PORTD
#define IGN_GPIO        					    PTD
#define IGN_PIN         					    16
#define IGN_IRQ                                 PORTD_IRQn
/** @} */ /* End of group Ignition configuration. */
#elif defined EVB                               
/**
 * @defgroup LEDs configuration.
 * @{
 */
#define LED0_PORT       					    PORTD
#define LED0_GPIO      						    PTD
#define LED0_PIN          					    0
#define LED1_PORT       					    PORTD
#define LED1_GPIO       					    PTD
#define LED1_PIN           					    16
#define LED2_PORT       					    PORTD
#define LED2_GPIO      						    PTD
#define LED2_PIN           					    15
#define LED_ON          					    0
#define LED_OFF         					    1
/** @} */ /* End of group LEDs configuration. */

/**
 * @defgroup Button configuration.
 * @{
 */
#define BTN_PORT        					    PORTC
#define BTN_GPIO        					    PTC
#define BTN_PIN         					    13
#define BTN_IRQ                                 PORTC_IRQn
/** @} */ /* End of group Button configuration. */
#elif defined FRDM
/**
 * @defgroup LEDs configuration.
 * @{
 */
#define LED0_PORT       					    PORTE
#define LED0_GPIO       					    PTE
#define LED0_PIN           					    8
#define LED1_PORT       					    PORTB
#define LED1_GPIO       					    PTB
#define LED1_PIN           					    5
#define LED2_PORT       					    PORTB
#define LED2_GPIO       					    PTB
#define LED2_PIN        	    			    4
#define LED_ON          					    1
#define LED_OFF         					    0
/** @} */ /* End of group LEDs configuration. */

/**
 * @defgroup Button configuration.
 * @{
 */
#define BTN_PORT        					    PORTD
#define BTN_GPIO        					    PTD
#define BTN_PIN         					    15
#define BTN_IRQ                                 PORTD_IRQn
/** @} */ /* End of group Button configuration. */
#else
#error Board type not defined!!!
#endif                                          

/**
 * @defgroup Pin operation macros.
 * @{
 */
#define GPIO_WRITE_PIN(GPIO, PIN, LEVEL)        PINS_DRV_WritePin(GPIO, PIN, LEVEL)
#define GPIO_TOGGLE_PIN(GPIO, PIN)              PINS_DRV_TogglePins(GPIO, (1 << PIN))
#define GPIO_READ_PIN(GPIO, PIN)                ((1 << PIN) & PINS_DRV_ReadPins(GPIO))
/** @} */ /* End of group Pin operation macros. */

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_S32K1XX_H__ */

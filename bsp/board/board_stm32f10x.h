/*
 * board_stm32f10x.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __BOARD_STM32F10X_H__
#define __BOARD_STM32F10X_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined CPATAIN_DESIGN
/**
 * @defgroup LEDs configuration.
 * @{
 */
#define LED0_GPIO       					    GPIOC
#define LED0_PIN            			        GPIO_Pin_0
#define LED0_GPIO_CLK_ENABLE()                  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define LED0_GPIO_CLK_DISABLE()                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE)
#define LED1_GPIO       					    GPIOC
#define LED1_PIN            			        GPIO_Pin_1
#define LED1_GPIO_CLK_ENABLE()                  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define LED1_GPIO_CLK_DISABLE()                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE)
#define LED2_GPIO       					    GPIOC
#define LED2_PIN            			        GPIO_Pin_2
#define LED2_GPIO_CLK_ENABLE()                  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE)
#define LED2_GPIO_CLK_DISABLE()                 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, DISABLE)
#define LED_ON          					    Bit_RESET
#define LED_OFF         					    Bit_SET
/** @} */ /* End of group LEDs configuration. */

/**
 * @defgroup Button configuration.
 * @{
 */
#define BTN_GPIO        					    GPIOB
#define BTN_PIN         					    GPIO_Pin_14
#define BTN_PORT_SRC        				    GPIO_PortSourceGPIOB
#define BTN_PIN_SRC        				        GPIO_PinSource14
#define BTN_EXTI_LINE      				        EXTI_Line14
#define BTN_IRQ         				        EXTI15_10_IRQn
#define BTN_IRQ_HANDLER                         EXTI15_10_IRQHandler
#define BTN_GPIO_CLK_ENABLE()                   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define BTN_GPIO_CLK_DISABLE()                  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE)
/** @} */ /* End of group Button configuration. */
#else
#error Board type not defined!!!
#endif

/**
 * @defgroup Pin operation macros.
 * @{
 */
#define GPIO_INIT(GPIO, INIT)                   GPIO_Init(GPIO, INIT)
#define GPIO_DEINIT(GPIO, PIN)                  GPIO_DeInit(GPIO)
#define GPIO_WRITE_PIN(GPIO, PIN, LEVEL)        GPIO_WriteBit(GPIO, PIN, (BitAction)LEVEL)
#define GPIO_TOGGLE_PIN(GPIO, PIN)              GPIO_WriteBit(GPIO, PIN, (Bit_RESET == GPIO_ReadOutputDataBit(GPIO, PIN) ? Bit_SET : Bit_RESET))
#define GPIO_READ_PIN(GPIO, PIN)                GPIO_ReadInputDataBit(GPIO, PIN)
/** @} */ /* End of group Pin operation macros. */

/**
 * @defgroup Independent watch dog.
 *
 * @details  Tout = (4*2^prv) / LSI * rlv (ms).
 *           LSI: LSI clock in kHz.
 *           prv: Prescaler(IWDOG_PRV).
 *           rlv: Reload value(IWDOG_RLV).
 * @{
 */
#define IWDOG_PRV                               IWDG_Prescaler_8
#define IWDOG_RLV                               0xFFF
/** @} */ /* End of group Independent watch dog. */

/**
 * @defgroup Window watch dog.
 *
 * @details  Tout = T(PCLK1) * 4096 * 2^WDGTB * (T[5:0]+1) (ms).
 *           T(PCLK1): APB1 clock period in milliseconds.
 *           WDGTB: Prescaler(WWDOG_PRV).
 *           T[5:0]: Download counter lower 6 bits(T[5:0] + 1 = WWDOG_RLV).
 */
#define WWDOG_PRV                               WWDG_Prescaler_8
#define WWDOG_WV                                0x7F
#define WWDOG_RLV                               0x7F
/** @} */ /* End of group Window watch dog. */

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_STM32F10X_H__ */

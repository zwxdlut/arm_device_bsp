/*
 * board_stm32fxxx.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __BOARD_STM32FXXX_H__
#define __BOARD_STM32FXXX_H__

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
#if defined MX_TB
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
/** 
 * @defgroup LEDs configuration.
 * @{
 */
#define LED0_GPIO       					    GPIOC
#define LED0_PIN            			        GPIO_PIN_2
#define LED0_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED0_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOC_CLK_DISABLE()
#define LED1_GPIO       					    GPIOC
#define LED1_PIN            			        GPIO_PIN_1
#define LED1_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOC_CLK_DISABLE()
#define LED2_GPIO       					    GPIOC
#define LED2_PIN            			        GPIO_PIN_0
#define LED2_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOC_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOC_CLK_DISABLE()
/** @} */ /* End of group LEDs configuration. */
 
/** 
 * @defgroup Button configuration.
 * @{
 */                                       
#define BTN_GPIO        					    GPIOC
#define BTN_PIN         					    GPIO_PIN_3
#define BTN_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BTN_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BTN_IRQ         				        EXTI3_IRQn
/** @} */ /* End of group Button configuration. */

/**
 * @defgroup Upper computer configuration.
 * @{
 */
#define UC_POWER_GPIO 					        GPIOB
#define UC_POWER_PIN						    GPIO_PIN_0
#define UC_POWER_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
#define UC_POWER_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOB_CLK_DISABLE()
#define UC_WAKEUP_GPIO 					        GPIOB
#define UC_WAKEUP_PIN						    GPIO_PIN_1
#define UC_WAKEUP_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define UC_WAKEUP_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define UC_RESET_GPIO 					        GPIOC
#define UC_RESET_PIN						    GPIO_PIN_5
#define UC_RESET_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()
#define UC_RESET_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOC_CLK_DISABLE()
/** @} */ /* End of group Upper computer configuration. */

/**
 * @defgroup Ignition configuration.
 * @{
 */
#define IGN_GPIO        					    GPIOC
#define IGN_PIN         					    GPIO_PIN_4
#define IGN_GPIO_CLK_ENABLE()                   __HAL_RCC_GPIOC_CLK_ENABLE()
#define IGN_GPIO_CLK_DISABLE()                  __HAL_RCC_GPIOC_CLK_DISABLE()
#define IGN_IRQ   					            EXTI4_IRQn
/** @} */ /* End of group Ignition configuration. */
#else
#error SDK type not defined!!!
#endif
/**
 * @defgroup IRQ handlers.
 * @{
 */
#define BTN_IRQ_HANDLER                         EXTI3_IRQHandler
#define IGN_IRQ_HANDLER                         EXTI4_IRQHandler
/** @} */ /* End of group IRQ handlers. */
#else
#if defined USE_STDPERIPH_DRIVER
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
#define BTN_GPIO_CLK_ENABLE()                   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define BTN_GPIO_CLK_DISABLE()                  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE)
#define BTN_PORT_SRC        				    GPIO_PortSourceGPIOB
#define BTN_PIN_SRC        				        GPIO_PinSource14
#define BTN_EXTI_LINE      				        EXTI_Line14
#define BTN_IRQ         				        EXTI15_10_IRQn
/** @} */ /* End of group Button configuration. */
#elif defined USE_HAL_DRIVER
#else
#error SDK type not defined!!!
#endif
/**
 * @defgroup IRQ handlers.
 * @{
 */
#define BTN_IRQ_HANDLER                         EXTI15_10_IRQHandler
/** @} */ /* End of group IRQ handlers. */
#endif

#if defined USE_STDPERIPH_DRIVER
#define LED_ON          					    Bit_RESET
#define LED_OFF         					    Bit_SET

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
#elif defined USE_HAL_DRIVER
#define LED_ON          					    GPIO_PIN_SET
#define LED_OFF         					    GPIO_PIN_RESET

/**
 * @defgroup Pin operation macros.
 * @{
 */
#define GPIO_INIT(GPIO, INIT)                   HAL_GPIO_Init(GPIO, INIT)
#define GPIO_DEINIT(GPIO, PIN)                  HAL_GPIO_DeInit(GPIO, PIN);
#define GPIO_WRITE_PIN(GPIO, PIN, LEVEL)        HAL_GPIO_WritePin(GPIO, PIN, (GPIO_PinState)LEVEL)
#define GPIO_TOGGLE_PIN(GPIO, PIN)              HAL_GPIO_TogglePin(GPIO, PIN)
#define GPIO_READ_PIN(GPIO, PIN)                HAL_GPIO_ReadPin(GPIO, PIN)
/** @} */ /* End of group Pin operation macros. */

/**
 * @defgroup Independent watch dog.
 *
 * @details  Tout = (4*2^prv) / LSI * rlv (ms).
 * 			 LSI: LSI clock in kHz.
 *           prv: Prescaler(IWDOG_PRV).
 *           rlv: Reload value(IWDOG_RLV).
 */
#define IWDOG_PRV                               IWDG_PRESCALER_8
#define IWDOG_RLV                               0xFFF
/** @} */ /* End of group Independent watch dog. */

/**
 * @defgroup Window watch dog.
 *
 * @details  Tout = 4096 * 2^WDGTB / PCLK1 * (T[5:0]+1) (ms).
 *           PCLK1:  APB1 clock.
 *           WDGTB:  Watch dog timer base(WWDOG_PRV >> WWDG_CFR_WDGTB_Pos).
 *           T[5:0]: Download counter lower 6 bits(T[6:0] = WWDOG_RLV).
 */
#define WWDOG_PRV                               WWDG_PRESCALER_8
#define WWDOG_WV                                0x7F
#define WWDOG_RLV                               0x7F
/** @} */ /* End of group Window watch dog. */
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_STM32FXXX_H__ */

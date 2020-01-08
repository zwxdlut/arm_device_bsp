/*
 * board_stm32fxxx_hal.h
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef __BOARD_STM32FXXX_HAL_H__
#define __BOARD_STM32FXXX_HAL_H__

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
#define UC_POWER_GPIO 					        GPIOC
#define UC_POWER_PIN						    GPIO_PIN_5
#define UC_POWER_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOC_CLK_ENABLE()
#define UC_POWER_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOC_CLK_DISABLE()
#define UC_WAKEUP_GPIO 					        GPIOB
#define UC_WAKEUP_PIN						    GPIO_PIN_1
#define UC_WAKEUP_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
#define UC_WAKEUP_GPIO_CLK_DISABLE()            __HAL_RCC_GPIOB_CLK_DISABLE()
#define UC_RESET_GPIO 					        GPIOB
#define UC_RESET_PIN						    GPIO_PIN_0
#define UC_RESET_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
#define UC_RESET_GPIO_CLK_DISABLE()             __HAL_RCC_GPIOB_CLK_DISABLE()
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

/**
 * @defgroup IRQ handlers.
 * @{
 */
#define BTN_IRQ_HANDLER                         EXTI3_IRQHandler
#define IGN_IRQ_HANDLER                         EXTI4_IRQHandler
/** @} */ /* End of group IRQ handlers. */
#else
/**
 * @defgroup IRQ handlers.
 * @{
 */
#define BTN_IRQ_HANDLER                         EXTI15_10_IRQHandler
/** @} */ /* End of group IRQ handlers. */
#endif

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

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_STM32FXXX_HAL_H__ */

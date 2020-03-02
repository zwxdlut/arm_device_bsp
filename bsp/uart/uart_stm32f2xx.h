/*
 * uart_stm32f2xx.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __UART_STM32F2XX_H__
#define __UART_STM32F2XX_H__

#include "stm32f2xx.h"
#include "stm32f2xx_ll_usart.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup UART data bits.
 * @{
 */
#define UART_DATA_BITS_8                       (uint32_t)UART_WORDLENGTH_8B
#define UART_DATA_BITS_9                       (uint32_t)UART_WORDLENGTH_9B
/** @} */ /* End of group UART data bits. */

/**
 * @defgroup UART stop bits.
 * @{
 */
#define UART_STOP_BITS_1                        (uint32_t)UART_STOPBITS_1
#define UART_STOP_BITS_2                        (uint32_t)UART_STOPBITS_2
/** @} */ /* End of group UART stop bits. */

/**
 * @defgroup UART parity.
 * @{
 */
#define UART_PARITY_MODE_NONE                   (uint32_t)UART_PARITY_NONE
#define UART_PARITY_MODE_EVEN                   (uint32_t)UART_PARITY_EVEN
#define UART_PARITY_MODE_ODD                    (uint32_t)UART_PARITY_ODD
/** @} */ /* End of group UART parity. */

/**
 * @defgroup UART configuration.
 * @{
 */
#if defined MX_TB
#define UART0_GPIO                              GPIOA
#define UART0_RX_PIN                            GPIO_PIN_3
#define UART0_TX_PIN                            GPIO_PIN_2
#define UART0_GPIO_AF                           GPIO_AF7_USART2
#define UART0_INST     		                    USART2
#define UART0_IRQ                               USART2_IRQn
#define UART0_IRQ_HANDLER                       USART2_IRQHandler
#define UART1_GPIO                              GPIOA
#define UART1_RX_PIN                            GPIO_PIN_10
#define UART1_TX_PIN                            GPIO_PIN_9
#define UART1_GPIO_AF                           GPIO_AF7_USART1
#define UART1_INST     		                    USART1
#define UART1_IRQ                               USART1_IRQn
#define UART1_IRQ_HANDLER                       USART1_IRQHandler
#define UART_GPIO_CLK_ENABLE(INDEX)             do { if(0 == (INDEX)) {__HAL_RCC_GPIOA_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOA_CLK_ENABLE();}} while(0)
#define UART_GPIO_CLK_DISABLE(INDEX)            do { if(0 == (INDEX)) {__HAL_RCC_GPIOA_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOA_CLK_DISABLE();}} while(0)
#define UART_CLK_ENABLE(INDEX)                  do { if(0 == (INDEX)) {__HAL_RCC_USART2_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_USART1_CLK_ENABLE();}} while(0)
#define UART_CLK_DISABLE(INDEX)                 do { if(0 == (INDEX)) {__HAL_RCC_USART2_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_USART1_CLK_DISABLE();}} while(0)
#define UART_FORCE_RESET(INDEX)                 do { if(0 == (INDEX)) {__HAL_RCC_USART2_FORCE_RESET();} else\
                                                                      {__HAL_RCC_USART1_FORCE_RESET();}} while(0)
#define UART_RELEASE_RESET(INDEX)               do { if(0 == (INDEX)) {__HAL_RCC_USART2_RELEASE_RESET();} else\
                                                                      {__HAL_RCC_USART1_RELEASE_RESET();}} while(0)
#else
#error Board type not defined!!!
#endif
/** @} */ /* End of group UART configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __UART_STM32F2XX_H__ */

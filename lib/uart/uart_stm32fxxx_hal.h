/*
 * uart_stm32fxxx.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __UART_STM32FXXX_HAL_H__
#define __UART_STM32FXXX_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
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
#define UART1_GPIO                              GPIOA
#define UART1_RX_PIN                            GPIO_PIN_10
#define UART1_TX_PIN                            GPIO_PIN_9
#define UART1_GPIO_AF                           GPIO_AF7_USART1
#define UART1_INST     		                    USART1
#define UART1_IRQ                               USART1_IRQn
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
#define UART0_IRQ_HANDLER                       USART2_IRQHandler
#define UART1_IRQ_HANDLER                       USART1_IRQHandler
#else
#define UART0_IRQ_HANDLER                       USART2_IRQHandler
#define UART1_IRQ_HANDLER                       USART3_IRQHandler
#endif
/** @} */ /* End of group UART configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __UART_STM32FXXX_HAL_H__ */

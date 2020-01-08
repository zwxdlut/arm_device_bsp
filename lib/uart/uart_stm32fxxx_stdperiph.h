/*
 * uart_stm32fxxx_stdperiph.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __UART_STM32FXXX_STDPERIPH_H__
#define __UART_STM32FXXX_STDPERIPH_H__

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
#define UART0_IRQ_HANDLER                       USART2_IRQHandler
#define UART1_IRQ_HANDLER                       USART1_IRQHandler
#else
#define UART0_GPIO                              GPIOA
#define UART0_RX_PIN                            GPIO_Pin_3
#define UART0_TX_PIN                            GPIO_Pin_2
#define UART0_INST     		                    USART2
#define UART0_IRQ     		                    USART2_IRQn
#define UART1_GPIO                              GPIOB
#define UART1_RX_PIN                            GPIO_Pin_11
#define UART1_TX_PIN                            GPIO_Pin_10
#define UART1_INST     		                    USART3
#define UART1_IRQ     		                    USART3_IRQn
#define UART_GPIO_CLK_ENABLE(INDEX)             do { if(0 == (INDEX)) {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);} else\
                                                                      {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);}} while(0)
#define UART_GPIO_CLK_DISABLE(INDEX)            do { if(0 == (INDEX)) {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, DISABLE);} else\
                                                                      {RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, DISABLE);}} while(0)
#define UART_CLK_ENABLE(INDEX)                  do { if(0 == (INDEX)) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);} else\
                                                                      {RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);}} while(0)
#define UART_CLK_DISABLE(INDEX)                 do { if(0 == (INDEX)) {RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, DISABLE);} else\
                                                                      {RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, DISABLE);}} while(0)
#define UART_FORCE_RESET(INDEX)                 do { if(0 == (INDEX)) {RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);} else\
                                                                      {RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);}} while(0)
#define UART_RELEASE_RESET(INDEX)               do { if(0 == (INDEX)) {RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);} else\
                                                                      {RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);}} while(0)
#define UART0_IRQ_HANDLER                       USART2_IRQHandler
#define UART1_IRQ_HANDLER                       USART3_IRQHandler
#endif
/** @} */ /* End of group UART configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __UART_STM32FXXX_STDPERIPH_H__ */

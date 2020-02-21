/*
 * uart_s32k1xx.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __UART_S32K1XX_H__
#define __UART_S32K1XX_H__

#include "Cpu.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup UART byte size.
 * @{
 */
#define UART_BYTE_SIZE_8B                       (uint32_t)LPUART_8_BITS_PER_CHAR
#define UART_BYTE_SIZE_9B                       (uint32_t)LPUART_9_BITS_PER_CHAR
#define UART_BYTE_SIZE_10B                      (uint32_t)LPUART_10_BITS_PER_CHAR
/** @} */ /* End of group UART byte size. */

/**
 * @defgroup UART stop bits.
 * @{
 */
#define UART_STOP_BITS_1                        (uint32_t)LPUART_ONE_STOP_BIT
#define UART_STOP_BITS_2                        (uint32_t)LPUART_TWO_STOP_BIT
/** @} */ /* End of group UART stop bits. */

/**
 * @defgroup UART parity.
 * @{
 */
#define UART_PARITY_MODE_NONE                   (uint32_t)LPUART_PARITY_DISABLED
#define UART_PARITY_MODE_EVEN                   (uint32_t)LPUART_PARITY_EVEN
#define UART_PARITY_MODE_ODD                    (uint32_t)LPUART_PARITY_ODD
/** @} */ /* End of group UART parity. */

/**
 * @defgroup UART configuration.
 * @{
 */
#if defined MX_TB
#define UART0_PORT                              PORTB
#define UART0_RX_PIN                            0
#define UART0_TX_PIN                            1
#define UART0_GPIO_AF                           PORT_MUX_ALT2
#define UART0_IRQ                               LPUART0_RxTx_IRQn
#define UART1_PORT                              PORTD
#define UART1_RX_PIN                            6
#define UART1_TX_PIN                            7
#define UART1_GPIO_AF                           PORT_MUX_ALT2
#define UART1_IRQ                               LPUART2_RxTx_IRQn
#else
#define UART0_PORT                              PORTB
#define UART0_RX_PIN                            0
#define UART0_TX_PIN                            1
#define UART0_GPIO_AF                           PORT_MUX_ALT2
#define UART0_IRQ                               LPUART0_RxTx_IRQn
#define UART1_PORT                              PORTC
#define UART1_RX_PIN                            6
#define UART1_TX_PIN                            7
#define UART1_GPIO_AF                           PORT_MUX_ALT2
#define UART1_IRQ                               LPUART1_RxTx_IRQn
#endif
/** @} */ /* End of group UART configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __UART_S32K1XX_H__ */

/*
 * uart.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>

#if defined S32K14x
#include "uart_s32k1xx.h"
#elif defined STM32F10X_CL
#include "uart_stm32f10x.h"
#elif defined STM32F205xx
#include "uart_stm32f2xx.h"
#else
#error Mcu type not defined!!!
#endif

#if defined USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup UART module index.
 * @{
 */
#define UART0_INDEX       		                0
#define UART1_INDEX       		                1
/** @} */ /* End of group UART module index. */

#define UART_BUFFER_SIZE                        1000 /**< UART buffer size. */

/**
 * @defgroup UART header define.
 * @{
 */
/*-----------------------------------------------------------------------------------
 * Header
 * +--------------------+-----------------+-----------------+-----------------+-----+
 * | byte #0            | byte #1         | byte #2         | byte #3         | ... |
 * +--------------------+-----------------+-----------------+-----------------+-----+
 * | 0xAA               | 0x55            | data size low   | data size high  | ... |
 * +--------------------+-----------------+-----------------+-----------------+-----+
 *---------------------------------------------------------------------------------*/
#define HEADER_FLAG      	                    0xAA55
#define HEADER_SIZE  	                        4
/** @} */ /* End of group UART header define. */

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * @brief  Initialize UART.
 *
 * @param  [in] _index     UART index.
 * @param  [in] _baudrate  Baud rate.
 * @param  [in] _data_bits Data bits:
 *              @arg UART_DATA_BITS_8  8 bits data.
 *              @arg UART_DATA_BITS_9  9 bits data.
 *              @arg UART_DATA_BITS_10 10 bits data.
 * @param  [in] _stop_bits Stop bits:
 *              @arg UART_STOP_BITS_0_5 0.5 stop bit.
 *              @arg UART_STOP_BITS_1   1 stop bit.
 *              @arg UART_STOP_BITS_1_5 1.5 stop bits.
 *              @arg UART_STOP_BITS_2   2 stop bits.
 * @param  [in] _parity Partity:
                @arg UART_PARITY_MODE_NONE No parity.
 *              @arg UART_PARITY_MODE_EVEN Even parity.
 *              @arg UART_PARITY_MODE_ODD  Odd parity.
 * @return Success(0) or failure(other values).
 */
int32_t uart_init(const uint8_t _index, const uint32_t _baudrate, const uint32_t _data_bits, const uint32_t _stop_bits, const uint32_t _parity);

/**
 * @brief  De-initialize UART.
 *
 * @param  [in] _index UART index.
 * @return Success(0) or failure(other values).
 */
int32_t uart_deinit(const uint8_t _index);

/**
 * @brief  Receive UART data.
 *
 * @param  [in]  _index UART index.
 * @param  [out] _buf   Receive buffer.
 * @param  [in]  _size  Receive size.
 * @return Received size.
 */
uint16_t uart_receive(const uint8_t _index, uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Receive UART data with header(0xAA55) in poll mode.
 *
 * @param  [in]  _index UART index.
 * @param  [out] _buf   Receive buffer.
 * @param  [in]  _size  Receive size.
 * @return Received size without header.
 */
uint16_t uart_receive_with_header_poll( const uint8_t _index, uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Transmit UART data.
 *
 * @param  [in] _index UART index.
 * @param  [in] _buf   Transmit buffer.
 * @param  [in] _size  Transmit size.
 * @return Transmitted size.
 */
uint16_t uart_transmit(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Transmit UART data with header.
 *
 * @param  [in] _index UART index.
 * @param  [in] _buf   Transmit buffer.
 * @param  [in] _size  Transmit size.
 * @return Transmitted size.
 */
uint16_t uart_transmit_with_header(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size);

/**
 * @brief Print debug information.
 *
 * @param [in] _info Information string.
 */
void debug(const char* _info, ...);

/**
 * @brief Print buffer.
 *
 * @param [in] _prefix Prefix information.
 * @param [in] _id     ID.
 * @param [in] _buf    Print buffer.
 * @param [in] _size   Print size.
 */
void print_buf(const char *_prefix, const uint32_t _id, const uint8_t *_buf, const uint16_t _size);

#ifdef __cplusplus
}
#endif

#endif /* __UART_H__ */

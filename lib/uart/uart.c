/*
 * uart.c
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#include <string.h>
#include <assert.h>

#include "uart.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
static uint8_t  g_rx_fifo[UART1_INDEX + 1][UART_FIFO_MAX_SIZE]; /**< Ring fifo */
static uint16_t g_rx_fifo_head[UART1_INDEX + 1];                /**< Ring fifo head */
static uint16_t g_rx_fifo_tail[UART1_INDEX + 1];                /**< Ring fifo tail */

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
#if defined S32K14x
#include "uart_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#if defined USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t g_tx_mutex[UART1_INDEX + 1] = {NULL, NULL}; /* Tx mutex*/
#endif

static void uart_irq_handler(const uint8_t _index);

#if defined USE_STDPERIPH_DRIVER
#include "uart_stm32fxxx_stdperiph.inc"
#elif defined USE_HAL_DRIVER
#include "uart_stm32fxxx_hal.inc"
#else
#error SDK type not defined!!!
#endif

uint16_t uart_receive(const uint8_t _index, uint8_t *const _buf, const uint16_t _size)
{
	assert(UART1_INDEX >= _index && NULL != _buf);

	uint16_t i = 0;

	/* Rx fifo is not empty */
	while(g_rx_fifo_head[_index] != g_rx_fifo_tail[_index] && i < _size && i < UART_FIFO_MAX_SIZE)
	{
		/* Pop rx fifo */
		_buf[i++] = g_rx_fifo[_index][g_rx_fifo_head[_index]++];
		g_rx_fifo_head[_index] = g_rx_fifo_head[_index] % UART_FIFO_MAX_SIZE;
	}

	return i;
}

uint16_t uart_transmit_with_header(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size)
{
	uint16_t size = 0;
	
#if defined USING_OS_FREERTOS
	xSemaphoreTakeRecursive( g_tx_mutex[_index], portMAX_DELAY);
#endif
#if defined (HEADER_FLAG) && defined (HEADER_SIZE)
	uint8_t	header[HEADER_SIZE];
	header[0] = HEADER_FLAG >> 8;
	header[1] = HEADER_FLAG & 0xFF;
	header[2] = _size;
	header[3] = _size >> 8;
	size = uart_transmit(_index, header, HEADER_SIZE);
#endif
	size += uart_transmit(_index, _buf, _size);
#if defined USING_OS_FREERTOS
	xSemaphoreGiveRecursive( g_tx_mutex[_index] );
#endif
	
	return size;
}

/**
 * @defgroup IRQ handlers.
 * @{
 */
/**
 * @brief UART0 IRQ handler.
 */
void UART0_IRQ_HANDLER(void)
{	
	uart_irq_handler(UART0_INDEX);
}

/**
 * @brief UART1 IRQ handler.
 */
void UART1_IRQ_HANDLER(void)
{
	uart_irq_handler(UART1_INDEX);
}
/** @} */ /* End of group IRQ handlers. */
#else
#error Mcu type not defined!!!
#endif

/*******************************************************************************
 * Functions
 ******************************************************************************/
/*******************************************************************************
 * Local Functions
 ******************************************************************************/

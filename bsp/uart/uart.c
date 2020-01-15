/*
 * uart.c
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#include "uart.h"

#if defined STM32F10X_CL || defined STM32F205xx
extern void uart_irq_handler(const uint8_t _index);

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
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined USING_OS_FREERTOS
SemaphoreHandle_t g_uart_tx_mutex[UART1_INDEX + 1] = {NULL, NULL}; /* Tx mutex*/
#endif

uint8_t  g_uart_rx_queue[UART1_INDEX + 1][UART_BUFFER_SIZE];       /**< Ring queue */
uint16_t g_uart_rx_queue_head[UART1_INDEX + 1] = {0, 0};           /**< Ring queue head */
uint16_t g_uart_rx_queue_tail[UART1_INDEX + 1] = {0, 0};           /**< Ring queue tail */

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
/*******************************************************************************
 * Functions
 ******************************************************************************/
uint16_t uart_receive(const uint8_t _index, uint8_t *const _buf, const uint16_t _size)
{
	assert(UART1_INDEX >= _index && NULL != _buf);

	uint16_t i = 0;

	/* Rx queue is not empty */
	while(g_uart_rx_queue_head[_index] != g_uart_rx_queue_tail[_index] && i < _size)
	{
		/* Pop rx queue */
		_buf[i++] = g_uart_rx_queue[_index][g_uart_rx_queue_head[_index]];
		g_uart_rx_queue_head[_index] = (g_uart_rx_queue_head[_index] + 1) % UART_BUFFER_SIZE;
	}

	return i;
}

uint16_t uart_receive_with_header_poll( const uint8_t _index, uint8_t *const _buf, const uint16_t _size)
{
	assert(NULL != _buf);

	uint16_t size = 0;
	uint16_t out_size = 0;

	/* Receive 0xAA */
	size = 1;
	if(size != uart_receive(_index, _buf, size) ||  (0xFF & (HEADER_FLAG >> 8)) != _buf[0])
		return 0;

	/* Receive 0x55 */
	size = 1;
	out_size = 0;
	while(out_size < size)
	{
#if defined USING_OS_FREERTOS
		vTaskDelay(pdMS_TO_TICKS(1));
#endif
		out_size += uart_receive(_index, _buf + out_size, size - out_size);
	}
	if((HEADER_FLAG & 0xFF) != _buf[0])
		return 0;

	/* Receive data length */
	size = HEADER_SIZE - 2;
	out_size = 0;
	while(out_size < size)
	{
#if defined USING_OS_FREERTOS
		vTaskDelay(pdMS_TO_TICKS(1));
#endif
		out_size += uart_receive(_index, _buf + out_size, size - out_size);
	}

	/* Receive data */
	memcpy(&size, _buf, HEADER_SIZE - 2);
	size = _size > size ? size : _size;
	out_size = 0;
	while(out_size < size)
	{
#if defined USING_OS_FREERTOS
		vTaskDelay(pdMS_TO_TICKS(1));
#endif
		out_size += uart_receive(_index, _buf + out_size, size - out_size);
	}

	return size;
}

uint16_t uart_transmit_with_header(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size)
{
	assert(UART1_INDEX >= _index && NULL != _buf);

	uint16_t size = 0;

#if defined USING_OS_FREERTOS
	xSemaphoreTakeRecursive( g_uart_tx_mutex[_index], portMAX_DELAY);
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
	xSemaphoreGiveRecursive( g_uart_tx_mutex[_index] );
#endif

	return size;
}

void debug(const char* _info, ...)
{
#ifdef UDEBUG
	va_list args;
	va_start(args, _info);
	vprintf(_info, args);
	va_end(args);
#endif
}

void print_buf(const char *_prefix, const uint32_t _id, const uint8_t *_buf, const uint16_t _size)
{
#if defined UDEBUG
#if defined USING_OS_FREERTOS
	static SemaphoreHandle_t g_debug_mutex;
	static bool init = false;
	if(!init)
	{
		g_debug_mutex = xSemaphoreCreateMutex();
		init = true;
	}
	xSemaphoreTake( g_debug_mutex, portMAX_DELAY);
#endif
	printf("%s(0x%X,%d): ", _prefix, (unsigned int)_id, _size);
	for (uint16_t i = 0; i < _size; i++)
		printf("%02X ", _buf[i]);
	printf("\n");
#if defined USING_OS_FREERTOS
	xSemaphoreGive( g_debug_mutex );
#endif
#endif
}

/**
 * @defgroup Retarget printf.
 * @{
 */
#if defined __EWL__
#include <sys/UART.h>
UARTError InitializeUART(UARTBaudRate baudRate)
{
#if defined UDEBUG
	return uart_init(UART1_INDEX, kBaud115200);
#else
	return kUARTNoError;
#endif
}

UARTError ReadUARTN(void* bytes, unsigned long limit)
{
#if defined UDEBUG
	uint16_t  size = 0;

	while(size < limit)
		size += uart_receive(UART1_INDEX, bytes + size, limit - size);
#endif
	return kUARTNoError;
}

UARTError WriteUARTN(const void * bytes, unsigned long length)
{
#if defined UDEBUG
	uart_transmit(UART1_INDEX, bytes, length);
#endif
	return kUARTNoError;
}
#elif defined __NEWLIB__
#if defined UDEBUG
int _write(int iFileHandle, char *pcBuffer, int iLength)
{
	static bool init = false;

	if(!init)
	{
		uart_init(UART1_INDEX, 115200);
		init = true;
	}

	return uart_transmit(UART1_INDEX, (uint8_t*)pcBuffer, iLength);
}
#endif
#elif defined __ARMCLIB_VERSION
#if defined UDEBUG
#pragma import(__use_no_semihosting)

void _sys_exit(int x)
{
    x = x;
}

void _ttywrch(int x)
{
    x = x;
}

struct __FILE
{
    int handle;
};

FILE __stdout;

int fputc(int ch, FILE *f)
{
	static bool init = false;

	if(!init)
	{
		uart_init(UART1_INDEX, 115200);
		init = true;
	}

	uart_transmit(UART1_INDEX, (uint8_t*)(&ch), 1);

	return (ch);
}
#endif
#endif
/** @} */ /* End of group Retarget printf. */

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
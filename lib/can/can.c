/*
 * flexcan.c
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#include <string.h>
#include <assert.h>

#include "can.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
	uint32_t id_;
	uint8_t  dlc_;
	uint8_t  data_[8];
}can_msg_t;

static can_msg_t g_rx_fifo[CAN1_INDEX + 1][CAN_FIFO_MAX_SIZE]; /**< Rx ring fifo */
static uint8_t   g_rx_fifo_head[CAN1_INDEX + 1];               /**< Rx fifo head */
static uint8_t   g_rx_fifo_tail[CAN1_INDEX + 1];               /**< Rx fifo tail */

/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
#if defined S32K14x
#include "can_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
#if defined USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t g_tx_mutex[CAN1_INDEX + 1] = {NULL, NULL}; /**< Tx mutex */
#endif

static void can_irq_handler(const uint8_t _index);

#if defined USE_STDPERIPH_DRIVER
#include "can_stm32fxxx_stdperiph.inc"
#elif defined USE_HAL_DRIVER
#include "can_stm32fxxx_hal.inc"
#else
#error SDK type not defined!!!
#endif

uint8_t can_receive(const uint8_t _index, uint32_t *const _id, uint8_t *const _buf,  const uint8_t _size)
{
	assert(CAN1_INDEX >= _index && NULL != _id && NULL != _buf);

	uint8_t size = 0;

	/* Rx fifo is not empty */
	if(g_rx_fifo_head[_index] != g_rx_fifo_tail[_index])
	{
		/* Pop rx fifo */
		*_id = g_rx_fifo[_index][g_rx_fifo_head[_index]].id_;
		size = _size > g_rx_fifo[_index][g_rx_fifo_head[_index]].dlc_ ? g_rx_fifo[_index][g_rx_fifo_head[_index]].dlc_ : _size;
		memcpy(_buf, g_rx_fifo[_index][g_rx_fifo_head[_index]].data_, size);
		g_rx_fifo_head[_index] = (g_rx_fifo_head[_index] + 1) % CAN_FIFO_MAX_SIZE;
	}

	return size;
}

/**
 * @defgroup IRQ handlers.
 * @{
 */
/*
 * @brief CAN0 RX IRQ handler.
 */
void CAN0_RX_IRQ_HANDLER(void)
{	
	can_irq_handler(CAN0_INDEX);
}

/**
 * @brief CAN1 RX IRQ handler.
 */
void CAN1_RX_IRQ_HANDLER(void)
{	
	can_irq_handler(CAN1_INDEX);
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

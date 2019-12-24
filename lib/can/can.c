/*
 * can.c
 *
 *  Created on: 2018Äê8ÔÂ21ÈÕ
 *      Author: Administrator
 */

#include "can.h"

#if defined STM32F10X_CL || defined STM32F205xx
#if defined USING_OS_FREERTOS
SemaphoreHandle_t g_can_tx_mutex[CAN1_INDEX + 1];  /**< Tx mutex */
#endif

extern void can_irq_handler(const uint8_t _index);

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
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
can_msg_t g_can_rx_queue[CAN1_INDEX + 1][CAN_RX_QUEUE_MAX_SIZE]; /**< Rx ring queue */
uint8_t   g_can_rx_queue_head[CAN1_INDEX + 1];                   /**< Rx queue head */
uint8_t   g_can_rx_queue_tail[CAN1_INDEX + 1];                   /**< Rx queue tail */

/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
/*******************************************************************************
 * Functions
 ******************************************************************************/
uint8_t can_receive(const uint8_t _index, uint32_t *const _id, uint8_t *const _buf,  const uint8_t _size)
{
	assert(CAN1_INDEX >= _index && NULL != _id && NULL != _buf);

	uint8_t size = 0;

	/* Rx queue is not empty */
	if(g_can_rx_queue_head[_index] != g_can_rx_queue_tail[_index])
	{
		/* Pop rx queue */
		*_id = g_can_rx_queue[_index][g_can_rx_queue_head[_index]].id_;
		size = _size > g_can_rx_queue[_index][g_can_rx_queue_head[_index]].dlc_ ? g_can_rx_queue[_index][g_can_rx_queue_head[_index]].dlc_ : _size;
		memcpy(_buf, g_can_rx_queue[_index][g_can_rx_queue_head[_index]].data_, size);
		g_can_rx_queue_head[_index] = (g_can_rx_queue_head[_index] + 1) % CAN_RX_QUEUE_MAX_SIZE;
	}

	return size;
}

/*******************************************************************************
 * Local Functions
 ******************************************************************************/

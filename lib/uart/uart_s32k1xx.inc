/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
	PORT_Type    *port_;
	uint8_t      rx_pin_;
	uint8_t      tx_pin_;
	port_mux_t   gpio_af_;
	IRQn_Type    irqs_[1];
}comm_config_t;

static comm_config_t g_comm_config[UART1_INDEX + 1] =
{
	{
		.port_    = UART0_PORT,
		.rx_pin_  = UART0_RX_PIN,
		.tx_pin_  = UART0_TX_PIN,
		.gpio_af_ = UART0_GPIO_AF,
		.irqs_    = {UART0_IRQ}
	},
	{
		.port_    = UART1_PORT,
		.rx_pin_  = UART1_RX_PIN,
		.tx_pin_  = UART1_TX_PIN,
		.gpio_af_ = UART1_GPIO_AF,
		.irqs_    = {UART1_IRQ}
	}
};

static uint8_t g_handle[UART1_INDEX + 1] =
{
#if defined INST_LPUART0
	INST_LPUART0,
#else
	0xFF,
#endif
#if defined INST_LPUART1
	INST_LPUART1
#else
	0xFF
#endif
};

static const lpuart_user_config_t *g_config[UART1_INDEX + 1] =
{
#if defined INST_LPUART0
	&lpuart0_InitConfig0,
#else
	NULL,
#endif
#if defined INST_LPUART1
	&lpuart1_InitConfig0
#else
	NULL
#endif
};

static lpuart_state_t *g_state[UART1_INDEX + 1] =
{
#if defined INST_LPUART0
	&lpuart0_State,
#else
	NULL,
#endif
#if defined INST_LPUART1
	&lpuart1_State
#else
	NULL
#endif
};

static uint8_t  g_rx_byte;
static mutex_t  g_tx_mutex[UART1_INDEX + 1]; /**< Tx mutex */

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
static void uart_irq_handler(void *_state, uart_event_t _event, void *_user_data);

/*******************************************************************************
 * Functions
 ******************************************************************************/
int32_t uart_init(const uint8_t _index)
{
	assert(UART1_INDEX >= _index);

	/* Initialize ring fifo */
	g_rx_fifo_head[_index] = 0;
	g_rx_fifo_tail[_index] = 0;
	OSIF_MutexCreate(&g_tx_mutex[_index]);

	/* GPIO initialization */
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].rx_pin_, g_comm_config[_index].gpio_af_);
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].tx_pin_, g_comm_config[_index].gpio_af_);
	PINS_DRV_SetPullSel(g_comm_config[_index].port_, g_comm_config[_index].rx_pin_, PORT_INTERNAL_PULL_UP_ENABLED);

	/* UART initialization */
	LPUART_DRV_Init(g_handle[_index], g_state[_index], g_config[_index]);
	LPUART_DRV_InstallRxCallback(g_handle[_index], uart_irq_handler, (void *)((uint32_t)_index));
#if defined USING_OS_FREERTOS
	/* The interrupt calls an interrupt safe API function - so its priority must
	   be equal to or lower than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY */
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_) && NotAvail_IRQn != g_comm_config[_index].irqs_[i]; i++)
		INT_SYS_SetPriority( g_comm_config[_index].irqs_[i], configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#endif

	return 0;
}

int32_t uart_deinit(const uint8_t _index)
{
	assert(UART1_INDEX >= _index);

	LPUART_DRV_AbortReceivingData(g_handle[_index]);
	LPUART_DRV_AbortSendingData(g_handle[_index]);
	LPUART_DRV_Deinit(g_handle[_index]);
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].rx_pin_, PORT_PIN_DISABLED);
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].tx_pin_, PORT_PIN_DISABLED);
	OSIF_MutexDestroy(&g_tx_mutex[_index]);

	return 0;
}

uint16_t uart_receive(const uint8_t _index, uint8_t *const _buf, const uint16_t _size)
{
	assert(UART1_INDEX >= _index && NULL != _buf);

	uint16_t i = 0;

	/* Trigger receiving */
	LPUART_DRV_ReceiveData( g_handle[_index], &g_rx_byte, 1);

	/* Rx fifo is not empty */
	while(g_rx_fifo_head[_index] != g_rx_fifo_tail[_index] && i < _size && i < UART_FIFO_MAX_SIZE)
	{
		/* Pop rx fifo */
		_buf[i++] = g_rx_fifo[_index][g_rx_fifo_head[_index]++];
		g_rx_fifo_head[_index] = g_rx_fifo_head[_index] % UART_FIFO_MAX_SIZE;
	}

	return i;
}

uint16_t uart_transmit(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size)
{
	assert(UART1_INDEX >= _index && NULL != _buf);

	uint16_t size = _size;

	OSIF_MutexLock(&g_tx_mutex[_index], OSIF_WAIT_FOREVER);
	if(STATUS_SUCCESS != LPUART_DRV_SendDataBlocking(g_handle[_index], _buf, _size, OSIF_WAIT_FOREVER))
		size = 0;
	OSIF_MutexUnlock(&g_tx_mutex[_index]);

	return size;
}

uint16_t uart_transmit_with_header(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size)
{
	assert(UART1_INDEX >= _index && NULL != _buf);

	uint16_t size = HEADER_SIZE + _size;

	OSIF_MutexLock(&g_tx_mutex[_index], OSIF_WAIT_FOREVER);
#if defined (HEADER_FLAG) && defined (HEADER_SIZE)
	uint8_t	header[HEADER_SIZE];
	header[0] = HEADER_FLAG >> 8;
	header[1] = HEADER_FLAG & 0xFF;
	header[2] = _size;
	header[3] = _size >> 8;
	if(STATUS_SUCCESS != LPUART_DRV_SendDataBlocking(g_handle[_index], header, HEADER_SIZE, OSIF_WAIT_FOREVER))
		size -= HEADER_SIZE;
#endif
	if(STATUS_SUCCESS != LPUART_DRV_SendDataBlocking(g_handle[_index], _buf, _size, OSIF_WAIT_FOREVER))
		size -= _size;
	OSIF_MutexUnlock(&g_tx_mutex[_index]);

	return size;
}

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
/**
 * @brief UART IRQ handler.
 *
 * @param [in] _state     Driver state.
 * @param [in] _event     Event type.
 * @param [in] _user_data Callback parameter.
 */
static void uart_irq_handler(void *_state, uart_event_t _event, void *_user_data)
{
	(void)_state;

	uint8_t index = (uint8_t)((uint32_t)_user_data);

    if (UART_EVENT_RX_FULL == _event)
    {
		/* Rx fifo is not full */
		if(g_rx_fifo_head[index] != (g_rx_fifo_tail[index] + 1) % UART_FIFO_MAX_SIZE)
		{
			/* Push rx fifo */
			g_rx_fifo[index][g_rx_fifo_tail[index]++] = g_rx_byte;
			g_rx_fifo_tail[index] = g_rx_fifo_tail[index] % UART_FIFO_MAX_SIZE;
		}

		/* Update rx buffer and trigger next receive */
		LPUART_DRV_SetRxBuffer(g_handle[index], &g_rx_byte, 1);
    }
}

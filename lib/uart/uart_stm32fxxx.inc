#ifdef USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t g_tx_mutex[UART1_INDEX + 1] = {NULL, NULL};
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
	GPIO_TypeDef *gpio_;
	uint16_t     rx_pin_;
	uint16_t     tx_pin_;
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	uint8_t      gpio_af_;
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
	IRQn_Type    irqs_[1];
}comm_config_t;

static comm_config_t g_comm_config[UART1_INDEX + 1] =
{
	{
		.gpio_    = UART0_GPIO,
		.rx_pin_  = UART0_RX_PIN,
		.tx_pin_  = UART0_TX_PIN,
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		.gpio_af_ = UART0_GPIO_AF,
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		.irqs_         = {UART0_IRQ}
	},
	{
		.gpio_   = UART1_GPIO,
		.rx_pin_ = UART1_RX_PIN,
		.tx_pin_ = UART1_TX_PIN,
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		.gpio_af_ = UART1_GPIO_AF,
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		.irqs_     = {UART1_IRQ}
	}
};

#if defined USE_STDPERIPH_DRIVER
static USART_TypeDef      *g_handle[UART1_INDEX + 1] = {UART0_INST, UART1_INST};
#elif defined USE_HAL_DRIVER
static UART_HandleTypeDef g_handle[UART1_INDEX + 1] = 
{
	{
		.Instance          = UART0_INST,
		.Init.BaudRate     = 115200,
		.Init.WordLength   = UART_WORDLENGTH_8B,
		.Init.StopBits     = UART_STOPBITS_1,
		.Init.Parity       = UART_PARITY_NONE,
		.Init.HwFlowCtl    = UART_HWCONTROL_NONE,
		.Init.Mode         = UART_MODE_TX_RX,
		.Init.OverSampling = UART_OVERSAMPLING_16
	},
	{
		.Instance          = UART1_INST,
		.Init.BaudRate     = 115200,
		.Init.WordLength   = UART_WORDLENGTH_8B,
		.Init.StopBits     = UART_STOPBITS_1,
		.Init.Parity       = UART_PARITY_NONE,
		.Init.HwFlowCtl    = UART_HWCONTROL_NONE,
		.Init.Mode         = UART_MODE_TX_RX,
		.Init.OverSampling = UART_OVERSAMPLING_16
	}
};
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

static uint8_t  g_rx_fifo[UART1_INDEX + 1][UART_FIFO_MAX_SIZE]; /**< Ring fifo */
static uint16_t g_rx_fifo_head[UART1_INDEX + 1];
static uint16_t g_rx_fifo_tail[UART1_INDEX + 1];

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
static void uart_irq_handler(const uint8_t _index);

/*******************************************************************************
 * Functions
 ******************************************************************************/
int32_t uart_init(const uint8_t _index)
{
	assert(UART1_INDEX >= _index);
	
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Initialize ring fifo */
	g_rx_fifo_head[_index] = 0;
	g_rx_fifo_tail[_index] = 0;
#ifdef USING_OS_FREERTOS
	g_tx_mutex[_index] = xSemaphoreCreateMutex();
#endif
	
#if defined USE_STDPERIPH_DRIVER
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;

	/* GPIO initialization */
	UART_GPIO_CLK_ENABLE(_index);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = g_comm_config[_index].rx_pin_;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = g_comm_config[_index].tx_pin_;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	
	/* UART initialization */
	UART_CLK_ENABLE(_index);
	USART_InitStructure.USART_BaudRate            = 115200;
	USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits            = USART_StopBits_1;
	USART_InitStructure.USART_Parity              = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(g_handle[_index], &USART_InitStructure);
	USART_ITConfig(g_handle[_index], USART_IT_RXNE, ENABLE);
	USART_Cmd(g_handle[_index], ENABLE);
	USART_ClearFlag(g_handle[_index], USART_FLAG_TC);
	
	/* NVIC initialization */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_); i++)
	{
		NVIC_InitStructure.NVIC_IRQChannel = g_comm_config[_index].irqs_[i];
		NVIC_Init(&NVIC_InitStructure);
	}
#elif defined USE_HAL_DRIVER
	/* GPIO initialization */
	UART_GPIO_CLK_ENABLE(_index);
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = g_comm_config[_index].gpio_af_;
	GPIO_InitStructure.Pin       = g_comm_config[_index].rx_pin_;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	HAL_GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	GPIO_InitStructure.Pin       = g_comm_config[_index].tx_pin_;
	GPIO_InitStructure.Pull      = GPIO_NOPULL;
	HAL_GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	
	/* UART initialization */
	UART_CLK_ENABLE(_index);
	HAL_UART_Init(&g_handle[_index]);
	LL_USART_EnableIT_RXNE(g_handle[_index].Instance);
	LL_USART_EnableIT_ERROR(g_handle[_index].Instance);
	
	/* NVIC initialization */
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_); i++)
	{
		HAL_NVIC_SetPriority(g_comm_config[_index].irqs_[i], 0, 0);
		HAL_NVIC_EnableIRQ(g_comm_config[_index].irqs_[i]);
	}
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

	return 0;
}

int32_t uart_deinit(const uint8_t _index)
{
	assert(UART1_INDEX >= _index);

#if defined USE_STDPERIPH_DRIVER
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_); i++)
	{
		NVIC_InitStructure.NVIC_IRQChannel = g_comm_config[_index].irqs_[i];
		NVIC_Init(&NVIC_InitStructure);
	}
	USART_ClearITPendingBit(g_handle[_index], USART_IT_RXNE);
	USART_ClearFlag(g_handle[_index], USART_FLAG_TC);
	USART_Cmd(g_handle[_index], DISABLE);
	USART_ITConfig(g_handle[_index], USART_IT_RXNE, DISABLE);
	USART_DeInit(g_handle[_index]);
	UART_CLK_DISABLE(_index);
	UART_FORCE_RESET(_index);
	UART_RELEASE_RESET(_index);
#elif defined USE_HAL_DRIVER
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_); i++)
		HAL_NVIC_DisableIRQ(g_comm_config[_index].irqs_[i]);
	HAL_UART_Abort(&g_handle[_index]);
	HAL_UART_AbortTransmit(&g_handle[_index]);
	HAL_UART_AbortReceive(&g_handle[_index]);
	HAL_UART_Abort_IT(&g_handle[_index]);
	HAL_UART_AbortTransmit_IT(&g_handle[_index]);
	HAL_UART_AbortReceive_IT(&g_handle[_index]);
	LL_USART_ClearFlag_RXNE(g_handle[_index].Instance);
	LL_USART_DisableIT_RXNE(g_handle[_index].Instance);
	LL_USART_DisableIT_ERROR(g_handle[_index].Instance);
	HAL_UART_DeInit(&g_handle[_index]);
	UART_CLK_DISABLE(_index);
	UART_FORCE_RESET(_index);
	UART_RELEASE_RESET(_index);
	HAL_GPIO_DeInit(g_comm_config[_index].gpio_, g_comm_config[_index].rx_pin_);
	HAL_GPIO_DeInit(g_comm_config[_index].gpio_, g_comm_config[_index].tx_pin_);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#ifdef USING_OS_FREERTOS
	vSemaphoreDelete(g_tx_mutex[_index]);
#endif

	return 0;
}

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

uint16_t uart_transmit(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size)
{
	assert(UART1_INDEX >= _index && NULL != _buf);

	uint16_t size = _size;
	
#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_tx_mutex[_index], portMAX_DELAY);
#endif
#if defined USE_STDPERIPH_DRIVER
	while(USART_GetFlagStatus(g_handle[_index], USART_FLAG_TC) == RESET){}
    for(uint16_t i = 0; i < _size; i++)        
    {           
		USART_SendData(g_handle[_index], _buf[i]); 
		while(USART_GetFlagStatus(g_handle[_index], USART_FLAG_TC) == RESET){}
    }
#elif defined USE_HAL_DRIVER
	if(HAL_OK != HAL_UART_Transmit(&g_handle[_index], (uint8_t*)_buf, _size, HAL_MAX_DELAY))
		size = 0;
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_tx_mutex[_index] );
#endif

	return size;
}

uint16_t uart_transmit_with_header(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size)
{
	uint16_t size = 0;

#if defined (HEADER_FLAG) && defined (HEADER_SIZE)
	uint8_t	header[HEADER_SIZE];
	header[0] = HEADER_FLAG >> 8;
	header[1] = HEADER_FLAG & 0xFF;
	header[2] = _size;
	header[3] = _size >> 8;
	size = uart_transmit(_index, header, HEADER_SIZE);
#endif
	size += uart_transmit(_index, _buf, _size);

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

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
/**
 * @brief UART IRQ handler.
 *
 * @param [in] _index UART index.
 */
static void uart_irq_handler(const uint8_t _index)
{
	/* RXNE */
#if defined USE_STDPERIPH_DRIVER
	if(RESET != USART_GetITStatus(g_handle[_index], USART_IT_RXNE))
	{		
		USART_ClearITPendingBit(g_handle[_index], USART_IT_RXNE);
#elif defined USE_HAL_DRIVER
	if(0 != LL_USART_IsActiveFlag_RXNE(g_handle[_index].Instance) && 0 != LL_USART_IsEnabledIT_RXNE(g_handle[_index].Instance))
	{
		LL_USART_ClearFlag_RXNE(g_handle[_index].Instance);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		/* Rx fifo is not full */
		if(g_rx_fifo_head[_index] != (g_rx_fifo_tail[_index] + 1) % UART_FIFO_MAX_SIZE)
		{
			/* Push rx fifo */
#if defined USE_STDPERIPH_DRIVER
			g_rx_fifo[_index][g_rx_fifo_tail[_index]++] = USART_ReceiveData(g_handle[_index]);
#elif defined USE_HAL_DRIVER
			g_rx_fifo[_index][g_rx_fifo_tail[_index]++] = LL_USART_ReceiveData8(g_handle[_index].Instance);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
			g_rx_fifo_tail[_index] = g_rx_fifo_tail[_index] % UART_FIFO_MAX_SIZE;
		}		
    }
}

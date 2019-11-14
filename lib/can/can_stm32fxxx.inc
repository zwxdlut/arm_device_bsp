#ifdef USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t g_tx_mutex[CAN1_INDEX + 1] = {NULL, NULL}; /**< CAN tx mutex */
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
	uint8_t      start_filter_num_;
#if defined MX_TB
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	GPIO_TypeDef *trans_stb_n_gpio_;
	uint16_t     trans_stb_n_pin_;
	GPIO_TypeDef *trans_en_gpio_;
	uint16_t     trans_en_pin_;
	GPIO_TypeDef *trans_inh_gpio_;
	uint16_t     trans_inh_pin_;
	IRQn_Type    trans_inh_irq_;
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#else
#endif
}comm_config_t;

typedef struct
{
	uint32_t id_;
	uint8_t  dlc_;
	uint8_t  data_[8];
}can_msg_t;

static comm_config_t g_comm_config[CAN1_INDEX + 1] =
{
	{
		.gpio_             = CAN0_GPIO,
		.rx_pin_           = CAN0_RX_PIN,
		.tx_pin_           = CAN0_TX_PIN,
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		.gpio_af_          = CAN0_GPIO_AF,
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		.irqs_             = {CAN0_RX_IRQ},
		.start_filter_num_ = 0,
#if defined MX_TB
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		.trans_stb_n_gpio_ = CAN0_TRANS_STB_N_GPIO,
		.trans_stb_n_pin_  = CAN0_TRANS_STB_N_PIN,
		.trans_en_gpio_    = CAN0_TRANS_EN_GPIO,
		.trans_en_pin_     = CAN0_TRANS_EN_PIN,
		.trans_inh_gpio_   = CAN0_TRANS_INH_GPIO,
		.trans_inh_pin_    = CAN0_TRANS_INH_PIN,
		.trans_inh_irq_    = CAN0_TRANS_INH_IRQ
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#else
#endif
	},
	{
		.gpio_             = CAN1_GPIO,
		.rx_pin_           = CAN1_RX_PIN,
		.tx_pin_           = CAN1_TX_PIN,
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		.gpio_af_          = CAN1_GPIO_AF,
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		.irqs_             = {CAN1_RX_IRQ},
		.start_filter_num_ = CAN_SLAVE_START_FILTER_BANK_NUM,
#if defined MX_TB
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		.trans_stb_n_gpio_ = CAN1_TRANS_STB_N_GPIO,
		.trans_stb_n_pin_  = CAN1_TRANS_STB_N_PIN,
		.trans_en_gpio_    = CAN1_TRANS_EN_GPIO,
		.trans_en_pin_     = CAN1_TRANS_EN_PIN,
		.trans_inh_gpio_   = CAN1_TRANS_INH_GPIO,
		.trans_inh_pin_    = CAN1_TRANS_INH_PIN,
		.trans_inh_irq_    = CAN1_TRANS_INH_IRQ
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#else
#endif
	}
};

/* Baudrate = APB1 clock(MHz) / prescaler /(1 + BS1 + BS2) = 0.5(500kbps)*/
#if defined USE_STDPERIPH_DRIVER
static CAN_TypeDef       *g_handle[CAN1_INDEX + 1] = {CAN0_INST, CAN1_INST};
#elif defined USE_HAL_DRIVER
static CanRxMsgTypeDef   g_rx_msg[CAN1_INDEX + 1];
static CanTxMsgTypeDef   g_tx_msg[CAN1_INDEX + 1];
static CAN_HandleTypeDef g_handle[CAN1_INDEX + 1] = 
{
	{
		.Instance       = CAN0_INST,
		.Init.Prescaler = 4,
		.Init.Mode      = CAN_MODE_NORMAL,
		.Init.SJW       = CAN_SJW_1TQ,
		.Init.BS1       = CAN_BS1_6TQ,
		.Init.BS2       = CAN_BS2_8TQ,
		.Init.TTCM      = DISABLE,
		.Init.ABOM      = DISABLE,
		.Init.AWUM      = DISABLE,
		.Init.NART	    = DISABLE,
		.Init.RFLM      = DISABLE,
		.Init.TXFP      = DISABLE,
		.pTxMsg         = &g_tx_msg[CAN0_INDEX],
		.pRxMsg         = &g_rx_msg[CAN0_INDEX]
	},
	{
		.Instance       = CAN1_INST,
		.Init.Prescaler = 4,
		.Init.Mode      = CAN_MODE_NORMAL,
		.Init.SJW       = CAN_SJW_1TQ,
		.Init.BS1       = CAN_BS1_6TQ,
		.Init.BS2       = CAN_BS2_8TQ,
		.Init.TTCM      = DISABLE,
		.Init.ABOM      = DISABLE,
		.Init.AWUM      = DISABLE,
		.Init.NART	    = DISABLE,
		.Init.RFLM      = DISABLE,
		.Init.TXFP      = DISABLE,
		.pTxMsg         = &g_tx_msg[CAN1_INDEX],
		.pRxMsg         = &g_rx_msg[CAN1_INDEX]
	}
};
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

static can_msg_t g_rx_fifo[CAN1_INDEX + 1][CAN_FIFO_MAX_SIZE]; /**< CAN rx ring fifo */
static uint8_t   g_rx_fifo_head[CAN1_INDEX + 1]; /**< CAN rx fifo head index */
static uint8_t   g_rx_fifo_tail[CAN1_INDEX + 1]; /**< CAN rx fifo tail index */

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
static void can_irq_handler(const uint8_t _index);

/*******************************************************************************
 * Functions
 ******************************************************************************/
int32_t can_init(const uint8_t _index, const uint32_t *_filter_id_list, const uint8_t _filter_id_num)
{
	assert(CAN1_INDEX >= _index);
		
	GPIO_InitTypeDef      GPIO_InitStructure;

#ifdef USING_OS_FREERTOS
	g_tx_mutex[_index] = xSemaphoreCreateMutex();
#endif
#if defined MX_TB
	/* CAN Transceiver initialization */
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	CAN_TRANS_STB_N_GPIO_CLK_ENABLE(_index);
	GPIO_InitStructure.Mode      = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	GPIO_InitStructure.Pin       = g_comm_config[_index].trans_stb_n_pin_;
	HAL_GPIO_Init(g_comm_config[_index].trans_stb_n_gpio_, &GPIO_InitStructure);
	HAL_GPIO_WritePin(g_comm_config[_index].trans_stb_n_gpio_, g_comm_config[_index].trans_stb_n_pin_, (GPIO_PinState)1);
	CAN_TRANS_EN_GPIO_CLK_ENABLE(_index);
	GPIO_InitStructure.Pin       = g_comm_config[_index].trans_en_pin_;
	HAL_GPIO_Init(g_comm_config[_index].trans_en_gpio_, &GPIO_InitStructure);
	HAL_GPIO_WritePin(g_comm_config[_index].trans_en_gpio_, g_comm_config[_index].trans_en_pin_, (GPIO_PinState)1);
	CAN_TRANS_INH_GPIO_CLK_ENABLE(_index);
	GPIO_InitStructure.Pin       = g_comm_config[_index].trans_inh_pin_;
	GPIO_InitStructure.Mode      = GPIO_MODE_IT_RISING;
	GPIO_InitStructure.Pull      = GPIO_PULLDOWN;
	HAL_GPIO_Init(g_comm_config[_index].trans_inh_gpio_, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(g_comm_config[_index].trans_inh_irq_, 0, 0);
    HAL_NVIC_EnableIRQ(g_comm_config[_index].trans_inh_irq_);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#else
#endif
#if defined USE_STDPERIPH_DRIVER
	CAN_InitTypeDef       CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;	
	NVIC_InitTypeDef      NVIC_InitStructure;
	
	/* GPIO initialization */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	CAN_GPIO_CLK_ENABLE(_index);
	if(CAN1 == g_handle[_index])
		GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = g_comm_config[_index].rx_pin_;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = g_comm_config[_index].tx_pin_;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	
	/* CAN initialization */
	CAN_CLK_ENABLE(_index);
	CAN_DeInit(g_handle[_index]);
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM      = DISABLE;
	CAN_InitStructure.CAN_ABOM      = DISABLE;
	CAN_InitStructure.CAN_AWUM      = DISABLE;
	CAN_InitStructure.CAN_NART      = DISABLE;
	CAN_InitStructure.CAN_RFLM      = DISABLE;
	CAN_InitStructure.CAN_TXFP      = DISABLE;
	CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1       = CAN_BS1_3tq;
	CAN_InitStructure.CAN_BS2       = CAN_BS2_2tq;
	CAN_InitStructure.CAN_Prescaler = 12;
	CAN_Init(g_handle[_index], &CAN_InitStructure);
    CAN_ITConfig(g_handle[_index], CAN_IT_FMP0, ENABLE);

	/* CAN filter initialization */
	CAN_SlaveStartBank(CAN_SLAVE_START_FILTER_BANK_NUM);
	CAN_FilterInitStructure.CAN_FilterIdHigh         = 0;
	CAN_FilterInitStructure.CAN_FilterIdLow          = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	CAN_FilterInitStructure.CAN_FilterNumber         = g_comm_config[_index].start_filter_num_;
	CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_16bit;
	CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
	if(NULL == _filter_id_list || 0 == _filter_id_num)
	{
		CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
		CAN_FilterInit(&CAN_FilterInitStructure);		
	}
	else
	{
		uint8_t i = 0;
		uint8_t bank_num =  (0 == CAN_FilterInitStructure.CAN_FilterNumber ? CAN_SLAVE_START_FILTER_BANK_NUM : 28);
		
		CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdList;
		while(CAN_FilterInitStructure.CAN_FilterNumber < bank_num)
		{
			if(i < _filter_id_num)
				CAN_FilterInitStructure.CAN_FilterIdHigh     = (uint16_t)(_filter_id_list[i++] << 5);
			if(i < _filter_id_num)
				CAN_FilterInitStructure.CAN_FilterIdLow      = (uint16_t)(_filter_id_list[i++] << 5);
			if(i < _filter_id_num)
				CAN_FilterInitStructure.CAN_FilterMaskIdHigh = (uint16_t)(_filter_id_list[i++]) << 5;
			if(i < _filter_id_num)
				CAN_FilterInitStructure.CAN_FilterMaskIdLow  = (uint16_t)(_filter_id_list[i++] << 5);
			CAN_FilterInit(&CAN_FilterInitStructure);
			if(i >= _filter_id_num)
				break;
			CAN_FilterInitStructure.CAN_FilterNumber++;
			CAN_FilterInitStructure.CAN_FilterIdHigh     = 0;
			CAN_FilterInitStructure.CAN_FilterIdLow      = 0;
			CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0;
			CAN_FilterInitStructure.CAN_FilterMaskIdLow  = 0;
		}
	}
	
	/* NVIC initialization */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_); i++)
	{
		NVIC_InitStructure.NVIC_IRQChannel = g_comm_config[_index].irqs_[i];
		NVIC_Init(&NVIC_InitStructure);
	}
#elif defined USE_HAL_DRIVER
	CAN_FilterConfTypeDef CAN_FilterInitStructure;
	
	/* GPIO initialization */
	CAN_GPIO_CLK_ENABLE(_index);
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = g_comm_config[_index].gpio_af_;
	GPIO_InitStructure.Pin       = g_comm_config[_index].rx_pin_;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	HAL_GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	GPIO_InitStructure.Pin       = g_comm_config[_index].tx_pin_;
	GPIO_InitStructure.Pull      = GPIO_NOPULL;
	HAL_GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	
	/* CAN initialization */
	CAN_CLK_ENABLE(_index);
	HAL_CAN_Init(&g_handle[_index]);
	__HAL_CAN_ENABLE_IT(&g_handle[_index], CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0 | CAN_IT_EWG | CAN_IT_EPV | CAN_IT_BOF | CAN_IT_LEC | CAN_IT_ERR);

	/* CAN filter initialization */
	CAN_FilterInitStructure.FilterIdHigh         = 0;
	CAN_FilterInitStructure.FilterIdLow          = 0;
	CAN_FilterInitStructure.FilterMaskIdHigh     = 0;
	CAN_FilterInitStructure.FilterMaskIdLow      = 0;
	CAN_FilterInitStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FilterInitStructure.FilterNumber         = g_comm_config[_index].start_filter_num_;
	CAN_FilterInitStructure.FilterScale          = CAN_FILTERSCALE_16BIT;
	CAN_FilterInitStructure.FilterActivation     = ENABLE;
	CAN_FilterInitStructure.BankNumber           = CAN_SLAVE_START_FILTER_BANK_NUM;
	if(NULL == _filter_id_list || 0 == _filter_id_num)
	{
		CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDMASK;
		HAL_CAN_ConfigFilter(&g_handle[_index], &CAN_FilterInitStructure);		
	}
	else
	{
		uint8_t i = 0;
		uint8_t bank_num =  (0 == CAN_FilterInitStructure.FilterNumber ? CAN_SLAVE_START_FILTER_BANK_NUM : 28);
		
		CAN_FilterInitStructure.FilterMode = CAN_FILTERMODE_IDLIST;
		while(CAN_FilterInitStructure.FilterNumber < bank_num)
		{
			if(i < _filter_id_num)
				CAN_FilterInitStructure.FilterIdHigh     = _filter_id_list[i++] << 5;
			if(i < _filter_id_num)
				CAN_FilterInitStructure.FilterIdLow      = _filter_id_list[i++] << 5;
			if(i < _filter_id_num)
				CAN_FilterInitStructure.FilterMaskIdHigh = _filter_id_list[i++] << 5;
			if(i < _filter_id_num)
				CAN_FilterInitStructure.FilterMaskIdLow  = _filter_id_list[i++] << 5;
			HAL_CAN_ConfigFilter(&g_handle[_index], &CAN_FilterInitStructure);
			if(i >= _filter_id_num)
				break;
			CAN_FilterInitStructure.FilterNumber++;
			CAN_FilterInitStructure.FilterIdHigh     = 0;
			CAN_FilterInitStructure.FilterIdLow      = 0;
			CAN_FilterInitStructure.FilterMaskIdHigh = 0;
			CAN_FilterInitStructure.FilterMaskIdLow  = 0;
		}
	}

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

int32_t can_deinit(const uint8_t _index)
{
	assert(CAN1_INDEX >= _index);
	
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
	CAN_FIFORelease(g_handle[_index], CAN_FIFO0);
	CAN_CancelTransmit(g_handle[_index], 0);
	CAN_CancelTransmit(g_handle[_index], 1);
	CAN_CancelTransmit(g_handle[_index], 2);
	CAN_ITConfig(g_handle[_index], CAN_IT_FMP0, DISABLE);
	CAN_DeInit(g_handle[_index]);
	CAN_CLK_DISABLE(_index);
	CAN_FORCE_RESET(_index);
	CAN_RELEASE_RESET(_index);
#elif defined USE_HAL_DRIVER
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_); i++)
		HAL_NVIC_DisableIRQ(g_comm_config[_index].irqs_[i]);
	__HAL_CAN_FIFO_RELEASE(&g_handle[_index], CAN_FIFO0);
	__HAL_CAN_CANCEL_TRANSMIT(&g_handle[_index], CAN_TXMAILBOX_0);
	__HAL_CAN_CANCEL_TRANSMIT(&g_handle[_index], CAN_TXMAILBOX_1);
	__HAL_CAN_CANCEL_TRANSMIT(&g_handle[_index], CAN_TXMAILBOX_2);
	__HAL_CAN_DISABLE_IT(&g_handle[_index],CAN_IT_FMP0 | CAN_IT_FF0 | CAN_IT_FOV0 | CAN_IT_EWG | CAN_IT_EPV | CAN_IT_BOF | CAN_IT_LEC | CAN_IT_ERR);
	HAL_CAN_DeInit(&g_handle[_index]);
	CAN_CLK_DISABLE(_index);
	CAN_FORCE_RESET(_index);
	CAN_RELEASE_RESET(_index);
	HAL_GPIO_DeInit(g_comm_config[_index].gpio_, g_comm_config[_index].rx_pin_);
	HAL_GPIO_DeInit(g_comm_config[_index].gpio_, g_comm_config[_index].tx_pin_);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#if defined MX_TB
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	__HAL_GPIO_EXTI_CLEAR_IT(g_comm_config[_index].trans_inh_pin_);
	HAL_GPIO_DeInit(g_comm_config[_index].trans_stb_n_gpio_, g_comm_config[_index].trans_stb_n_pin_);
	HAL_GPIO_DeInit(g_comm_config[_index].trans_en_gpio_, g_comm_config[_index].trans_en_pin_);
	HAL_GPIO_DeInit(g_comm_config[_index].trans_inh_gpio_, g_comm_config[_index].trans_inh_pin_);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#else
#endif
#ifdef USING_OS_FREERTOS
	vSemaphoreDelete(g_tx_mutex[_index]);
#endif

	return 0;
}

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

uint8_t can_transmit(const uint8_t _index, const uint32_t _id, const uint8_t *const _buf, const uint8_t _size)
{
	assert(CAN1_INDEX >= _index && NULL != _buf);

	uint16_t size = _size;

#ifdef USING_OS_FREERTOS	
	xSemaphoreTake( g_tx_mutex[_index], portMAX_DELAY);
#endif
#if defined USE_STDPERIPH_DRIVER
	CanTxMsg msg;
	uint8_t  mailbox = 0;
	msg.StdId = _id;
	msg.ExtId = 0;
	msg.IDE   = CAN_ID_STD;
	msg.RTR   = CAN_RTR_DATA;
	msg.DLC   = _size >= 8 ? 8 : _size;
	memcpy(msg.Data, _buf, msg.DLC);
	mailbox = CAN_Transmit(g_handle[_index], &msg);
	if(CAN_TxStatus_Ok != CAN_TransmitStatus(g_handle[_index], mailbox))
	{
		CAN_CancelTransmit(g_handle[_index], mailbox);
		size = 0;
	}
#elif defined USE_HAL_DRIVER
	g_handle[_index].pTxMsg->StdId = _id;
	g_handle[_index].pTxMsg->ExtId = 0;
	g_handle[_index].pTxMsg->IDE   = CAN_ID_STD;
	g_handle[_index].pTxMsg->RTR   = CAN_RTR_DATA;
	g_handle[_index].pTxMsg->DLC   = _size >= 8 ? 8 : _size;
	memcpy(g_handle[_index].pTxMsg->Data, _buf, g_handle[_index].pTxMsg->DLC);
	if(HAL_OK != HAL_CAN_Transmit(&g_handle[_index], 0))
		size = 0; 
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#ifdef USING_OS_FREERTOS	
	xSemaphoreGive( g_tx_mutex[_index] );
#endif

	return size;
}

int32_t can_pwr_mode_trans(const uint8_t _index, const uint8_t _mode)
{
	assert(CAN1_INDEX >= _index);

	switch(_mode)
	{
	case CAN_PWR_MODE_SLEEP:
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		HAL_CAN_Sleep(&g_handle[_index]);
#if defined MX_TB
		HAL_GPIO_WritePin(g_comm_config[_index].trans_stb_n_gpio_, g_comm_config[_index].trans_stb_n_pin_, (GPIO_PinState)0);
#endif
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		break;
	case CAN_PWR_MODE_RUN:	
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
#if defined MX_TB
		HAL_GPIO_WritePin(g_comm_config[_index].trans_stb_n_gpio_, g_comm_config[_index].trans_stb_n_pin_, (GPIO_PinState)1);
#endif
		HAL_CAN_WakeUp(&g_handle[_index]);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		break;
	default:
		break;
	}

    return 0;
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

#if defined MX_TB
/**
 * @brief CAN0 transcevier INH IRQ handler.
 */
void CAN0_TRANS_INH_IRQ_HANDLER(void)
{
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	HAL_GPIO_EXTI_IRQHandler(CAN0_TRANS_INH_PIN);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}

/**
 * @brief CAN1 transcevier INH IRQ handler.
 */
void CAN1_TRANS_INH_IRQ_HANDLER(void)
{
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	HAL_GPIO_EXTI_IRQHandler(CAN1_TRANS_INH_PIN);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}
#else
#endif
/** @} */ /* End of group IRQ handlers. */

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
/**
 * @brief CAN IRQ handler.
 *
 * @param [in] _index CAN index.
 */
static void can_irq_handler(const uint8_t _index)
{
	assert(CAN1_INDEX >=  _index);

#if defined USE_STDPERIPH_DRIVER
	/* FIFO 0 message pending */
	if(0 != CAN_MessagePending(g_handle[_index], CAN_FIFO0))
	{		
		CanRxMsg msg;
	
		/* Receive message */
		CAN_Receive(g_handle[_index], CAN_FIFO0, &msg);
	
		/* Rx fifo is not full */
		if(g_rx_fifo_head[_index] != (g_rx_fifo_tail[_index] + 1) % CAN_FIFO_MAX_SIZE)
		{
			/* Push rx fifo */
			g_rx_fifo[_index][g_rx_fifo_tail[_index]].id_ = (CAN_ID_STD ==  msg.IDE) ? msg.StdId : msg.ExtId;
			g_rx_fifo[_index][g_rx_fifo_tail[_index]].dlc_ = msg.DLC > 8u ? 8u : msg.DLC;
			memcpy(g_rx_fifo[_index][g_rx_fifo_tail[_index]].data_, msg.Data, g_rx_fifo[_index][g_rx_fifo_tail[_index]].dlc_);
			g_rx_fifo_tail[_index] = (g_rx_fifo_tail[_index] + 1u) % CAN_FIFO_MAX_SIZE;
		}
	}
#elif defined USE_HAL_DRIVER
	/* FIFO 0 message pending */
	if(0 != __HAL_CAN_MSG_PENDING(&g_handle[_index], CAN_FIFO0) && RESET != __HAL_CAN_GET_IT_SOURCE(&g_handle[_index], CAN_IT_FMP0))
	{
		/* Get ID */
		g_handle[_index].pRxMsg->IDE = (uint8_t)0x04 & g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RIR;
		if (g_handle[_index].pRxMsg->IDE == CAN_ID_STD)
			g_handle[_index].pRxMsg->StdId = 0x000007FFU & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RIR >> 21U);
		else
			g_handle[_index].pRxMsg->ExtId = 0x1FFFFFFFU & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RIR >> 3U);
		/* Get the RTR */
		g_handle[_index].pRxMsg->RTR = (uint8_t)0x02 & g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RIR;
		/* Get the DLC */
		g_handle[_index].pRxMsg->DLC = (uint8_t)0x0F & g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDTR;
		/* Get the FIFONumber */
		g_handle[_index].pRxMsg->FIFONumber = CAN_FIFO0;
		/* Get the FMI */
		g_handle[_index].pRxMsg->FMI = (uint8_t)0xFF & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDTR >> 8U);
		/* Get the data field */
		g_handle[_index].pRxMsg->Data[0] = (uint8_t)0xFF &  g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDLR;
		g_handle[_index].pRxMsg->Data[1] = (uint8_t)0xFF & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDLR >> 8U);
		g_handle[_index].pRxMsg->Data[2] = (uint8_t)0xFF & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDLR >> 16U);
		g_handle[_index].pRxMsg->Data[3] = (uint8_t)0xFF & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDLR >> 24U);
		g_handle[_index].pRxMsg->Data[4] = (uint8_t)0xFF &  g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDHR;
		g_handle[_index].pRxMsg->Data[5] = (uint8_t)0xFF & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDHR >> 8U);
		g_handle[_index].pRxMsg->Data[6] = (uint8_t)0xFF & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDHR >> 16U);
		g_handle[_index].pRxMsg->Data[7] = (uint8_t)0xFF & (g_handle[_index].Instance->sFIFOMailBox[CAN_FIFO0].RDHR >> 24U);
		
		/* Release FIFO0 */
		__HAL_CAN_FIFO_RELEASE(&g_handle[_index], CAN_FIFO0);
		
		/* Rx fifo is not full */
		if(g_rx_fifo_head[_index] != (g_rx_fifo_tail[_index] + 1) % CAN_FIFO_MAX_SIZE)
		{
			/* Push rx fifo */
			g_rx_fifo[_index][g_rx_fifo_tail[_index]].id_ = (CAN_ID_STD ==  g_handle[_index].pRxMsg->IDE) ?  g_handle[_index].pRxMsg->StdId :g_handle[_index].pRxMsg->ExtId;
			g_rx_fifo[_index][g_rx_fifo_tail[_index]].dlc_ = g_handle[_index].pRxMsg->DLC > 8u ? 8u : g_handle[_index].pRxMsg->DLC;
			memcpy(g_rx_fifo[_index][g_rx_fifo_tail[_index]].data_, g_handle[_index].pRxMsg->Data, g_rx_fifo[_index][g_rx_fifo_tail[_index]].dlc_);
			g_rx_fifo_tail[_index] = (g_rx_fifo_tail[_index] + 1u) % CAN_FIFO_MAX_SIZE;
		}
	}
	/* Error warning */
	if(0 != __HAL_CAN_GET_FLAG(&g_handle[_index], CAN_FLAG_EWG) && RESET != __HAL_CAN_GET_IT_SOURCE(&g_handle[_index], CAN_IT_EWG)){}
	/* Error passive */
	if(0 != __HAL_CAN_GET_FLAG(&g_handle[_index], CAN_FLAG_EPV) && RESET != __HAL_CAN_GET_IT_SOURCE(&g_handle[_index], CAN_IT_EPV)){}
	/* Bus-off */	
	if(0 != __HAL_CAN_GET_FLAG(&g_handle[_index], CAN_FLAG_BOF) && RESET !=  __HAL_CAN_GET_IT_SOURCE(&g_handle[_index], CAN_IT_BOF)){}
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}

/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
	uint16_t  counter_clk_;
	uint16_t  period_;
	IRQn_Type irq_;
}timer_confg_t;

static timer_confg_t g_timer_config[TIMER0_INDEX + 1] =
{
	{
		.counter_clk_  = TIMER0_COUNTER_CLK,
		.period_       = TIMER0_PERIOD,
		.irq_          = TIMER0_IRQ
	}
};

#if defined USE_STDPERIPH_DRIVER
static TIM_TypeDef       *g_handle[TIMER0_INDEX + 1] = {TIMER0_INST};
#elif defined USE_HAL_DRIVER
static TIM_HandleTypeDef g_handle[TIMER0_INDEX + 1] = 
{
	{
		.Instance               = TIMER0_INST,
		.Init.CounterMode       = TIM_COUNTERMODE_UP,
		.Init.Period            = TIMER0_PERIOD - 1,
		.Init.ClockDivision     = 0,
		.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE
	}
};
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
void timer_irq_handler(const uint8_t _index);

/******************************************************************************
 * Functions
 ******************************************************************************/
int32_t timer_init(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

	TIMER_CLK_ENABLE(_index);
#if defined USE_STDPERIPH_DRIVER
	RCC_ClocksTypeDef        clks;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
 	/* Timer initialization */
	RCC_GetClocksFreq(&clks);
	TIM_TimeBaseStructure.TIM_Prescaler     = 2 * clks.PCLK1_Frequency / g_timer_config[_index].counter_clk_ - 1;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period        = g_timer_config[_index].period_ - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseInit(g_handle[_index], &TIM_TimeBaseStructure);
	TIM_ITConfig(g_handle[_index], TIM_IT_Update, ENABLE);
	
	/* NVIC initialization */
	NVIC_InitStructure.NVIC_IRQChannel                   = g_timer_config[_index].irq_;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
#elif defined USE_HAL_DRIVER
	/* Timer initialization */
	g_handle[_index].Init.Prescaler = 2 * HAL_RCC_GetPCLK1Freq() / g_timer_config[_index].counter_clk_ - 1;
	HAL_TIM_Base_Init(&g_handle[_index]);
	
	/* NVIC initialization */
	HAL_NVIC_SetPriority(g_timer_config[_index].irq_, 0, 0);
	HAL_NVIC_EnableIRQ(g_timer_config[_index].irq_);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

    return 0;
}

int32_t timer_deinit(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

#if defined USE_STDPERIPH_DRIVER
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel                   = g_timer_config[_index].irq_;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(g_handle[_index], DISABLE); 
	TIM_ClearITPendingBit(g_handle[TIMER0_INDEX], TIM_IT_Update);
	TIM_ITConfig(g_handle[_index], TIM_IT_Update, DISABLE);
	TIM_DeInit(g_handle[_index]);
#elif defined USE_HAL_DRIVER
	HAL_NVIC_DisableIRQ(g_timer_config[_index].irq_);
	HAL_TIM_Base_Stop_IT(&g_handle[_index]);
	__HAL_TIM_CLEAR_IT(&g_handle[TIMER0_INDEX], TIM_IT_UPDATE);
	HAL_TIM_Base_DeInit(&g_handle[_index]);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
	TIMER_CLK_DISABLE(_index);

	return 0;
}

int32_t timer_start(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

#if defined USE_STDPERIPH_DRIVER
	TIM_Cmd(g_handle[_index], ENABLE); 
#elif defined USE_HAL_DRIVER
	HAL_TIM_Base_Start_IT(&g_handle[_index]);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

    return 0;
}

int32_t timer_stop(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

#if defined USE_STDPERIPH_DRIVER
	TIM_Cmd(g_handle[_index], DISABLE);
#elif defined USE_HAL_DRIVER
	HAL_TIM_Base_Stop_IT(&g_handle[_index]);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif	

	return 0;
}

__attribute__((weak)) void timer_irq_callback(const uint8_t _index)
{
	(void)_index;
}

/**
 * @defgroup IRQ handlers.
 * @{
 */
/**
 * @brief Timer0 IRQ handler.
 */
void TIMER0_IRQ_HANDLER(void)   
{
	timer_irq_handler(TIMER0_INDEX);
}
/** @} */ /* End of group IRQ handlers. */

/******************************************************************************
 * Local Functions
 ******************************************************************************/
/**
 * @brief Timer IRQ handler.
 *
 * @param [in] _index Timer index.
 */
void timer_irq_handler(const uint8_t _index)
{
	/* Timer Update event */
#if defined USE_STDPERIPH_DRIVER
	if(TIM_GetITStatus(g_handle[_index], TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(g_handle[_index], TIM_IT_Update);
		timer_irq_callback(_index);	
	}
#elif defined USE_HAL_DRIVER
	if (RESET != __HAL_TIM_GET_FLAG(&g_handle[_index], TIM_FLAG_UPDATE) && RESET != __HAL_TIM_GET_IT_SOURCE(&g_handle[_index], TIM_IT_UPDATE))
	{
		__HAL_TIM_CLEAR_IT(&g_handle[_index], TIM_IT_UPDATE);
		timer_irq_callback(_index);	
	}	
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif	
}

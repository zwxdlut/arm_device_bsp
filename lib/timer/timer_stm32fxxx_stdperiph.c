/*
 * timer_stm32fxxx_stdperiph.c
 *
 *  Created on: 2018Äê10ÔÂ23ÈÕ
 *      Author: Administrator
 */

#include "timer.inc"

/******************************************************************************
 * Definitions
 ******************************************************************************/
static TIM_TypeDef *g_handle[TIMER0_INDEX + 1] = {TIMER0_INST};

/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
/******************************************************************************
 * Functions
 ******************************************************************************/
int32_t timer_init(const uint8_t _index, const uint32_t _period)
{
	assert(TIMER0_INDEX >= _index);

	RCC_ClocksTypeDef        clks;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef         NVIC_InitStructure;
	
	TIMER_CLK_ENABLE(_index);

 	/* Timer initialization */
	RCC_GetClocksFreq(&clks);
	TIM_TimeBaseStructure.TIM_Prescaler     = 2 * clks.PCLK1_Frequency / g_timer_config[_index].clk_ - 1;
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period        = _period * g_timer_config[_index].clk_ / 1000 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseInit(g_handle[_index], &TIM_TimeBaseStructure);
	TIM_ITConfig(g_handle[_index], TIM_IT_Update, ENABLE);
	
	/* NVIC initialization */
	NVIC_InitStructure.NVIC_IRQChannel                   = g_timer_config[_index].irq_;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 

    return 0;
}

int32_t timer_deinit(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

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
	TIMER_CLK_DISABLE(_index);

	return 0;
}

int32_t timer_start(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

	TIM_Cmd(g_handle[_index], ENABLE); 

    return 0;
}

int32_t timer_stop(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

	TIM_Cmd(g_handle[_index], DISABLE);

	return 0;
}

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
	if(TIM_GetITStatus(g_handle[_index], TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(g_handle[_index], TIM_IT_Update);
		timer_irq_callback(_index);	
	}	
}

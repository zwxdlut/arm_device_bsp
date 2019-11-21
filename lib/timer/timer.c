/*
 * timer.c
 *
 *  Created on: 2018Äê10ÔÂ23ÈÕ
 *      Author: Administrator
 */

#include <assert.h>

#include "timer.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/
/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
#if defined S32K14x
#include "timer_s32k1xx.inc"
#elif defined STM32F10X_CL || defined STM32F205xx
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

void timer_irq_handler(const uint8_t _index);

#if defined USE_STDPERIPH_DRIVER
#include "timer_stm32fxxx_stdperiph.inc"
#elif defined USE_HAL_DRIVER
#include "timer_stm32fxxx_hal.inc"
#else
#error SDK type not defined!!!
#endif

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

#else
#error Mcu type not defined!!!
#endif

/******************************************************************************
 * Functions
 ******************************************************************************/
__attribute__((weak)) void timer_irq_callback(const uint8_t _index)
{
	(void)_index;
}

/******************************************************************************
 * Local Functions
 ******************************************************************************/

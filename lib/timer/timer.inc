/*
 * timer.inc
 *
 *  Created on: 2018Äê10ÔÂ23ÈÕ
 *      Author: Administrator
 */

#include <assert.h>

#include "timer.h"

#if defined STM32F10X_CL || defined STM32F205xx
typedef struct
{ 
	uint16_t  clk_;
	IRQn_Type irq_;
}timer_confg_t;

static timer_confg_t g_timer_config[TIMER0_INDEX + 1] =
{
	{
		.clk_  = TIMER0_CLK,
		.irq_  = TIMER0_IRQ
	}
};

void timer_irq_handler(const uint8_t _index);

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
#endif

/******************************************************************************
 * Definitions
 ******************************************************************************/
/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
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

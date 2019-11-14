/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
	IRQn_Type irq_;
	isr_t     irq_handler;
}timer_confg_t;

static void timer0_irq_handler();
static timer_confg_t g_timer_config[TIMER0_INDEX + 1] =
{
	{
		.irq_        = TIMER0_IRQ,
		.irq_handler = timer0_irq_handler
	}
};

static uint8_t g_handle[TIMER0_INDEX + 1] =
{
#if defined INST_LPTMR0
	INST_LPTMR0,
#else
	0xFF
#endif
};

static const lptmr_config_t *g_config[TIMER0_INDEX + 1] =
{
#if defined INST_LPTMR0
	&lpTmr0_config0,
#else
	NULL
#endif
};

/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
/******************************************************************************
 * Functions
 ******************************************************************************/
int32_t timer_init(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

	/* Timer initialization */
	/* Initialize LPTMR as timer:
	   - interrupt after 1 second
	   - SIRC as clock source
	   - counter disabled */
	LPTMR_DRV_Init(g_handle[_index], g_config[_index], false);

	/* NVIC initialization*/
	/* Install IRQ handler for LPTMR interrupt */
	INT_SYS_InstallHandler(g_timer_config[_index].irq_, g_timer_config[_index].irq_handler, (isr_t *)0);
	/* Enable IRQ for LPTMR */
	INT_SYS_EnableIRQ(g_timer_config[_index].irq_);

    return 0;
}

int32_t timer_deinit(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

	INT_SYS_DisableIRQ(g_timer_config[_index].irq_);
    LPTMR_DRV_ClearCompareFlag(g_handle[_index]);
	LPTMR_DRV_Deinit(g_handle[_index]);

	return 0;
}

int32_t timer_start(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

    /* Start LPTMR counter */
    LPTMR_DRV_StartCounter(g_handle[_index]);

    return 0;
}

int32_t timer_stop(const uint8_t _index)
{
	assert(TIMER0_INDEX >= _index);

    /* Start LPTMR counter */
	LPTMR_DRV_StopCounter(g_handle[_index]);

	return 0;
}

__attribute__((weak)) void timer_irq_callback(const uint8_t _index)
{
	(void)_index;
}

/******************************************************************************
 * Local Functions
 ******************************************************************************/
/**
 * @brief Timer0 IRQ handler.
 */
static void timer0_irq_handler(void)
{
    /* Clear compare flag */
    LPTMR_DRV_ClearCompareFlag(g_handle[TIMER0_INDEX]);
    timer_irq_callback(TIMER0_INDEX);
}

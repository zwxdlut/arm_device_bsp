/*******************************************************************************
 * Definitions
 ******************************************************************************/
static uint32_t g_sys_tick_cnt = 0; /**< SysTick count. */

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/


/*******************************************************************************
 * Functions
 ******************************************************************************/
int32_t sys_init(void)
{
	/* Setup SysTick */
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);

	return 0;
}

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* LEDs initialization */
	LED0_GPIO_CLK_ENABLE();
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin   = LED0_PIN;
	GPIO_INIT(LED0_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_OFF);
	LED1_GPIO_CLK_ENABLE();	
	GPIO_InitStructure.GPIO_Pin = LED1_PIN; 
	GPIO_INIT(LED1_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(LED1_GPIO, LED1_PIN, LED_OFF);
	LED2_GPIO_CLK_ENABLE();
	GPIO_InitStructure.GPIO_Pin = LED2_PIN;
	GPIO_INIT(LED2_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(LED2_GPIO, LED2_PIN, LED_OFF);
	
	/* Button initialization */
	BTN_GPIO_CLK_ENABLE();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitStructure.GPIO_Pin  = BTN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_INIT(BTN_GPIO, &GPIO_InitStructure);
	GPIO_EXTILineConfig(BTN_PORT_SRC, BTN_PIN_SRC);
  	EXTI_InitStructure.EXTI_Line    = BTN_EXTI_LINE;
  	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);
   	NVIC_InitStructure.NVIC_IRQChannel                   = BTN_IRQ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
#if defined MX_TB
	/* Upper computer(EC20) initialization */
	/* Ignition initialization */
#else
#endif
}

void gpio_deinit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
#if defined MX_TB
	NVIC_InitStructure.NVIC_IRQChannel                   = IGN_IRQ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
  	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(IGN_EXTI_LINE);
	GPIO_DEINIT(UC_POWER_GPIO, UC_POWER_PIN);
	GPIO_DEINIT(UC_WAKEUP_GPIO, UC_WAKEUP_PIN);
	GPIO_DEINIT(UC_RESET_GPIO, UC_RESET_PIN);
	GPIO_DEINIT(IGN_GPIO, IGN_PIN);
	UC_POWER_GPIO_CLK_DISABLE();
	UC_WAKEUP_GPIO_CLK_DISABLE();
	UC_RESET_GPIO_CLK_DISABLE();
	IGN_GPIO_CLK_DISABLE();
#else
#endif
	NVIC_InitStructure.NVIC_IRQChannel                   = BTN_IRQ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
  	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(BTN_EXTI_LINE);
	EXTI_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, DISABLE);
	GPIO_DEINIT(LED0_GPIO, LED0_PIN);
	GPIO_DEINIT(LED1_GPIO, LED1_PIN);
	GPIO_DEINIT(LED2_GPIO, LED2_PIN);
	GPIO_DEINIT(BTN_GPIO, BTN_PIN);
	LED0_GPIO_CLK_DISABLE();
	LED1_GPIO_CLK_DISABLE();
	LED2_GPIO_CLK_DISABLE();
	BTN_GPIO_CLK_DISABLE();
}

uint32_t sys_time(void)
{
	return g_sys_tick_cnt;
}

void delay(const uint32_t _ms)
{
	uint32_t start = g_sys_tick_cnt;
    uint32_t delta = 0;
    while (delta < _ms)
		delta = g_sys_tick_cnt - start;
}

void sys_reset(void)
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}

void pwr_mode_trans(const uint8_t _mode)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	/* Suspend SysTick */
	SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk); 
	switch(_mode)
	{
	case PWR_MODE_SLEEP:
		PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
		break;
	case PWR_MODE_DEEPSLEEP:
	    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
	default:
		break;
	}
	SystemInit();
	/* Resume SysTick */
	SysTick->CTRL |= (SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, DISABLE);
}

int32_t wdog_enable(void)
{
#if 0 /* Individual watch dog */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDOG_PRV);
	IWDG_SetReload(IWDOG_RLV);
	IWDG_ReloadCounter();
	IWDG_Enable();
#endif
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	WWDG_ClearFlag();
	WWDG_SetPrescaler(WWDOG_PRV);
	WWDG_SetWindowValue(WWDOG_WV);    
	WWDG_Enable(WWDOG_RLV);
	WWDG_EnableIT();
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);

	return 0;
}

int32_t wdog_refresh(void)
{
#if 0 /* Individual watch dog */
	IWDG_ReloadCounter();	
#endif
	WWDG_SetCounter(WWDOG_RLV);

	return 0;
}

int32_t wdog_disable(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; 
	NVIC_Init(&NVIC_InitStructure);
	WWDG_ClearFlag();
	WWDG_DeInit();
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);

	return 0;
}

/**
 * @defgroup IRQ handlers.
 * @{
 */
/**
 * @brief System tick timer handler.
 */
void SysTick_Handler(void)
{
	g_sys_tick_cnt++;
#if defined USING_OS_FREERTOS
	xPortSysTickHandler();
#endif
}

/**
 * @brief Button IRQ handler.
 */
void BTN_IRQ_HANDLER(void)
{
	if(RESET != EXTI_GetITStatus(BTN_EXTI_LINE))
		EXTI_ClearITPendingBit(BTN_EXTI_LINE);
}

#if defined MX_TB
/**
 * @brief Ignition IRQ handler.
 */
void IGN_IRQ_HANDLER(void)
{
	if(RESET != EXTI_GetITStatus(IGN_EXTI_LINE))
		EXTI_ClearITPendingBit(IGN_EXTI_LINE);
}
#else
#endif

/**
 * @brief Window watch dog IRQ handler.
 */
void WWDG_IRQHandler(void)
{
	sys_reset();
}
/** @} */ /* End of group IRQ handlers. */

/*******************************************************************************
 * Local Functions
 ******************************************************************************/

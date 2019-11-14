#if defined USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
extern void xPortSysTickHandler(void);
#else
#if defined USE_STDPERIPH_DRIVER
static uint32_t g_sys_tick_cnt = 0; /**< SysTick count. */
#elif defined USE_HAL_DRIVER
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
static WWDG_HandleTypeDef g_wdog_handle;
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
static int32_t sys_clk_config(void);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

/*******************************************************************************
 * Functions
 ******************************************************************************/
int32_t sys_init(void)
{
#if defined USE_STDPERIPH_DRIVER
	/* Setup SysTick */
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);
#elif defined USE_HAL_DRIVER
	HAL_Init();
	sys_clk_config();
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
	
	return 0;
}

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
#if defined USE_STDPERIPH_DRIVER
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
#elif defined USE_HAL_DRIVER
	/* LEDs initialization */
	LED0_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH; 
	GPIO_InitStructure.Pin   = LED0_PIN;
	GPIO_INIT(LED0_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(LED0_GPIO, LED0_PIN, LED_OFF);
	LED1_GPIO_CLK_ENABLE();	
	GPIO_InitStructure.Pin = LED1_PIN;
	GPIO_INIT(LED1_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(LED1_GPIO, LED1_PIN, LED_OFF);
	LED2_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Pin = LED2_PIN;  
	GPIO_INIT(LED2_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(LED2_GPIO, LED2_PIN, LED_OFF);
	
	/* Button initialization */
	BTN_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Pin  = BTN_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_INIT(BTN_GPIO, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(BTN_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(BTN_IRQ);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#if defined MX_TB
	/* Upper computer(EC20) initialization */
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	UC_POWER_GPIO_CLK_ENABLE();	
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_NOPULL;	
	GPIO_InitStructure.Pin  = UC_POWER_PIN;
	GPIO_INIT(UC_POWER_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(UC_POWER_GPIO, UC_POWER_PIN, 0);
	UC_WAKEUP_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Pin = UC_WAKEUP_PIN; 
	GPIO_INIT(UC_WAKEUP_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(UC_WAKEUP_GPIO, UC_WAKEUP_PIN, 0);
	UC_RESET_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Pin = UC_RESET_PIN;
	GPIO_INIT(UC_RESET_GPIO, &GPIO_InitStructure);
	GPIO_WRITE_PIN(UC_RESET_GPIO, UC_RESET_PIN, 0);
	
    /* Ignition initialization */
	IGN_GPIO_CLK_ENABLE();
	GPIO_InitStructure.Pin  = IGN_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_INIT(IGN_GPIO, &GPIO_InitStructure);
	HAL_NVIC_SetPriority(IGN_IRQ, 0, 0);
    HAL_NVIC_EnableIRQ(IGN_IRQ);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#else
#endif
}

void gpio_deinit(void)
{
#if defined MX_TB
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	HAL_NVIC_DisableIRQ(IGN_IRQ);
	__HAL_GPIO_EXTI_CLEAR_IT(IGN_PIN);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
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
#if defined USE_STDPERIPH_DRIVER
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel                   = BTN_IRQ;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
  	NVIC_Init(&NVIC_InitStructure);
	EXTI_ClearITPendingBit(BTN_EXTI_LINE);
	EXTI_DeInit();
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, DISABLE);
#elif defined USE_HAL_DRIVER
	HAL_NVIC_DisableIRQ(BTN_IRQ);
	__HAL_GPIO_EXTI_CLEAR_IT(BTN_PIN);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
	GPIO_DEINIT(LED0_GPIO, LED0_PIN);
	GPIO_DEINIT(LED1_GPIO, LED1_PIN);
	GPIO_DEINIT(LED2_GPIO, LED2_PIN);
	GPIO_DEINIT(BTN_GPIO, BTN_PIN);
	LED0_GPIO_CLK_DISABLE();
	LED1_GPIO_CLK_DISABLE();
	LED2_GPIO_CLK_DISABLE();
	BTN_GPIO_CLK_DISABLE();
}

uint32_t sys_time_ms(void)
{
#if defined USE_STDPERIPH_DRIVER
	return g_sys_tick_cnt;
#elif defined USE_HAL_DRIVER
	return HAL_GetTick();
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}

void sys_delay_ms(const uint32_t _ms)
{
#if defined USE_STDPERIPH_DRIVER
	uint32_t start = g_sys_tick_cnt;
    uint32_t delta = 0;
    while (delta < _ms)
		delta = g_sys_tick_cnt - start;
#elif defined USE_HAL_DRIVER
	HAL_Delay(_ms);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}

void sys_reset(void)
{
	__set_FAULTMASK(1);
	NVIC_SystemReset();
}

void pwr_mode_trans(const uint8_t _mode)
{
#if defined USE_STDPERIPH_DRIVER
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
#elif defined USE_HAL_DRIVER
	__HAL_RCC_PWR_CLK_ENABLE();
	/* Suspend Tick increment to prevent wakeup by Systick interrupt
	   Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base) */
	HAL_SuspendTick();
	switch(_mode)
	{
	case PWR_MODE_SLEEP:
		HAL_PWR_EnterSLEEPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
		break;
	case PWR_MODE_DEEPSLEEP:
		HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_SLEEPENTRY_WFI);
	default:
		break;
	}
	sys_clk_config();
	/* Resume Tick interrupt if disabled prior to sleep mode entry */
	HAL_ResumeTick();
	__HAL_RCC_PWR_CLK_DISABLE();
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}

int32_t wdog_enable(void)
{
#if 0 /* Individual watch dog */
#if defined USE_STDPERIPH_DRIVER
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDOG_PRV);
	IWDG_SetReload(IWDOG_RLV);
	IWDG_ReloadCounter();
	IWDG_Enable();
#elif defined USE_HAL_DRIVER
	g_wdog_handle.Instance       = IWDG;
	g_wdog_handle.Init.Prescaler = IWDOG_PRV;
	g_wdog_handle.Init.Reload    = IWDOG_RLV;
	HAL_IWDG_Init(&g_wdog_handle);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#endif
#if defined USE_STDPERIPH_DRIVER
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
#elif defined USE_HAL_DRIVER
	__HAL_RCC_WWDG_CLK_ENABLE();
	g_wdog_handle.Instance       = WWDG;
	g_wdog_handle.Init.Prescaler = WWDOG_PRV;
	g_wdog_handle.Init.Window    = WWDOG_WV;
	g_wdog_handle.Init.Counter   = WWDOG_RLV;
	g_wdog_handle.Init.EWIMode   = WWDG_EWI_ENABLE;
	HAL_WWDG_Init(&g_wdog_handle);
	HAL_NVIC_SetPriority(WWDG_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

	return 0;
}

int32_t wdog_refresh(void)
{
#if 0 /* Individual watch dog */
#if defined USE_STDPERIPH_DRIVER
	IWDG_ReloadCounter();	
#elif defined USE_HAL_DRIVER
	HAL_IWDG_Refresh(&g_wdog_handle);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#endif
#if defined USE_STDPERIPH_DRIVER
	WWDG_SetCounter(WWDOG_RLV);
#elif defined USE_HAL_DRIVER
	HAL_WWDG_Refresh(&g_wdog_handle);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

	return 0;
}

int32_t wdog_disable(void)
{
#if defined USE_STDPERIPH_DRIVER
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE; 
	NVIC_Init(&NVIC_InitStructure);
	WWDG_ClearFlag();
	WWDG_DeInit();
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);
#elif defined USE_HAL_DRIVER
	HAL_NVIC_DisableIRQ(WWDG_IRQn);
	__HAL_WWDG_CLEAR_FLAG(&g_wdog_handle, WWDG_FLAG_EWIF);
	__HAL_RCC_WWDG_CLK_DISABLE();
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

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
#if defined USE_STDPERIPH_DRIVER
	g_sys_tick_cnt++;
#elif defined USE_HAL_DRIVER
	HAL_IncTick();
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#if defined USING_OS_FREERTOS
	xPortSysTickHandler();
#endif
}

/**
 * @brief Button IRQ handler.
 */
void BTN_IRQ_HANDLER(void)
{
#if defined USE_STDPERIPH_DRIVER
	if(RESET != EXTI_GetITStatus(BTN_EXTI_LINE))
		EXTI_ClearITPendingBit(BTN_EXTI_LINE);
#elif defined USE_HAL_DRIVER
	HAL_GPIO_EXTI_IRQHandler(BTN_PIN);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}

#if defined MX_TB
/**
 * @brief Ignition IRQ handler.
 */
void IGN_IRQ_HANDLER(void)
{
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	HAL_GPIO_EXTI_IRQHandler(IGN_PIN);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}
#else
#endif

/**
 * @brief Window watch dog IRQ handler.
 */
void WWDG_IRQHandler(void)
{
#if defined USE_STDPERIPH_DRIVER
	sys_reset();
#elif defined USE_HAL_DRIVER
	/* Early wakeup */
	if(RESET != __HAL_WWDG_GET_FLAG(&g_wdog_handle, WWDG_FLAG_EWIF) && RESET != __HAL_WWDG_GET_IT_SOURCE(&g_wdog_handle, WWDG_IT_EWI))
	{
		/* Clear early wakeup flag */
		__HAL_WWDG_CLEAR_FLAG(&g_wdog_handle, WWDG_FLAG_EWIF);
		sys_reset();
	}
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
}
/** @} */ /* End of group IRQ handlers. */

/*******************************************************************************
 * Local Functions
 ******************************************************************************/
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
/**
 * @brief   System Clock Configuration.
 *
 * @details The system Clock is configured as follow: 
 *          - System Clock source            = PLL (HSE)
 *          - SYSCLK(Hz)                     = 120000000
 *          - HCLK(Hz)                       = 120000000
 *          - AHB Prescaler                  = 1
 *          - APB1 Prescaler                 = 4
 *          - APB2 Prescaler                 = 2
 *          - HSE Frequency(Hz)              = 8000000
 *          - PLL_M                          = 8
 *          - PLL_N                          = 240
 *          - PLL_P                          = 2
 *          - PLL_Q                          = 5
 *          - VDD(V)                         = 3.3
 *          - Flash Latency(WS)              = 3
 *
 * @return  Success(0) or failure(other values).
 */
static int32_t sys_clk_config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	
	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 240;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 5;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);
	
	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
	   clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
	
	return 0;
}
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

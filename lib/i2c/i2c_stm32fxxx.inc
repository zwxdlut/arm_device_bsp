#ifdef USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
static SemaphoreHandle_t g_mutex[I2C0_INDEX + 1] = {NULL};
#endif

/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
	GPIO_TypeDef *gpio_;
	uint16_t     scl_pin_;
	uint16_t     sda_pin_;
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	uint8_t      gpio_af_;
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
	IRQn_Type    irqs_[2];
}comm_config_t;

static comm_config_t g_comm_config[I2C0_INDEX + 1] =
{
	{
		.gpio_     = I2C0_GPIO,
		.scl_pin_  = I2C0_SCL_PIN,
		.sda_pin_  = I2C0_SDA_PIN,
#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
		.gpio_af_ = I2C0_GPIO_AF,
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
		.irqs_ = {I2C0_EV_IRQ, I2C0_ER_IRQ}
	}
};

#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
static I2C_HandleTypeDef g_handle[I2C0_INDEX + 1] = 
{
	{
		.Instance             = I2C0_INST,
		.Init.ClockSpeed      = 400000,
		.Init.DutyCycle       = I2C_DUTYCYCLE_2,
		.Init.OwnAddress1     = 0,
		.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT,
		.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE,
		.Init.OwnAddress2     = 0,
		.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE,
		.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE,
	}
};
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

/******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
static int32_t i2c_master_read(const uint16_t _dev_addr, const uint8_t _mem__addr, uint8_t *const _buf, const uint16_t _size);

/******************************************************************************
 * Functions
 ******************************************************************************/
int32_t i2c_master_init(const uint8_t _index)
{
	assert(I2C0_INDEX >= _index);

	GPIO_InitTypeDef  GPIO_InitStructure;
	
#ifdef USING_OS_FREERTOS
	g_mutex[I2C0_INDEX] = xSemaphoreCreateMutex();
#endif

#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	/* GPIO initialization */
	I2C_GPIO_CLK_ENABLE(_index);
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_OD;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStructure.Alternate = g_comm_config[_index].gpio_af_;
	GPIO_InitStructure.Pin       = g_comm_config[_index].scl_pin_;
	HAL_GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	GPIO_InitStructure.Pin       = g_comm_config[_index].sda_pin_;
	HAL_GPIO_Init(g_comm_config[_index].gpio_, &GPIO_InitStructure);
	
	/* I2C initialization */
	I2C_CLK_ENABLE(_index);
	HAL_I2C_Init(&g_handle[_index]);
	
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

int32_t i2c_master_deinit(const uint8_t _index)
{
	assert(I2C0_INDEX >= _index);

#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_); i++)
		HAL_NVIC_DisableIRQ(g_comm_config[_index].irqs_[i]);
	HAL_I2C_DeInit(&g_handle[_index]);
	I2C_CLK_DISABLE(_index);
	I2C_FORCE_RESET(_index);
	I2C_RELEASE_RESET(_index);
	HAL_GPIO_DeInit(g_comm_config[_index].gpio_, g_comm_config[_index].scl_pin_);
	HAL_GPIO_DeInit(g_comm_config[_index].gpio_, g_comm_config[_index].sda_pin_);
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif
#ifdef USING_OS_FREERTOS
	vSemaphoreDelete(g_mutex[I2C0_INDEX]);
#endif

	return 0;
}

int32_t i2c_master_transmit(const uint8_t _index, const uint16_t _dev_addr, const uint8_t *const _buf, const uint16_t _size, const bool _stop)
{
	assert(I2C0_INDEX >= _index && NULL != _buf);

#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	while(HAL_I2C_Master_Transmit_IT(&g_handle[_index], _dev_addr << 1, (uint8_t*)_buf, _size) != HAL_OK){}
	while (HAL_I2C_GetState(&g_handle[_index]) != HAL_I2C_STATE_READY){}
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

	return 0;
}

int32_t i2c_master_receive(const uint8_t _index, const uint16_t _dev_addr, uint8_t *const _buf, const uint16_t _size, const bool _stop)
{
	assert(I2C0_INDEX >= _index && NULL != _buf);

#if defined USE_STDPERIPH_DRIVER
#elif defined USE_HAL_DRIVER
	while(HAL_I2C_Master_Receive_IT(&g_handle[_index], _dev_addr << 1, (uint8_t*)_buf, _size)!= HAL_OK){}
	while (HAL_I2C_GetState(&g_handle[_index]) != HAL_I2C_STATE_READY){}
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

	return 0;
}

int32_t eeprom_write(const uint8_t _mem__addr, const uint8_t *const _buf, const uint16_t _size)
{
	assert(NULL != _buf);

	int32_t  ret = 0;
	uint8_t  addr = _mem__addr;
	uint8_t  buf[EEPROM_PAGE_SIZE + 1];
	uint16_t size = _size;
	uint16_t n = 0;

#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_mutex[I2C0_INDEX], portMAX_DELAY);
#endif
	/* Page write */
	while(0 == ret && 0 < size)
	{
		n = 8 - addr % 8;
		n = n >= size ? size : n;
		buf[0] = addr;
		memcpy(buf + 1, _buf + _size - size, n);
		ret = i2c_master_transmit(I2C0_INDEX, EEPROM_ADDR, buf, n + 1, true);
		addr += n;
		size -= n;
	}
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_mutex[I2C0_INDEX] );
#endif

	return ret;
}

int32_t eeprom_read(const uint8_t _mem__addr, uint8_t *const _buf, const uint16_t _size)
{
	assert(NULL != _buf);

	int32_t ret = 0;

#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_mutex[I2C0_INDEX], portMAX_DELAY);
#endif
	/* Sequential read */
	ret = i2c_master_read(EEPROM_ADDR, _mem__addr, _buf, _size);
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_mutex[I2C0_INDEX] );
#endif

	return ret;
}

int32_t accr_reset(void)
{
	int32_t ret = 0;
	uint8_t buf[2];

#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_mutex[I2C0_INDEX], portMAX_DELAY);
#endif
	/* Write ACTIVE bit to desired system mode */
	buf[0] = ACCR_CTRL_REG2_REG;
	if(0 == (ret = i2c_master_read(ACCR_ADDR, buf[0], buf + 1, 1)))
	{
		buf[1] |= 0x40;
		ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);
	}
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_mutex[I2C0_INDEX] );
#endif

	return ret;
}

int32_t accr_init(const uint8_t _int_src)
{
	int32_t ret = 0;
	uint8_t buf[2];

#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_mutex[I2C0_INDEX], portMAX_DELAY);
#endif
	/* Configure XYZ data */
	buf[0] = ACCR_XYZ_DATA_CFG_REG;
	buf[1] = 0x01;
	ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);

	/* Configure data rate 100HZ */
	buf[0] = ACCR_CTRL_REG1_REG;
	if(0 == ret && 0 == (ret = i2c_master_read(ACCR_ADDR, buf[0], buf + 1, 1)))
	{
		buf[1] = (0xC7 & buf[1]) | 0x18;
		ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);		
	}

	if(0 == ret && (ACCR_INT_PULSE_MASK & _int_src))
	{
		/* Pulse detection. Configure register PULSE_CFG, PULSE_THSX,Y,Z, PULSE_TMLT, PULSE_LTCY, PULSE_WIND */

		/* Configure configured for single tap and double tap with latch enabled */
		buf[0] = ACCR_PULSE_CFG_REG;
		buf[1] = 0x7F;
		ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);

		/* Set x threshold to 32 counts or 2g */
		buf[0] = ACCR_PULSE_THSX_REG;
		buf[1] = 0x20;
		if(0 == ret)
			ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);

		/* Set y threshold to 32 counts or 2g */
		buf[0] = ACCR_PULSE_THSY_REG;
		buf[1] = 0x20;
		if(0 == ret)
			ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);

		/* Set z threshold to 48 counts or 3g */
		buf[0] = ACCR_PULSE_THSZ_REG;
		buf[1] = 0x30;
		if(0 == ret)
			ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);

		/* Set the pulse time limit to 30 ms at 100Hz ODR in normal mode without LPF: 30 ms/2.5 ms = 12 counts */
		buf[0] = ACCR_PULSE_TMLT_REG;
		buf[1] = 0x0C;
		if(0 == ret)
			ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);

		/* Set the pulse latency timer to 200ms, 100Hz ODR in normal mode without LPF: 200 ms/5.0 ms = 40 counts */
		buf[0] = ACCR_PULSE_LTCY_REG;
		buf[1] = 0x28;
		if(0 == ret)
			ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);

		/* Set the pulse window to 300 ms, 800Hz ODR in normal mode, without LPF: 300 ms/5 ms = 60 counts */
		buf[0] = ACCR_PULSE_WIND_REG;
		buf[1] = 0x3C;
		if(0 == ret)
			ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);
	}

	/* Enable interrupt */
	buf[0] = ACCR_CTRL_REG4_REG;
	buf[1] = _int_src;
	if(0 == ret)
		ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_mutex[I2C0_INDEX] );
#endif

	return ret;
}

int32_t accr_deinit(void)
{
	return 0;
}

int32_t accr_sys_mode_trans(const uint8_t _mode)
{
	int32_t ret = 0;
	uint8_t buf[2];

#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_mutex[I2C0_INDEX], portMAX_DELAY);
#endif
	/* Write ACTIVE bit to desired system mode */
	buf[0] = ACCR_CTRL_REG1_REG;
	if(0 == (ret = i2c_master_read(ACCR_ADDR, buf[0], buf + 1, 1)))
	{
		switch(_mode)
		{
		case ACCR_SYSMOD_STANDBY:
			buf[1] &= 0xFE;
			break;
		case ACCR_SYSMOD_WAKE:
			buf[1] |= 0x01;
			break;
		default:
			break;
		}
	}

	if(0 == ret)
		ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_mutex[I2C0_INDEX] );
#endif

	return ret;
}

uint8_t accr_get_int_src(void)
{
	uint8_t ret = 0;

#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_mutex[I2C0_INDEX], portMAX_DELAY);
#endif
	/* Read interrupt source register */
	i2c_master_read(ACCR_ADDR, ACCR_INT_SOURCE_REG, &ret, 1);
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_mutex[I2C0_INDEX] );
#endif

	return ret;
}

int32_t accr_get_xyz_sample(uint8_t *const _buf, const uint8_t _size)
{
	assert(NULL != _buf);

	int32_t ret = 0;

#ifdef USING_OS_FREERTOS
	xSemaphoreTake( g_mutex[I2C0_INDEX], portMAX_DELAY);
#endif
	/* Read X, Y, Z-axis sample data */
	ret = i2c_master_read(ACCR_ADDR, ACCR_OUT_X_MSB_REG, _buf, _size);
#ifdef USING_OS_FREERTOS 
	xSemaphoreGive( g_mutex[I2C0_INDEX] );
#endif

	return ret;
}

/**
 * @defgroup IRQ handlers.
 * @{
 */
/**
 * @brief I2C0 event IRQ handler.
 */
void I2C0_EV_IRQ_HANDLER(void)
{
  HAL_I2C_EV_IRQHandler(&g_handle[I2C0_INDEX]);
}

/**
 * @brief I2C0 error IRQ handler.
 */
void I2C0_ER_IRQ_HANDLER(void)
{
  HAL_I2C_ER_IRQHandler(&g_handle[I2C0_INDEX]);
}
/** @} */ /* End of group IRQ handlers. */

/******************************************************************************
 * Local Functions
 ******************************************************************************/
/**
 * @brief  Read data from specified slave device memory.
 *
 * @param  [in] _dev_addr  Slave device address.
 * @param  [in] _mem_addr  Slave device memory address.
 * @param  [in] _buf       Read buffer.
 * @param  [in] _size      Read size.
 * @return Success(0) or failure(other values).
 */
static int32_t i2c_master_read(const uint16_t _dev_addr, const uint8_t _mem_addr, uint8_t *const _buf, const uint16_t _size)
{
	assert(NULL != _buf);

	if(0 != i2c_master_transmit(I2C0_INDEX, _dev_addr, &_mem_addr, 1, false))
		return -1;
	return i2c_master_receive(I2C0_INDEX, _dev_addr, _buf, _size, true);
}

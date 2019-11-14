/******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct
{
	PORT_Type    *port_;
	uint8_t      scl_pin_;
	uint8_t      sda_pin_;
	port_mux_t   gpio_af_;
	IRQn_Type    irqs_[1];
}comm_config_t;

static comm_config_t g_comm_config[I2C0_INDEX + 1] =
{
	{
		.port_    = I2C0_PORT,
		.scl_pin_ = I2C0_SCL_PIN,
		.sda_pin_ = I2C0_SDA_PIN,
		.gpio_af_ = I2C0_GPIO_AF,
		.irqs_    = {I2C0_IRQ}
	}
};

static uint8_t g_handle[I2C0_INDEX + 1] =
{
#if defined INST_LPI2C0
	INST_LPI2C0,
#else
	0xFF
#endif
};

static const lpi2c_master_user_config_t *g_config[I2C0_INDEX + 1] =
{
#if defined INST_LPI2C0
	&lpi2c0_MasterConfig0,
#else
	NULL
#endif
};

static lpi2c_master_state_t g_state[I2C0_INDEX + 1];
static mutex_t              g_mutex;

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

	OSIF_MutexCreate(&g_mutex);

	/* GPIO initialization*/
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].scl_pin_, g_comm_config[_index].gpio_af_);
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].sda_pin_, g_comm_config[_index].gpio_af_);

	/* I2C initialization*/
    /* Initialize LPI2C Master configuration:
       - Slave address 0x50
       - Fast operating mode, 400 KHz SCL frequency
       - See LPI2C components for configuration details */
	LPI2C_DRV_MasterInit(g_handle[_index], g_config[_index], &g_state[_index]);

#ifdef USING_OS_FREERTOS
	/* The interrupt calls an interrupt safe API function - so its priority must
	 * be equal to or lower than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY. */
	for(uint8_t i = 0; i < sizeof(g_comm_config[_index].irqs_) && NotAvail_IRQn != g_comm_config[_index].irqs_[i]; i++)
		INT_SYS_SetPriority( g_comm_config[_index].irqs_[i], configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
#endif

	return 0;
}

int32_t i2c_master_deinit(const uint8_t _index)
{
	assert(I2C0_INDEX >= _index);

	LPI2C_DRV_MasterAbortTransferData(g_handle[_index]);
	LPI2C_DRV_MasterDeinit(g_handle[_index]);
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].scl_pin_, PORT_PIN_DISABLED);
	PINS_DRV_SetMuxModeSel(g_comm_config[_index].port_, g_comm_config[_index].sda_pin_, PORT_PIN_DISABLED);
	OSIF_MutexDestroy(&g_mutex);

	return 0;
}

int32_t i2c_master_transmit(const uint8_t _index, const uint16_t _dev_addr, const uint8_t *const _buf, const uint16_t _size, const bool _stop)
{
	assert(I2C0_INDEX >= _index && NULL != _buf);
	LPI2C_DRV_MasterSetSlaveAddr(g_handle[_index], _dev_addr, g_config[_index]->is10bitAddr);
	return LPI2C_DRV_MasterSendDataBlocking(g_handle[_index], _buf, _size, _stop, OSIF_WAIT_FOREVER);
}

int32_t i2c_master_receive(const uint8_t _index, const uint16_t _dev_addr, uint8_t *const _buf, const uint16_t _size, const bool _stop)
{
	assert(I2C0_INDEX >= _index && NULL != _buf);
	LPI2C_DRV_MasterSetSlaveAddr(g_handle[_index], _dev_addr, g_config[_index]->is10bitAddr);
	return LPI2C_DRV_MasterReceiveDataBlocking(g_handle[_index], _buf, _size > 256 ? 256 : _size, _stop, OSIF_WAIT_FOREVER);
}

int32_t eeprom_write(const uint8_t _mem__addr, const uint8_t *const _buf, const uint16_t _size)
{
	assert(NULL != _buf);

	int32_t  ret = 0;
	uint8_t  addr = _mem__addr;
	uint8_t  buf[EEPROM_PAGE_SIZE + 1];
	uint16_t size = _size;
	uint16_t n = 0;

	OSIF_MutexLock(&g_mutex, OSIF_WAIT_FOREVER);
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
	OSIF_MutexUnlock(&g_mutex);

	return ret;
}

int32_t eeprom_read(const uint8_t _mem__addr, uint8_t *const _buf, const uint16_t _size)
{
	assert(NULL != _buf);

	int32_t ret = 0;

	OSIF_MutexLock(&g_mutex, OSIF_WAIT_FOREVER);
	/* Sequential read */
	ret = i2c_master_read(EEPROM_ADDR, _mem__addr, _buf, _size);
	OSIF_MutexUnlock(&g_mutex);

	return ret;
}

int32_t accr_reset(void)
{
	int32_t ret = 0;
	uint8_t buf[2];

	OSIF_MutexLock(&g_mutex, OSIF_WAIT_FOREVER);
	/* Write ACTIVE bit to desired system mode */
	buf[0] = ACCR_CTRL_REG2_REG;
	if(0 == (ret = i2c_master_read(ACCR_ADDR, buf[0], buf + 1, 1)))
	{
		buf[1] |= 0x40;
		ret = i2c_master_transmit(I2C0_INDEX, ACCR_ADDR, buf, 2, true);
	}
	OSIF_MutexUnlock(&g_mutex);

	return ret;
}

int32_t accr_init(const uint8_t _int_src)
{
	int32_t ret = 0;
	uint8_t buf[2];

	OSIF_MutexLock(&g_mutex, OSIF_WAIT_FOREVER);
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
	OSIF_MutexUnlock(&g_mutex);

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

	OSIF_MutexLock(&g_mutex, OSIF_WAIT_FOREVER);
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
	OSIF_MutexUnlock(&g_mutex);

	return ret;
}

uint8_t accr_get_int_src(void)
{
	uint8_t ret = 0;

	OSIF_MutexLock(&g_mutex, OSIF_WAIT_FOREVER);
	/* Read interrupt source register */
	i2c_master_read(ACCR_ADDR, ACCR_INT_SOURCE_REG, &ret, 1);
	OSIF_MutexUnlock(&g_mutex);

	return ret;
}

int32_t accr_get_xyz_sample(uint8_t *const _buf, const uint8_t _size)
{
	assert(NULL != _buf);

	int32_t ret = 0;

	OSIF_MutexLock(&g_mutex, OSIF_WAIT_FOREVER);
	/* Read X, Y, Z-axis sample data */
	ret = i2c_master_read(ACCR_ADDR, ACCR_OUT_X_MSB_REG, _buf, _size);
	OSIF_MutexUnlock(&g_mutex);

	return ret;
}

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

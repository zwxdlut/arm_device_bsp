/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
static uint32_t get_sector(uint32_t _addr);

/*******************************************************************************
 * Functions
 ******************************************************************************/
int32_t flash_ctrl_init(void)
{
	return 0;
}

int32_t flash_ctrl_deinit(void)
{
	return 0;
}

int32_t flash_ctrl_erase_sector(const uint32_t _addr, const uint32_t _size)
{
	assert(_addr >= FLASH_BASE_ADDR && (_addr + _size <= FLASH_BASE_ADDR + FLASH_TOTAL_SIZE));
	
	int32_t  ret = 0;
	
#if defined USE_STDPERIPH_DRIVER
#if defined STM32F10X_CL
	uint32_t start_sector_addr = get_sector(_addr) * FLASH_SECTOR_SIZE + FLASH_BASE_ADDR;
	uint32_t end_sector_addr   = get_sector(_addr + sizeof - 1) * FLASH_SECTOR_SIZE + FLASH_BASE_ADDR;
	ret = FLASH_COMPLETE;
	FLASH_Unlock();
	while(FLASH_COMPLETE == ret && start_sector_addr <= end_sector_addr)
	{
		ret = FLASH_ErasePage(start_sector_addr);
		start_sector_addr += FLASH_SECTOR_SIZE;
	}
	FLASH_Lock();
	if(FLASH_COMPLETE == ret)
		ret = 0;
#elif defined STM32F205xx
#else
#error Mcu type not defined!!!
#endif
#elif defined USE_HAL_DRIVER
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t               SECTORError = 0;
	EraseInitStruct.TypeErase     = TYPEERASE_SECTORS;
	EraseInitStruct.Sector        = get_sector(_addr);
	EraseInitStruct.NbSectors     = get_sector(_addr + _size - 1) - EraseInitStruct.Sector + 1;
	EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	HAL_FLASH_Unlock();
	ret = HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
	HAL_FLASH_Lock();
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

    return ret;
}

int32_t flash_ctrl_verify_sector(const uint32_t _addr, const uint32_t _size)
{
	assert(_addr >= FLASH_BASE_ADDR && (_addr + _size <= FLASH_BASE_ADDR + FLASH_TOTAL_SIZE));

	int32_t  ret = 0;
	uint32_t i = 0;
	
	while(i < _size)
	{
		if(_size - i >= 4)
		{
			if(0xFFFFFFFF != *(__IO uint32_t *)(_addr + i))
				ret++;
			i += 4;
		}
		else if(_size - i >= 2)
		{
			if(0xFFFF != *(__IO uint16_t *)(_addr + i))
				ret++;
			i += 2;
		}
		else
		{
			if(0xFF != *(__IO uint8_t *)(_addr + i))
				ret++;
			i++;
		}
	}

    return ret;
}

bool flash_ctrl_is_sector_aligned(const uint32_t _addr)
{
	assert(_addr >= FLASH_BASE_ADDR && _addr <= FLASH_BASE_ADDR + FLASH_TOTAL_SIZE);

#if defined STM32F10X_CL
	return (0 == (_addr - FLASH_BASE_ADDR) % FLASH_SECTOR_SIZE );
#elif defined STM32F205xx
	if((ADDR_FLASH_SECTOR_0 == _addr) || (ADDR_FLASH_SECTOR_1 == _addr) || (ADDR_FLASH_SECTOR_2 == _addr) ||
	   (ADDR_FLASH_SECTOR_3 == _addr) || (ADDR_FLASH_SECTOR_4 == _addr) || (ADDR_FLASH_SECTOR_5 == _addr) ||
	   (ADDR_FLASH_SECTOR_6 == _addr) || (ADDR_FLASH_SECTOR_7 == _addr) || (ADDR_FLASH_SECTOR_8 == _addr) ||
	   (ADDR_FLASH_SECTOR_9 == _addr) || (ADDR_FLASH_SECTOR_10 == _addr) || (ADDR_FLASH_SECTOR_11 == _addr))
		return true;
	else
		return false;
#else
#error Mcu type not defined!!!
#endif	
}

int32_t flash_ctrl_program(const uint32_t _addr, const uint32_t _size, const uint8_t *const _buf)
{
	assert(_addr >= FLASH_BASE_ADDR && (_addr + _size <= FLASH_BASE_ADDR + FLASH_TOTAL_SIZE) && NULL != _buf);
	
	int32_t  ret = 0;
	uint32_t i = 0;
	
#if defined USE_STDPERIPH_DRIVER
	assert(0 != _size % 2);
	ret = FLASH_COMPLETE;
	FLASH_Unlock();
	while(FLASH_COMPLETE == ret && i < _size)
	{
		if(_size - i >= 4)
		{			
			ret = FLASH_ProgramWord( _addr + i, *((uint32_t*)(_buf + i)));
			i += 4;
		}
		else
		{
			ret = FLASH_ProgramHalfWord(_addr + i, *((uint16_t*)(_buf + i)));
			i += 2;
		}
	}
	FLASH_Lock();
	if(FLASH_COMPLETE == ret)
		ret = 0;
#elif defined USE_HAL_DRIVER
	HAL_FLASH_Unlock();
	while(HAL_OK == ret && i < _size)
	{
		if(_size - i >= 4)
		{
			ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, _addr + i, *((uint32_t*)(_buf + i)));
			i += 4;
		}
		else if(_size - i >= 2)
		{
			ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, _addr + i, *((uint16_t*)(_buf + i)));
			i += 2;
		}
		else
		{
			ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, _addr + i, *(_buf + i));
			i++;
		}
	}
	HAL_FLASH_Lock();
#else
#error Please define SDK type(USE_STDPERIPH_DRIVER or USE_HAL_DRIVER)!!!
#endif

    return ret;
}

int32_t flash_ctrl_program_verify(const uint32_t _addr, const uint32_t _size, const uint8_t *const _buf)
{
	assert(_addr >= FLASH_BASE_ADDR && (_addr + _size <= FLASH_BASE_ADDR + FLASH_TOTAL_SIZE) && NULL != _buf);

	int32_t  ret = 0;
	uint32_t i = 0;

	while(i < _size)
	{
		if(_size - i >= 4)
		{
			if(*((uint32_t *)(_buf + i)) != *(__IO uint32_t *)(_addr + i))
				ret++;
			i += 4;
		}
		else if(_size - i >= 2)
		{
			if(*((uint16_t *)(_buf + i)) != *(__IO uint16_t *)(_addr + i))
				ret++;
			i += 2;
		}
		else
		{
			if(*((uint8_t *)(_buf + i)) != *(__IO uint8_t *)(_addr + i))
				ret++;
			i++;
		}
    }

    return ret;
}

int32_t flash_ctrl_write_e2(const uint32_t _addr, const uint32_t _size, const uint8_t *const _buf)
{
	return 0;
}

/*******************************************************************************
 * Local Function prototypes
 ******************************************************************************/
/**
 * @brief  Get the sector of a given address
 *
 * @param  [in] _addr Address.
 * @return Sector of the given address
 */
static uint32_t get_sector(const uint32_t _addr)
{
	assert(_addr >= FLASH_BASE_ADDR && _addr <= FLASH_BASE_ADDR + FLASH_TOTAL_SIZE);

	uint32_t sector = 0;

#if defined STM32F10X_CL
	sector = (_addr - FLASH_BASE_ADDR) / FLASH_SECTOR_SIZE;
#elif defined STM32F205xx
	if((_addr < ADDR_FLASH_SECTOR_1) && (_addr >= ADDR_FLASH_SECTOR_0))
	{
		sector = FLASH_SECTOR_0;
	}
	else if((_addr < ADDR_FLASH_SECTOR_2) && (_addr >= ADDR_FLASH_SECTOR_1))
	{
		sector = FLASH_SECTOR_1;
	}
	else if((_addr < ADDR_FLASH_SECTOR_3) && (_addr >= ADDR_FLASH_SECTOR_2))
	{
		sector = FLASH_SECTOR_2;
	}
	else if((_addr < ADDR_FLASH_SECTOR_4) && (_addr >= ADDR_FLASH_SECTOR_3))
	{
		sector = FLASH_SECTOR_3;
	}
	else if((_addr < ADDR_FLASH_SECTOR_5) && (_addr >= ADDR_FLASH_SECTOR_4))
	{
		sector = FLASH_SECTOR_4;
	}
	else if((_addr < ADDR_FLASH_SECTOR_6) && (_addr >= ADDR_FLASH_SECTOR_5))
	{
		sector = FLASH_SECTOR_5;
	}
	else if((_addr < ADDR_FLASH_SECTOR_7) && (_addr >= ADDR_FLASH_SECTOR_6))
	{
		sector = FLASH_SECTOR_6;
	}
	else if((_addr < ADDR_FLASH_SECTOR_8) && (_addr >= ADDR_FLASH_SECTOR_7))
	{
		sector = FLASH_SECTOR_7;
	}
	else if((_addr < ADDR_FLASH_SECTOR_9) && (_addr >= ADDR_FLASH_SECTOR_8))
	{
		sector = FLASH_SECTOR_8;
	}
	else if((_addr < ADDR_FLASH_SECTOR_10) && (_addr >= ADDR_FLASH_SECTOR_9))
	{
		sector = FLASH_SECTOR_9;
	}
	else if((_addr < ADDR_FLASH_SECTOR_11) && (_addr >= ADDR_FLASH_SECTOR_10))
	{
		sector = FLASH_SECTOR_10;
	}
	else /* (Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11) */
	{
		sector = FLASH_SECTOR_11;
	}
#else
#error Mcu type not defined!!!
#endif
	
	return sector;
}

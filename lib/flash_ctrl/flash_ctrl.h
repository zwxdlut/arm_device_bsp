/*
 * flash_ctrl.h
 *
 *  Created on: 2018Äê10ÔÂ17ÈÕ
 *      Author: Administrator
 */

#ifndef __FLASH_CTRL_H__
#define __FLASH_CTRL_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** 
 * @defgroup Flash configuration.
 * @{
 */
#if defined S32K14x
#include "Cpu.h"
#define FLASH_BASE_ADDR 			                                                 0x00000000
#define FLASH_SECTOR_SIZE                                                            4096                   /**< 4k. */
#define FLASH_TOTAL_SIZE                                                             0x80000                /**< 512k. */
#elif defined STM32F10X_CL
#include "stm32f10x.h"
#define FLASH_BASE_ADDR 			                                                 0x08000000
#define FLASH_SECTOR_SIZE                                                            2048                   /**< 2k. */
#define FLASH_TOTAL_SIZE                                                             0x40000                /**< 256k. */
#elif defined STM32F205xx
#include "stm32f2xx.h"
#define FLASH_BASE_ADDR 			                                                 0x08000000
#define ADDR_FLASH_SECTOR_0                                                          FLASH_BASE_ADDR        /**< Base @ of Sector 0,  16 Kbytes  */
#define ADDR_FLASH_SECTOR_1                                                          ((uint32_t)0x08004000) /**< Base @ of Sector 1,  16 Kbytes  */
#define ADDR_FLASH_SECTOR_2                                                          ((uint32_t)0x08008000) /**< Base @ of Sector 2,  16 Kbytes  */
#define ADDR_FLASH_SECTOR_3                                                          ((uint32_t)0x0800C000) /**< Base @ of Sector 3,  16 Kbytes  */
#define ADDR_FLASH_SECTOR_4                                                          ((uint32_t)0x08010000) /**< Base @ of Sector 4,  64 Kbytes  */
#define ADDR_FLASH_SECTOR_5                                                          ((uint32_t)0x08020000) /**< Base @ of Sector 5,  128 Kbytes */
#define ADDR_FLASH_SECTOR_6                                                          ((uint32_t)0x08040000) /**< Base @ of Sector 6,  128 Kbytes */
#define ADDR_FLASH_SECTOR_7                                                          ((uint32_t)0x08060000) /**< Base @ of Sector 7,  128 Kbytes */
#define ADDR_FLASH_SECTOR_8                                                          ((uint32_t)0x08080000) /**< Base @ of Sector 8,  128 Kbytes */
#define ADDR_FLASH_SECTOR_9                                                          ((uint32_t)0x080A0000) /**< Base @ of Sector 9,  128 Kbytes */
#define ADDR_FLASH_SECTOR_10                                                         ((uint32_t)0x080C0000) /**< Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11                                                         ((uint32_t)0x080E0000) /**< Base @ of Sector 11, 128 Kbytes */
#define FLASH_TOTAL_SIZE                                                             0x80000    /**< 512k. */
#else
#error Mcu type not defined!!!
#endif
/** @} */ /* End of group Flash configuration.*/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * @brief  Initialize flash control module.
 *
 * @return Success(0) or failure(other values).
 */
int32_t flash_ctrl_init(void);

/**
 * @brief  De-initialize flash control module.
 *
 * @return Success(0) or failure(other values).
 */
int32_t flash_ctrl_deinit(void);

/** 
 * @brief  Erase continuous sectors.
 *
 * @param  [in] _addr Sector start address.
 * @param  [in] _size Erase size aligned to sector.
 * @return Success(0) or failure(other values).
 */
int32_t flash_ctrl_erase_sector(const uint32_t _addr, const uint32_t _size);

/**
 * @brief  Verify continuous sectors.
 *
 * @param  [in] _addr Sector start address.
 * @param  [in] _size Erase size aligned to sector.
 * @return Success(0) or failure(other values).
 */
int32_t flash_ctrl_verify_sector(const uint32_t _addr, const uint32_t _size);

/**
 * @brief  Check if the the address is sector aligned.
 *
 * @param  [in] _addr Address.
 * @return Aligned(true) or not(false).
 */
bool flash_ctrl_is_sector_aligned(const uint32_t _addr);

/**
 * @brief  Program flash memory.
 *
 * @param  [in] _addr Program start address.
 * @param  [in] _size Program size.
 * @param  [in] _buf  Program buffer.
 * @return Success(0) or failure(other values).
 */
int32_t flash_ctrl_program(const uint32_t _addr, const uint32_t _size, const uint8_t *const _buf);

/**
 * @brief  Verify programed flash memory.
 *
 * @param  [in] _addr Verify start address.
 * @param  [in] _size Verify size.
 * @param  [in] _buf  Verify buffer.
 * @return Success(0) or failure(other values).
 */
int32_t flash_ctrl_program_verify(const uint32_t _addr, const uint32_t _size, const uint8_t *const _buf);

/**
 * @brief  Write data to a flash area which is partitioned as EEPROM.
 *
 * @param  [in] _addr Write start address.
 * @param  [in] _size Write size.
 * @param  [in] _buf  Write buffer.
 * @return Success(0) or failure(other values).
 */
int32_t flash_ctrl_write_e2(const uint32_t _addr, const uint32_t _size, const uint8_t *const _buf);

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_CTRL_H__ */

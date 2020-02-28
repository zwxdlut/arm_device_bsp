/*
 * i2c.h
 *
 *  Created on: 2019��1��9��
 *      Author: Administrator
 */

#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#if defined S32K14x
#include "i2c_s32k1xx.h"
#elif defined STM32F10X_CL
#include "i2c_stm32f10x.h"
#elif defined STM32F205xx
#include "i2c_stm32f2xx.h"
#else
#error Mcu type not defined!!!
#endif

#if defined USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup I2C module index.
 * @{
 */
#define I2C0_INDEX       		                0
/** @} */ /* End of group I2C module index. */

/** 
 * @defgroup EEPROM configuration.
 * @brief    Two-wire serial EEPROM AT24C02 2K(256 * 8 bit) 32 pages of 8 bytes each
 * @{
 */
#define EEPROM_ADDR              			     0x50 /**< Device address, 7bit without R/W bit. */
#define EEPROM_PAGE_SIZE                         8    /**< Page size in bytes. */
#define EEPROM_TOTAL_SIZE                        256  /**< Total size in bytes. */
#define EEPROM_ADDR_RESET_TYPE                   0x00
#define EEPROM_SIZE_RESET_TYPE                   1
#define EEPROM_ADDR_SSECUHWVN                    (EEPROM_ADDR_RESET_TYPE + EEPROM_SIZE_RESET_TYPE)
#define EEPROM_SIZE_SSECUHWVN                    16
#define EEPROM_ADDR_VIN                          (EEPROM_ADDR_SSECUHWVN + EEPROM_SIZE_SSECUHWVN)
#define EEPROM_SIZE_VIN                          17
#define EEPROM_ADDR_ECUMD                        (EEPROM_ADDR_VIN + EEPROM_SIZE_VIN)
#define EEPROM_SIZE_ECUMD                        4
#define EEPROM_ADDR_TBTN                         (EEPROM_ADDR_ECUMD + EEPROM_SIZE_ECUMD)
#define EEPROM_SIZE_TBTN                         15
#define EEPROM_ADDR_IMEI                         (EEPROM_ADDR_TBTN + EEPROM_SIZE_TBTN)
#define EEPROM_SIZE_IMEI                         15
#define EEPROM_ADDR_ICCID                        (EEPROM_ADDR_IMEI + EEPROM_SIZE_IMEI)
#define EEPROM_SIZE_ICCID                        20
#define EEPROM_ADDR_PK                           (EEPROM_ADDR_ICCID + EEPROM_SIZE_ICCID)
#define EEPROM_SIZE_PK                           16
#define EEPROM_ADDR_FP1                          (EEPROM_ADDR_PK + EEPROM_SIZE_PK)
#define EEPROM_SIZE_FP1                          9
#define EEPROM_ADDR_FP2                          (EEPROM_ADDR_FP1 + EEPROM_SIZE_FP1)
#define EEPROM_SIZE_FP2                          20
#define EEPROM_ADDR_EOL                          (EEPROM_ADDR_FP2 + EEPROM_SIZE_FP2)
#define EEPROM_SIZE_EOL                          56
#define EEPROM_ADDR_DTC                          (EEPROM_ADDR_EOL + EEPROM_SIZE_EOL)
#define EEPROM_SIZE_DTC                          21
#define EEPROM_ADDR_INIT                         EEPROM_TOTAL_SIZE - 1
#define EEPROM_SIZE_INIT                         1
/** @} */ /* End of group EEPROM configuration. */

/** 
 * @defgroup Accelerometer configuration.
 * @brief    3-axis, 12-bit/8-bit digital accelerometer MMA8452Q
 * @{
 */
#define ACCR_ADDR                       		0x1C /**< Device address, 7bit without R/W bit. */
#define ACCR_STATUS_REG	                        0x00
#define ACCR_OUT_X_MSB_REG	                    0x01
#define ACCR_OUT_X_LSB_REG	                    0x02
#define ACCR_OUT_Y_MSB_REG	                    0x03
#define ACCR_OUT_Y_LSB_REG	                    0x04
#define ACCR_OUT_Z_MSB_REG	                    0x05
#define ACCR_OUT_Z_LSB_REG	                    0x06
//#define ACCR_RESERVED_REG	                    0x07
//#define ACCR_RESERVED_REG	                    0x08
#define ACCR_SYSMOD_REG	                        0x0B
#define ACCR_INT_SOURCE_REG	                    0x0C
#define ACCR_WHO_AM_I_REG	                    0x0D
#define ACCR_XYZ_DATA_CFG_REG	                0x0E
#define ACCR_P_FILTER_CUTOFF_REG	            0x0F
#define ACCR_PL_STATUS_REG	                    0x10
#define ACCR_PL_CFG_REG	                        0x11
#define ACCR_PL_COUNT_REG	                    0x12
#define ACCR_PL_BF_ZCOMP_REG	                0x13
#define ACCR_P_L_THS_REG_REG	                0x14
#define ACCR_FF_MT_CFG_REG	                    0x15
#define ACCR_FF_MT_SRC_REG	                    0x16
#define ACCR_FF_MT_THS_REG	                    0x17
#define ACCR_FF_MT_COUNT_REG	                0x18
//#define ACCR_RESERVED_REG	                    0x19-0x1C
#define ACCR_TRANSIENT_CFG_REG	                0x1D
#define ACCR_TRANSIENT_SRC_REG	                0x1E
#define ACCR_TRANSIENT_THS_REG	                0x1F
#define ACCR_RANSIENT_COUNT_REG	                0x20
#define ACCR_PULSE_CFG_REG		                0x21
#define ACCR_PULSE_SRC_REG		                0x22
#define ACCR_PULSE_THSX_REG		                0x23
#define ACCR_PULSE_THSY_REG		                0x24
#define ACCR_PULSE_THSZ_REG		                0x25
#define ACCR_PULSE_TMLT_REG		                0x26
#define ACCR_PULSE_LTCY_REG		                0x27
#define ACCR_PULSE_WIND_REG		                0x28
#define ACCR_ASLP_COUNT_REG		                0x29
#define ACCR_CTRL_REG1_REG		                0x2A
#define ACCR_CTRL_REG2_REG		                0x2B
#define ACCR_CTRL_REG3_REG		                0x2C
#define ACCR_CTRL_REG4_REG		                0x2D
#define ACCR_CTRL_REG5_REG		                0x2E
#define ACCR_OFF_X_REG			                0x2F
#define ACCR_OFF_Y_REG			                0x30
#define ACCR_OFF_Z_REG			                0x31
//#define ACCR_RESERVED_REG		                0x40-0x7F
/** @} */ /* End of group Accelerometer configuration. */

/** 
 * @defgroup Accelerometer system modes.
 * @{
 */
#define ACCR_SYSMOD_STANDBY                     0u
#define ACCR_SYSMOD_ACTIVE                      1u
/** @} */ /* End of group Accelerometer system modes. */

/** 
 * @defgroup Accelerometer interrupt source mask.
 * @{
 */
#define ACCR_INT_NONE_MASK                      0x00
#define ACCR_INT_ASLP_MASK                      0x80
#define ACCR_INT_TRANS_MASK                     0x20
#define ACCR_INT_LNDPRT_MASK                    0x10
#define ACCR_INT_PULSE_MASK                     0x08
#define ACCR_INT_FF_MT_MASK                     0x04
#define ACCR_INT_DRDY_MASK                      0x01
/** @} */ /* End of group Accelerometer interrupt source mask. */

/******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * @brief  Initialize I2C bus controller as master.
 *
 * @param  [in] _index         I2C index.
 * @param  [in] _baudrate      Baud rate.
 * @param  [in] _is_10bit_addr Selects 7-bit or 10-bit slave address.
 * @return Success(0) or failure(other values).
 */
int32_t i2c_master_init(const uint8_t _index, const uint32_t _baudrate, const bool _is_10bit_addr);

/**
 * @brief  De-initialize I2C bus controller as master.
 *
 * @param  [in] _index I2C index.
 * @return Success(0) or failure(other values).
 */
int32_t i2c_master_deinit(const uint8_t _index);

/**
 * @brief  Receive data from specified slave device.
 *
 * @param  [in]  _index I2C index.
 * @param  [in]  _addr  Slave address(7 bit without R/W bit).
 * @param  [out] _buf   Receive buffer.
 * @param  [in]  _size  Receive size.
 * @param  [in]  _stop  Specify whether generate stop condition after sending.
 * @return Success(0) or failure(other values).
 */
int32_t i2c_master_receive(const uint8_t _index, const uint16_t _addr, uint8_t *const _buf, const uint16_t _size, const bool _stop);

/**
 * @brief  Transmit data to specified slave device.
 *
 * @param  [in] _index I2C index.
 * @param  [in] _addr  Slave address(7 bit without R/W bit).
 * @param  [in] _buf   Transmit buffer.
 * @param  [in] _size  Transmit size.
 * @param  [in] _stop  Specify whether generate stop condition after sending.
 * @return Success(0) or failure(other values).
 */
int32_t i2c_master_transmit(const uint8_t _index, const uint16_t _addr, const uint8_t *const _buf, const uint16_t _size, const bool _stop);

/**
 * @brief  Read data from EEPROM.
 *
 * @param  [in]  _addr EEPROM memory address.
 * @param  [out] _buf  Read buffer.
 * @param  [in]  _size Read size.
 * @return Success(0) or failure(other values).
 */
int32_t eeprom_read(const uint8_t _addr, uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Write data to EEPROM.
 *
 * @param  [in] _addr EEPROM memory address.
 * @param  [in] _buf  Write buffer.
 * @param  [in] _size Write size.
 * @return Success(0) or failure(other values).
 */
int32_t eeprom_write(const uint8_t _addr, const uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Reset accelerometer.
 *
 * @return Success(0) or failure(other values).
 */
int32_t accr_reset(void);

/**
 * @brief  Initialize accelerometer.
 *
 * @param  [in] _int_src Interrupt source, "OR" operation by ACCR_INT_XXX_MASK.
 * @return Success(0) or failure(other values).
 */
int32_t accr_init(const uint8_t _int_src);

/**
 * @brief  De-initialize accelerometer.
 *
 * @return Success(0) or failure(other values).
 */
int32_t accr_deinit(void);

/**
 * @brief  Transfer accelerometer system mode.
 *
 * @param  [in] _mode System mode:
 *                    @arg ACCR_SYSMOD_STANDBY Standby.
 *				      @arg ACCR_SYSMOD_ACTIVE  Active.
 * @return Success(0) or failure(other values).
 */
int32_t accr_sys_mode_trans(const uint8_t _mode);

/**
 * @brief  Get interrupt source.
 *
 * @return Interrupt source combination of ACCR_INT_XXX.
 */
uint8_t accr_get_int_src(void);

/**
 * @brief  Get X, Y, Z-axis sample data(MSB).
 *
 * @param  [in] _buf  Data buffer.
 * @param  [in] _size Data size.
 * @return Success(0) or failure(other values).
 */
int32_t accr_get_xyz_sample(uint8_t *const _buf, const uint8_t _size);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

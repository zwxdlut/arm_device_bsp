/*
 * spi.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __SPI_H__
#define __SPI_H__

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup SPI module index.
 * @{
 */
#define SPI0_INDEX       		                0
/** @} */ /* End of group SPI module index. */

/**
 * @defgroup SPI clock polarity.
 * @{
 */
#define SPI_CPOL_LOW      		                0
#define SPI_CPOL_HIGH      		                1
/** @} */ /* End of group SPI clock polarity. */

/**
 * @defgroup SPI clock phase.
 * @{
 */
#define SPI_CPHA_1EDGE      		            0
#define SPI_CPHA_2EDGE      		            1
/** @} */ /* End of group SPI clock phase. */

/**
 * @defgroup SPI data bits.
 * @{
 */
#define SPI_DATA_BITS_8      		            8
#define SPI_DATA_BITS_16      		            16
/** @} */ /* End of group SPI data bits. */

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * @brief  Initialize master SPI.
 *
 * @param  [in] _index    SPI index.
 * @param  [in] _baudrate Baud rate.
 * @param  [in] _cpol     Clock polarity:
 *              @arg SPI_CPOL_LOW  Clock is idle low.
 *              @arg SPI_CPOL_HIGH Clock is idle high.
 * @param  [in] _cpha Clock phase:
 *              @arg SPI_CPHA_1EDGE Data is sampled on clock 1st edge, changed on 2nd.
 *              @arg SPI_CPHA_2EDGE Data is sampled on clock 2nd edge, changed on 1st.
 * @param  [in] _data_bits Data bits:
 *              @arg SPI_DATA_BITS_8  8 bits data.
 *              @arg SPI_DATA_BITS_16 16 bits data.
 * @param  [in] _lsb_first Select LSB or MSB first.
 * @return Success(0) or failure(other values).
 */
int32_t spi_master_init(const uint8_t _index, const uint32_t _baudrate, const uint8_t _cpol, const uint8_t _cpha, const uint8_t _data_bits, const bool _lsb_first);

/**
 * @brief  De-initialize master SPI.
 *
 * @param  [in] _index SPI index.
 * @return Success(0) or failure(other values).
 */
int32_t spi_master_deinit(const uint8_t _index);

/**
 * @brief  Receive master SPI data.
 *
 * @param  [in]  _index SPI index.
 * @param  [out] _buf   Receive buffer.
 * @param  [in]  _size  Receive size.
 * @return Success(0) or failure(other values).
 */
int32_t spi_master_receive(const uint8_t _index, uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Transmit master SPI data.
 *
 * @param  [in] _index SPI index.
 * @param  [in] _buf   Transmit buffer.
 * @param  [in] _size  Transmit size.
 * @return Success(0) or failure(other values).
 */
int32_t spi_master_transmit(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Initialize slave SPI.
 *
 * @param  [in] _index SPI index.
 * @param  [in] _cpol  Clock polarity:
 *              @arg SPI_CPOL_LOW  Signal is idle low.
 *              @arg SPI_CPOL_HIGH Signal is idle high.
 * @param  [in] _cpha Clock phase:
 *              @arg SPI_CPOL_1EDGE Data is sampled on SCK 1st edge, changed on 2nd.
 *              @arg SPI_CPOL_2EDGE Data is sampled on SCK 2nd edge, changed on 1st.
 * @param  [in] _data_bits Data bits:
 *              @arg SPI_DATA_BITS_8  8 bits data.
 *              @arg SPI_DATA_BITS_16 16 bits data.
 * @param  [in] _lsb_first Select LSB or MSB first.
 * @return Success(0) or failure(other values).
 */
int32_t spi_slave_init(const uint8_t _index, const uint8_t _cpol, const uint8_t _cpha, const uint8_t _data_bits, const bool _lsb_first);

/**
 * @brief  De-initialize slave SPI.
 *
 * @param  [in] _index SPI index.
 * @return Success(0) or failure(other values).
 */
int32_t spi_slave_deinit(const uint8_t _index);

/**
 * @brief  Receive slave SPI data.
 *
 * @param  [in]  _index SPI index.
 * @param  [out] _buf   Receive buffer.
 * @param  [in]  _size  Receive size.
 * @return Success(0) or failure(other values).
 */
int32_t spi_slave_receive(const uint8_t _index, uint8_t *const _buf, const uint16_t _size);

/**
 * @brief  Transmit slave SPI data.
 *
 * @param  [in] _index SPI index.
 * @param  [in] _buf   Transmit buffer.
 * @param  [in] _size  Transmit size.
 * @return Success(0) or failure(other values).
 */
int32_t spi_slave_transmit(const uint8_t _index, const uint8_t *const _buf, const uint16_t _size);

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */
/*
 * spi_stm32f10x.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __SPI_STM32F10X_H__
#define __SPI_STM32F10X_H__

#include "stm32f10x.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup SPI configuration.
 * @{
 */
#if defined CPATAIN_DESIGN
#else
#error Board type not defined!!!
#endif
/** @} */ /* End of group SPI configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_STM32F10X_H__ */

/*
 * spi_s32k1xx.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __SPI_S32K1XX_H__
#define __SPI_S32K1XX_H__

#include "Cpu.h"

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
#if defined MX_TB || defined EVB || defined FRDM
#define SPI0_SCK_PORT                           PORTB
#define SPI0_SCK_PIN                            14
#define SPI0_SDI_PORT                           PORTD
#define SPI0_SDI_PIN                            1
#define SPI0_SDO_PORT                           PORTD
#define SPI0_SDO_PIN                            2
#define SPI0_CS_PORT                            PORTD
#define SPI0_CS_PIN                             3
#define SPI0_GPIO_AF                            PORT_MUX_ALT3
#define SPI0_IRQ                                LPSPI1_IRQn
#else
#error Board type not defined!!!
#endif
/** @} */ /* End of group SPI configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_S32K1XX_H__ */

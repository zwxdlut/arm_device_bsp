/*
 * spi_stm32f2xx.h
 *
 *  Created on: 2018��10��16��
 *      Author: Administrator
 */

#ifndef __SPI_STM32F2XX_H__
#define __SPI_STM32F2XX_H__

#include "stm32f2xx.h"

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
#if defined MX_TB
#define SPI0_SCK_GPIO                           GPIOC
#define SPI0_SCK_PIN                            GPIO_PIN_10
#define SPI0_MISO_GPIO                          GPIOC
#define SPI0_MISO_PIN                           GPIO_PIN_11
#define SPI0_MOSI_GPIO                          GPIOC
#define SPI0_MOSI_PIN                           GPIO_PIN_12
#define SPI0_CS_GPIO                            GPIOA
#define SPI0_CS_PIN                             GPIO_PIN_4
#define SPI0_GPIO_AF                            GPIO_AF6_SPI3
#define SPI0_INST     		                    SPI3
#define SPI0_IRQ                                SPI3_IRQn
#define SPI0_IRQ_HANDLER                        SPI3_IRQHandler
#define SPI0_RX_DMA_STREAM                      DMA1_Stream0
#define SPI0_RX_DMA_CHANNEL                     DMA_CHANNEL_0
#define SPI0_TX_DMA_STREAM                      DMA1_Stream5
#define SPI0_TX_DMA_CHANNEL                     DMA_CHANNEL_0
#define SPI0_RX_DMA_IRQ                         DMA1_Stream0_IRQn
#define SPI0_RX_DMA_IRQ_HANDLER                 DMA1_Stream0_IRQHandler
#define SPI0_TX_DMA_IRQ                         DMA1_Stream5_IRQn
#define SPI0_TX_DMA_IRQ_HANDLER                 DMA1_Stream5_IRQHandler
#define SPI_SCK_GPIO_CLK_ENABLE(INDEX)          do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_ENABLE();}} while(0)
#define SPI_SCK_GPIO_CLK_DISABLE(INDEX)         do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_DISABLE();}} while(0)
#define SPI_MISO_GPIO_CLK_ENABLE(INDEX)         do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_ENABLE();}} while(0)
#define SPI_MISO_GPIO_CLK_DISABLE(INDEX)        do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_DISABLE();}} while(0)
#define SPI_MOSI_GPIO_CLK_ENABLE(INDEX)         do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_ENABLE();}} while(0)
#define SPI_MOSI_GPIO_CLK_DISABLE(INDEX)        do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_DISABLE();}} while(0)
#define SPI_CS_GPIO_CLK_ENABLE(INDEX)           do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_ENABLE();}} while(0)
#define SPI_CS_GPIO_CLK_DISABLE(INDEX)          do { if(0 == (INDEX)) {__HAL_RCC_GPIOC_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_GPIOC_CLK_DISABLE();}} while(0)
#define SPI_CLK_ENABLE(INDEX)                   do { if(0 == (INDEX)) {__HAL_RCC_SPI3_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_SPI3_CLK_ENABLE();}} while(0)
#define SPI_CLK_DISABLE(INDEX)                  do { if(0 == (INDEX)) {__HAL_RCC_SPI3_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_SPI3_CLK_DISABLE();}} while(0)
#define SPI_FORCE_RESET(INDEX)                  do { if(0 == (INDEX)) {__HAL_RCC_SPI3_FORCE_RESET();} else\
                                                                      {__HAL_RCC_SPI3_FORCE_RESET();}} while(0)
#define SPI_RELEASE_RESET(INDEX)                do { if(0 == (INDEX)) {__HAL_RCC_SPI3_RELEASE_RESET();} else\
                                                                      {__HAL_RCC_SPI3_RELEASE_RESET();}} while(0)
#define SPI_DMA_CLK_ENABLE(INDEX)               do { if(0 == (INDEX)) {__HAL_RCC_DMA1_CLK_ENABLE();} else\
                                                                      {__HAL_RCC_DMA1_CLK_ENABLE();}} while(0)
#define SPI_DMA_CLK_DISABLE(INDEX)              do { if(0 == (INDEX)) {__HAL_RCC_DMA1_CLK_DISABLE();} else\
                                                                      {__HAL_RCC_DMA1_CLK_DISABLE();}} while(0)
#else
#error Board type not defined!!!
#endif
/** @} */ /* End of group SPI configuration. */

#ifdef __cplusplus
}
#endif

#endif /* __SPI_STM32F2XX_H__ */

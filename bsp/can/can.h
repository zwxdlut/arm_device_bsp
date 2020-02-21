/*
 * can.h
 *
 *  Created on: 2018��8��21��
 *      Author: Administrator
 */

#ifndef __CAN_H__
#define __CAN_H__

#include <stdint.h>
#include <string.h>
#include <assert.h>

#if defined S32K14x
#include "can_s32k1xx.h"
#elif defined STM32F10X_CL || defined STM32F205xx
#if defined STM32F10X_CL
#include "can_stm32f10x.h"
#elif defined STM32F205xx
#include "can_stm32f2xx.h"
#endif
#if defined USING_OS_FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#endif
#define CAN_SLAVE_START_FILTER_BANK_NUM         21 /**< Slave CAN start filter bank number */
#else
#error Mcu type not defined!!!
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/**
 * @defgroup CAN module index.
 * @{
 */
#define CAN0_INDEX                              0
#define CAN1_INDEX                              1
/** @} */ /* End of group CAN module index. */

/**
 * @defgroup CAN power modes.
 * @{
 */
#define CAN_PWR_MODE_SLEEP                      0
#define CAN_PWR_MODE_RUN                        1
/** @} */ /* End of group CAN power modes. */

#define CAN_BUFFER_SIZE                         10 /**< CAN rx queue max size */

/**
 * @brief CAN message.
 */
typedef struct
{
	uint32_t id_;
	uint8_t  dlc_;
	uint8_t  data_[8];
}can_msg_t;

/*******************************************************************************
 * Function prototypes
 ******************************************************************************/
/**
 * @brief  Initialize CAN module.
 *
 * @param  [in] _index          CAN index.
 * @param  [in] _filter_id_list Filter id list.
 * @param  [in] _filter_id_num  Filter id number.
 * @return Success(0) or failure(other values).
 */
int32_t can_init(const uint8_t _index, const uint32_t *_filter_id_list, const uint8_t _filter_id_num);

/**
 * @brief  Deinitialize CAN module.
 * @param  [in] _index CAN index.
 * @return Success(0) or failure(other values).
 */
int32_t can_deinit(const uint8_t _index);

/**
 * @brief  Receive CAN message.
 *
 * @param  [in]  _index CAN index.
 * @param  [out] _id    CAN ID.
 * @param  [out] _buf   Receive buffer.
 * @param  [in]  _size  Receive size.
 * @return Received size.
 */
uint8_t can_receive(const uint8_t _index, uint32_t *const _id, uint8_t *const _buf,  const uint8_t _size);

/**
 * @brief  Transmit CAN message.
 *
 * @param  [in] _index CAN index.
 * @param  [in] _id    CAN ID.
 * @param  [in] _buf   Transmit buffer.
 * @param  [in] _size  Transmit size.
 * @return Transmitted size.
 */
uint8_t can_transmit(const uint8_t _index, const uint32_t _id, const uint8_t *const _buf, const uint8_t _size);

/**
 * @brief  Transfer CAN power mode.
 *
 * @param  [in] _index CAN index.
 * @param  [in] _mode  Power mode:
				       @arg CAN_PWR_MODE_SLEEP Sleep.
					   @arg CAN_PWR_MODE_RUN   Run.
 * @return Success(0) or failure(other values).
 */
int32_t can_pwr_mode_trans(const uint8_t _index, const uint8_t _mode);

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

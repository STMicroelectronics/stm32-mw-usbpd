/**
  ******************************************************************************
  * @file    usbpd_utils_tracer_hw_if.h
  * @brief   Header file for usbpd_utils_trace_hw_if.c.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#ifndef USBPD_UTILS_TRACER_HW_IF_H
#define USBPD_UTILS_TRACER_HW_IF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS
  * @{
  */

/** @addtogroup USBPD_UTILS_TRACER_HW_IF
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_HW_IF_Exported_Constants USBPD_UTILS_TRACER_HW_IF Exported Constants
  * @{
  */

#define USBPD_TRACER_HW_IF_BUFFER_SIZE (4096U) /*<! Size of the tracer buffer in bytes.
                                                    Increase this value if tracer overflows occur */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_UTILS_TRACER_HW_IF_Exported_Types USBPD_UTILS_TRACER_HW_IF Exported Types
  * @{
  */

/**
  * @brief Callbacks exposed by the USBPD tracer hardware interface to the USBPD tracer.
  */
typedef struct
{
  /**
    * @brief  The reception of data byte as been completed.
    * @param  rx_byte The received byte.
    * @param  flag_error Flag to indicate reception error.
    */
  void (*USBPD_TRACER_HW_IF_RxCplt)(uint8_t rx_byte, uint8_t flag_error);

  /**
    * @brief The transmission of data as been completed.
    */
  void (*USBPD_TRACER_HW_IF_TxCplt)(void);
} usbpd_trace_if_callbacks_t;

typedef void (* usbpd_trace_hw_if_rx_cb_t)(uint8_t, uint8_t); /*<! Trace interface Rx callback type */
typedef void (* usbpd_trace_hw_if_tx_cb_t)(void);             /*<! Trace interface Tx callback type */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_HW_IF_Exported_Functions USBPD_UTILS_TRACER_HW_IF Exported Functions
  * @{
  */

void         USBPD_UTILS_TRACER_HW_IF_Init(void);
void         USBPD_UTILS_TRACER_HW_IF_DeInit(void);

hal_status_t USBPD_UTILS_TRACER_HW_IF_RegisterRxCallback(usbpd_trace_hw_if_rx_cb_t p_callback);
hal_status_t USBPD_UTILS_TRACER_HW_IF_RegTxCpltCb(usbpd_trace_hw_if_tx_cb_t p_callback);

hal_status_t USBPD_UTILS_TRACER_HW_IF_StartRX(void);

void         USBPD_UTILS_TRACER_HW_IF_SendData(const uint8_t *pdata, uint32_t size);

uint32_t     USBPD_UTILS_TRACER_HW_IF_GetTick(void);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* USBPD_UTILS_TRACER_HW_IF_H */

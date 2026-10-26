/**
  ******************************************************************************
  * @file    usbpd_utils_tracer.h
  * @brief   Header file for usbpd_utils_tracer.c.
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

#ifndef USBPD_UTILS_TRACER_H
#define USBPD_UTILS_TRACER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_utils_tracer_hw_if.h"

#include <string.h> /* Include for memset function */

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS
  * @{
  */

/** @addtogroup USBPD_UTILS_TRACER
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_Exported_Constants USBPD_UTILS_TRACER Exported Constants
  * @{
  */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_UTILS_TRACER_Exported_Types USBPD_UTILS_TRACER Exported Types
  * @{
  */

/**
  * @brief  Tracer status.
  */
typedef enum
{
  USBPD_UTILS_TRACER_OK,   /*!< Tracer status OK    */
  USBPD_UTILS_TRACER_ERROR /*!< Tracer status ERROR */
} usbpd_tracer_status_t;

typedef void (* usbpd_tracer_rx_cb_t)(uint8_t, uint8_t); /*<! Trace interface Rx callback type */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_Exported_Functions USBPD_UTILS_TRACER Exported Functions
  * @{
  */

void USBPD_UTILS_TRACER_Init(void);
void USBPD_UTILS_TRACER_DeInit(void);

void USBPD_UTILS_TRACER_Add(const uint8_t *p_data, uint32_t size);
void USBPD_UTILS_TRACER_WriteData(uint16_t pos, uint8_t data);
void USBPD_UTILS_TRACER_SendData(void);

void USBPD_UTILS_TRACER_Lock(void);
void USBPD_UTILS_TRACER_UnLock(void);

void USBPD_UTILS_TRACER_Register_Rx_Cplt_Callback(void (*callbackRX)(uint8_t, uint8_t));

void USBPD_UTILS_TRACER_StartRX(void);

int32_t USBPD_UTILS_TRACER_AllocateBufer(uint32_t size);
usbpd_tracer_status_t USBPD_UTILS_TRACER_EnableOverFlow(const uint8_t *p_data, uint8_t size);


void USBPD_UTILS_TRACER_LowPowerInit(void);
void USBPD_UTILS_TRACER_LowPowerDeInit(void);

void USBPD_UTILS_TRACER_LowPowerSendData(void);
void USBPD_UTILS_TRACER_LowPowerSendDataComplete(void);

uint32_t USBPD_UTILS_TRACER_GetTick(void);

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

#endif /* USBPD_UTILS_TRACER_H */

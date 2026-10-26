/**
  ******************************************************************************
  * @file    usbpd_timersserver.h
  * @brief   Header file for usbpd_timerserver.c.
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

#ifndef USBPD_TIMERSSERVER_H
#define USBPD_TIMERSSERVER_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "usbpd_timerserver_hw_if.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_TIMESERVER
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_Exported_Constants USBPD_TIMESERVER Exported Constants
  * @{
  */

#define USBPD_TIMERSERVER_MAX_TIME USBPD_TIMERSERVER_HW_IF_MAX_TIME /*!< Maximum time in us */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_TIMESERVER_Exported_Types USBPD_TIMESERVER Exported Types
  * @{
  */

/**
  * @brief USBPD Timer server identifiers.
  */
typedef enum
{
  TIMERSERVER_PORT0_CRC   = 0U, /*!< USBPD TIMERSERVER Port 0 CRC timer   */
  TIMERSERVER_PORT0_RETRY = 1U, /*!< USBPD TIMERSERVER Port 0 retry timer */
  TIMERSERVER_PORT1_CRC   = 2U, /*!< USBPD TIMERSERVER Port 1 CRC timer   */
  TIMERSERVER_PORT1_RETRY = 3U, /*!< USBPD TIMERSERVER Port 1 retry timer */
  TIMERSERVER_PORT2_CRC   = 4U, /*!< USBPD TIMERSERVER Port 2 CRC timer   */
  TIMERSERVER_PORT2_RETRY = 5U, /*!< USBPD TIMERSERVER Port 2 retry timer */
} usbpd_timerserver_identifier_t;

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_Exported_Functions USBPD_TIMESERVER Exported Functions
  * @{
  */

void     USBPD_TIMERSERVER_Init(void);
void     USBPD_TIMERSERVER_DeInit(void);
void     USBPD_TIMERSERVER_Start(usbpd_timerserver_identifier_t id, uint32_t time_us);
uint32_t USBPD_TIMERSERVER_IsExpired(usbpd_timerserver_identifier_t id);


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

#endif /* USBPD_TIMERSSERVER_H */


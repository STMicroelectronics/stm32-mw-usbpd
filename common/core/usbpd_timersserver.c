/**
  ******************************************************************************
  * @file    usbpd_timersserver.c
  * @brief   Timer server module.
  *          This file provides a set of functions to manage USBPD timers.
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

/* Includes ------------------------------------------------------------------*/
#include "usbpd_timersserver.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_TIMESERVER
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_Private_Types USBPD_TIMESERVER Private Types
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_Private_Constants USBPD_TIMESERVER Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_Private_Macros USBPD_TIMESERVER Private Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_Private_Functions USBPD_TIMESERVER Private Functions
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_Private_Variables USBPD_TIMESERVER Private Variables
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_TIMESERVER_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize USBPD timer to manage protocol timing in us.
  */
void USBPD_TIMERSERVER_Init()
{
  (void)USBPD_TIMERSERVER_HW_IF_Init();
}

/**
  * @brief  DeInitialize USBPD timer.
  */
void USBPD_TIMERSERVER_DeInit(void)
{
  USBPD_TIMERSERVER_HW_IF_DeInit();
}

/**
  * @brief  Start the timer counting.
  * @param  id Timer ID of type @ref usbpd_timerserver_identifier_t.
  * @param  time_us Time in us.
  */
void USBPD_TIMERSERVER_Start(usbpd_timerserver_identifier_t id, uint32_t time_us)
{
  (void)USBPD_TIMERSERVER_HW_IF_Start((uint8_t)id, time_us);
}

/**
  * @brief  Check timer expiration.
  * @param  if Timer ID of type @ref usbpd_timerserver_identifier_t.
  */
uint32_t USBPD_TIMERSERVER_IsExpired(usbpd_timerserver_identifier_t id)
{
  return USBPD_TIMERSERVER_HW_IF_IsExpired((uint8_t)id);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

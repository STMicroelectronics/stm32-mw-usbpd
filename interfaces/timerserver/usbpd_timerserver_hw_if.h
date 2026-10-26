/**
  ******************************************************************************
  * @file    usbpd_timerserver_hw_if.h
  * @brief   Header file for usbpd_timerserver_hw_if.c.
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

#ifndef USBPD_TIMERSERVER_HW_IF_H
#define USBPD_TIMERSERVER_HW_IF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"
#include "usbpd_core.h"
#include "usbpd_config.h"
/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_TIMESERVER_HW_IF
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_HW_IF_Exported_Constants USBPD_TIMESERVER_HW_IF Exported Constants
  * @{
  */

#define USBPD_TIMERSERVER_HW_IF_MAX_TIME (10000U) /*!< Maximum time in us */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_TIMESERVER_HW_IF_Exported_Types USBPD_TIMESERVER_HW_IF Exported Types
  * @{
  */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_HW_IF_Exported_Functions USBPD_TIMESERVER_HW_IF Exported Functions
  * @{
  */
#if defined(USBPD_TIMERSERVER_TIM_MS)
usbpd_status_t USBPD_TIMERSERVER_HW_IF_Ms_Init(void);
#endif /* USBPD_TIMERSERVER_TIM_MS */
hal_status_t USBPD_TIMERSERVER_HW_IF_Init(void);
void         USBPD_TIMERSERVER_HW_IF_DeInit(void);

hal_status_t USBPD_TIMERSERVER_HW_IF_Start(uint8_t timer_id, uint32_t time_us);
uint32_t     USBPD_TIMERSERVER_HW_IF_IsExpired(uint8_t timer_id);

void     USBPD_TIMERSERVER_TimerCounter(void);
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

#endif /* USBPD_TIMERSERVER_HW_IF_H */

/**
  ******************************************************************************
  * @file    usbpd_cad_hw_if.h
  * @brief   Header file for usbpd_cad_hw_if.c.
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
#ifndef USBPD_CAD_HW_IF_H
#define USBPD_CAD_HW_IF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "usbpd_pwr.h"
#include "usbpd_phy_hw_if.h"
#include "usbpd_config.h"
/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_CAD_HW_IF
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_CAD_HW_IF_Exported_Constants USBPD_CAD_HW_IF Exported Constants
  * @{
  */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_CAD_HW_IF_Exported_Types USBPD_CAD_HW_IF Exported Types
  * @{
  */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_CAD_HW_IF_Exported_Functions USBPD_CAD_HW_IF Exported Functions
  * @{
  */

hal_status_t USBPD_CAD_HW_IF_Init(uint8_t port_num, usbpd_settings_t *p_settings,
                                  usbpd_params_t *p_params, void (*p_wake_up_cb)(void));
void         USBPD_CAD_HW_IF_DeInit(uint8_t port_num);

void         USBPD_CAD_HW_IF_Enter_ErrorRecovery(uint8_t port_num);
#if defined(USBPDCORE_DRP) || defined(USBPDCORE_SRC)
uint32_t     USBPD_CAD_HW_IF_Set_ResistorRp(uint8_t port_num, usbpd_cad_rp_value_t rp_val);
#endif /* USBPDCORE_DRP || USBPDCORE_SRC */

void         USBPD_CAD_HW_IF_AssertRp(uint8_t port_num);
void         USBPD_CAD_HW_IF_AssertRd(uint8_t port_num);

uint32_t     USBPD_CAD_HW_IF_StateMachine(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin);
#if !defined(USBPDCORE_LIB_NO_PD)
void        USBPD_CAD_HW_IF_GetRxOrderedSet(uint8_t port_num, usbpd_settings_t *p_settings);
#endif /* USBPDCORE_LIB_NO_PD */
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

#endif /* USBPD_CAD_HW_IF_H */


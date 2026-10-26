/**
  ******************************************************************************
  * @file    usbpd_phy.h
  * @brief   Header file for usbpd_phy.c.
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

#ifndef USBPD_PHY_H
#define USBPD_PHY_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_phy_hw_if.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_PHY
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_Exported_Constants USBPD_PHY Exported Constants
  * @{
  */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_PHY_Exported_Types USBPD_PHY Exported Types
  * @{
  */

/**
  * @brief CallBacks exposed by the @ref USBPD_PHY to the USBPD_CORE_PRL.
  */
typedef struct
{
  /**
    * @brief  Reports that a message has been received on a specified port.
    * @param  port_num The port number.
    * @param  sop_type The type of the message received @ref usbpd_sop_type_t.
    * @note   Received data are stored inside port_num->pRxBuffPtr.
    *         function called in the interrupt context.
      */
  void (*USBPD_PHY_MessageReceived)(uint8_t port_num, usbpd_sop_type_t sop_type);

  /**
    * @brief  Reports to the PRL that a Reset received from channel.
    * @param  port_num The port number.
    * @param  sop_type The type of reset performed @ref USBPD_SOPTYPE_HARD_RESET or @ref USBPD_SOPTYPE_CABLE_RESET.
      */
  void (*USBPD_PHY_ResetIndication)(uint8_t port_num, usbpd_sop_type_t sop_type);

  /**
    * @brief  Reports to the PRL that a Reset operation has been completed.
    * @param  port_num The port number.
    * @param  sop_type The type of reset performed @ref USBPD_SOPTYPE_HARD_RESET or @ref USBPD_SOPTYPE_CABLE_RESET.
      */
  void (*USBPD_PHY_ResetCompleted)(uint8_t port_num, usbpd_sop_type_t sop_type);

  /**
    * @brief  Reports to the PRL that a BIST operation has been completed.
    * @param  port_num The port number.
    * @param  bistmode The type of Bist performed @ref usbpd_bist_mode_t.
      */
  void (*USBPD_PHY_BistCompleted)(uint8_t port_num, usbpd_bist_mode_t bistmode);

  /**
    * @brief  Reports to the PRL that a Tx operation has been completed.
    * @param  port_num The port number.
    * @param  status The Tx status.
      */
  void (*USBPD_PHY_TxCompleted)(uint8_t port_num, usbpd_phy_tx_status_t status);

  /**
    * @brief  Reports to the PRL that an FRS has been detected.
    * @param  port_num The port number.
      */
  void (*USBPD_PHY_FastRoleSwapReception)(uint8_t port_num);

} usbpd_phy_callbacks_t;

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_Exported_Functions USBPD_PHY Exported Functions
  * @{
  */

usbpd_status_t      USBPD_PHY_Init(uint8_t port_num, const usbpd_phy_callbacks_t *cbs, uint8_t *pRxBuffer,
                                   usbpd_port_pwr_role_t pwr_role, uint32_t sop_support);
void                USBPD_PHY_DeInit(uint8_t port_num);

#if !defined(USBPDCORE_LIB_NO_PD)
void                USBPD_PHY_Reset(uint8_t port_num);

uint16_t            USBPD_PHY_GetRetryTimerValue(uint8_t port_num);
uint16_t            USBPD_PHY_GetMinGOODCRCTimerValue(uint8_t port_num);

usbpd_status_t      USBPD_PHY_ResetRequest(uint8_t port_num, usbpd_sop_type_t sop_type);
usbpd_status_t      USBPD_PHY_SendMessage(uint8_t port_num, usbpd_sop_type_t sop_type, uint8_t *pBuffer, uint16_t size);
usbpd_status_t      USBPD_PHY_SendGoodCRC(uint8_t port_num, usbpd_sop_type_t sop_type, uint8_t *pBuffer, uint16_t size);
usbpd_status_t      USBPD_PHY_Start_Tx_BIST_Pattern(uint8_t port_num);
usbpd_status_t      USBPD_PHY_Stop_Tx_BIST_Pattern(uint8_t port_num);

void                USBPD_PHY_SetResistor_SinkTxNG(uint8_t port_num);
void                USBPD_PHY_SetResistor_SinkTxOK(uint8_t port_num);
uint8_t             USBPD_PHY_IsResistor_SinkTxOk(uint8_t port_num);

void                USBPD_PHY_FastRoleSwapSignalling(uint8_t port_num);

void                USBPD_PHY_SOPSupported(uint8_t port_num, uint32_t sop_support);

void                USBPD_PHY_EnableRX(uint8_t port_num);
void                USBPD_PHY_DisableRX(uint8_t port_num);
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

#endif /* USBPD_PHY_H */


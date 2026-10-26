/**
  ******************************************************************************
  * @file    usbpd_phy_hw_if.h
  * @brief   Header file for usbpd_phy_hw_if.c.
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

#ifndef USBPD_PHY_HW_IF_H
#define USBPD_PHY_HW_IF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_trace.h"
#include "usbpd_pwr.h"

#if !defined(USBPDCORE_LIB_NO_PD)
#include "usbpd_timersserver.h"
#endif /* !USBPDCORE_LIB_NO_PD */

#if defined(_LOW_POWER)
#include "usbpd_lowpower.h"
#endif /* _LOW_POWER */

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_PHY_HW_IF
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Exported_Constants USBPD_PHY_HW_IF Exported Constants
  * @{
  */

#define USBPD_SIZE_MAX_PD_TRANSACTION_CHUNK   (30U)       /*!< Maximum size of a chunked PD transaction    */
#define USBPD_SIZE_MAX_PD_TRANSACTION_UNCHUNK (260U + 4U) /*!< Maximum size of an unchunked PD transaction */

#define USBPD_DELAY_READ_CC_STATUS (300U)                 /*!< Delay to wait for CC status to be stable    */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_PHY_HW_IF_Exported_Types USBPD_PHY_HW_IF Exported Types
  * @{
  */

/**
  * @brief Callbacks exposed by the HW_IF to the PHY.
  */
typedef struct
{
  /**
    * @brief  The message transfer has been completed.
    * @param  port_num Port number.
    * @param  status Status (0 means OK).
    */
  void (*USBPD_PHY_HW_IF_TxCompleted)(uint8_t port_num, usbpd_phy_tx_status_t status);

  /**
    * @brief  Bist data sent callback from PHY_HW_IF.
    * @param  port_num Port number.
    * @param  bistmode Bist mode.
    */
  void (*USBPD_PHY_HW_IF_BistCompleted)(uint8_t port_num, usbpd_bist_mode_t bistmode);

  /**
    * @brief  The reception phase of an hard reset is completed notify it.
    * @param  port_num Port number.
    * @param  sop_type SOP Message Type based on @ref usbpd_sop_type_t.
    */
  void (*USBPD_PHY_HW_IF_RX_ResetIndication)(uint8_t port_num, usbpd_sop_type_t sop_type);

  /**
    * @brief  The reception phase of the current message is completed and notify it.
    * @param  port_num Port number.
    * @param  msg_type Message Type.
    */
  void (*USBPD_PHY_HW_IF_RX_Completed)(uint8_t port_num, uint32_t msg_type);

  /**
    * @brief  The emission of HRST has been completed.
    * @param  port_num Port number.
    * @param  sop_type SOP Message Type based on @ref usbpd_sop_type_t.
    */
  void (*USBPD_PHY_HW_IF_TX_HardResetCompleted)(uint8_t port_num, usbpd_sop_type_t sop_type);

  /**
    * @brief  FRS reception.
    * @param  port_num Port number.
    */
  void (*USBPD_PHY_HW_IF_TX_FRSReception)(uint8_t port_num);

} usbpd_phy_hw_if_callbacks_t;

/** @defgroup usbpd_port_handle_t USBPD handle structure definition for USBPD_PHY_HW_IF
  * @brief  USBPD PORT handle structure definition.
  * @{
  */
typedef struct
{
  hal_ucpd_handle_t               *hucpd;             /*!< HAL UCPD Handle                                 */

  usbpd_settings_t                *settings;          /*!< USBPD settings                                  */
  usbpd_params_t                  *params;            /*!< USBPD parameters                                */

  usbpd_phy_hw_if_callbacks_t     cbs;                /*!< USBPD_PHY_HW_IF callbacks                       */

  uint8_t                         *ptr_rx_buffer;     /*!< Pointer to Raw Rx transfer Buffer               */

  usbpd_cc_line_t                 active_cc;          /*!< CC pin used for communication                   */

  volatile uint8_t                flag_rx_status;     /*!< Tracks the reception of a message to forbid any
                                                           new TX transaction until message completion     */
  volatile uint8_t                flag_rx_cplt_abort; /*!< Tracks Rx abort after Rx complete interrupts    */
  volatile uint8_t                flag_rx_error;      /*!< Tracks Rx error                                 */

  void (*USBPD_CAD_WakeUp)(void);                     /*!< Function used to wake up CAD task               */
} usbpd_port_handle_t;

extern usbpd_port_handle_t h_phy_hw_if[USBPD_PORT_COUNT];

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Exported_Functions USBPD_PHY_HW_IF Exported Functions
  * @{
  */

void           USBPD_PHY_HW_IF_Init(uint8_t port_num);
void           USBPD_PHY_HW_IF_DeInit(uint8_t port_num);

usbpd_status_t USBPD_PHY_HW_IF_Start_Tx_BIST_Pattern(uint8_t port_num);
usbpd_status_t USBPD_PHY_HW_IF_Stop_Tx_BIST_Pattern(uint8_t port_num);

usbpd_status_t USBPD_PHY_HW_IF_SendBuffer(uint8_t port_num, usbpd_sop_type_t Type, uint8_t *p_buffer_tx,
                                          uint32_t size);
usbpd_status_t USBPD_PHY_HW_IF_SendGoodCRC(uint8_t port_num, usbpd_sop_type_t Type, uint8_t *p_buffer_tx,
                                           uint32_t size);

void           USBPD_PHY_HW_IF_AssertRp(uint8_t port_num);
void           USBPD_PHY_HW_IF_AssertRd(uint8_t port_num);

void           USBPD_PHY_HW_IF_EnterErrorRecovery(uint8_t port_num);
void           USBPD_PHY_HW_IF_Set_CC(uint8_t port_num, usbpd_cc_line_t cc);
void           USBPD_PHY_HW_IF_EnableRX(uint8_t port_num);
void           USBPD_PHY_HW_IF_DisableRX(uint8_t port_num);

void           USBPD_PHY_HW_IF_SignalAttachement(uint8_t port_num, usbpd_cc_line_t cc);
void           USBPD_PHY_HW_IF_SignalDetachment(uint8_t port_num);

void           USBPD_PHY_HW_IF_SetRpSnkTxNG(uint8_t port_num);
void           USBPD_PHY_HW_IF_SetRpSnkTxOK(uint8_t port_num);
uint8_t        USBPD_PHY_HW_IF_IsResistor_SinkTxOk(uint8_t port_num);

void           USBPD_PHY_HW_IF_FastRoleSwapSignalling(uint8_t port_num);

void           USBPD_PHY_HW_IF_RxResetIndication(uint8_t port_num, usbpd_sop_type_t sop_type);

usbpd_status_t USBPD_PHY_HW_IF_RegisterCallbacks(uint8_t port_num, const usbpd_phy_hw_if_callbacks_t *p_callbacks);
usbpd_status_t USBPD_PHY_HW_IF_SetRxBuffer(uint8_t port_num, uint8_t *p_buffer);

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

#endif /* USBPD_PHY_HW_IF_H */

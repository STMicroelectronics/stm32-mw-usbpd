/**
  ******************************************************************************
  * @file    usbpd_phy_hw_if.c
  * @brief   PHY hardware interface control module.
  *          This file provides a set of firmware functions to manage the PHY
  *          layer hardware interface.
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

#define USBPD_TRACE_VERBOSE_LEVEL     USBPD_TRACE_VERBOSE_LEVEL_INFO /*<! Trace verbosity level for this file. */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_phy_hw_if.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_PHY_HW_IF
  * @brief   This file contains PHY hardware interface functions.
  * @{
  */

#if !defined (USE_HAL_UCPD_MODULE) || (USE_HAL_UCPD_MODULE != 1)
#error USE_HAL_UCPD_MODULE is required for this interface
#endif /* !USE_HAL_UCPD_MODULE */

#if !defined (USE_HAL_UCPD_REGISTER_CALLBACKS) || (USE_HAL_UCPD_REGISTER_CALLBACKS != 1)
#error USE_HAL_UCPD_REGISTER_CALLBACKS is required for this interface
#endif /* !USE_HAL_UCPD_REGISTER_CALLBACKS */

#if !defined (USE_HAL_UCPD_GET_LAST_ERRORS) || (USE_HAL_UCPD_GET_LAST_ERRORS != 1)
#error USE_HAL_UCPD_GET_LAST_ERRORS is required for this interface
#endif /* !USE_HAL_UCPD_GET_LAST_ERRORS */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Types USBPD_PHY_HW_IF Private Types
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Constants USBPD_PHY_HW_IF Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Macros USBPD_PHY_HW_IF Private Macros
  * @{
  */

/**
  * @brief  Enter PHY critical section by disabling interrupts.
  */
#define PHY_ENTER_CRITICAL_SECTION()  \
  uint32_t primask = __get_PRIMASK(); \
  __disable_irq();

/**
  * @brief  Exit PHY critical section by enabling interrupts.
  */
#define PHY_LEAVE_CRITICAL_SECTION() \
  __set_PRIMASK(primask);

/**
  * @brief Get the port number from the handle.
  * @param hucpd Handle of the UCPD.
  * @param port_num Port number to be returned.
  */
#define GET_PORT_NUM_FROM_UCPD_HANDLE(h_ucpd, port_num)                 \
  do                                                                    \
  {                                                                     \
    for (uint8_t index = 0; index < (uint8_t)USBPD_PORT_COUNT; index++) \
    {                                                                   \
      if (h_phy_hw_if[index].hucpd == (h_ucpd))                         \
      {                                                                 \
        (port_num) = index;                                             \
        break;                                                          \
      }                                                                 \
    }                                                                   \
  } while(0);

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Functions USBPD_PHY_HW_IF Private Functions
  * @{
  */

#if !defined(USBPDCORE_LIB_NO_PD)
static void USBPD_PHY_HW_IF_TxCpltCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_TxGoodCRCCpltCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_TxDiscardCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_TxAbortCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_RxCpltCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_RxOrdDetCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_HRstSentCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_HRstRxCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_HRstDiscardCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_FRSEvtCallback(hal_ucpd_handle_t *hucpd);
static void USBPD_PHY_HW_IF_AbortCpltCallback(hal_ucpd_handle_t *hucpd);
#endif /* USBPDCORE_LIB_NO_PD */
static void USBPD_PHY_HW_IF_TypeCEventCCxCallback(hal_ucpd_handle_t *hucpd, hal_ucpd_cc_line_t cc_line);
static void USBPD_PHY_HW_IF_ErrorCallback(hal_ucpd_handle_t *hucpd);

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Variables USBPD_PHY_HW_IF Private Variables
  * @{
  */

usbpd_port_handle_t h_phy_hw_if[USBPD_PORT_COUNT];

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_PHY_HW_IF_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize the PHY hardware interface.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_Init(uint8_t port_num)
{
  USBPD_TRACE_Add(USBPD_TRACE_PHY_NOTFRWD, port_num, 0xF1U, NULL, 0U);

  /* Apply Rp/Rd resistors trimming on concerned series */
#if defined(HAL_UCPD_SOFT_TRIM)
  (void)HAL_UCPD_ApplyTrimmingRp(h_phy_hw_if[port_num].hucpd,
                                 (hal_ucpd_trim_rp_value_t)h_phy_hw_if[port_num].params->rp_resistor);
  (void)HAL_UCPD_ApplyTrimmingRd(h_phy_hw_if[port_num].hucpd);
#endif /* HAL_UCPD_SOFT_TRIM */

  /* Register all HAL UCPD callbacks */
#if !defined(USBPDCORE_LIB_NO_PD)
  (void)HAL_UCPD_RegisterTxCpltCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_TxCpltCallback);
  (void)HAL_UCPD_RegisterTxGoodCRCCpltCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_TxGoodCRCCpltCallback);
  (void)HAL_UCPD_RegisterTxDiscardCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_TxDiscardCallback);
  (void)HAL_UCPD_RegisterTxAbortCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_TxAbortCallback);
  (void)HAL_UCPD_RegisterRxCpltCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_RxCpltCallback);
  (void)HAL_UCPD_RegisterRxOrdDetCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_RxOrdDetCallback);
  (void)HAL_UCPD_RegisterHRstSentCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_HRstSentCallback);
  (void)HAL_UCPD_RegisterHRstRxCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_HRstRxCallback);
  (void)HAL_UCPD_RegisterHRstDiscardCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_HRstDiscardCallback);
  (void)HAL_UCPD_RegisterFRSEvtCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_FRSEvtCallback);
  (void)HAL_UCPD_RegisterAbortCpltCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_AbortCpltCallback);
#endif /* USBPDCORE_LIB_NO_PD */
  (void)HAL_UCPD_RegisterTypeCEventCCxCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_TypeCEventCCxCallback);
  (void)HAL_UCPD_RegisterErrorCallback(h_phy_hw_if[port_num].hucpd, USBPD_PHY_HW_IF_ErrorCallback);
}

/**
  * @brief  DeInitialize the PHY hardware interface.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_DeInit(uint8_t port_num)
{
  USBPD_TRACE_Add(USBPD_TRACE_PHY_NOTFRWD, port_num, 0xF2U, NULL, 0U);

  /* Reset all HAL UCPD callbacks */
#if !defined(USBPDCORE_LIB_NO_PD)
  (void)HAL_UCPD_RegisterTxCpltCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TxCpltCallback);
  (void)HAL_UCPD_RegisterTxGoodCRCCpltCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TxGoodCRCCpltCallback);
  (void)HAL_UCPD_RegisterTxDiscardCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TxDiscardCallback);
  (void)HAL_UCPD_RegisterTxAbortCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TxAbortCallback);
  (void)HAL_UCPD_RegisterRxCpltCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RxCpltCallback);
  (void)HAL_UCPD_RegisterRxOrdDetCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RxOrdDetCallback);
  (void)HAL_UCPD_RegisterHRstSentCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_HRstSentCallback);
  (void)HAL_UCPD_RegisterHRstRxCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_HRstRxCallback);
  (void)HAL_UCPD_RegisterHRstDiscardCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_HRstDiscardCallback);
  (void)HAL_UCPD_RegisterFRSEvtCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_FRSEvtCallback);
  (void)HAL_UCPD_RegisterAbortCpltCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_AbortCpltCallback);
#endif /* USBPDCORE_LIB_NO_PD */
  (void)HAL_UCPD_RegisterErrorCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_ErrorCallback);
  (void)HAL_UCPD_RegisterTypeCEventCCxCallback(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TypeCEventCCxCallback);
}

#if !defined(USBPDCORE_LIB_NO_PD)
/**
  * @brief  Sends the BIST pattern.
  * @param  port_num Port.
  */
usbpd_status_t USBPD_PHY_HW_IF_Start_Tx_BIST_Pattern(uint8_t port_num)
{
  /* Start the BIST infinite sequence transmission */
  (void)HAL_UCPD_SendBIST(h_phy_hw_if[port_num].hucpd);
  return USBPD_OK;
}

/**
  * @brief  Stop BIST carrier mode 2.
  * @param  port_num Port.
  */
usbpd_status_t USBPD_PHY_HW_IF_Stop_Tx_BIST_Pattern(uint8_t port_num)
{
  /* Exit the BIST infinite sequence */
  (void)HAL_UCPD_StopBIST(h_phy_hw_if[port_num].hucpd);
  return USBPD_OK;
}

/**
  * @brief  Send a Buffer on the CC line.
  * @param  port_num Port.
  * @param  sop_type SOP Message Type based on @ref usbpd_sop_type_t.
  * @param  p_buffer_tx Data buffer to be transmitted.
  * @param  size The number of bytes to be transmitted.
  * @note   The data will be converted in bmc and send through the CC line.
  * @retval USBPD Status.
  */
usbpd_status_t USBPD_PHY_HW_IF_SendBuffer(uint8_t port_num, usbpd_sop_type_t sop_type, uint8_t *p_buffer_tx,
                                          uint32_t size)
{
  usbpd_status_t status = USBPD_OK;
  USBPD_TRACE_DEBUG_PRINTF(port_num, "%s type = %d, size = %d", __func__, sop_type, size);

  hal_ucpd_state_t hal_ucpd_state = HAL_UCPD_GetState(h_phy_hw_if[port_num].hucpd);

  if (USBPD_SOPTYPE_HARD_RESET == sop_type)
  {
    (void)HAL_UCPD_SendHardReset(h_phy_hw_if[port_num].hucpd);
  }
  else
  {
    PHY_ENTER_CRITICAL_SECTION();

    /* If Rx is ongoing or if a DMA transfer is active then discard the buffer sending */
    if ((h_phy_hw_if[port_num].flag_rx_status == USBPD_TRUE)
        || (hal_ucpd_state == HAL_UCPD_STATE_TX) || (hal_ucpd_state == HAL_UCPD_STATE_ABORT))
    {
      PHY_LEAVE_CRITICAL_SECTION();
      USBPD_TRACE_DEBUG_PRINTF(port_num, "flag_rx_status = true, aborting");
      status = USBPD_ERROR;
    }
    else
    {
      PHY_LEAVE_CRITICAL_SECTION();

      switch (sop_type)
      {
        case USBPD_SOPTYPE_SOP :
        {
          (void)HAL_UCPD_SetTxOrderedSet(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_ORDERED_SET_SOP);
          (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_NORMAL);
          break;
        }
        case USBPD_SOPTYPE_SOP1 :
        {
          (void)HAL_UCPD_SetTxOrderedSet(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_ORDERED_SET_SOP1);
          (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_NORMAL);
          break;
        }
        case USBPD_SOPTYPE_SOP2 :
        {
          (void)HAL_UCPD_SetTxOrderedSet(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_ORDERED_SET_SOP2);
          (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_NORMAL);
          break;
        }
        case USBPD_SOPTYPE_CABLE_RESET :
        {
          (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_CABLE_RESET);
          break;
        }
        case USBPD_SOPTYPE_BIST_MODE_2 :
        {
          (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_BIST);
          break;
        }
        default :
          status = USBPD_ERROR;
          break;
      }

      if (USBPD_OK == status)
      {
#if defined(_LOW_POWER)
        UTIL_LPM_SetStopMode((0U == port_num) ? LPM_PE_0 : LPM_PE_1, UTIL_LPM_DISABLE);
#endif /* _LOW_POWER */

        /* Abort Tx DMA process if still active */
        if (h_phy_hw_if[port_num].hucpd->hdma_tx->global_state == HAL_DMA_STATE_ACTIVE)
        {
          if (HAL_DMA_Abort(h_phy_hw_if[port_num].hucpd->hdma_tx) != HAL_OK)
          {
            status = USBPD_ERROR;
            return status;
          }
        }

        /* Transmit message in DMA mode */
        USBPD_TRACE_DEBUG_PRINTF(port_num, "HAL_UCPD_Transmit_DMA");
        if (HAL_UCPD_Transmit_DMA(h_phy_hw_if[port_num].hucpd, p_buffer_tx, size) != HAL_OK)
        {
          status = USBPD_ERROR;
          return status;
        }
      }
    }
  }
  return status;
}

/**
  * @brief  Send a GoodCRC on the CC line.
  * @param  port_num Port.
  * @param  sop_type SOP Message Type based on @ref usbpd_sop_type_t.
  * @param  p_buffer_tx Data buffer to be transmitted.
  * @param  size The number of bytes to be transmitted.
  * @note   The data will be converted in bmc and send through the CC line.
  * @retval USBPD Status.
  */
usbpd_status_t USBPD_PHY_HW_IF_SendGoodCRC(uint8_t port_num, usbpd_sop_type_t sop_type, uint8_t *p_buffer_tx,
                                           uint32_t size)
{
  usbpd_status_t status = USBPD_OK;
  USBPD_TRACE_DEBUG_PRINTF(port_num, "%s type = %d, size = %d", __func__, sop_type, size);

  (void)size; /* Size is fixed to 2 bytes for a goodCRC */

  hal_ucpd_state_t hal_ucpd_state = HAL_UCPD_GetState(h_phy_hw_if[port_num].hucpd);

  PHY_ENTER_CRITICAL_SECTION();

  /* If Rx is ongoing or if a DMA transfer is active then discard the buffer sending */
  if ((h_phy_hw_if[port_num].flag_rx_status == USBPD_TRUE)
      || (hal_ucpd_state == HAL_UCPD_STATE_TX) || (hal_ucpd_state == HAL_UCPD_STATE_ABORT))
  {
    PHY_LEAVE_CRITICAL_SECTION();
    USBPD_TRACE_DEBUG_PRINTF(port_num, "flag_rx_status = true, aborting");
    status = USBPD_ERROR;
  }
  else
  {
    PHY_LEAVE_CRITICAL_SECTION();

    switch (sop_type)
    {
      case USBPD_SOPTYPE_SOP :
      {
        (void)HAL_UCPD_SetTxOrderedSet(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_ORDERED_SET_SOP);
        (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_NORMAL);
        break;
      }
      case USBPD_SOPTYPE_SOP1 :
      {
        (void)HAL_UCPD_SetTxOrderedSet(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_ORDERED_SET_SOP1);
        (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_NORMAL);
        break;
      }
      case USBPD_SOPTYPE_SOP2 :
      {
        (void)HAL_UCPD_SetTxOrderedSet(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_ORDERED_SET_SOP2);
        (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_NORMAL);
        break;
      }
      case USBPD_SOPTYPE_CABLE_RESET :
      {
        (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_CABLE_RESET);
        break;
      }
      case USBPD_SOPTYPE_BIST_MODE_2 :
      {
        (void)HAL_UCPD_SetTxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TX_MODE_BIST);
        break;
      }
      default :
        status = USBPD_ERROR;
        break;
    }

    if (USBPD_OK == status)
    {
#if defined(_LOW_POWER)
      UTIL_LPM_SetStopMode((0U == port_num) ? LPM_PE_0 : LPM_PE_1, UTIL_LPM_DISABLE);
#endif /* _LOW_POWER */

      /* Update the GoodCRC buffer to be sent */
      (void)HAL_UCPD_SetGoodCRCData(h_phy_hw_if[port_num].hucpd,
                                    ((uint16_t)p_buffer_tx[1U] << 8U) + (uint16_t)p_buffer_tx[0U]);
      USBPD_TRACE_DEBUG_PRINTF(port_num, "HAL_UCPD_SetGoodCRCData: 0x%04x",
                               (h_phy_hw_if[port_num].hucpd->buff_goodcrc));
    }
  }
  return status;
}
#endif /* !USBPDCORE_LIB_NO_PD */

/**
  * @brief  Assert Rp resistors: connect the Rp resistors on the CC lines.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_AssertRp(uint8_t port_num)
{
  switch ((usbpd_cad_rp_value_t)h_phy_hw_if[port_num].params->rp_resistor)
  {
    case USBPD_CAD_RP_DEFAULT:
      (void)HAL_UCPD_SetRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RP_DEFAULT);
      break;

    case USBPD_CAD_RP_1A5:
      (void)HAL_UCPD_SetRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RP_1_5A);
      /* Apply Rp resistor trimming on concerned series */
#if defined(HAL_UCPD_SOFT_TRIM)
      (void)HAL_UCPD_ApplyTrimmingRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TRIM_RP_1_5A);
#endif /* HAL_UCPD_SOFT_TRIM */
      break;

    case USBPD_CAD_RP_3A0:
      (void)HAL_UCPD_SetRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RP_3_0A);
      /* Apply Rp resistor trimming on concerned series */
#if defined(HAL_UCPD_SOFT_TRIM)
      (void)HAL_UCPD_ApplyTrimmingRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TRIM_RP_3_0A);
#endif /* HAL_UCPD_SOFT_TRIM */
      break;

    default:
      (void)HAL_UCPD_SetRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RP_DEFAULT);
      break;
  }

  (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_NONE);
  (void)HAL_UCPD_SetRole(h_phy_hw_if[port_num].hucpd, HAL_UCPD_ROLE_SOURCE);

  if (h_phy_hw_if[port_num].active_cc == CCNONE)
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC1CC2);
  }
  else if (h_phy_hw_if[port_num].active_cc == CC1)
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC1);
  }
  else
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC2);
  }

  (void)USBPD_PWR_SetRole(port_num, USBPD_PWR_ROLE_SOURCE);
}

/**
  * @brief  Assert Rd resistors: connect the Rd resistors on the CC line.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_AssertRd(uint8_t port_num)
{
  (void)HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  (void)HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

#if defined(HAL_UCPD_SOFT_TRIM)
  (void)HAL_UCPD_ApplyTrimmingRd(h_phy_hw_if[port_num].hucpd);
#endif /* HAL_UCPD_SOFT_TRIM */

  (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_NONE);
  (void)HAL_UCPD_SetRole(h_phy_hw_if[port_num].hucpd, HAL_UCPD_ROLE_SINK);

  if (h_phy_hw_if[port_num].active_cc == CCNONE)
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC1CC2);
  }
  else if (h_phy_hw_if[port_num].active_cc == CC1)
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC1);
  }
  else
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC2);
  }

#if !defined(_LOW_POWER)
  (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#endif /* !_LOW_POWER */

  (void)USBPD_PWR_SetRole(port_num, USBPD_PWR_ROLE_SINK);
}

/**
  * @brief  Enter error recovery procedure.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_EnterErrorRecovery(uint8_t port_num)
{
  USBPD_TRACE_DEBUG_PRINTF(port_num, "%s", __func__);
  (void)HAL_UCPD_SetRole(h_phy_hw_if[port_num].hucpd, HAL_UCPD_ROLE_SOURCE);
  (void)HAL_UCPD_SetRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RP_NONE);

#if !defined(USBPDCORE_LIB_NO_PD)
  /* Stop DMA Rx/Tx */
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Stop DMA Rx/Tx: HAL_UCPD_Abort_IT");
  (void)HAL_UCPD_Abort_IT(h_phy_hw_if[port_num].hucpd);

  (void)HAL_UCPD_DisableRx(h_phy_hw_if[port_num].hucpd);

  if (h_phy_hw_if[port_num].settings->pe_pd3_support.b.pe_fast_role_swap_support == USBPD_TRUE)
  {
    (void)HAL_UCPD_DisableFRS_Rx(h_phy_hw_if[port_num].hucpd);
  }
#endif /* USBPDCORE_LIB_NO_PD */
}

/**
  * @brief  Set the Active CC pin.
  * @param  port_num Port.
  * @param  cc CC pin based on @ref usbpd_cc_line_t.
  */
void USBPD_PHY_HW_IF_Set_CC(uint8_t port_num, usbpd_cc_line_t cc)
{
  /* Set the correct pin on the comparator */
  h_phy_hw_if[port_num].active_cc = cc;
  if (cc == CC1)
  {
    (void)HAL_UCPD_SetActiveCC(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  }
  else
  {
    (void)HAL_UCPD_SetActiveCC(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
  }
}


#if !defined(USBPDCORE_LIB_NO_PD)
/**
  * @brief  Enable Rx.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_EnableRX(uint8_t port_num)
{
  (void)HAL_UCPD_EnableRx(h_phy_hw_if[port_num].hucpd);
}

/**
  * @brief  Disable Rx.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_DisableRX(uint8_t port_num)
{
  (void)HAL_UCPD_DisableRx(h_phy_hw_if[port_num].hucpd);
}
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @brief  Signal a cable attachment.
  * @param  port_num Port.
  * @param  cc CC pin based on @ref usbpd_cc_line_t.
  */
void USBPD_PHY_HW_IF_SignalAttachement(uint8_t port_num, usbpd_cc_line_t cc)
{
  USBPD_TRACE_DEBUG_PRINTF(port_num, "%s", __func__);

#if !defined(USBPDCORE_LIB_NO_PD)
  /* Init timer to detect the reception of goodCRC */
  USBPD_TIMERSERVER_Init();
#endif /* !USBPDCORE_LIB_NO_PD */

  /* Handle CC enable */
  h_phy_hw_if[port_num].active_cc = cc;

#if !defined(USBPDCORE_LIB_NO_PD)
  /* Enable Hard reset reception */
  (void)HAL_UCPD_EnableHardReset_Rx(h_phy_hw_if[port_num].hucpd);

  /* Set CC pin for PD message */
  if (h_phy_hw_if[port_num].active_cc == CC1)
  {
    (void)HAL_UCPD_SetActiveCC(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  }
  else
  {
    (void)HAL_UCPD_SetActiveCC(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
  }

  /* Initialize Vconn management */
  (void)USBPD_PWR_VConnInit(port_num);

  if (h_phy_hw_if[port_num].settings->pe_pd3_support.b.pe_fast_role_swap_support == USBPD_TRUE)
  {
    /* Enable FRS Rx */
    (void)HAL_UCPD_EnableFRS_Rx(h_phy_hw_if[port_num].hucpd);
  }

  /* Disable the Resistor on Vconn PIN */
  if (h_phy_hw_if[port_num].active_cc == CC1)
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC1);
  }
  else
  {
    (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC2);
  }

  /* Prepare the rx processing */
  (void)HAL_UCPD_SetRxMode(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RX_MODE_NORMAL);

  if (h_phy_hw_if[port_num].hucpd->global_state != HAL_UCPD_STATE_RX)
  {
    /* Prepare next reception */
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Prepare next reception: HAL_UCPD_Receive_DMA");
    (void)HAL_UCPD_Receive_DMA(h_phy_hw_if[port_num].hucpd, h_phy_hw_if[port_num].ptr_rx_buffer,
                               USBPD_SIZE_MAX_PD_TRANSACTION_UNCHUNK);
  }
#endif /* !USBPDCORE_LIB_NO_PD */
}

/**
  * @brief  Signal a cable detachment.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_SignalDetachment(uint8_t port_num)
{
  USBPD_TRACE_DEBUG_PRINTF(port_num, "%s", __func__);

#if !defined(USBPDCORE_LIB_NO_PD)
  /* Stop DMA Rx/Tx */
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Stop DMA Rx/Tx: HAL_UCPD_Abort_IT");
  (void)HAL_UCPD_Abort_IT(h_phy_hw_if[port_num].hucpd);

  (void)HAL_UCPD_DisableRx(h_phy_hw_if[port_num].hucpd);

#if !defined(_LOW_POWER)
  /* Enable only detection interrupt */
  (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#else
  if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
  {
    /* Enable detection interrupt */
    (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
    (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
  }
#endif /* !_LOW_POWER */

  (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC1CC2);

  if (USBPD_PORT_PWR_ROLE_SNK == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
  {
    /* DeInitialize Vconn management */
    USBPD_PWR_VConnDeInit(port_num);

    /* DeInitialise VBUS power */
    USBPD_PWR_VBUSDeInit(port_num);
  }

  if (h_phy_hw_if[port_num].settings->pe_pd3_support.b.pe_fast_role_swap_support == USBPD_TRUE)
  {
    /* Set GPIO to disallow the FRS Rx handling */
    (void)HAL_UCPD_DisableFRS_Rx(h_phy_hw_if[port_num].hucpd);
  }

#endif /* !USBPDCORE_LIB_NO_PD */
  h_phy_hw_if[port_num].active_cc = CCNONE;
#if !defined(USBPDCORE_LIB_NO_PD)
  /* DeInit timer to detect the reception of goodCRC */
  (void)USBPD_TIMERSERVER_DeInit();
#endif /* !USBPDCORE_LIB_NO_PD */
}

/**
  * @brief  Set resistors to SinkTxNG.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_SetRpSnkTxNG(uint8_t port_num)
{
  /* Set the resistor SinkTxNG 1.5A5V */
  (void)HAL_UCPD_SetRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RP_1_5A);

  /* Apply Rp resistor trimming on concerned series */
#if defined(HAL_UCPD_SOFT_TRIM)
  (void)HAL_UCPD_ApplyTrimmingRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TRIM_RP_1_5A);
#endif /* HAL_UCPD_SOFT_TRIM */
}

/**
  * @brief  Set resistors to SinkTxOk.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_SetRpSnkTxOK(uint8_t port_num)
{
  /* Set the resistor SinkTxNG 3.0A5V */
  (void)HAL_UCPD_SetRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_RP_3_0A);

  /* Apply Rp resistor trimming on concerned series */
#if defined(HAL_UCPD_SOFT_TRIM)
  (void)HAL_UCPD_ApplyTrimmingRp(h_phy_hw_if[port_num].hucpd, HAL_UCPD_TRIM_RP_3_0A);
#endif /* HAL_UCPD_SOFT_TRIM */
}

/**
  * @brief  Check if resistors are set to SinkTxOk.
  * @param  port_num Port.
  * @retval USBPD_TRUE if resistor is set to SinkTxOk, else USBPD_FALSE.
  */
uint8_t USBPD_PHY_HW_IF_IsResistor_SinkTxOk(uint8_t port_num)
{
#if defined(_LOW_POWER)
  /* When in low power mode, the type C state machine is turned off.
     To retrieve any potential updates of the SR register, the state machine needs to be re-enabled briefly. */

  /* Enable type C state machine */
  (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

  /* Let time for internal state machine to refresh his state */
  for (uint32_t index = 0U; index < PHY_DELAY_READ_CC_STATUS; index++)
  {
    __DSB();
  }

  /* Disable type C state machine */
  (void)HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  (void)HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#endif /* _LOW_POWER */

  switch (h_phy_hw_if[port_num].active_cc)
  {
    case CC1 :
      if (HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1)
          == HAL_UCPD_VSTATE_SNK_VRP30A)
      {
        return USBPD_TRUE;
      }
      break;

    case CC2 :
      if (HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2)
          == HAL_UCPD_VSTATE_SNK_VRP30A)
      {
        return USBPD_TRUE;
      }
      break;

    default:
      return USBPD_FALSE;
      break;
  }

  return USBPD_FALSE;
}

/**
  * @brief  Signal a Fast Role Swap.
  * @param  port_num Port.
  */
void USBPD_PHY_HW_IF_FastRoleSwapSignalling(uint8_t port_num)
{
#if !defined(USBPDCORE_LIB_NO_PD)
  (void)HAL_UCPD_SendFRS(h_phy_hw_if[port_num].hucpd);
#endif /* USBPDCORE_LIB_NO_PD */
}

/**
  * @brief  Signal a Rx Reset indication.
  * @param  port_num Port number.
  * @param  sop_type SOP Message Type based on @ref usbpd_sop_type_t.
  */
void USBPD_PHY_HW_IF_RxResetIndication(uint8_t port_num, usbpd_sop_type_t sop_type)
{
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_RX_ResetIndication(port_num, sop_type);
}

/**
  * @brief  Register all PHY_HW_IF callbacks.
  * @param  port_num The port number.
  * @param  p_callbacks Pointer to the @ref usbpd_phy_hw_if_callbacks_t callbacks functions.
  * @retval USBPD_ERROR p_callback is NULL.
  * @retval USBPD_OK    Register completed successfully.
  */
usbpd_status_t USBPD_PHY_HW_IF_RegisterCallbacks(uint8_t port_num, const usbpd_phy_hw_if_callbacks_t *p_callbacks)
{
  /* Check the parameters */
  if (p_callbacks == NULL)
  {
    return USBPD_ERROR;
  }

  /* Set all callbacks */
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TxCompleted           = p_callbacks->USBPD_PHY_HW_IF_TxCompleted;
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_BistCompleted         = p_callbacks->USBPD_PHY_HW_IF_BistCompleted;
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_RX_ResetIndication    = p_callbacks->USBPD_PHY_HW_IF_RX_ResetIndication;
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_RX_Completed          = p_callbacks->USBPD_PHY_HW_IF_RX_Completed;
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TX_HardResetCompleted = p_callbacks->USBPD_PHY_HW_IF_TX_HardResetCompleted;
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TX_FRSReception       = p_callbacks->USBPD_PHY_HW_IF_TX_FRSReception;

  return USBPD_OK;
}

/**
  * @brief  Set PHY_HW_IF Rx buffer.
  * @param  port_num The port number.
  * @param  p_buffer Pointer to the Rx buffer.
  * @retval USBPD_ERROR p_buffer is NULL.
  * @retval USBPD_OK    Register completed successfully.
  */
usbpd_status_t USBPD_PHY_HW_IF_SetRxBuffer(uint8_t port_num, uint8_t *p_buffer)
{
  /* Check the parameters */
  if (p_buffer == NULL)
  {
    return USBPD_ERROR;
  }

  h_phy_hw_if[port_num].ptr_rx_buffer = p_buffer;

  return USBPD_OK;
}

/**
  * @}
  */

/** @addtogroup USBPD_PHY_HW_IF_Private_Functions USBPD_PHY_HW_IF Private Functions
  * @{
  */

#if !defined(USBPDCORE_LIB_NO_PD)
/**
  * @brief  Callback for Tx message sent.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_TxCpltCallback(hal_ucpd_handle_t *hucpd)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);

  h_phy_hw_if[port_num].flag_rx_error = USBPD_FALSE;

  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TxCompleted(port_num, USBPD_PHY_TX_CPLT);
#if defined(_LOW_POWER)
  UTIL_LPM_SetStopMode((0U == port_num) ? LPM_PE_0 : LPM_PE_1, UTIL_LPM_ENABLE);
#endif /* _LOW_POWER */
}

/**
  * @brief  Callback for Tx GoodCRC message sent.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_TxGoodCRCCpltCallback(hal_ucpd_handle_t *hucpd)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);

  h_phy_hw_if[port_num].flag_rx_error = USBPD_FALSE;

  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TxCompleted(port_num, USBPD_PHY_TX_CPLT);
#if defined(_LOW_POWER)
  UTIL_LPM_SetStopMode((0U == port_num) ? LPM_PE_0 : LPM_PE_1, UTIL_LPM_ENABLE);
#endif /* _LOW_POWER */
}

/**
  * @brief  Callback for Tx message discarded.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_TxDiscardCallback(hal_ucpd_handle_t *hucpd)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);

  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TxCompleted(port_num, USBPD_PHY_TX_DISCARD);
}

/**
  * @brief  Callback for Tx message aborted.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_TxAbortCallback(hal_ucpd_handle_t *hucpd)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);

  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TxCompleted(port_num, USBPD_PHY_TX_ABORT);
}

/**
  * @brief  Callback for Rx message received.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_RxCpltCallback(hal_ucpd_handle_t *hucpd)
{
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);

  /* Flag to follow end of abort in USBPD_PHY_HW_IF_AbortCpltCallback */
  h_phy_hw_if[port_num].flag_rx_cplt_abort = 1U;

  /* Abort the remaining DMA transfer */
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Abort remaining DMA transfer: HAL_UCPD_Abort_IT");
  (void)HAL_UCPD_Abort_IT(hucpd);
}

/**
  * @brief  Callback for Rx ordered set (4 K-codes) detected.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_RxOrdDetCallback(hal_ucpd_handle_t *hucpd)
{
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);
  /* Get the detected ordered set */
  hal_ucpd_detected_rx_ordered_set_t rx_ordset = HAL_UCPD_GetDetectedRxOrderedSet(hucpd);
  if (rx_ordset == HAL_UCPD_RX_ORDERED_SET_DETECT_CABLE_RESET)
  {
    /* Cable reset detected */
    h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_RX_ResetIndication(port_num, USBPD_SOPTYPE_CABLE_RESET);
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Cable reset detected");
  }

#if defined(_LOW_POWER)
  UTIL_LPM_SetStopMode((0U == port_num) ? LPM_PE_0 : LPM_PE_1, UTIL_LPM_DISABLE);
#endif /* _LOW_POWER */

  /* Forbid message sending */
  h_phy_hw_if[port_num].flag_rx_status = USBPD_TRUE;
}

/**
  * @brief  Callback for hard reset sent.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_HRstSentCallback(hal_ucpd_handle_t *hucpd)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TX_HardResetCompleted(port_num, USBPD_SOPTYPE_HARD_RESET);
}

/**
  * @brief  Callback for Rx hard reset received.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_HRstRxCallback(hal_ucpd_handle_t *hucpd)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);
  h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_RX_ResetIndication(port_num, USBPD_SOPTYPE_HARD_RESET);
}

/**
  * @brief  Callback for hard reset discarded.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_HRstDiscardCallback(hal_ucpd_handle_t *hucpd)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  (void)port_num; /* Unused parameter in case debug trace is not active */
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);
  /* Nothing to do */
}

/**
  * @brief  Callback for Fast Role Swap event detection.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_FRSEvtCallback(hal_ucpd_handle_t *hucpd)
{
#if defined(_FRS)
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);
  if (((usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role   == USBPD_PORT_PWR_ROLE_SNK)
      && (h_phy_hw_if[port_num].params->pe_flag_swap_ongoing == USBPD_FALSE))
  {
    /* Confirm the FRS by checking if an RP is always present on the current CC line.
       We must wait for maximum FRS timing */
    USBPD_TIMERSERVER_Start((usbpd_timerserver_identifier_t)(2U * port_num), 150U);
    while ((USBPD_TIMERSERVER_IsExpired((usbpd_timerserver_identifier_t)(2U * port_num)) == 0U));

    hal_ucpd_vstate_t cc1_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(hucpd, HAL_UCPD_LINE_CC1);
    hal_ucpd_vstate_t cc2_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(hucpd, HAL_UCPD_LINE_CC2);

    if ((cc1_vstate != HAL_UCPD_VSTATE_SRC_VRA) && (cc1_vstate != HAL_UCPD_VSTATE_SNK_VRA)
        && (cc2_vstate != HAL_UCPD_VSTATE_SRC_VRA) && (cc2_vstate != HAL_UCPD_VSTATE_SNK_VRA)
        && (USBPD_PWR_STATE_EXPLICITCONTRACT == (usbpd_pwr_state_t)h_phy_hw_if[port_num].params->pe_pwr_status))
    {
      /* Switch the power to take the control of VBUS.
         When VBUS go under VSAFE5V the sink must switch ON VBUS in timing < tSrcFRSwap */
      USBPD_PWR_FRSVBUSEnable(port_num);
      USBPD_TRACE_INFO_PRINTF(port_num, "FRS Rx detected");
      h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_TX_FRSReception(port_num);
    }
  }
#else
  STM32_UNUSED(hucpd);
#endif /* _FRS */
}

/**
  * @brief  Abort complete callback.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_AbortCpltCallback(hal_ucpd_handle_t *hucpd)
{
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);
  /* Check flag coming from HAL_UCPD_RxCpltCallback */
  if (h_phy_hw_if[port_num].flag_rx_cplt_abort == 1U)
  {
    h_phy_hw_if[port_num].flag_rx_cplt_abort = 0U;
    h_phy_hw_if[port_num].flag_rx_status = USBPD_FALSE;

    /* Prepare next reception */
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Prepare next reception: HAL_UCPD_Receive_DMA");
    (void)HAL_UCPD_Receive_DMA(hucpd, h_phy_hw_if[port_num].ptr_rx_buffer, USBPD_SIZE_MAX_PD_TRANSACTION_UNCHUNK);

#if defined(_LOW_POWER)
    UTIL_LPM_SetStopMode((0U == port_num) ? LPM_PE_0 : LPM_PE_1, UTIL_LPM_ENABLE);
#endif /* _LOW_POWER */

    USBPD_TRACE_DEBUG_PRINTF(port_num, "flag_rx_error = %d", h_phy_hw_if[port_num].flag_rx_error);
    if (h_phy_hw_if[port_num].flag_rx_error == USBPD_FALSE)
    {
      /* Rx message has been received without error */
      hal_ucpd_detected_rx_ordered_set_t rx_ordset = HAL_UCPD_GetDetectedRxOrderedSet(hucpd);
      h_phy_hw_if[port_num].cbs.USBPD_PHY_HW_IF_RX_Completed(port_num, (uint32_t)rx_ordset);
    }
    h_phy_hw_if[port_num].flag_rx_error = USBPD_FALSE;
  }
}
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @brief  Callback for Type-C event detection on CCx.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  * @param  cc_line CC line of type @ref hal_ucpd_cc_line_t structure.
  */
static void USBPD_PHY_HW_IF_TypeCEventCCxCallback(hal_ucpd_handle_t *hucpd, hal_ucpd_cc_line_t cc_line)
{
  const hal_ucpd_handle_t *hucpd_cst = hucpd;
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd_cst, port_num);
  STM32_UNUSED(cc_line);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s  CC%d", __func__, (cc_line + 1U));
  /* Wakeup CAD to check the detection event */
  h_phy_hw_if[port_num].USBPD_CAD_WakeUp();
}

/**
  * @brief  Error callback.
  * @param  hucpd Pointer to a @ref hal_ucpd_handle_t structure.
  */
static void USBPD_PHY_HW_IF_ErrorCallback(hal_ucpd_handle_t *hucpd)
{
#if !defined(USBPDCORE_LIB_NO_PD)
  uint8_t port_num = USBPD_PORT_0;
  GET_PORT_NUM_FROM_UCPD_HANDLE(hucpd, port_num);
  USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: %s", __func__);
  if (STM32_IS_BIT_SET(hucpd->last_error_codes, HAL_UCPD_ERROR_RXERR))
  {
    STM32_CLEAR_BIT(hucpd->last_error_codes, HAL_UCPD_ERROR_RXERR);
    h_phy_hw_if[port_num].flag_rx_error = USBPD_TRUE;
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Flag is set: HAL_UCPD_ERROR_RXERR");

    USBPD_TRACE_DEBUG_PRINTF(port_num, "Call RxCplt reset procedure");
    USBPD_PHY_HW_IF_RxCpltCallback(hucpd);
  }
  if (STM32_IS_BIT_SET(hucpd->last_error_codes, HAL_UCPD_ERROR_TXUND))
  {
    /* Nothing to do */
    STM32_CLEAR_BIT(hucpd->last_error_codes, HAL_UCPD_ERROR_TXUND);
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Flag is set: HAL_UCPD_ERROR_TXUND");
  }
  if (STM32_IS_BIT_SET(hucpd->last_error_codes, HAL_UCPD_ERROR_RXOVR))
  {
    /* Nothing to do, the message will be discarded and port Partner will try sending again. */
    STM32_CLEAR_BIT(hucpd->last_error_codes, HAL_UCPD_ERROR_RXOVR);
    h_phy_hw_if[port_num].flag_rx_status = USBPD_FALSE;
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Flag is set: HAL_UCPD_ERROR_RXOVR");
  }
  if (STM32_IS_BIT_SET(hucpd->last_error_codes, HAL_UCPD_ERROR_DMA))
  {
    STM32_CLEAR_BIT(hucpd->last_error_codes, HAL_UCPD_ERROR_DMA);
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Flag is set: HAL_UCPD_ERROR_DMA");
  }
#endif /* USBPDCORE_LIB_NO_PD */
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

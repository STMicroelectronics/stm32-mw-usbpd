/**
  ******************************************************************************
  * @file    usbpd_phy.c
  * @brief   PHY Layer module.
  *          This file provides a set of functions to manage the PHY layer.
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
#include "usbpd_phy.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_PHY
  * @brief   This file contains PHY layer functions.
  * @details Receive from PRL a message and create a structured packet (according to the USBPD specifications):
  *          |SOP|DATA:[HEADER|DATAOBJECTS]|CRC|EOP|
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_PHY_Private_Types USBPD_PHY Private Types
  * @{
  */

/**
  * @brief Prototype definition shared in several callbacks.
  */
typedef void (*PHY_CB_t)(uint8_t port_num, usbpd_sop_type_t sop_type); /*!<  Common callback definition */

/**
  * @brief Handle of the PHY layer.
  */
typedef struct
{
  /**
    * @brief  Reports that a message has been received on a specified port.
    * @param  port_num The handle of the port.
    * @param  sop_type The type of the message received @ref usbpd_sop_type_t.
    * @note   Received data are stored inside port_num->pRxBuffPtr
    *         function called in the interrupt context.
    */
  PHY_CB_t USBPD_PHY_MessageReceived;

  uint32_t  sop_support; /*!< sop_support in bit field format: | SOP"Debug | SOP'Debug | SOP" | SOP' | SOP | */
} usbpd_phy_handle_t;

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_Private_Constants USBPD_PHY Private Constants
  * @{
  */

#define PHY_RETRY_TIME      (905U) /*!< Retry counter time in us. Used to determine when PRL must
                                        resend a message not acknowledged by a GoodCRC.                    */
#define PHY_MINGOODCRC_TIME (30U)  /*!< Min time to wait before sending a GoodCRC in us. Used to guarantee
                                        the minimum time between two PD messages.                          */
/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_PHY_Private_Macros USBPD_PHY Private Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_Private_Functions USBPD_PHY Private Functions
  * @{
  */

#if !defined(USBPDCORE_LIB_NO_PD)
static void PHY_Rx_Completed(uint8_t port_num, uint32_t msg_type);
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_Private_Variables USBPD_PHY Private Variables
  * @{
  */

#if !defined(USBPDCORE_LIB_NO_PD)
static usbpd_phy_handle_t h_phy[USBPD_PORT_COUNT]; /*!< PHY handles */
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_PHY_Exported_Functions USBPD_PHY Exported Functions
  * @{
  */

/**
  * @brief  Initialize the PHY of a specified port.
  * @param  port_num Number of the port.
  * @param  pCallbacks PHY callbacks.
  * @param  pRxBuffer Buffer to store received messages.
  * @param  pwr_role Power Role of the board.
  * @param  sop_support Bitfield of the supported SOP.
  * @retval USBPD status.
  */
usbpd_status_t USBPD_PHY_Init(uint8_t port_num, const usbpd_phy_callbacks_t *pCallbacks, uint8_t *pRxBuffer,
                              usbpd_port_pwr_role_t pwr_role, uint32_t sop_support)
{
#if !defined(USBPDCORE_LIB_NO_PD)
  (void)pwr_role; /* Prevent unused warning */

  /* Initialize port related functionalities inside this layer */
  h_phy[port_num].sop_support = sop_support;
  h_phy[port_num].USBPD_PHY_MessageReceived = pCallbacks->USBPD_PHY_MessageReceived;
#endif /* USBPDCORE_LIB_NO_PD */

  /* Initialize the hardware interface */
  USBPD_PHY_HW_IF_Init(port_num);

#if !defined(USBPDCORE_LIB_NO_PD)
  /* Register all callbacks */
  usbpd_phy_hw_if_callbacks_t cbs =
  {
    .USBPD_PHY_HW_IF_BistCompleted = pCallbacks->USBPD_PHY_BistCompleted,
    .USBPD_PHY_HW_IF_TX_FRSReception = pCallbacks->USBPD_PHY_FastRoleSwapReception,
    .USBPD_PHY_HW_IF_RX_Completed = PHY_Rx_Completed,
    .USBPD_PHY_HW_IF_TX_HardResetCompleted = pCallbacks->USBPD_PHY_ResetCompleted,
    .USBPD_PHY_HW_IF_RX_ResetIndication = pCallbacks->USBPD_PHY_ResetIndication,
    .USBPD_PHY_HW_IF_TxCompleted = pCallbacks->USBPD_PHY_TxCompleted,
  };
  if (USBPD_PHY_HW_IF_RegisterCallbacks(port_num, &cbs) != USBPD_OK)
  {
    return USBPD_ERROR;
  }

  if (USBPD_PHY_HW_IF_SetRxBuffer(port_num, pRxBuffer) != USBPD_OK)
  {
    return USBPD_ERROR;
  }
#endif /* USBPDCORE_LIB_NO_PD */

  return USBPD_OK;
}

/**
  * @brief  DeInitialize the PHY of a specified port.
  * @param  port_num Number of the port.
  */
void USBPD_PHY_DeInit(uint8_t port_num)
{
#if !defined(USBPDCORE_LIB_NO_PD)
  /* Reset all callbacks */
  usbpd_phy_hw_if_callbacks_t cbs =
  {
    .USBPD_PHY_HW_IF_BistCompleted = NULL,
    .USBPD_PHY_HW_IF_TX_FRSReception = NULL,
    .USBPD_PHY_HW_IF_RX_Completed = NULL,
    .USBPD_PHY_HW_IF_TX_HardResetCompleted = NULL,
    .USBPD_PHY_HW_IF_RX_ResetIndication = NULL,
    .USBPD_PHY_HW_IF_TxCompleted = NULL,
  };
  (void)USBPD_PHY_HW_IF_RegisterCallbacks(port_num, &cbs);

  (void)USBPD_PHY_HW_IF_SetRxBuffer(port_num, NULL);

  /* DeInitialize port related functionalities inside this layer */
  h_phy[port_num].sop_support = 0U;
  h_phy[port_num].USBPD_PHY_MessageReceived = NULL;
#endif /* USBPDCORE_LIB_NO_PD */

  USBPD_PHY_HW_IF_DeInit(port_num);
}

#if !defined(USBPDCORE_LIB_NO_PD)

/**
  * @brief  Returns the retry counter value in us.
  * @param  port_num Number of the port.
  * @note   This is the time used to determine when the protocol layer must
  *         resend a message not acknowledged by a GoodCRC.
  * @retval Retry counter time value in us.
  */
uint16_t USBPD_PHY_GetRetryTimerValue(uint8_t port_num)
{
  (void)port_num;
  return PHY_RETRY_TIME;
}

/**
  * @brief  Returns the min time to wait before sending a GoodCRC to ack a message (in us).
  * @param  port_num Number of the port.
  * @note   This is the time used to guarantee the minimum time between two PD messages.
  * @retval MingoodCRC time value in us.
  */
uint16_t USBPD_PHY_GetMinGOODCRCTimerValue(uint8_t port_num)
{
  (void)port_num;
  return PHY_MINGOODCRC_TIME;
}

/**
  * @brief  Reset the PHY of a specified port.
  * @param  port_num Number of the port.
  */
void USBPD_PHY_Reset(uint8_t port_num)
{
  (void)port_num;
  /* Nothing to do */
}

/**
  * @brief  Request to send a reset on a port.
  * @param  port_num Number of the port.
  * @param  sop_type Sop_type of reset (hard or cable reset) @ref USBPD_SOPTYPE_HARD_RESET or
  *                     @ref USBPD_SOPTYPE_CABLE_RESET.
  * @retval USBPD_ERROR Error in reset request.
  * @retval USBPD_OK    Operation completed successfully.
  */
usbpd_status_t USBPD_PHY_ResetRequest(uint8_t port_num, usbpd_sop_type_t sop_type)
{
  /* Send the requested reset */
  return USBPD_PHY_SendMessage(port_num, sop_type, NULL, 0U);
}

/**
  * @brief  Send a Message.
  * @param  port_num Number of the port.
  * @param  sop_type Sop_type of the message.
  * @param  pBuffer Pointer to the buffer to be transmitted.
  * @param  size size of the buffer (bytes).
  * @retval USBPD_ERROR Error in sending message.
  * @retval USBPD_OK    Operation completed successfully.
  */
usbpd_status_t USBPD_PHY_SendMessage(uint8_t port_num, usbpd_sop_type_t sop_type, uint8_t *pBuffer,
                                     uint16_t size)
{
  return USBPD_PHY_HW_IF_SendBuffer(port_num, sop_type, pBuffer,  size);
}

/**
  * @brief  Send a GoodCRC.
  * @param  port_num Number of the port.
  * @param  sop_type Sop_type of the message.
  * @param  pBuffer Pointer to the buffer to be transmitted.
  * @param  size Size of the buffer (bytes).
  * @retval USBPD_ERROR Error in sending GoodCRC.
  * @retval USBPD_OK    Operation completed successfully.
  */
usbpd_status_t USBPD_PHY_SendGoodCRC(uint8_t port_num, usbpd_sop_type_t sop_type, uint8_t *pBuffer,
                                     uint16_t size)
{
  return USBPD_PHY_HW_IF_SendGoodCRC(port_num, sop_type, pBuffer,  size);
}

/**
  * @brief  Start transmission of infinite BIST pattern.
  * @param  port_num Number of the port.
  * @retval USBPD_ERROR Error in starting BIST mode.
  * @retval USBPD_OK    Operation completed successfully.
  */
usbpd_status_t USBPD_PHY_Start_Tx_BIST_Pattern(uint8_t port_num)
{
  return USBPD_PHY_HW_IF_Start_Tx_BIST_Pattern(port_num);
}

/**
  * @brief  Stop transmission of infinite BIST pattern.
  * @param  port_num Number of the port.
  * @retval USBPD_ERROR Error in stopping BIST mode.
  * @retval USBPD_OK    Operation completed successfully.
  */
usbpd_status_t USBPD_PHY_Stop_Tx_BIST_Pattern(uint8_t port_num)
{
  return USBPD_PHY_HW_IF_Stop_Tx_BIST_Pattern(port_num);
}

/**
  * @brief  Set the SinkTxNg value of the resistor.
  * @param  port_num Number of the port.
  * @note   used to manage the collision avoidance.
  */
void USBPD_PHY_SetResistor_SinkTxNG(uint8_t port_num)
{
  USBPD_PHY_HW_IF_SetRpSnkTxNG(port_num);
}

/**
  * @brief  Set the resistors to SinkTxOK.
  * @param  port_num Number of the port.
  * @note   used to manage the collision avoidance.
  */
void USBPD_PHY_SetResistor_SinkTxOK(uint8_t port_num)
{
  USBPD_PHY_HW_IF_SetRpSnkTxOK(port_num);
}

/**
  * @brief  Set the supported SOP.
  * @param  port_num Number of the port.
  * @param  SOPSupported List of the supported SOP.
  */
void USBPD_PHY_SOPSupported(uint8_t port_num, uint32_t SOPSupported)
{
  h_phy[port_num].sop_support = SOPSupported;
}

/**
  * @brief  Checks if SinkTxOK is set.
  * @param  port_num Number of the port.
  * @note   used to manage the collision avoidance.
  * @retval USBPD_TRUE when SinkTxOk is set, USBPD_FALSE otherwise.
  */
uint8_t USBPD_PHY_IsResistor_SinkTxOk(uint8_t port_num)
{
  return USBPD_PHY_HW_IF_IsResistor_SinkTxOk(port_num);
}

/**
  * @brief  Generate an FRS signalling.
  * @param  port_num Number of the port.
  */
void USBPD_PHY_FastRoleSwapSignalling(uint8_t port_num)
{
  USBPD_PHY_HW_IF_FastRoleSwapSignalling(port_num);
}

/**
  * @brief  Enable Rx.
  * @param  port_num Number of the port.
  */
void USBPD_PHY_EnableRX(uint8_t port_num)
{
  USBPD_PHY_HW_IF_EnableRX(port_num);
}

/**
  * @brief  Disable Rx.
  * @param  port_num Number of the port.
  */
void USBPD_PHY_DisableRX(uint8_t port_num)
{
  USBPD_PHY_HW_IF_DisableRX(port_num);
}
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @}
  */

/** @defgroup USBPD_PHY_Private_Functions USBPD_PHY Private Functions
  * @{
  */

#if !defined(USBPDCORE_LIB_NO_PD)

/**
  * @brief  Callback to notify the end of the current reception.
  * @param  port_num Number of the port.
  * @param  msg_type SOP Message sop_type.
  */
static void PHY_Rx_Completed(uint8_t port_num, uint32_t msg_type)
{
  usbpd_sop_type_t _msgtype;
  const usbpd_sop_type_t tab_sop_value[] =
  {
    USBPD_SOPTYPE_SOP,        USBPD_SOPTYPE_SOP1,       USBPD_SOPTYPE_SOP2,
    USBPD_SOPTYPE_SOP1_DEBUG, USBPD_SOPTYPE_SOP2_DEBUG, USBPD_SOPTYPE_CABLE_RESET
  };

  _msgtype = tab_sop_value[msg_type];

  /* Check if the message must be forwarded to the USBPD stack */
  switch (_msgtype)
  {
    case USBPD_SOPTYPE_CABLE_RESET:
      if (0x1EU == (h_phy[port_num].sop_support & 0x1EU))
      {
        /* Nothing to do: the message will be discarded, and the port partner will retry the transmission */
        USBPD_PHY_HW_IF_RxResetIndication(port_num, USBPD_SOPTYPE_CABLE_RESET);
      }
      break;

    case USBPD_SOPTYPE_SOP:
    case USBPD_SOPTYPE_SOP1:
    case USBPD_SOPTYPE_SOP2:
    case USBPD_SOPTYPE_SOP1_DEBUG:
    case USBPD_SOPTYPE_SOP2_DEBUG:

      /* Check if the message is supported */
      if (!((uint8_t)(0x1U << _msgtype) != (h_phy[port_num].sop_support & (uint8_t)(0x1U << _msgtype))))
      {
        /* The message is not supported */
        h_phy[port_num].USBPD_PHY_MessageReceived(port_num, _msgtype);
      }
      break;

    default:
      break;
  }
}

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


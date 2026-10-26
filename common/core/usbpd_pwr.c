/**
  ******************************************************************************
  * @file    usbpd_pwr.c
  * @brief   PWR control module.
  *          This file provides a set of firmware functions to manage the PWR
  *          layer.
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
#include "usbpd_pwr.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_PHY_HW_IF
  * @brief   This file contains PHY hardware interface functions.
  * @{
  */

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

#define PWR_LAST_SPR_PDO_INDEX  (7U)    /*!< Last index for Standard Power Range PDOs  */
#define PWR_FIRST_EPR_PDO_INDEX (8U)    /*!< First index for Extended Power Range PDOs */
#define VBUS_ON_TIMEOUT         (2000U) /*!< VBUS ON timeout in ms                     */
#define PWR_PDO_TYPE_FIXED_MSK    (0x00000000UL) /*!< Fixed PDO type bits      */
#define PWR_PDO_TYPE_BATTERY_MSK  (0x40000000UL) /*!< Battery PDO type bits    */
#define PWR_PDO_TYPE_VARIABLE_MSK (0x80000000UL) /*!< Variable PDO type bits   */
#define PWR_PDO_TYPE_APDO_MSK     (0xC0000000UL) /*!< APDO/AVS PDO type bits   */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Macros USBPD_PHY_HW_IF Private Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Functions USBPD_PHY_HW_IF Private Functions
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_PHY_HW_IF_Private_Variables USBPD_PHY_HW_IF Private Variables
  * @{
  */

static uint32_t vbus_disconnect[USBPD_PORT_COUNT] = {0U}; /* VBUS disconnection threshold */
static uint32_t vbus_transition[USBPD_PORT_COUNT] = {0U}; /* VBUS transition threshold */

usbpd_dpm_handle_t h_usbpd_dpm[USBPD_PORT_COUNT]; /*<! DPM handle used to run the DPM process */

static const usbpd_pwr_cb_t *pwr_hw_if_callbacks = NULL;

#if !defined(USBPDCORE_LIB_NO_PD)
usbpd_pwr_port_pdo_storage_t USBPD_PWR_Port_PDO_Storage[USBPD_PORT_COUNT]; /*!< PDO Storage array declaration */
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_PHY_HW_IF_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize the PWR layer for a specified port.
  * @param  port_num Port number.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_Init(uint8_t port_num)
{
#if !defined(USBPDCORE_LIB_NO_PD)
  extern uint32_t USBPD_PWR_PDO_ListSRC[USBPD_PORT_COUNT][USBPD_MAX_NB_PDO]; /*< List of Source PDOs */
  extern uint32_t USBPD_PWR_PDO_ListSNK[USBPD_PORT_COUNT][USBPD_MAX_NB_PDO]; /*< List of Sink PDOs */
  static uint8_t USBPD_PWR_NbPDO[USBPD_PORT_COUNT][USBPD_PWR_PDO_SNK_SRC];   /*!< Source and Sink number of PDOs */

  for (uint8_t port = 0U; port < (uint8_t)USBPD_PORT_COUNT; port++)
  {
    uint8_t countSRC = 0U;
    uint8_t countSNK = 0U;
    uint32_t *pdo_list_src = USBPD_PWR_PDO_ListSRC[port];
    uint32_t *pdo_list_snk = USBPD_PWR_PDO_ListSNK[port];

    /* Count non-zero entries in Source PDO list */
    for (uint8_t i = 0U; i < USBPD_MAX_NB_PDO; i++)
    {
      if (pdo_list_src[i] != 0U)
      {
        countSRC++;
      }
      else
      {
        /* Stop counting when a zero is found */
        break;
      }
    }

    /* Count non-zero entries in Sink PDO list */
    for (uint8_t i = 0U; i < USBPD_MAX_NB_PDO; i++)
    {
      if (pdo_list_snk[i] != 0U)
      {
        countSNK++;
      }
      else
      {
        /* Stop counting when a zero is found */
        break;
      }
    }

    /* Check that all PDO defined after the count are zero */
    for (uint8_t i = countSNK; i < USBPD_MAX_NB_PDO; i++)
    {
      if (pdo_list_snk[i] != 0U)
      {
        return USBPD_ERROR;
      }
    }
    for (uint8_t i = countSRC; i < USBPD_MAX_NB_PDO; i++)
    {
      if (pdo_list_src[i] != 0U)
      {
        return USBPD_ERROR;
      }
    }

    /* Update counts for both Source and Sink PDOs */
    USBPD_PWR_NbPDO[port][USBPD_PWR_NB_PDO_SNK_INDEX] = countSNK; /* Sink PDO count   */
    USBPD_PWR_NbPDO[port][USBPD_PWR_NB_PDO_SRC_INDEX] = countSRC; /* Source PDO count */
  }

  /* Set links to PDO values and number for Port 0 (defined in PDO arrays in H file). */
  USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_list = USBPD_PWR_PDO_ListSRC[port_num];
  USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_count = &USBPD_PWR_NbPDO[port_num][USBPD_PWR_NB_PDO_SRC_INDEX];

  USBPD_PWR_Port_PDO_Storage[port_num].sink_pdos.pdo_list = USBPD_PWR_PDO_ListSNK[port_num];
  USBPD_PWR_Port_PDO_Storage[port_num].sink_pdos.pdo_count = &USBPD_PWR_NbPDO[port_num][USBPD_PWR_NB_PDO_SNK_INDEX];
#endif /* USBPDCORE_LIB_NO_PD */

  USBPD_PWR_ResetVbusThreshold(port_num);

  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_Init == NULL)
      || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_Init(port_num) != USBPD_OK))
  {
    return USBPD_ERROR;
  }

  return USBPD_OK;
}

/**
  * @brief  De-Initialize the PWR layer for a specified port.
  * @param  port_num Port number.
  */
void USBPD_PWR_DeInit(uint8_t port_num)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_DeInit == NULL))
  {
    return;
  }

  pwr_hw_if_callbacks->USBPD_PWR_HW_IF_DeInit(port_num);
}

/**
  * @brief  Initialize the VBUS module for a specified port.
  * @param  port_num Port number.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_VBUSInit(uint8_t port_num)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSInit == NULL))
  {
    return USBPD_ERROR;
  }

  return pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSInit(port_num);
}

/**
  * @brief  De-Initialize the VBUS module for a specified port.
  * @param  port_num Port number.
  */
void USBPD_PWR_VBUSDeInit(uint8_t port_num)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSDeInit == NULL))
  {
    return;
  }

  pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSDeInit(port_num);
}

/**
  * @brief  Enable VBUS power on a specified port.
  * @param  port_num Port number.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_EnableVBUS(uint8_t port_num)
{
  usbpd_status_t ret = USBPD_OK;
  const usbpd_params_t *dpm_params;

  dpm_params = USBPD_CAD_Get_DPM_Params(port_num);

  if (dpm_params == NULL)
  {
    return USBPD_ERROR;
  }

  if (dpm_params->vconn_status == USBPD_TRUE)
  {
    if (USBPD_PWR_EnableVConn(port_num, (usbpd_cc_line_t)dpm_params->vconn_cc) != USBPD_OK)
    {
      ret = USBPD_ERROR;
    }
  }

  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_EnableVBUS == NULL)
      || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_EnableVBUS(port_num) != USBPD_OK))
  {
    ret = USBPD_ERROR;
  }

  /* Get tick count */
  uint32_t tickstart = HAL_GetTick();
  /* Wait for VBUS to be above VSAFE 5V before continuing to void sending source cap. too early */
  while (USBPD_PWR_IsSupplyReady(port_num, USBPD_VSAFE_5V) != USBPD_OK)
  {
    if ((HAL_GetTick() - tickstart) > VBUS_ON_TIMEOUT)
    {
      return USBPD_TIMEOUT;
    }
  }

  return ret;
}

/**
  * @brief  Disable VBUS power on a specified port.
  * @param  port_num Port number.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_DisableVBUS(uint8_t port_num)
{
  usbpd_status_t ret = USBPD_OK;
  usbpd_params_t *dpm_params;

  dpm_params = USBPD_CAD_Get_DPM_Params(port_num);

  if (dpm_params == NULL)
  {
    return USBPD_ERROR;
  }

  if (dpm_params->vconn_status == USBPD_TRUE)
  {
    if (USBPD_PWR_DisableVConn(port_num, (usbpd_cc_line_t)dpm_params->vconn_cc) != USBPD_OK)
    {
      ret = USBPD_ERROR;
    }
  }

  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_DisableVBUS == NULL)
      || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_DisableVBUS(port_num) != USBPD_OK))
  {
    ret = USBPD_ERROR;
  }

  return ret;
}

/**
  * @brief  Checks if the VBUS power on a specified port is enabled.
  * @param  port_num Port number.
  * @retval USBPD_ENABLE if VBUS is enabled.
  * @retval USBPD_DISABLE if VBUS is disabled.
  */
usbpd_functional_status_t USBPD_PWR_IsEnabledVBUS(uint8_t port_num)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_IsEnabledVBUS == NULL))
  {
    return USBPD_DISABLE;
  }
  /* Get the Status of the port */
  return pwr_hw_if_callbacks->USBPD_PWR_HW_IF_IsEnabledVBUS(port_num);
}

/**
  * @brief  Initialize VConn for a specified port.
  * @param  port_num Port number.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_VConnInit(uint8_t port_num)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VConnInit == NULL))
  {
    return USBPD_ERROR;
  }

  return pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VConnInit(port_num);
}

/**
  * @brief  De-Initialize VConn for a specified port.
  * @param  port_num Port number.
  */
void USBPD_PWR_VConnDeInit(uint8_t port_num)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VConnDeInit == NULL))
  {
    return;
  }

  pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VConnDeInit(port_num);
}

/**
  * @brief  Enables VConn on the port.
  * @param  port_num Port number.
  * @param  cc_pin Specifies the CCx on which to enable VConn, of type @ref usbpd_cc_line_t.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_EnableVConn(uint8_t port_num, usbpd_cc_line_t cc_pin)
{
  usbpd_status_t ret;
  usbpd_params_t *dpm_params;

  dpm_params = USBPD_CAD_Get_DPM_Params(port_num);

  if (dpm_params == NULL)
  {
    return USBPD_ERROR;
  }

  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_EnableVConn == NULL))
  {
    return USBPD_ERROR;
  }

  ret = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_EnableVConn(port_num, cc_pin);
  if (ret == USBPD_OK)
  {
    dpm_params->vconn_status = USBPD_TRUE;
  }
  return ret;
}

/**
  * @brief  Disable VConn on the port.
  * @param  port_num Port number.
  * @param  cc_pin Specifies the CCx on which to disable VConn, of type @ref usbpd_cc_line_t.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_DisableVConn(uint8_t port_num, usbpd_cc_line_t cc_pin)
{
  usbpd_status_t ret;
  usbpd_params_t *dpm_params;

  dpm_params = USBPD_CAD_Get_DPM_Params(port_num);

  if (dpm_params == NULL)
  {
    return USBPD_ERROR;
  }

  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_DisableVConn == NULL))
  {
    return USBPD_ERROR;
  }

  ret = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_DisableVConn(port_num, cc_pin);
  if (ret == USBPD_OK)
  {
    dpm_params->vconn_status = USBPD_FALSE;
  }
  return ret;
}

/**
  * @brief  Checks if VConn power on a specified port is enabled.
  * @param  port_num Port number.
  * @param  cc_pin Specifies the CCx on which to check VConn, of type @ref usbpd_cc_line_t.
  * @retval USBPD_ENABLE if VConn is enabled.
  * @retval USBPD_DISABLE if VConn is disabled.
  */
usbpd_functional_status_t USBPD_PWR_IsEnabledVConn(uint8_t port_num, usbpd_cc_line_t cc_pin)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_IsEnabledVConn == NULL))
  {
    return USBPD_DISABLE;
  }

  return pwr_hw_if_callbacks->USBPD_PWR_HW_IF_IsEnabledVConn(port_num, cc_pin);
}

/**
  * @brief  Assign Power role of Port.
  * @param  port_num Port number.
  * @param  pwr_role Type-C port role of type @ref usbpd_pwr_role_t.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_SetRole(uint8_t port_num, usbpd_pwr_role_t pwr_role)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_SetRole == NULL))
  {
    return USBPD_ERROR;
  }

  return pwr_hw_if_callbacks->USBPD_PWR_HW_IF_SetRole(port_num, (usbpd_pwr_hw_if_role_t)pwr_role);
}

/**
  * @brief  Set operating mode of Port.
  * @param  port_num Port number.
  * @param  pwr_mode Type-C port power saving mode of type @ref usbpd_pwr_mode_t.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_SetPowerMode(uint8_t port_num, usbpd_pwr_mode_t pwr_mode)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_SetPowerMode == NULL))
  {
    return USBPD_ERROR;
  }

  return pwr_hw_if_callbacks->USBPD_PWR_HW_IF_SetPowerMode(port_num, (usbpd_pwr_hw_if_mode_t)pwr_mode);
}

#if !defined(USBPDCORE_LIB_NO_PD)
/**
  * @brief  Sets the required power profile.
  * @param  port_num Port number.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_SetProfile(uint8_t port_num)
{
  usbpd_status_t  ret = USBPD_ERROR;
  usbpd_pdo_t    pdo =
  {
    .d32 = 0U
  };
  usbpd_rdo_snk_t rdo;
  uint32_t             pdo_type = 0U;
  uint32_t             vbus_traget_mV;
  uint32_t             vbus_traget_min_mV;
  uint32_t             vbus_traget_max_mV;
  uint32_t             operating_current;
  uint32_t             max_operating_current;
  uint32_t             operating_power;
  uint32_t             max_operating_power;
  int16_t              delta_vbus_mV;
  uint32_t             power_profile_selected;

  static usbpd_rdo_snk_t rdo_previous =
  {
    .d32 = 0U
  };

  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_Fixed == NULL)
      || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_Variable == NULL)
      || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_APDO == NULL)
      || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_Battery == NULL))
  {
    return ret;
  }

  power_profile_selected = h_usbpd_dpm[port_num].rdo_position - 1U;

  /* Check if the selected power profile is valid */
  if ((!USBPD_PWR_ISPORTVALID(port_num))
      || (power_profile_selected >= *USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_count)
#if defined(_EPR_SRC) || defined(_EPR_DRP)
      && (power_profile_selected < PWR_IF_LAST_SPR_PDO_INDEX))
    || (power_profile_selected > (PWR_IF_LAST_SPR_PDO_INDEX +
                                  *USBPD_PWR_Port_PDO_Storage[port_num].SourceEPRPDO.pdo_count))
#else
     )
#endif /* _EPR_SRC || _EPR_DRP */
  {
    return ret;
  }

  /* Retrieve the PDO to be applied */
  if (power_profile_selected < PWR_LAST_SPR_PDO_INDEX)
  {
    pdo.d32 = USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_list[power_profile_selected];
  }
#if defined(_EPR_SRC) || defined(_EPR_DRP)
  else
  {
    pdo.d32 =
      USBPD_PWR_Port_PDO_Storage[port_num].SourceEPRPDO.pdo_list[power_profile_selected - PWR_LAST_SPR_PDO_INDEX];
  }
#else
  else
  {
    return ret;
  }
#endif /* _EPR_SRC || _EPR_DRP */

  /* Retrieve the RDO to be applied */
  rdo.d32 = h_usbpd_dpm[port_num].rcv_request_do_msg;
  pdo_type = (pdo.d32 & PWR_PDO_TYPE_APDO_MSK);

  if (pdo_type == PWR_PDO_TYPE_FIXED_MSK)
  {
    vbus_traget_mV        = USBPD_PWR_DECODE_50MV(pdo.pdo_src_fixed.voltage_50mv_units);
    operating_current     = USBPD_PWR_DECODE_10MA(rdo.rdo_fixed_variable.operating_current_10ma_units);
    max_operating_current = USBPD_PWR_DECODE_10MA(rdo.rdo_fixed_variable.operating_current_max_10ma_units);

    ret = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_Fixed(port_num, vbus_traget_mV,
                                                                    operating_current, max_operating_current);
  }
  else if (pdo_type == PWR_PDO_TYPE_VARIABLE_MSK)
  {
    vbus_traget_min_mV    = USBPD_PWR_DECODE_50MV(pdo.pdo_src_variable.voltage_min_50mv_units);
    vbus_traget_max_mV    = USBPD_PWR_DECODE_50MV(pdo.pdo_src_variable.voltage_max_50mv_units);
    operating_current     = USBPD_PWR_DECODE_10MA(rdo.rdo_fixed_variable.operating_current_10ma_units);
    max_operating_current = USBPD_PWR_DECODE_10MA(rdo.rdo_fixed_variable.operating_current_max_10ma_units);

    ret = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_Variable(port_num, vbus_traget_min_mV,
                                                                       vbus_traget_max_mV, operating_current,
                                                                       max_operating_current);
  }
  else if (pdo_type == PWR_PDO_TYPE_APDO_MSK)
  {
    if (pdo.pdo_src_snk_apdo.programmable_pwr_supply == 0U)
    {
      vbus_traget_mV        = USBPD_PWR_DECODE_20MV(rdo.rdo_programmable.output_voltage_20mv_units);
      operating_current     = USBPD_PWR_DECODE_50MA(rdo.rdo_programmable.operating_current_50ma_units);
      delta_vbus_mV         = 0;
      if (rdo.rdo_programmable.object_position == rdo_previous.rdo_programmable.object_position)
      {
        delta_vbus_mV = USBPD_PWR_DECODE_20MV_SIGNED((int16_t)rdo_previous.rdo_programmable.output_voltage_20mv_units
                                                     - (int16_t)rdo.rdo_programmable.output_voltage_20mv_units);
      }
      ret = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_APDO(port_num, vbus_traget_mV, operating_current,
                                                                     delta_vbus_mV);
    }
    else
    {
      /* AVSPDO (EPR AVS) shares APDO type bits and is handled in this branch. */
      vbus_traget_mV        = USBPD_PWR_DECODE_25MV(rdo.rdo_avs.output_voltage_25mv_units);
      operating_current     = USBPD_PWR_DECODE_50MA(rdo.rdo_avs.operating_current_50ma_units);

      ret = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_Fixed(port_num, vbus_traget_mV, operating_current,
                                                                      operating_current);
    }
  }
  else if (pdo_type == PWR_PDO_TYPE_BATTERY_MSK)
  {
    vbus_traget_min_mV    = USBPD_PWR_DECODE_50MV(pdo.pdo_src_battery.voltage_min_50mv_units);
    vbus_traget_max_mV    = USBPD_PWR_DECODE_50MV(pdo.pdo_src_battery.voltage_max_50mv_units);
    operating_power       = USBPD_PWR_DECODE_10MA(rdo.rdo_battery.operating_pwr_250mw_units);
    max_operating_power   = USBPD_PWR_DECODE_250MW(rdo.rdo_battery.operating_pwr_max_250mw_units);

    ret = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSSetVoltage_Battery(port_num, vbus_traget_min_mV,
                                                                      vbus_traget_max_mV, operating_power,
                                                                      max_operating_power);
  }
  else
  {
    ret = USBPD_ERROR;
  }

  /* Save the previous RDO */
  if (ret == USBPD_OK)
  {
    rdo_previous.d32 = rdo.d32;
  }
  else
  {
    rdo_previous.d32 = 0;
  }

  return ret;
}
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @brief  Checks if the power on a specified port is ready.
  * @param  port_num Port number.
  * @param  vsafe_status Vsafe_status status based on @ref usbpd_vsafe_status_t.
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_IsSupplyReady(uint8_t port_num, usbpd_vsafe_status_t vsafe_status)
{
  usbpd_status_t status;
  uint32_t voltage;

  /* check for valid port */
  if ((!USBPD_PWR_ISPORTVALID(port_num)) || (pwr_hw_if_callbacks == NULL)
      || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSGetVoltage == NULL))
  {
    return USBPD_ERROR;
  }

  voltage = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSGetVoltage(port_num);

  if (USBPD_VSAFE_0V == vsafe_status)
  {
    /* Vsafe0V */
    status = ((voltage < USBPD_PWR_HW_IF_LOW_VBUS_THRESHOLD) ? USBPD_OK : USBPD_ERROR);
  }
  else
  {
    /* Vsafe5V */
    status = ((voltage > USBPD_PWR_HW_IF_HIGH_VBUS_THRESHOLD) ? USBPD_OK : USBPD_ERROR);
  }

  return status;
}

#if !defined(USBPDCORE_LIB_NO_PD)
/**
  * @brief  Get the current port PDOs.
  * @param  port_num Port number.
  * @param  data_id Type of data to be read from PWR of type @ref usbpd_core_datainfo_type_t.
  * @param  ptr Pointer on address where PDO values will be written.
  * @param  size Pointer on a variable to store the number of PDOs.
  */
void USBPD_PWR_GetPortPDOs(uint8_t port_num, usbpd_core_datainfo_type_t data_id, uint8_t *ptr, uint32_t *size)
{
  uint32_t    nbpdo;
  uint32_t    index;
  uint32_t    nb_valid_pdo = 0U;
  uint32_t    *ptpdoarray = NULL;
  usbpd_pdo_t pdo_first;
  usbpd_pdo_t pdo;
  const usbpd_params_t *dpm_params;

  dpm_params = USBPD_PE_Get_DPM_Params(port_num);

  if ((!USBPD_PWR_ISPORTVALID(port_num)) || (dpm_params == NULL))
  {
    return;
  }

  /* According to type of PDO to be read, set pointer on values and nb of elements */
  switch (data_id)
  {
    case USBPD_CORE_DATAINFO_DATATYPE_SRC_PDO :
      nbpdo = *USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_count;
      ptpdoarray = USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_list;

      /* Save the 1st PDO */
      pdo_first.d32 = *ptpdoarray;

      /* Reset unchunked bit if current revision is PD2.0*/
      if (USBPD_SPECIFICATION_REV2 == (usbpd_spec_rev_t)dpm_params->pe_spec_revision)
      {
        pdo_first.pdo_src_fixed.unchunked_extended_message  = USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED;
      }
      break;

    case USBPD_CORE_DATAINFO_DATATYPE_SNK_PDO:
      nbpdo       = *USBPD_PWR_Port_PDO_Storage[port_num].sink_pdos.pdo_count;
      ptpdoarray  = USBPD_PWR_Port_PDO_Storage[port_num].sink_pdos.pdo_list;

      /* Save the 1st PDO */
      pdo_first.d32 = *ptpdoarray;

      /* Reset FRS bit if current revision is PD2.0 */
      if (USBPD_SPECIFICATION_REV2 == (usbpd_spec_rev_t)dpm_params->pe_spec_revision)
      {
        pdo_first.pdo_snk_fixed.fast_role_swap_required_current = USBPD_PDO_SNK_FIXED_FRS_NOT_SUPPORTED;
      }
      break;

    default:
      nbpdo = 0U;
      break;
  }

  /* Copy PDO data in output buffer */
  for (index = 0U; index < nbpdo; index++)
  {
    pdo.d32 = *ptpdoarray;

    /* Copy only PDO (and not APDO in case of current revision is PD2.0) */
    if ((USBPD_SPECIFICATION_REV2 == (usbpd_spec_rev_t)dpm_params->pe_spec_revision)
        && ((usbpd_core_pdo_type_t)pdo.pdo_generic.pdo_type == USBPD_CORE_PDO_TYPE_APDO))
    {
      /* Skip APDOs for PD2.0 */
    }
    else
    {
      /* Copy 1st PDO as potentially FRS or UNCHUNKED bits have been reset */
      if (0U == index)
      {
        ptr[0] = ((uint8_t *)&pdo_first.d32)[0U];
        ptr[1] = ((uint8_t *)&pdo_first.d32)[1U];
        ptr[2] = ((uint8_t *)&pdo_first.d32)[2U];
        ptr[3] = ((uint8_t *)&pdo_first.d32)[3U];
      }
      else
      {
        uint32_t offset = nb_valid_pdo * 4U;
        ptr[offset]     = ((uint8_t *)ptpdoarray)[0U];
        ptr[offset + 1U] = ((uint8_t *)ptpdoarray)[1U];
        ptr[offset + 2U] = ((uint8_t *)ptpdoarray)[2U];
        ptr[offset + 3U] = ((uint8_t *)ptpdoarray)[3U];
      }
      nb_valid_pdo++;
    }
    ptpdoarray++;
  }

  /* Set nb of read PDO (nb of u32 elements) */
  *size = nb_valid_pdo;
}

/**
  * @brief  Search and get the requested PDO from the list of Source PDOs.
  * @param  port_num Port number.
  * @param  rdo_pos RDO Position in list of provided PDO.
  * @param  pPdo Pointer on PDO value pointed out by RDO position (u32 pointer).
  * @retval USBPD_OK Src PDO found for requested DO position (output pPdo parameter is set).
  * @retval USBPD_FAIL Position is not compliant with current Src PDO for this port (no corresponding PDO value).
  */
usbpd_status_t USBPD_PWR_SearchRequestedPDO(uint8_t port_num, uint32_t rdo_pos, uint32_t *pPdo)
{
  if ((rdo_pos == 0U) || (rdo_pos > *USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_count))
  {
    /* Invalid PDO index */
    return USBPD_FAIL;
  }
  *pPdo = USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_list[rdo_pos - 1U];
  return USBPD_OK;
}
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @brief Get the VBUS status.
  * @param  port_num Port number.
  * @param  pwr_vbus_status Power type status based on @ref usbpd_pwr_vbus_vsafe_status_t.
  * @retval UBBPD_TRUE or USBPD_FALSE.
  */
uint8_t USBPD_PWR_GetVBUSStatus(uint8_t port_num, usbpd_pwr_vbus_vsafe_status_t pwr_vbus_status)
{
  uint8_t status = USBPD_FALSE;

  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSGetVoltage == NULL))
  {
    return status;
  }

  uint32_t vbus = pwr_hw_if_callbacks->USBPD_PWR_HW_IF_VBUSGetVoltage(port_num);

  switch (pwr_vbus_status)
  {
    case USBPD_PWR_BELOW_VSAFE0V:
      if (vbus < USBPD_PWR_HW_IF_LOW_VBUS_THRESHOLD)
      {
        status = USBPD_TRUE;
      }
      break;

    case USBPD_PWR_ABOVE_VSAFE0V:
      if (vbus >= USBPD_PWR_HW_IF_LOW_VBUS_THRESHOLD)
      {
        status = USBPD_TRUE;
      }
      break;

    case USBPD_PWR_ABOVE_VSAFE5V:
      if (vbus >= USBPD_PWR_HW_IF_HIGH_VBUS_THRESHOLD)
      {
        status = USBPD_TRUE;
      }
      break;

    case USBPD_PWR_BELOW_VSAFE5V:
      if (vbus < USBPD_PWR_HW_IF_HIGH_VBUS_THRESHOLD)
      {
        status = USBPD_TRUE;
      }
      break;

    case USBPD_PWR_SNKDETACH:
      if (vbus_transition[port_num] != vbus_disconnect[port_num])
      {
        if (vbus_transition[port_num] > vbus_disconnect[port_num])
        {
          /* Voltage increase the disconnect value is under vbus_transition */
          if (vbus > ((vbus_transition[port_num] * 11U) / 10U))
          {
            /* The power transition is complete: disconnect threshold is updated */
            vbus_disconnect[port_num] = vbus_transition[port_num];
          }
        }
        else
        {
          /* Voltage decrease: the disconnect threshold is switched to the new level */
          vbus_disconnect[port_num] = vbus_transition[port_num];
        }
      }

      /* Check disconnect according the current threshold */
      if (vbus < vbus_disconnect[port_num])
      {
        status = USBPD_TRUE;
      }
      break;

    default:
      break;
  }
  return status;
}

/**
  * @brief  Set the VBUS threshold.
  * @param  port_num Port number.
  */
void USBPD_PWR_UpdateVbusThreshold(uint8_t port_num)
{
  usbpd_rdo_snk_t rdo;
  usbpd_pdo_t    SelectedPDO;

  rdo.d32 = h_usbpd_dpm[port_num].rdo_msg;
  SelectedPDO.d32 = h_usbpd_dpm[port_num].p_list_rcv_src_pdo[rdo.rdo_generic.object_position - 1U];

  switch ((usbpd_core_pdo_type_t)SelectedPDO.pdo_generic.pdo_type)
  {
    case USBPD_CORE_PDO_TYPE_FIXED:
    {
      switch (SelectedPDO.pdo_src_fixed.voltage_50mv_units * 50U)
      {
        case USBPD_PWR_VBUS_5V:
          vbus_transition[port_num] = USBPD_PWR_HW_IF_VBUS_THRESHOLD_5V;
          break;
        case USBPD_PWR_VBUS_9V:
          vbus_transition[port_num] = USBPD_PWR_HW_IF_VBUS_THRESHOLD_9V;
          break;
        case USBPD_PWR_VBUS_15V:
          vbus_transition[port_num] = USBPD_PWR_HW_IF_VBUS_THRESHOLD_15V;
          break;
        case USBPD_PWR_VBUS_20V:
          vbus_transition[port_num] = USBPD_PWR_HW_IF_VBUS_THRESHOLD_20V;
          break;
        default:
          vbus_transition[port_num] = USBPD_PWR_HW_IF_VBUS_THRESHOLD_5V;
          break;
      }
      break;
    }

#if defined(USBPDCORE_PPS)
    case USBPD_CORE_PDO_TYPE_APDO:
    {
      vbus_transition[port_num] = USBPD_PWR_HW_IF_VBUS_THRESHOLD_APDO;
      break;
    }
#endif /* USBPDCORE_PPS */

    case USBPD_CORE_PDO_TYPE_BATTERY:
    case USBPD_CORE_PDO_TYPE_VARIABLE:
    {
      /* Not yet handled */
      break;
    }
    default:
      break;
  }
}

/**
  * @brief  Reset VBUS threshold.
  * @param  port_num Port number.
  */
void USBPD_PWR_ResetVbusThreshold(uint8_t port_num)
{
  vbus_disconnect[port_num] = USBPD_PWR_HW_IF_HIGH_VBUS_THRESHOLD;
  vbus_transition[port_num] = USBPD_PWR_HW_IF_HIGH_VBUS_THRESHOLD;
}

/**
  * @brief  Register USB Type-C Current callback function.
  * @param  port_num Port number.
  * @param  pCallback Callback function pointer.
  * @note   Callback function invoked when VBUS rises (VBUS present) or
  *         when VBUS falls below programmed threshold (VBUS not present).
  * @return USBPD status of type @ref usbpd_status_t.
  */
usbpd_status_t USBPD_PWR_RegisterVBUSDetectCallback(uint8_t port_num,
                                                    usbpd_pwr_vbusdetect_cb_t pCallback)
{
  if ((pwr_hw_if_callbacks == NULL) || (pwr_hw_if_callbacks->USBPD_PWR_HW_IF_RegisterVBUSDetectCallback == NULL))
  {
    return USBPD_ERROR;
  }

  return pwr_hw_if_callbacks->USBPD_PWR_HW_IF_RegisterVBUSDetectCallback(port_num, pCallback);
}

/**
  * @brief  Return a pointer to the DPM handle associated with the given port.
  * @param  PortNum USBPD port number.
  * @retval Pointer to the corresponding @ref usbpd_dpm_handle_t instance.
  * @retval NULL if PortNum is out of range.
  */
usbpd_dpm_handle_t *USBPD_PWR_Get_DPM_Handler(uint8_t PortNum)
{
  if (PortNum >= (uint8_t)USBPD_PORT_COUNT)
  {
    return NULL;
  }

  return &h_usbpd_dpm[PortNum];
}

/**
  * @brief  Register the power hardware interface callback table.
  * @param  pwr_hw_if_cbs Pointer to callback table of type @ref usbpd_pwr_cb_t.
  * @note   This function links the generic PWR layer to the board-specific
  *         hardware implementation.
  * @note   The callback table must remain valid for the whole lifetime of the
  *         USBPD PWR layer usage.
  * @retval USBPD_OK if the callback table has been successfully registered.
  * @retval USBPD_ERROR if @p pwr_hw_if_cbs is NULL.
  */
usbpd_status_t USBPD_PWR_Register_HW_IF_Callbacks(const usbpd_pwr_cb_t *pwr_hw_if_cbs)
{
  if (pwr_hw_if_cbs == NULL)
  {
    return USBPD_ERROR;
  }

  pwr_hw_if_callbacks = pwr_hw_if_cbs;
  return USBPD_OK;
}

/**
  * @}
  */

/** @addtogroup USBPD_PHY_HW_IF_Private_Functions USBPD_PHY_HW_IF Private Functions
  * @{
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

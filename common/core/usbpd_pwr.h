/**
  ******************************************************************************
  * @file    usbpd_pwr.h
  * @brief   Header file for usbpd_pwr.c.
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

#ifndef USBPD_PWR_H
#define USBPD_PWR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#include "usbpd_dpm.h"
#include <string.h>

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_PWR
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_PWR_Exported_Constants USBPD_PWR Exported Constants
  * @{
  */

#define USBPD_PWR_VBUS_5V          (5000U)  /*!< 5V power definition  */
#define USBPD_PWR_VBUS_9V          (9000U)  /*!< 9V power definition  */
#define USBPD_PWR_VBUS_15V         (15000U) /*!< 15V power definition */
#define USBPD_PWR_VBUS_20V         (20000U) /*!< 20V power definition */

#define USBPD_PWR_NB_PDO_SNK_INDEX (0U)     /*!< Index in PDO count array for SNK PDOs */
#define USBPD_PWR_NB_PDO_SRC_INDEX (1U)     /*!< Index in PDO count array for SRc PDOs */
#define USBPD_PWR_PDO_SNK_SRC      (2U)     /*!< Number of element in PDO count array, one for Sink, one for Sounrce */

#define USBPD_PWR_HW_IF_HIGH_VBUS_THRESHOLD     (4750U)  /*!< VBUS threshold for Vsafe5V */
#define USBPD_PWR_HW_IF_LOW_VBUS_THRESHOLD      (800U)   /*!< VBUS threshold for Vsafe0V */

#define USBPD_PWR_HW_IF_VBUS_THRESHOLD_5V       (3900U)  /*!< VBUS threshold for 5V      */
#define USBPD_PWR_HW_IF_VBUS_THRESHOLD_9V       (7000U)  /*!< VBUS threshold for 9V      */
#define USBPD_PWR_HW_IF_VBUS_THRESHOLD_15V      (12500U) /*!< VBUS threshold for 15V     */
#define USBPD_PWR_HW_IF_VBUS_THRESHOLD_20V      (17000U) /*!< VBUS threshold for 20V     */
#define USBPD_PWR_HW_IF_VBUS_THRESHOLD_APDO     (2150U)  /*!< VBUS threshold for APDO    */

#define USBPD_PWR_HW_IF_VBUS_5V                 (5000U)  /*!< VBUS value for 5V. Do not modify */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_PWR_Exported_Types USBPD_PWR Exported Types
  * @{
  */

/**
  * @brief  Power roles.
  */
typedef enum
{
  USBPD_PWR_HW_IF_ROLE_SOURCE = 0U, /*!< Power role source */
  USBPD_PWR_HW_IF_ROLE_SINK   = 1U, /*!< Power role sink   */
  USBPD_PWR_HW_IF_ROLE_DUAL   = 2U, /*!< Power role dual   */
} usbpd_pwr_hw_if_role_t;

/**
  * @brief  Power modes of TypeC ports.
  */
typedef enum
{
  USBPD_PWR_HW_IF_MODE_OFF       = 0U, /*!< Port is off               */
  USBPD_PWR_HW_IF_MODE_HIBERNATE = 1U, /*!< Port is in hibernate mode */
  USBPD_PWR_HW_IF_MODE_LOWPOWER  = 2U, /*!< Port is in low power mode */
  USBPD_PWR_HW_IF_MODE_NORMAL    = 3U, /*!< Port is in normal mode    */
} usbpd_pwr_hw_if_mode_t;

/**
  * @brief  Power PDO Structure definition.
  */
typedef struct
{
  uint32_t *pdo_list;  /*!< Pointer on Power Data Objects list, defining port capabilities */
  uint8_t  *pdo_count; /*!< Number of Power Data Objects defined in pdo_list
                            This parameter must be set at max to @ref USBPD_MAX_NB_PDO value */
} usbpd_pwr_port_pdo_t;

/**
  * @brief  Power PDO Storage Structure definition.
  */
typedef struct
{
  usbpd_pwr_port_pdo_t source_pdos; /*!< SRC Power Data Objects */
  usbpd_pwr_port_pdo_t sink_pdos;   /*!< SNK Power Data Objects */
} usbpd_pwr_port_pdo_storage_t;

/**
  * @brief  Power VBUS status.
  */
typedef enum
{
  USBPD_PWR_BELOW_VSAFE0V = 1U, /*!< VBUS is below Vsafe0V                           */
  USBPD_PWR_ABOVE_VSAFE0V = 2U, /*!< VBUS is above Vsafe0V                           */
  USBPD_PWR_BELOW_VSAFE5V = 3U, /*!< VBUS is below Vsafe5V                           */
  USBPD_PWR_ABOVE_VSAFE5V = 4U, /*!< VBUS is above Vsafe5V                           */
  USBPD_PWR_SNKDETACH     = 5U, /*!< VBUS is low enough to be considered as detached */
} usbpd_pwr_vbus_vsafe_status_t;

/**
  * @brief  Power roles.
  */
typedef enum
{
  USBPD_PWR_ROLE_SOURCE = USBPD_PWR_HW_IF_ROLE_SOURCE, /*!< Power role source */
  USBPD_PWR_ROLE_SINK   = USBPD_PWR_HW_IF_ROLE_SINK,   /*!< Power role sink   */
  USBPD_PWR_ROLE_DUAL   = USBPD_PWR_HW_IF_ROLE_DUAL,   /*!< Power role dual   */
} usbpd_pwr_role_t;

/**
  * @brief  Power modes of TypeC ports.
  */
typedef enum
{
  USBPD_PWR_MODE_OFF       = USBPD_PWR_HW_IF_MODE_OFF,       /*!< Port is off               */
  USBPD_PWR_MODE_HIBERNATE = USBPD_PWR_HW_IF_MODE_HIBERNATE, /*!< Port is in hibernate mode */
  USBPD_PWR_MODE_LOWPOWER  = USBPD_PWR_HW_IF_MODE_LOWPOWER,  /*!< Port is in low power mode */
  USBPD_PWR_MODE_NORMAL    = USBPD_PWR_HW_IF_MODE_NORMAL,    /*!< Port is in normal mode    */
} usbpd_pwr_mode_t;

/**
  * @brief  Check if the port number is valid.
  * @param  port_num Port number.
  * @retval boolean comparison result.
  */
#define USBPD_PWR_ISPORTVALID(port_num) ((port_num) < ((uint8_t)USBPD_PORT_COUNT))

/**
  * @brief  Power hardware interface callbacks exposed by the PWR HW layer.
  */
typedef struct
{
  /**
    * @brief  Initialize the power hardware interface for a specified port.
    * @param  port_num Port number.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_Init)(uint8_t port_num);

  /**
    * @brief  De-initialize the power hardware interface for a specified port.
    * @param  port_num Port number.
    */
  void (*USBPD_PWR_HW_IF_DeInit)(uint8_t port_num);

  /**
    * @brief  Initialize the VBUS hardware interface for a specified port.
    * @param  port_num Port number.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_VBUSInit)(uint8_t port_num);

  /**
    * @brief  De-initialize the VBUS hardware interface for a specified port.
    * @param  port_num Port number.
    */
  void (*USBPD_PWR_HW_IF_VBUSDeInit)(uint8_t port_num);

  /**
    * @brief  Enable VBUS power path on a specified port.
    * @param  port_num Port number.
    * @retval USBPD status of type @ref usbpd_status_t.
    * @note   This callback is typically used when the port acts as Source.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_EnableVBUS)(uint8_t port_num);

  /**
    * @brief  Disable VBUS power path on a specified port.
    * @param  port_num Port number.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_DisableVBUS)(uint8_t port_num);

  /**
    * @brief  Check whether VBUS power path is enabled on a specified port.
    * @param  port_num Port number.
    * @retval USBPD_ENABLE if VBUS is enabled.
    * @retval USBPD_DISABLE if VBUS is disabled.
    */
  usbpd_functional_status_t (*USBPD_PWR_HW_IF_IsEnabledVBUS)(uint8_t port_num);

  /**
    * @brief  Initialize the VCONN hardware interface for a specified port.
    * @param  port_num Port number.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_VConnInit)(uint8_t port_num);

  /**
    * @brief  De-initialize the VCONN hardware interface for a specified port.
    * @param  port_num Port number.
    */
  void (*USBPD_PWR_HW_IF_VConnDeInit)(uint8_t port_num);

  /**
    * @brief  Enable VCONN on the selected CC pin for a specified port.
    * @param  port_num Port number.
    * @param  cc_pin CC line on which VCONN must be enabled.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_EnableVConn)(uint8_t port_num, usbpd_cc_line_t cc_pin);

  /**
    * @brief  Disable VCONN on a specified port.
    * @param  port_num Port number.
    * @param  cc_pin CC line associated with VCONN.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_DisableVConn)(uint8_t port_num, usbpd_cc_line_t cc_pin);

  /**
    * @brief  Check whether VCONN is enabled on the selected CC pin.
    * @param  port_num Port number.
    * @param  cc_pin CC line to be checked.
    * @retval USBPD_ENABLE if VCONN is enabled on the selected CC pin.
    * @retval USBPD_DISABLE if VCONN is disabled.
    */
  usbpd_functional_status_t (*USBPD_PWR_HW_IF_IsEnabledVConn)(uint8_t port_num, usbpd_cc_line_t cc_pin);

  /**
    * @brief  Get the VBUS voltage on a specified port.
    * @param  port_num Port number.
    * @retval VBUS voltage in mV.
    */
  uint32_t (*USBPD_PWR_HW_IF_VBUSGetVoltage)(uint8_t port_num);

  /**
    * @brief  Get the VBUS current on a specified port.
    * @param  port_num Port number.
    * @retval VBUS current in mA.
    * @note   This callback can return 0 if current measurement is not supported by the hardware.
    */
  int16_t (*USBPD_PWR_HW_IF_VBUSGetCurrent)(uint8_t port_num);

  /**
    * @brief  Configure a fixed PDO voltage on a specified port.
    * @param  port_num Port number.
    * @param  vbus_target_mv Target VBUS voltage in mV.
    * @param  operating_current_ma Operating current in mA.
    * @param  max_operating_current_ma Maximum operating current in mA.
    * @retval USBPD status of type @ref usbpd_status_t.
    * @note   This callback is used when a Fixed PDO has been selected.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_VBUSSetVoltage_Fixed)(uint8_t port_num,
                                                         uint32_t vbus_target_mv,
                                                         uint32_t operating_current_ma,
                                                         uint32_t max_operating_current_ma);

  /**
    * @brief  Configure a variable PDO voltage on a specified port.
    * @param  port_num Port number.
    * @param  vbus_target_max_mv Maximum target VBUS voltage in mV.
    * @param  vbus_target_min_mv Minimum target VBUS voltage in mV.
    * @param  operating_current_ma Operating current in mA.
    * @param  max_operating_current_ma Maximum operating current in mA.
    * @retval USBPD status of type @ref usbpd_status_t.
    * @note   This callback is used when a Variable PDO has been selected.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_VBUSSetVoltage_Variable)(uint8_t port_num,
                                                            uint32_t vbus_target_max_mv,
                                                            uint32_t vbus_target_min_mv,
                                                            uint32_t operating_current_ma,
                                                            uint32_t max_operating_current_ma);

  /**
    * @brief  Configure a battery PDO voltage on a specified port.
    * @param  port_num Port number.
    * @param  vbus_target_min_mv Minimum target VBUS voltage in mV.
    * @param  vbus_target_max_mv Maximum target VBUS voltage in mV.
    * @param  operating_power_mw Operating power in mW.
    * @param  max_operating_power_mw Maximum operating power in mW.
    * @retval USBPD status of type @ref usbpd_status_t.
    * @note   This callback is used when a Battery PDO has been selected.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_VBUSSetVoltage_Battery)(uint8_t port_num,
                                                           uint32_t vbus_target_min_mv,
                                                           uint32_t vbus_target_max_mv,
                                                           uint32_t operating_power_mw,
                                                           uint32_t max_operating_power_mw);

  /**
    * @brief  Configure an APDO voltage on a specified port.
    * @param  port_num Port number.
    * @param  vbus_target_mv Target VBUS voltage in mV.
    * @param  operating_current_ma Operating current in mA.
    * @param  delta_mv Voltage variation from previous setting in mV.
    * @retval USBPD status of type @ref usbpd_status_t.
    * @note   This callback is used for PPS / APDO handling.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_VBUSSetVoltage_APDO)(uint8_t port_num,
                                                        uint32_t vbus_target_mv,
                                                        uint32_t operating_current_ma,
                                                        int32_t  delta_mv);

  /**
    * @brief  Set the power role for a specified port.
    * @param  port_num Port number.
    * @param  pwr_role Power role of type @ref usbpd_pwr_hw_if_role_t.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_SetRole)(uint8_t port_num, usbpd_pwr_hw_if_role_t pwr_role);

  /**
    * @brief  Set the power mode for a specified port.
    * @param  port_num Port number.
    * @param  pwr_mode Power mode of type @ref usbpd_pwr_hw_if_mode_t.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_SetPowerMode)(uint8_t port_num, usbpd_pwr_hw_if_mode_t pwr_mode);

  /**
    * @brief  Register a callback for VBUS detection events.
    * @param  port_num Port number.
    * @param  pCallback Callback function pointer of type @ref usbpd_pwr_vbusdetect_cb_t.
    * @note   The callback is invoked when VBUS becomes present or absent.
    * @note   Passing NULL unregisters the callback.
    * @retval USBPD status of type @ref usbpd_status_t.
    */
  usbpd_status_t (*USBPD_PWR_HW_IF_RegisterVBUSDetectCallback)(uint8_t port_num,
                                                               const usbpd_pwr_vbusdetect_cb_t pCallback);
} usbpd_pwr_cb_t;

/**
  * @}
  */

/* Exported variables ---------------------------------------------------------*/
/** @defgroup USBPD_PWR_Exported_Variables USBPD_PWR Exported Variables
  * @{
  */

extern usbpd_dpm_handle_t h_usbpd_dpm[USBPD_PORT_COUNT]; /*!< Exported USBPD DPM handle */

#if !defined(USBPDCORE_LIB_NO_PD)
extern usbpd_pwr_port_pdo_storage_t USBPD_PWR_Port_PDO_Storage[USBPD_PORT_COUNT]; /*!< Exported PDO storage */
#endif /* USBPDCORE_LIB_NO_PD */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_PWR_Exported_Functions USBPD_PWR Exported Functions
  * @{
  */

/**
  * @brief  Initialize structures and variables related to power board profiles.
  *         used by Sink and Source, for all available ports.
  * @retval USBPD status.
  */
usbpd_status_t            USBPD_PWR_Init(uint8_t port_num);
void                      USBPD_PWR_DeInit(uint8_t port_num);

usbpd_status_t            USBPD_PWR_VBUSInit(uint8_t port_num);
void                      USBPD_PWR_VBUSDeInit(uint8_t port_num);

usbpd_status_t            USBPD_PWR_EnableVBUS(uint8_t port_num);
usbpd_status_t            USBPD_PWR_DisableVBUS(uint8_t port_num);
usbpd_functional_status_t USBPD_PWR_IsEnabledVBUS(uint8_t port_num);

usbpd_status_t            USBPD_PWR_VConnInit(uint8_t port_num);
void                      USBPD_PWR_VConnDeInit(uint8_t port_num);

usbpd_status_t            USBPD_PWR_EnableVConn(uint8_t port_num, usbpd_cc_line_t cc_pin);
usbpd_status_t            USBPD_PWR_DisableVConn(uint8_t port_num, usbpd_cc_line_t cc_pin);
usbpd_functional_status_t USBPD_PWR_IsEnabledVConn(uint8_t port_num, usbpd_cc_line_t cc_pin);

usbpd_status_t            USBPD_PWR_SetRole(uint8_t port_num, usbpd_pwr_role_t pwr_role);
usbpd_status_t            USBPD_PWR_SetPowerMode(uint8_t port_num, usbpd_pwr_mode_t pwr_mode);

usbpd_status_t            USBPD_PWR_SetProfile(uint8_t port_num);

usbpd_status_t            USBPD_PWR_IsSupplyReady(uint8_t port_num, usbpd_vsafe_status_t vsafe_status);

void                      USBPD_PWR_GetPortPDOs(uint8_t port_num, usbpd_core_datainfo_type_t data_id, uint8_t *ptr,
                                                uint32_t *size);

usbpd_status_t            USBPD_PWR_SearchRequestedPDO(uint8_t port_num, uint32_t rdo_pos, uint32_t *pPdo);

uint8_t                   USBPD_PWR_GetVBUSStatus(uint8_t port_num, usbpd_pwr_vbus_vsafe_status_t pwr_vbus_status);

void                      USBPD_PWR_UpdateVbusThreshold(uint8_t port_num);

void                      USBPD_PWR_ResetVbusThreshold(uint8_t port_num);

usbpd_status_t            USBPD_PWR_RegisterVBUSDetectCallback(uint8_t port_num, usbpd_pwr_vbusdetect_cb_t pCallback);

usbpd_dpm_handle_t        *USBPD_PWR_Get_DPM_Handler(uint8_t PortNum);

usbpd_status_t            USBPD_PWR_HW_IF_Init(uint8_t port_num);
void                      USBPD_PWR_HW_IF_DeInit(uint8_t port_num);

usbpd_status_t            USBPD_PWR_HW_IF_VBUSInit(uint8_t port_num);
void                      USBPD_PWR_HW_IF_VBUSDeInit(uint8_t port_num);

usbpd_status_t            USBPD_PWR_HW_IF_EnableVBUS(uint8_t port_num);
usbpd_status_t            USBPD_PWR_HW_IF_DisableVBUS(uint8_t port_num);
usbpd_functional_status_t USBPD_PWR_HW_IF_IsEnabledVBUS(uint8_t port_num);

usbpd_status_t            USBPD_PWR_HW_IF_VConnInit(uint8_t port_num);
void                      USBPD_PWR_HW_IF_VConnDeInit(uint8_t port_num);

usbpd_status_t            USBPD_PWR_HW_IF_EnableVConn(uint8_t port_num, usbpd_cc_line_t cc_pin);
usbpd_status_t            USBPD_PWR_HW_IF_DisableVConn(uint8_t port_num, usbpd_cc_line_t cc_pin);
usbpd_functional_status_t USBPD_PWR_HW_IF_IsEnabledVConn(uint8_t port_num, usbpd_cc_line_t cc_pin);

uint32_t                  USBPD_PWR_HW_IF_VBUSGetVoltage(uint8_t port_num);
int16_t                   USBPD_PWR_HW_IF_VBUSGetCurrent(uint8_t port_num);

usbpd_status_t            USBPD_PWR_HW_IF_VBUSSetVoltage_Fixed(uint8_t port_num,
                                                               uint32_t vbus_target_mv,
                                                               uint32_t operating_current_ma,
                                                               uint32_t max_operating_current_ma);

usbpd_status_t            USBPD_PWR_HW_IF_VBUSSetVoltage_Variable(uint8_t port_num,
                                                                  uint32_t vbus_target_max_mv,
                                                                  uint32_t vbus_target_min_mv,
                                                                  uint32_t operating_current_ma,
                                                                  uint32_t max_operating_current_ma);

usbpd_status_t            USBPD_PWR_HW_IF_VBUSSetVoltage_Battery(uint8_t port_num,
                                                                 uint32_t vbus_target_min_mv,
                                                                 uint32_t vbus_target_max_mv,
                                                                 uint32_t operating_power_mw,
                                                                 uint32_t max_operating_power_mw);

usbpd_status_t            USBPD_PWR_HW_IF_VBUSSetVoltage_APDO(uint8_t port_num,
                                                              uint32_t vbus_target_mv,
                                                              uint32_t operating_current_ma,
                                                              int32_t  delta_mv);

usbpd_status_t            USBPD_PWR_HW_IF_SetRole(uint8_t port_num, usbpd_pwr_hw_if_role_t pwr_role);
usbpd_status_t            USBPD_PWR_HW_IF_SetPowerMode(uint8_t port_num, usbpd_pwr_hw_if_mode_t pwr_mode);

usbpd_status_t            USBPD_PWR_HW_IF_RegisterVBUSDetectCallback(uint8_t port_num,
                                                                     const usbpd_pwr_vbusdetect_cb_t pCallback);

usbpd_status_t USBPD_PWR_Register_HW_IF_Callbacks(const usbpd_pwr_cb_t *pwr_hw_if_cbs);
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

#endif /* USBPD_PWR_H */

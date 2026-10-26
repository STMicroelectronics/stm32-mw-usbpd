/**
  ******************************************************************************
  * @file    usbpd_cad_hw_if.c
  * @brief   CAD hardware interface module.
  *          This file provides a set of functions to manage the CAD hardware
  *          interface.
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
#include "usbpd_cad_hw_if.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_CAD_HW_IF
  * @brief   This file contains CAD hardware interface functions.
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_CAD_HW_IF_Private_Types USBPD_CAD_HW_IF Private Types
  * @{
  */

/**
  * @brief CAD state Machine function pointer.
  */
typedef uint32_t usbpd_cad_state_machine_p(uint8_t port_num, usbpd_cad_event_t *Event, usbpd_cc_line_t *CCXX);

/**
  * @brief CAD States.
  */
typedef enum
{
  USBPD_CAD_STATE_RESET                   = 0U,  /*!< CAD state Reset                                    */
  USBPD_CAD_STATE_DETACHED                = 1U,  /*!< CAD state Detached: no cable detected              */
  USBPD_CAD_STATE_ATTACHED_WAIT           = 2U,  /*!< CAD state Attached Wait: port partner detected     */
  USBPD_CAD_STATE_ATTACHED                = 3U,  /*!< CAD state Attached: port partner attached          */
#if defined(USBPD_DRP) || defined(USBPD_SRC)
  USBPD_CAD_STATE_EMC                     = 4U,  /*!< CAD state Electronically Marked Cable detected     */
  USBPD_CAD_STATE_ATTEMC                  = 5U,  /*!< CAD state Port Partner detected through EMC        */
#if defined(_ACCESSORY_SRC)
  USBPD_CAD_STATE_DEBUG                   = 7U,  /*!< CAD state Debug detected                           */
  USBPD_CAD_STATE_ACCESSORY               = 6U,  /*!< CAD state Accessory detected                       */
#endif /* _ACCESSORY_SRC */
#endif /* USBPD_DRP || USBPD_SRC */
  USBPD_CAD_STATE_SWITCH_TO_SRC           = 8U,  /*!< CAD state Switch to Source                         */
  USBPD_CAD_STATE_SWITCH_TO_SNK           = 9U,  /*!< CAD state Switch to Sink                           */
  USBPD_CAD_STATE_UNKNOW                  = 10U, /*!< CAD state Unknown                                  */
  USBPD_CAD_STATE_DETACH_SRC              = 11U, /*!< CAD state Detach Source                            */
  USBPD_CAD_STATE_ERRORRECOVERY           = 12U, /*!< CAD state Error recovery                           */
  USBPD_CAD_STATE_ERRORRECOVERY_EXIT      = 13U, /*!< CAD state Exit error recovery                      */
#if defined(USBPD_SNK) && defined(_ACCESSORY_SNK)
  USBPD_CAD_STATE_UNATTACHED_ACCESSORY    = 14U,  /*!< CAD state Unattached Accessory                    */
  USBPD_CAD_STATE_AUDIO_ACCESSORY         = 15U,  /*!< CAD state Audio Accessory                         */
#endif /* USBPD_SNK && _ACCESSORY_SNK */
  USBPD_CAD_STATE_ATTACHED_ACCESSORY_WAIT = 16U,  /*!< CAD state Wait attached after Accessory detection */
#if defined(USBPDCORE_VPD)
  USBPD_CAD_STATE_POWERED_ACCESSORY       = 17U,  /*!< CAD state Powered Accessory                       */
  USBPD_CAD_STATE_UNSUPPORTED_ACCESSORY   = 18U,  /*!< CAD state Unsupported Accessory                   */
  USBPD_CAD_STATE_CTVPD_UNATTACHED        = 19U,  /*!< CAD state CTVPD Unattached                        */
  USBPD_CAD_STATE_CTVPD_ATTACHED          = 20U   /*!< CAD state CTVPD Attached                          */
#endif /* USBPDCORE_VPD */
} usbpd_cad_state_t;
/**
  * @}
  */

/**
  * @brief CAD CC lines hardware condition.
  */
typedef enum
{
  USBPD_CAD_HW_COND_Detachment                 = 0x00UL, /*!< Nothing attached                                        */
  USBPD_CAD_HW_COND_Attachment                 = 0x01UL, /*!< Sink attached                                           */
  USBPD_CAD_HW_COND_PwrCable_NoSink_Attachment = 0x02UL, /*!< Powered cable without Sink attached                     */
  USBPD_CAD_HW_COND_PwrCable_Sink_Attachment   = 0x03UL, /*!< Powered cable with Sink / VCONN-pwrd accessory attached */
  USBPD_CAD_HW_COND_Debug_Attachment           = 0x04UL, /*!< Debug Accessory Mode attached                           */
  USBPD_CAD_HW_COND_AudioAdapter_Attachment    = 0x05UL, /*!< Audio Adapter Accessory Mode attached                   */
} usbpd_cad_hw_condition_t;

/**
  * @brief CAD hardware handle.
  * @{
  */
typedef struct
{
  uint32_t                  cc                      : 2U;  /*!< CC line selected                              */
  uint32_t                  current_hw_condition    : 3U;  /*!< Current HW condition                          */
  uint32_t                  flag_debounce           : 1U;  /*!< Debounce flag                                 */
  uint32_t                  flag_accessory_debounce : 1U;  /*!< Debounce flag for accessory                   */
  uint32_t                  flag_error_recovery     : 1U;  /*!< Error recovery flag                           */
  uint32_t                  flag_resistor_update    : 1U;  /*!< Resistor update flag                          */
  uint32_t                  current_state           : 5U;  /*!< Current state                                 */
  uint32_t                  flag_accessory_src      : 1U;  /*!< Accessory flag for SRC                        */
  uint32_t                  flag_accessory_snk      : 1U;  /*!< Accessory flag for SNK                        */
  uint32_t                  reserved                : 1U;  /*!< Reserved                                      */
  uint32_t                  previous_state          : 5U;  /*!< Previous state                                */
#if defined(USBPDCORE_VPD)
  uint32_t                  flag_vpd_src            : 1U;  /*!< VPD flag for SRC                              */
  uint32_t                  flag_vpd_snk            : 1U;  /*!< VPD flag for SNK                              */
  uint32_t                  reserved2               : 8U;  /*!< Reserved                                      */
#else
  uint32_t                  reserved2               : 10U; /*!< Reserved                                      */
#endif /* USBPDCORE_VPD */

#if defined(USBPD_DRP) || defined(_ACCESSORY_SNK)
  uint32_t                  accessory_toggle_start_time;   /*!< Toggle start time for snk accessory detection */
#endif /* USBPD_DRP */
  uint32_t                  debounce_start_time;           /*!< Debounce start time                           */
  usbpd_cad_state_machine_p *state_machine_p;              /*!< Pointer on the CDA state machine              */
} usbpd_cad_hw_if_handle_t;

static const usbpd_cad_hw_if_handle_t CAD_HW_IF_HANDLE_RESET_VALUE =
{
  .cc = (uint32_t)CCNONE,
  .current_hw_condition = (uint32_t)0U,
  .flag_debounce = 0U,
  .flag_accessory_debounce = 0U,
  .flag_error_recovery = 0U,
  .flag_resistor_update = 0U,
  .current_state = (uint32_t)0U,
  .flag_accessory_src = 0U,
  .flag_accessory_snk = 0U,
  .reserved = 0U,
  .previous_state = (uint32_t)0U,
#if defined(USBPDCORE_VPD)
  .flag_vpd_src = 0U,
  .flag_vpd_snk = 0U,
  .reserved2 = 0U,
#else
  .reserved2 = 0U,
#endif /* USBPDCORE_VPD */
#if defined(USBPD_DRP) || defined(_ACCESSORY_SNK)
  .accessory_toggle_start_time = 0U,
#endif /* USBPD_DRP || _ACCESSORY_SNK */
  .debounce_start_time = 0U,
  .state_machine_p = NULL,
};

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_CAD_HW_IF_Private_Constants USBPD_CAD_HW_IF Private Constants
  * @{
  */

/** @defgroup  USBPD_CAD_HW_IF_Timings USBPD_CAD_HW_IF related timings
  * @{
  */

#define CAD_TPDDEBOUNCE_THRESHOLD       (12U)         /*!< tPDDebounce threshold between 10 to 20ms           */
#define CAD_TCCDEBOUNCE_THRESHOLD       (120U)        /*!< tCCDebounce threshold between 100 to 200ms         */
#define CAD_TSRCDISCONNECT_THRESHOLD    (2U)          /*!< tSRCDisconnect detach threshold between 0 to 20ms  */
#define CAD_INFINITE_TIME               (0XFFFFFFFFU) /*!< Infinite time to wait a new interrupt event        */
#define CAD_TERROR_RECOVERY_TIME        (26U)         /*!< tErrorRecovery min 25ms                            */
#define CAD_DEFAULT_TIME                (2U)          /*!< Default transition timing of the state machine     */
#define CAD_ACCESSORY_TOGGLE            (40U)         /*!< Toggle time for snk accessory detection            */
#define CAD_TVPDDETACH                  (10U)         /*!< tVPDDetach timing between 10 and 20 ms             */

#if defined(USBPD_DRP) || defined(USBPD_SRC)
#define CAD_DETACH_POLLING              (40U)         /*!< Detach polling time in source or drp mode          */
#elif defined(USBPD_SNK)
#define CAD_DETACH_POLLING              (100U)        /*!< Detach polling time in sink mode                   */
#endif /* USBPD_DRP || USBPD_SRC */

#if defined(_LOW_POWER)
#define CAD_VBUS_POLLING_TIME           (38U)         /*!< VBUS polling time in low power mode                */
#else
#define CAD_VBUS_POLLING_TIME           (10U)         /*!< VBUS polling time                                  */
#endif /* _LOW_POWER */

/**
  * @}
  */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_CAD_HW_IF_Private_Macros USBPD_CAD_HW_IF Private Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_CAD_HW_IF_Private_Functions USBPD_CAD_HW_IF Private Functions
  * @{
  */

static void CAD_HW_IF_VBUSDetectCallback(uint8_t port_num,
                                         usbpd_pwr_vbus_connection_status_t vbus_connection_status);

#if defined(USBPD_DRP) || defined(USBPD_SRC)
static void CAD_Check_HW_SRC(uint8_t port_num);
#endif /* USBPD_DRP || USBPD_SRC */

#if defined(USBPD_DRP) || defined(USBPD_SNK)
static uint32_t CAD_ManageStateAttachedWait_SNK(uint8_t port_num, usbpd_cad_event_t *p_event,
                                                usbpd_cc_line_t *p_cc_pin);
static uint32_t CAD_ManageStateAttached_SNK(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin);
static uint32_t CAD_ManageStateDetached_SNK(uint8_t port_num);
static void     CAD_Check_HW_SNK(uint8_t port_num);
#endif /* USBPD_DRP || USBPD_SNK */

#if defined(USBPD_DRP)
static uint32_t CAD_ManageStateAttachedWait_DRP(uint8_t port_num, usbpd_cad_event_t *p_event,
                                                usbpd_cc_line_t *p_cc_pin);
static uint32_t CAD_ManageStateAttached_DRP(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin);
#endif /* USBPD_DRP */

#if defined(USBPD_DRP) || defined(USBPD_SRC)
static uint32_t CAD_ManageStateAttachedWait_SRC(uint8_t port_num, usbpd_cad_event_t *p_event,
                                                usbpd_cc_line_t *p_cc_pin);
#endif /* USBPD_DRP || USBPD_SRC || (_ACCESSORY && USBPD_SNK) */

#if defined(USBPD_SRC) || defined(USBPD_DRP)
static uint32_t CAD_ManageStateDetached_SRC(uint8_t port_num);
#endif /* USBPD_SRC */

#if defined(USBPD_DRP) || defined(USBPD_SRC)
static uint32_t CAD_ManageStateEMC(uint8_t port_num, const usbpd_cad_event_t *p_event,
                                   const usbpd_cc_line_t *p_cc_pin);
static uint32_t CAD_ManageStateAttached_SRC(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin);
#endif /* USBPD_DRP || USBPD_SRC */

#if defined(USBPD_DRP)
static uint32_t CAD_ManageStateDetached_DRP(uint8_t port_num);
#endif /* USBPD_DRP */

#if defined(USBPD_SNK)
static uint32_t CAD_StateMachine_SNK(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin);
#endif /* USBPD_SNK */

#if defined(USBPD_SRC)
static uint32_t CAD_StateMachine_SRC(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin);
#endif /* USBPD_SRC */

#if defined(USBPD_DRP)
static uint32_t CAD_StateMachine_DRP(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin);
#endif /* USBPD_DRP */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_CAD_HW_IF_Private_Variables USBPD_CAD_HW_IF Private Variables
  * @{
  */

static usbpd_cad_hw_if_handle_t h_cad_hw_if[USBPD_PORT_COUNT]; /*!< CAD HW handles */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_CAD_HW_IF_Exported_Functions USBPD_CAD_HW_IF Exported Functions
  * @{
  */

/**
  * @brief  Function to initialize the cable detection state machine.
  * @param  port_num Port number.
  * @param  p_settings Pointer on PD settings based on @ref usbpd_settings_t.
  * @param  p_params Pointer on PD parameters based on @ref usbpd_params_t.
  * @param  p_wake_up_cb Wake-up callback function used for waking up CAD.
  */
hal_status_t USBPD_CAD_HW_IF_Init(uint8_t port_num, usbpd_settings_t *p_settings, usbpd_params_t *p_params,
                                  void (*p_wake_up_cb)(void))
{
  hal_status_t ret = HAL_OK;

  USBPD_TRACE_DEBUG_PRINTF(port_num, "USBPD_CAD_HW_IF_Init");

  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];

  h_phy_hw_if[port_num].params                        = p_params;
  h_phy_hw_if[port_num].settings                      = p_settings;
  h_phy_hw_if[port_num].params->rp_resistor           = (uint32_t)h_phy_hw_if[port_num].settings->cad_default_resistor;
  h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_UNDEF;

  /* Reset handle */
  *handle = CAD_HW_IF_HANDLE_RESET_VALUE;

  /* Register CAD wake up callback */
  h_phy_hw_if[port_num].USBPD_CAD_WakeUp = p_wake_up_cb;
#if defined(USBPDCORE_LIB_NO_PD)
  /* Retrieve the UCPD peripheral handle */
#if (USBPD_PORT_COUNT == 1U)
  h_phy_hw_if[port_num].hucpd = (hal_ucpd_handle_t *)usbpd_ucpd_0_gethandle();
#else
  if (port_num == USBPD_PORT_0)
  {
    h_phy_hw_if[port_num].hucpd = (hal_ucpd_handle_t *)usbpd_ucpd_0_gethandle();
  }
  else
  {
    h_phy_hw_if[port_num].hucpd = (hal_ucpd_handle_t *)usbpd_ucpd_1_gethandle();
  }
#endif /* USBPD_PORT_COUNT == 1U */
#endif /* USBPDCORE_LIB_NO_PD */
  /* Start UCPD */
  (void)HAL_UCPD_Start(h_phy_hw_if[port_num].hucpd);

  /* Enable both CC lines */
  (void)HAL_UCPD_SetCCLines(h_phy_hw_if[port_num].hucpd, HAL_UCPD_CC_ENABLE_CC1CC2);

#if defined(_LOW_POWER)
  (void)HAL_UCPD_EnableWakeupMode(h_phy_hw_if[port_num].hucpd);
#endif /* _LOW_POWER */

  /* Init power */
  if (USBPD_PWR_Init(port_num) != USBPD_OK)
  {
    ret = HAL_ERROR;
    return ret;
  }

  /* Register VBUS detect callback */
  if (USBPD_PWR_RegisterVBUSDetectCallback(port_num,
                                           (usbpd_pwr_vbusdetect_cb_t)CAD_HW_IF_VBUSDetectCallback) != USBPD_OK)
  {
    ret = HAL_ERROR;
    return ret;
  }

#if defined(USBPD_SRC) || defined(USBPD_DRP)
  /* Initialize usbpd interrupt */
  if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
  {
    USBPD_PHY_HW_IF_AssertRp(port_num);

    /* Switch to Low Power mode */
    if (USBPD_PWR_SetPowerMode(port_num, USBPD_PWR_MODE_LOWPOWER) != USBPD_OK)
    {
      ret = HAL_ERROR;
      return ret;
    }
  }
#endif /* USBPD_SRC || USBPD_DRP */
#if defined(USBPD_DRP)
  else
#endif /* USBPD_DRP */
#if defined(USBPD_SNK) || defined(USBPD_DRP)
  {
    USBPD_PHY_HW_IF_AssertRd(port_num);

    /* Switch to Low Power mode */
    if (USBPD_PWR_SetPowerMode(port_num, USBPD_PWR_MODE_LOWPOWER) != USBPD_OK)
    {
      ret = HAL_ERROR;
      return ret;
    }
  }
#endif /* USBPD_SNK || USBPD_DRP */

  /* Set the state machine according the SW configuration */
#if !defined(USBPDCORE_LIB_NO_PD)
#if defined(USBPD_DRP)
  if (h_phy_hw_if[port_num].settings->cad_role_toggle_support == USBPD_TRUE)
  {
    /* Set current state machine to DRP state machine */
    handle->state_machine_p = CAD_StateMachine_DRP;
    handle->flag_accessory_src = h_phy_hw_if[port_num].settings->cad_accessory_support;
#if defined(USBPDCORE_VPD)
    handle->flag_vpd_src = h_phy_hw_if[port_num].settings->CAD_VPDSupport;
#endif /* USBPDCORE_VPD */
  }
  else
#endif /* USBPD_DRP */
  {
#if defined(USBPD_SRC)
    /* If default role is source */
    if (USBPD_PORT_PWR_ROLE_SRC == h_phy_hw_if[port_num].settings->pe_default_role)
    {
      /* Set current state machine to SRC state machine */
      handle->state_machine_p = CAD_StateMachine_SRC;
      handle->flag_accessory_src = h_phy_hw_if[port_num].settings->cad_accessory_support;
#if defined(USBPDCORE_VPD)
      handle->flag_vpd_src = h_phy_hw_if[port_num].settings->CAD_VPDSupport;
#endif /* USBPDCORE_VPD */
    }
    else
#endif /* USBPD_SRC */
    {
#if defined(USBPD_SNK)
      /* Set current state machine to SNK state machine */
      handle->state_machine_p = CAD_StateMachine_SNK;
      handle->flag_accessory_snk = h_phy_hw_if[port_num].settings->cad_accessory_support;
#if defined(USBPDCORE_VPD)
      handle->flag_vpd_snk = h_phy_hw_if[port_num].settings->vpd_support;
#endif /* USBPDCORE_VPD */
#endif /* USBPD_SNK */
    }
  }
#else  /* USBPDCORE_LIB_NO_PD */
#if defined(USBPD_SRC)
  /* If default role is source */
  if (USBPD_PORT_PWR_ROLE_SRC == h_phy_hw_if[port_num].settings->pe_default_role)
  {
    /* Set current state machine to SRC state machine */
    handle->state_machine_p = CAD_StateMachine_SRC;
#if defined(USBPDCORE_VPD)
    handle->flag_vpd_src = h_phy_hw_if[port_num].settings->CAD_VPDSupport;
#endif /* USBPDCORE_VPD */
  }
  else
#endif /* USBPD_SRC */
  {
#if defined(USBPD_SNK)
    /* Set current state machine to SNK state machine */
    handle->state_machine_p = CAD_StateMachine_SNK;
#if defined(USBPDCORE_VPD)
    handle->flag_vpd_snk = h_phy_hw_if[port_num].settings->CAD_VPDSupport;
#endif /* USBPDCORE_VPD */
#endif /* USBPD_SNK */
  }
#endif  /* USBPDCORE_LIB_NO_PD */

  return ret;
}

/**
  * @brief  Function to deinitialize the cable detection state machine.
  * @param  port_num Port number.
  */
void USBPD_CAD_HW_IF_DeInit(uint8_t port_num)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];

  USBPD_TRACE_DEBUG_PRINTF(port_num, "USBPD_CAD_HW_IF_DeInit");

  /* Signal detachment */
  USBPD_PHY_HW_IF_SignalDetachment(port_num);

  /* Deinit power */
  USBPD_PWR_DeInit(port_num);

#if defined(_LOW_POWER)
  HAL_UCPD_DisableWakeupMode(h_phy_hw_if[port_num].hucpd);
#endif /* _LOW_POWER */

  /* Start UCPD */
  (void)HAL_UCPD_Stop(h_phy_hw_if[port_num].hucpd);

  /* Reset handle */
  *handle = CAD_HW_IF_HANDLE_RESET_VALUE;

  /* Register CAD wake up callback */
  h_phy_hw_if[port_num].USBPD_CAD_WakeUp = NULL;

  /* Reset HW handle */
  h_phy_hw_if[port_num].hucpd = NULL;
}

/**
  * @brief  Function to force CAD state machine into error recovery state.
  * @param  port_num Index of current used port.
  */
void USBPD_CAD_HW_IF_Enter_ErrorRecovery(uint8_t port_num)
{
  USBPD_TRACE_DEBUG_PRINTF(port_num, "USBPD_CAD_HW_IF_Enter_ErrorRecovery");
  /* Remove the ucpd resistor */
  USBPD_PHY_HW_IF_EnterErrorRecovery(port_num);
  /* Set the error recovery flag to allow the stack to switch into errorRecovery Flag */
  h_cad_hw_if[port_num].flag_error_recovery = USBPD_TRUE;
  /* Wake up CAD task */
  h_phy_hw_if[port_num].USBPD_CAD_WakeUp();
}

#if defined(USBPDCORE_DRP) || defined(USBPDCORE_SRC)
/**
  * @brief  Function to force the value of the RP resistor.
  * @param  port_num Index of current used port.
  * @param  rp_val RP value to set in devices based on @ref usbpd_cad_rp_value_t.
  * @note   Must be called only if you want change the settings value.
  * @retval 0 Success else error.
  */
uint32_t USBPD_CAD_HW_IF_Set_ResistorRp(uint8_t port_num, usbpd_cad_rp_value_t rp_val)
{
  /* Update the information about the default resistor value presented in detach mode */
  h_phy_hw_if[port_num].params->rp_resistor = (uint32_t)rp_val;

  /* Inform state machine about a resistor update */
  h_cad_hw_if[port_num].flag_resistor_update = USBPD_TRUE;

  /* Wake up CAD task */
  h_phy_hw_if[port_num].USBPD_CAD_WakeUp();

  return 0U;
}
#endif /* USBPDCORE_DRP || USBPDCORE_SRC */

/**
  * @brief  Assert Rp resistors: connect the Rp resistors on the CC lines.
  * @param  port_num Port number.
  */
void USBPD_CAD_HW_IF_AssertRp(uint8_t port_num)
{
  USBPD_PHY_HW_IF_AssertRp(port_num);
}

/**
  * @brief  Assert Rd resistors: connect the Rd resistors on the CC line.
  * @param  port_num Port number.
  */
void USBPD_CAD_HW_IF_AssertRd(uint8_t port_num)
{
  USBPD_PHY_HW_IF_AssertRd(port_num);
}

#if !defined(USBPDCORE_LIB_NO_PD)
/**
  * @brief  Main CAD state machine.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
uint32_t USBPD_CAD_HW_IF_StateMachine(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  USBPD_TRACE_DEBUG_PRINTF(port_num, "USBPD_CAD_HW_IF_StateMachine state=%d", handle->current_state);

  /* Set by default event to none */
  *p_event = USBPD_CAD_EVENT_NONE;

  /* If a swap is on going, return default timing */
  if (USBPD_TRUE == h_phy_hw_if[port_num].params->pe_flag_swap_ongoing)
  {
    return _timing;
  }

  if (handle->flag_error_recovery == USBPD_TRUE)
  {
    /* Force the state error recovery */
    handle->flag_error_recovery = USBPD_FALSE;
    handle->current_state = USBPD_CAD_STATE_ERRORRECOVERY;

    USBPD_TRACE_Add(USBPD_TRACE_CAD_LOW, port_num, (uint8_t)handle->current_state, NULL, 0U);
  }

  switch ((usbpd_cad_state_t)handle->current_state)
  {
    case USBPD_CAD_STATE_RESET:
    {
#if !defined(_LOW_POWER)
      /* Enable TypeCEvents Interrupts */
      (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
      (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#elif defined(_LOW_POWER)
      /* If port role is source */
      if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
      {
        /* Enable TypeCEvents Interrupts */
        (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
        (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
      }
#endif /* !_LOW_POWER */

      /* Enable IRQ */
      if (port_num == USBPD_PORT_0)
      {
        usbpd_it_0_enable();
      }
#if (USBPD_PORT_COUNT > 1U)
      else
      {
        usbpd_it_1_enable();
      }
#endif /* (USBPD_PORT_COUNT > 1U) */

#if defined(USBPD_DRP) || defined(_ACCESSORY_SNK)
      handle->accessory_toggle_start_time = HAL_GetTick();
#endif /* USBPD_DRP || _ACCESSORY_SNK */
      handle->current_state = USBPD_CAD_STATE_DETACHED;
      break;
    }

    case USBPD_CAD_STATE_ERRORRECOVERY :
    {
      /* Remove the resistor */
      /* Enter recovery = Switch to SRC with no resistor */
      USBPD_PHY_HW_IF_EnterErrorRecovery(port_num);

      /* Forward detach event to DPM */
      h_phy_hw_if[port_num].active_cc = CCNONE;
      *p_cc_pin = CCNONE;
      handle->cc = (uint32_t)CCNONE;
      *p_event = USBPD_CAD_EVENT_DETACHED;

      /* Start tErrorRecovery timeout */
      handle->debounce_start_time = HAL_GetTick();
      _timing = CAD_TERROR_RECOVERY_TIME;
      handle->current_state = USBPD_CAD_STATE_ERRORRECOVERY_EXIT;
      break;
    }

    case USBPD_CAD_STATE_ERRORRECOVERY_EXIT :
    {
      if ((HAL_GetTick() - handle->debounce_start_time) >  CAD_TERROR_RECOVERY_TIME)
      {
        /* Reconfigure the port
        port source  to src
        port snk     to snk
        port drp     to src   */

#if defined(USBPD_SRC) || defined(USBPD_DRP)
        if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
        {
          USBPD_PHY_HW_IF_AssertRp(port_num);
        }
#endif /* USBPD_SRC || USBPD_DRP */
#if defined(USBPD_DRP)
        else
#endif /* USBPD_DRP */
#if defined(USBPD_SNK) || defined(USBPD_DRP)
        {
          USBPD_PHY_HW_IF_AssertRd(port_num);
        }
#endif /* USBPD_SNK || USBPD_DRP */
        /* Switch to state detach */
#if defined(USBPD_DRP) || defined(_ACCESSORY_SNK)
        handle->accessory_toggle_start_time = HAL_GetTick();
#endif /* USBPD_DRP || _ACCESSORY_SNK */
        handle->current_state = USBPD_CAD_STATE_DETACHED;
      }
      break;
    }

    default:
    {
      /* Call the state machine corresponding to the port SNK or SRC or DRP */
      _timing = handle->state_machine_p(port_num, p_event, p_cc_pin);
      break;
    }
  }

#if defined(USBPD_TRACE)
  if (handle->current_state != handle->previous_state)
  {
    handle->previous_state = handle->current_state;
    USBPD_TRACE_Add(USBPD_TRACE_CAD_LOW, port_num, (uint8_t)handle->current_state, NULL, 0U);
#if defined(CAD_DEBUG_TRACE)
    if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
    {
      USBPD_TRACE_Add(USBPD_TRACE_CAD_LOW, port_num, (uint8_t)(0xC0U | handle->current_hw_condition), NULL, 0U);
    }
    else
    {
      USBPD_TRACE_Add(USBPD_TRACE_CAD_LOW, port_num,
                      (uint8_t)(0x80U | (h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach << 4U) |
                                handle->current_hw_condition), NULL, 0U);
    }
#endif /* CAD_DEBUG_TRACE */
  }
#endif /* USBPD_TRACE */

  return _timing;
}

#else /* USBPDCORE_LIB_NO_PD */
/**
  * @brief  Main CAD state machine.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
uint32_t USBPD_CAD_HW_IF_StateMachine(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  USBPD_TRACE_DEBUG_PRINTF(port_num, "USBPD_CAD_HW_IF_StateMachine state=%d", handle->current_state);

  /* Set by default event to none */
  *p_event = USBPD_CAD_EVENT_NONE;

  switch ((usbpd_cad_state_t)handle->current_state)
  {
    case USBPD_CAD_STATE_RESET:
    {
#if !defined(_LOW_POWER)
      /* Enable TypeCEvents Interrupts */
      (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
      (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#else
      /* If port role is source */
      if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
      {
        /* Enable TypeCEvents Interrupts */
        (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
        (void)HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
      }
#endif /* !_LOW_POWER */

      /* Enable IRQ */
      if (port_num == USBPD_PORT_0)
      {
        usbpd_it_0_enable();
      }
#if (USBPD_PORT_COUNT > 1U)
      else
      {
        usbpd_it_1_enable();
      }
#endif /* (USBPD_PORT_COUNT > 1U) */

#if defined(USBPD_DRP) || defined(_ACCESSORY_SNK)
      handle->accessory_toggle_start_time = HAL_GetTick();
#endif /* USBPD_DRP || _ACCESSORY_SNK */
      handle->current_state = USBPD_CAD_STATE_DETACHED;
      break;
    }

    default:
    {
      /* Call the state machine corresponding to the port SNK or SRC or DRP */
      _timing = handle->state_machine_p(port_num, p_event, p_cc_pin);
      break;
    }
  }

#if defined(USBPD_TRACE)
  if (handle->current_state != handle->previous_state)
  {
    handle->previous_state = handle->current_state;
    USBPD_TRACE_Add(USBPD_TRACE_CAD_LOW, port_num, (uint8_t)handle->current_state, NULL, 0U);
#if defined(CAD_DEBUG_TRACE)
    if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
    {
      USBPD_TRACE_Add(USBPD_TRACE_CAD_LOW, port_num, (uint8_t)(0xC0U | handle->current_hw_condition), NULL, 0U);
    }
    else
    {
      USBPD_TRACE_Add(USBPD_TRACE_CAD_LOW, port_num,
                      (uint8_t)(0x80U
                                | (h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach << 4U)
                                | handle->current_hw_condition), NULL, 0U);
    }
#endif /* CAD_DEBUG_TRACE */
  }
#endif /* USBPD_TRACE */

  return _timing;
}
#endif /* !USBPDCORE_LIB_NO_PD */

/**
  * @}
  */

/** @addtogroup USBPD_CAD_HW_IF_Private_Functions USBPD_CAD_HW_IF Private Functions
  * @{
  */

/**
  * @brief  VBUS detect callback.
  * @param  port_num Port number.
  * @param  vbus_connection_status VBUS connection status, of type @ref usbpd_pwr_hw_if_vbus_connection_status_t.
  */
static void CAD_HW_IF_VBUSDetectCallback(uint8_t port_num,
                                         usbpd_pwr_vbus_connection_status_t vbus_connection_status)
{
  /* If VBUS is connected */
  if (vbus_connection_status == USBPD_PWR_VBUS_CONNECTED)
  {
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: CAD_HW_IF_VBUSDetectCallback VBUS_CONNECTED");
  }
  else
  {
    USBPD_TRACE_DEBUG_PRINTF(port_num, "Callback: CAD_HW_IF_VBUSDetectCallback VBUS_NOT_CONNECTED");

    /* USBPD_PWR_VBUS_NOT_CONNECTED indications could be caused by false OCP/OVP errors detected at part drivers level.
       If reported here, it is assumed that it might be possible to recover from error.
       If error could not be recovered, or is assumed to be related to a true safety issue, it will not be notified
       by part drivers */
#if defined(USBPDCORE_LIB_NO_PD)
    usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];

    /* USBPD_PWR_VBUS_NOT_CONNECTED indication management in case of NO_PD configuration */
    if ((usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role == USBPD_PORT_PWR_ROLE_SRC)
    {
      /* Current role is SRC when USBPD_PWR_VBUS_NOT_CONNECTED signal is received */
      USBPD_PHY_HW_IF_SignalDetachment(port_num);
      handle->flag_debounce = USBPD_FALSE;
      handle->current_state = USBPD_CAD_STATE_DETACH_SRC;
    }
    else
    {
      /* Current role is SNK when USBPD_PWR_VBUS_NOT_CONNECTED signal is received */
      USBPD_PHY_HW_IF_SignalDetachment(port_num);
      handle->current_hw_condition = (uint32_t)USBPD_CAD_HW_COND_Detachment;
      handle->current_state = USBPD_CAD_STATE_DETACHED;
    }
#else
    /* USBPD_PWR_VBUS_NOT_CONNECTED indication management: Error has to be handled through a Detach/Attach procedure.
       handled using ErrorRecovery mechanism */
    USBPD_CAD_HW_IF_Enter_ErrorRecovery(port_num);
#endif /* USBPDCORE_LIB_NO_PD */
  }
}

#if defined(USBPD_DRP) || defined(USBPD_SNK)
/**
  * @brief  Check CCx HW condition.
  * @param  port_num Port number.
  */
static void CAD_Check_HW_SNK(uint8_t port_num)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  hal_ucpd_vstate_t cc1_vstate;
  hal_ucpd_vstate_t cc2_vstate;

  /*
  ----------------------------------------------------------------------------
  | ANAMODE   |  ANASUBMODE[1:0]  |  Notes      |  TYPEC_VSTATE_CCx[1:0]      |
  |           |                   |             |  00  |  01  |  10  |  11    |
  ----------------------------------------------------------------------------
  | 0: Source | 00: Disabled      |Disabled N/A |         NA                  |
  |           | 01: Default USB Rp|             |vRaDef|vRdDef|vOPENDef|      |
  |           | 10: 1.5A Rp       |             |vRa1.5|vRd1.5|vOPEN1.5| NA   |
  |           | 11: 3.0A Rp       |             |vRa3.0|vRd3.0|vOPEN3.0| NA   |
  -----------------------------------------------------------------------------
  | 1: Sink   |                   |             |xx vRa|vRdUSB| vRd1.5 |vRd3.0|
  -----------------------------------------------------------------------------
  */

#if defined(_LOW_POWER)
  /* Enable type C state machine */
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

  for (int32_t index = 0; index < USBPD_DELAY_READ_CC_STATUS; index++)
  {
    __DSB();
  };

  /* Read the CC lines */
  cc1_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  cc2_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

  /* Disable the C state machine */
  HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

#else
  cc1_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  cc2_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#endif /* _LOW_POWER */

  handle->cc  = (uint32_t)CCNONE;
  handle->current_hw_condition     = (uint32_t)USBPD_CAD_HW_COND_Detachment;

  if ((cc1_vstate != HAL_UCPD_VSTATE_SNK_VRA) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRA))
  {
    handle->current_hw_condition = (uint32_t)USBPD_CAD_HW_COND_Attachment;
    handle->cc = (uint32_t)CC1;
    switch (cc1_vstate)
    {
      case HAL_UCPD_VSTATE_SNK_VRPDFLT:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_VRDUSB;
        break;
      case HAL_UCPD_VSTATE_SNK_VRP15A:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_VRD1A5;
        break;
      case HAL_UCPD_VSTATE_SNK_VRP30A:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_VRD3A0;
        break;
      default:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_UNDEF;
        break;
    }
  }

  if ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRA) && (cc2_vstate != HAL_UCPD_VSTATE_SNK_VRA))
  {
    handle->current_hw_condition = (uint32_t)USBPD_CAD_HW_COND_Attachment;
    handle->cc = (uint32_t)CC2;
    switch (cc2_vstate)
    {
      case HAL_UCPD_VSTATE_SNK_VRPDFLT:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_VRDUSB;
        break;
      case HAL_UCPD_VSTATE_SNK_VRP15A:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_VRD1A5;
        break;
      case HAL_UCPD_VSTATE_SNK_VRP30A:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_VRD3A0;
        break;
      default:
        h_phy_hw_if[port_num].params->rp_resistor_exposed_at_attach = (uint32_t)USBPD_CAD_SOURCE_CURRENT_ADV_UNDEF;
        break;
    }
  }
}
#endif /* USBPD_DRP || USBPD_SNK */

#if defined(USBPD_DRP) || defined(USBPD_SRC)
/**
  * @brief  Check CCx HW condition.
  * @param  port_num Port number.
  */
static void CAD_Check_HW_SRC(uint8_t port_num)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  hal_ucpd_vstate_t cc1_vstate;
  hal_ucpd_vstate_t cc2_vstate;
  uint32_t          cc1_value;
  uint32_t          cc2_value;

  /*
  ----------------------------------------------------------------------------
  | ANAMODE   |  ANASUBMODE[1:0]  |  Notes      |  TYPEC_VSTATE_CCx[1:0]      |
  |           |                   |             |  00  |  01  |  10  |  11    |
  ----------------------------------------------------------------------------
  | 0: Source | 00: Disabled      |Disabled N/A |         NA                  |
  |           | 01: Default USB Rp|             |vRaDef|vRdDef|vOPENDef|      |
  |           | 10: 1.5A Rp       |             |vRa1.5|vRd1.5|vOPEN1.5| NA   |
  |           | 11: 3.0A Rp       |             |vRa3.0|vRd3.0|vOPEN3.0| NA   |
  -----------------------------------------------------------------------------
  | 1: Sink   |                   |             |xx vRa|vRdUSB| vRd1.5 |vRd3.0|
  -----------------------------------------------------------------------------
  */
#if defined(_LOW_POWER)
  /* Enable type C state machine */
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

  for (int32_t index = 0U; index < USBPD_DELAY_READ_CC_STATUS; index++)
  {
    __DSB();
  };

  /* Read the CC lines */
  cc1_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  cc2_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

  /* Disable the C state machine */
  HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#else
  cc1_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  cc2_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#endif /* _LOW_POWER */

  /* Define the connection type for each possible CC line state */
  const usbpd_cc_line_t table_cc[] =
  {
    CCNONE,  CC2,       CC2,
    CC1,     CCNONE,    CC1,
    CC1,     CC2,       CCNONE
  };

  /* Define the hardware condition for each possible CC line state */
  const usbpd_cad_hw_condition_t table_current_hw_condition[] =
  {
    USBPD_CAD_HW_COND_AudioAdapter_Attachment,    USBPD_CAD_HW_COND_PwrCable_Sink_Attachment,
    USBPD_CAD_HW_COND_PwrCable_NoSink_Attachment, USBPD_CAD_HW_COND_PwrCable_Sink_Attachment,
    USBPD_CAD_HW_COND_Debug_Attachment,           USBPD_CAD_HW_COND_Attachment,
    USBPD_CAD_HW_COND_PwrCable_NoSink_Attachment, USBPD_CAD_HW_COND_Attachment,
    USBPD_CAD_HW_COND_Detachment
  };

  switch (cc1_vstate)
  {
    case HAL_UCPD_VSTATE_SRC_VRA:
      cc1_value = 0U;
      break;
    case HAL_UCPD_VSTATE_SRC_VRD:
      cc1_value = 1U;
      break;
    case HAL_UCPD_VSTATE_SRC_VOPEN:
      cc1_value = 2U;
      break;
    case HAL_UCPD_VSTATE_SRC_INVALID:
      cc1_value = 3U;
      break;
    default:
      cc1_value = 0U;
      break;
  }

  switch (cc2_vstate)
  {
    case HAL_UCPD_VSTATE_SRC_VRA:
      cc2_value = 0U;
      break;
    case HAL_UCPD_VSTATE_SRC_VRD:
      cc2_value = 1U;
      break;
    case HAL_UCPD_VSTATE_SRC_VOPEN:
      cc2_value = 2U;
      break;
    case HAL_UCPD_VSTATE_SRC_INVALID:
      cc2_value = 3U;
      break;
    default:
      cc2_value = 0U;
      break;
  }

  /* Calculate index based on CC1 and CC2 values */
  uint8_t index = (uint8_t)((cc1_value * 3U) + cc2_value);

  if (index < 9U)
  {
    handle->cc                   = (uint32_t)table_cc[index];
    handle->current_hw_condition = (uint32_t)table_current_hw_condition[index];
  }
}
#endif /* USBPD_DRP || USBPD_SRC */

#if defined(USBPD_DRP) || defined(USBPD_SNK)
/**
  * @brief  Manage the detached state for sink role.
  * @param  port_num Port number.
  * @retval Timeout value.
  */
static uint32_t CAD_ManageStateDetached_SNK(uint8_t port_num)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  CAD_Check_HW_SNK(port_num);
  /* Change the status on the basis of the HW event given by CAD_Check_HW() */
  if ((usbpd_cad_hw_condition_t)handle->current_hw_condition == USBPD_CAD_HW_COND_Detachment)
  {
#if defined(_LOW_POWER)
    _timing = CAD_DETACH_POLLING;
#else
    _timing = CAD_INFINITE_TIME;
#endif /* _LOW_POWER */

#if defined(_ACCESSORY_SNK)
    if ((USBPD_TRUE == handle->flag_accessory_snk)
#if defined(USBPDCORE_VPD)
        || (USBPD_TRUE == handle->flag_vpd_snk)
#endif /* USBPDCORE_VPD */
       )
    {
      /* A Sink with Accessory support must transition to Unattached.Accessory within tDRPTransition
      after the state of both the CC1 and CC2 pins is SNK.Open for tDRP - dcSRC.DRP * tDRP, or if directed.*/
      if ((HAL_GetTick() - handle->accessory_toggle_start_time) > CAD_ACCESSORY_TOGGLE)
      {
        handle->current_state = USBPD_CAD_STATE_UNATTACHED_ACCESSORY;
        handle->accessory_toggle_start_time = HAL_GetTick();
        USBPD_PHY_HW_IF_AssertRp(port_num);
      }
      _timing = CAD_DEFAULT_TIME;
    }
#endif /* _ACCESSORY_SNK */
  }
  else
  {
    /* Get the time of this event */
    handle->debounce_start_time = HAL_GetTick();
    handle->current_state = USBPD_CAD_STATE_ATTACHED_WAIT;

    /* Let time to internal state machine update */
    HAL_Delay(1U);

    CAD_Check_HW_SNK(port_num);

    if ((usbpd_cad_hw_condition_t)handle->current_hw_condition == USBPD_CAD_HW_COND_Detachment)
    {
      handle->current_state = USBPD_CAD_STATE_DETACHED;
    }
    else
    {
      (void)USBPD_PWR_VBUSInit(port_num);
    }
  }
  return _timing;
}
#endif /* USBPD_DRP || USBPD_SNK */

#if defined(USBPD_SRC) || defined(USBPD_DRP)
/**
  * @brief  Manage the detached state for source role.
  * @param  port_num Port number.
  * @retval Timeout value.
  */
static uint32_t CAD_ManageStateDetached_SRC(uint8_t port_num)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  if (handle->flag_resistor_update == USBPD_TRUE)
  {
    /* Update the resistor value */
    USBPD_PHY_HW_IF_AssertRp(port_num);
    handle->flag_resistor_update = USBPD_FALSE;

    /* Let time to internal state machine update */
    HAL_Delay(1U);
  }

  CAD_Check_HW_SRC(port_num);
  /* Change the status on the basis of the HW event given by CAD_Check_HW() */
  if ((usbpd_cad_hw_condition_t)handle->current_hw_condition == USBPD_CAD_HW_COND_Detachment)
  {
#if defined(_LOW_POWER)
    /* Value returned for a SRC */
    _timing = CAD_DETACH_POLLING;
#else
    _timing = CAD_INFINITE_TIME;
#endif /* _LOW_POWER */
  }
  else
  {
    if ((usbpd_cad_hw_condition_t)handle->current_hw_condition == USBPD_CAD_HW_COND_PwrCable_NoSink_Attachment)
    {
      handle->current_state = USBPD_CAD_STATE_EMC;
    }
    else
    {
      /* Get the time of this event */
      handle->debounce_start_time = HAL_GetTick();
      handle->current_state = USBPD_CAD_STATE_ATTACHED_WAIT;

      (void)USBPD_PWR_VBUSInit(port_num);
    }
  }
  return _timing;
}
#endif /* USBPD_SRC || USBPD_DRP */

#if defined(USBPD_DRP)
/**
  * @brief  Manage the detached state for dual role.
  * @param  port_num Port number.
  * @retval Timeout value.
  */
static uint32_t CAD_ManageStateDetached_DRP(uint8_t port_num)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
  {
    (void)CAD_ManageStateDetached_SRC(port_num);
  }
  else
  {
    (void)CAD_ManageStateDetached_SNK(port_num);
  }

  /* Manage the toggle */
  if ((usbpd_cad_hw_condition_t)handle->current_hw_condition == USBPD_CAD_HW_COND_Detachment)
  {
    switch ((usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
    {
      case USBPD_PORT_PWR_ROLE_SRC :
        if ((HAL_GetTick() - handle->accessory_toggle_start_time) >
            h_phy_hw_if[port_num].settings->cad_src_toggle_time_ms)
        {
          handle->accessory_toggle_start_time = HAL_GetTick();
          h_phy_hw_if[port_num].params->pe_power_role = (uint32_t)USBPD_PORT_PWR_ROLE_SNK;
          h_phy_hw_if[port_num].params->pe_data_role = (uint32_t)USBPD_PORTDATAROLE_UFP;
          _timing = h_phy_hw_if[port_num].settings->cad_snk_toggle_time_ms;
          USBPD_PHY_HW_IF_AssertRd(port_num);
        }
        break;
      case USBPD_PORT_PWR_ROLE_SNK :
        if ((HAL_GetTick() - handle->accessory_toggle_start_time) >
            h_phy_hw_if[port_num].settings->cad_snk_toggle_time_ms)
        {
          handle->accessory_toggle_start_time = HAL_GetTick();
          h_phy_hw_if[port_num].params->pe_power_role = (uint32_t)USBPD_PORT_PWR_ROLE_SRC;
          h_phy_hw_if[port_num].params->pe_data_role = (uint32_t)USBPD_PORTDATAROLE_DFP;
          _timing = h_phy_hw_if[port_num].settings->cad_src_toggle_time_ms;
          USBPD_PHY_HW_IF_AssertRp(port_num);
        }
        break;
      default:
        break;
    }
  }

  return _timing;
}
#endif /* USBPD_DRP */

#if defined(USBPD_DRP) || defined(USBPD_SRC)
/**
  * @brief  Manage the attached wait state for source role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
static uint32_t CAD_ManageStateAttachedWait_SRC(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  /* Evaluate elapsed time in Attach_Wait state */
  uint32_t CAD_tDebounce = HAL_GetTick() - handle->debounce_start_time;
  CAD_Check_HW_SRC(port_num);

  if (((usbpd_cad_hw_condition_t)handle->current_hw_condition != USBPD_CAD_HW_COND_Detachment)
      && ((usbpd_cad_hw_condition_t)handle->current_hw_condition != USBPD_CAD_HW_COND_PwrCable_NoSink_Attachment))
  {
    if (USBPD_FALSE == USBPD_PWR_GetVBUSStatus(port_num, USBPD_PWR_BELOW_VSAFE0V))
    {
      /* Reset the timing because VBUS threshold not yet reach */
      handle->debounce_start_time = HAL_GetTick();
      return CAD_TCCDEBOUNCE_THRESHOLD;
    }

    /* Check tCCDebounce */
    if (CAD_tDebounce > CAD_TCCDEBOUNCE_THRESHOLD)
    {
      switch ((usbpd_cad_hw_condition_t)handle->current_hw_condition)
      {
        case USBPD_CAD_HW_COND_Attachment:
          USBPD_PHY_HW_IF_SignalAttachement(port_num, (usbpd_cc_line_t)handle->cc);
          handle->current_state = USBPD_CAD_STATE_ATTACHED;
          *p_event = USBPD_CAD_EVENT_ATTACHED;
          *p_cc_pin = (usbpd_cc_line_t)handle->cc;
          break;

        case USBPD_CAD_HW_COND_PwrCable_Sink_Attachment:
          USBPD_PHY_HW_IF_SignalAttachement(port_num, (usbpd_cc_line_t)handle->cc);
          handle->current_state = USBPD_CAD_STATE_ATTEMC;
          *p_event = USBPD_CAD_EVENT_ATTEMC;
          break;

        case USBPD_CAD_HW_COND_PwrCable_NoSink_Attachment:
          USBPD_PWR_VBUSDeInit(port_num);
          handle->current_state = USBPD_CAD_STATE_EMC;
          *p_event = USBPD_CAD_EVENT_EMC;
          *p_cc_pin = (usbpd_cc_line_t)handle->cc;
          break;

#if defined(_ACCESSORY_SRC)
        case USBPD_CAD_HW_COND_Debug_Attachment:
          handle->current_state = USBPD_CAD_STATE_DEBUG;
          *p_event = USBPD_CAD_EVENT_DEBUG;
          break;

        case USBPD_CAD_HW_COND_AudioAdapter_Attachment:
          handle->current_state = USBPD_CAD_STATE_ACCESSORY;
          *p_event = USBPD_CAD_EVENT_ACCESSORY;
          break;
#endif /* _ACCESSORY_SRC */

        case USBPD_CAD_HW_COND_Detachment:
        default:
#if !defined(_ACCESSORY_SRC)
          handle->current_state = USBPD_CAD_STATE_DETACH_SRC;
#endif /* _ACCESSORY_SRC */
          break;
      }

      *p_cc_pin = (usbpd_cc_line_t)handle->cc;
      _timing = CAD_DEFAULT_TIME;
    }
    /* Reset the flag for CAD_tDebounce */
    handle->flag_debounce = USBPD_FALSE;
  }
  else /* CAD_HW_Condition[port_num] = USBPD_CAD_HW_COND_Detachment */
  {
    /* Start counting of CAD_tDebounce */
    if (USBPD_FALSE == handle->flag_debounce)
    {
      handle->debounce_start_time  = HAL_GetTick();
      handle->flag_debounce   = USBPD_TRUE;
      _timing                       = CAD_TSRCDISCONNECT_THRESHOLD;
    }
    else /* CAD_tDebounce already running */
    {
      /* Evaluate CAD_tDebounce */
      CAD_tDebounce = HAL_GetTick() - handle->debounce_start_time;
      if (CAD_tDebounce > CAD_TSRCDISCONNECT_THRESHOLD)
      {
        handle->flag_debounce = USBPD_FALSE;
        handle->current_state             = USBPD_CAD_STATE_DETACH_SRC;
        _timing = 0U;
      }
    }
  }
  return _timing;
}
#endif /* USBPD_DRP || USBPD_SRC || (_ACCESSORY && USBPD_SNK) */

#if defined(USBPD_DRP) || defined(USBPD_SRC)
/**
  * @brief  Manage the EMC state for source role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
static uint32_t CAD_ManageStateEMC(uint8_t port_num, const usbpd_cad_event_t *p_event,
                                   const usbpd_cc_line_t *p_cc_pin)
{
  uint32_t _timing = CAD_INFINITE_TIME;
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  STM32_UNUSED(p_cc_pin);
  STM32_UNUSED(p_event);

  CAD_Check_HW_SRC(port_num);
  /* Change the status on the basis of the HW event given by CAD_Check_HW() */
  switch ((usbpd_cad_hw_condition_t)handle->current_hw_condition)
  {
    case USBPD_CAD_HW_COND_Detachment :
      handle->current_state = USBPD_CAD_STATE_SWITCH_TO_SNK;
      _timing = 1U;
      break;
    case USBPD_CAD_HW_COND_PwrCable_Sink_Attachment:
    case USBPD_CAD_HW_COND_Attachment :
      handle->current_state = USBPD_CAD_STATE_ATTACHED_WAIT;
      handle->debounce_start_time = HAL_GetTick() - 5U;  /* This is only to check cable presence */
      (void)USBPD_PWR_VBUSInit(port_num);
      _timing = CAD_DEFAULT_TIME;
      break;
    case USBPD_CAD_HW_COND_PwrCable_NoSink_Attachment:
    default :
      /* Nothing to do still the same status */
#if defined(USBPD_DRP)
      if (USBPD_TRUE == h_phy_hw_if[port_num].settings->cad_role_toggle_support)
      {
        if ((HAL_GetTick() - handle->accessory_toggle_start_time) >
            h_phy_hw_if[port_num].settings->cad_src_toggle_time_ms)
        {
          handle->current_state = USBPD_CAD_STATE_SWITCH_TO_SNK;
        }
        _timing = 0U;
      }
#else
      _timing = CAD_DEFAULT_TIME;
#endif /* USBPD_DRP */
      break;
  }
  return _timing;
}
#endif /* USBPD_DRP || USBPD_SRC */

#if defined(USBPD_DRP)
/**
  * @brief  Manage the attached state for dual role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Time in ms before next CAD event must be triggered.
  */
static uint32_t CAD_ManageStateAttached_DRP(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
  {
    return CAD_ManageStateAttached_SRC(port_num, p_event, p_cc_pin);
  }
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_ManageStateAttached_SNK(port_num, p_event, p_cc_pin);

  /* Toggle management */
  if ((usbpd_cad_hw_condition_t)handle->current_hw_condition == USBPD_CAD_HW_COND_Detachment)
  {
    handle->current_state = USBPD_CAD_STATE_SWITCH_TO_SRC;
    _timing = 0U;
  }
  return _timing;
}
#endif /* USBPD_DRP */

#if defined(USBPD_DRP) || (defined(_ACCESSORY) && defined(USBPD_SNK))
/**
  * @brief  Manage the attached wait state for dual role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
static uint32_t CAD_ManageStateAttachedWait_DRP(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  if (USBPD_PORT_PWR_ROLE_SRC == (usbpd_port_pwr_role_t)h_phy_hw_if[port_num].params->pe_power_role)
  {
    return CAD_ManageStateAttachedWait_SRC(port_num, p_event, p_cc_pin);
  }
  return CAD_ManageStateAttachedWait_SNK(port_num, p_event, p_cc_pin);
}
#endif /* USBPD_DRP || (_ACCESSORY && USBPD_SNK) */

#if defined(USBPD_SRC) || defined(USBPD_DRP)
/**
  * @brief  Manage the attached state for source role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Time in ms before next CAD event must be triggered.
  */
static uint32_t CAD_ManageStateAttached_SRC(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;
  hal_ucpd_vstate_t cc_vstate;

#if defined(_LOW_POWER)
  /* Enable type C state machine */
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

  for (int32_t index = 0U; index < USBPD_DELAY_READ_CC_STATUS; index++)
  {
    __DSB();
  };
#endif /* _LOW_POWER */

  if (h_phy_hw_if[port_num].active_cc == CC1)
  {
    cc_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  }
  else
  {
    cc_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
  }

  /* Check if CC lines is opened or switch to debug accessory */
  if (cc_vstate != HAL_UCPD_VSTATE_SRC_VRD)
  {
    /* Start counting of CAD_tDebounce */
    if (USBPD_FALSE == handle->flag_debounce)
    {
      handle->flag_debounce       = USBPD_TRUE;
      handle->debounce_start_time = HAL_GetTick();
      _timing                      = CAD_TSRCDISCONNECT_THRESHOLD;
    }
    else /* CAD_tDebounce already running */
    {
      /* Evaluate CAD_tDebounce */
      uint32_t CAD_tDebounce = HAL_GetTick() - handle->debounce_start_time;
      if (CAD_tDebounce > CAD_TSRCDISCONNECT_THRESHOLD)
      {
        USBPD_PHY_HW_IF_SignalDetachment(port_num);
#if defined(USBPD_DRP)
        if (USBPD_TRUE == h_phy_hw_if[port_num].settings->cad_role_toggle_support)
        {
          USBPD_PHY_HW_IF_AssertRd(port_num);
        }
#endif /* USBPD_DRP */
        handle->flag_debounce = USBPD_FALSE;
        /* Move inside state DETACH to avoid wrong VCONN level*/
        handle->current_state = USBPD_CAD_STATE_DETACH_SRC;
        *p_event                = USBPD_CAD_EVENT_DETACHED;
        *p_cc_pin                 = CCNONE;
        _timing                = 0U;
      }
    }
  }
  else
  {
    /* Reset tPDDebounce flag*/
    handle->flag_debounce   = USBPD_FALSE;
#if defined(_LOW_POWER)
    _timing = CAD_VBUS_POLLING_TIME;
#else
    _timing = CAD_INFINITE_TIME;
#endif /* _LOW_POWER */
  }

  return _timing;
}
#endif /* USBPD_SRC || USBPD_DRP */

#if defined(USBPD_SNK) || defined(USBPD_DRP)
/**
  * @brief  Manage the attached wait state for sink role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Time in ms before next CAD event must be triggered.
  */
static uint32_t CAD_ManageStateAttachedWait_SNK(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  uint32_t CAD_tDebounce = HAL_GetTick() - handle->debounce_start_time;
  CAD_Check_HW_SNK(port_num);
  if ((usbpd_cad_hw_condition_t)handle->current_hw_condition == USBPD_CAD_HW_COND_Attachment)
  {
    if (CAD_tDebounce > CAD_TCCDEBOUNCE_THRESHOLD)
    {
      if (USBPD_TRUE == USBPD_PWR_GetVBUSStatus(port_num, USBPD_PWR_ABOVE_VSAFE5V)) /* Check if Vbus is on */
      {
        USBPD_PHY_HW_IF_SignalAttachement(port_num, (usbpd_cc_line_t)handle->cc);
        /* Go to attached state */
        handle->current_state = USBPD_CAD_STATE_ATTACHED;
        *p_event = USBPD_CAD_EVENT_ATTACHED;
        *p_cc_pin = (usbpd_cc_line_t)handle->cc;
      }
    }
    handle->flag_debounce = USBPD_FALSE;
  }
  else
  {
    /* Start counting of CAD_tDebounce */
    if (USBPD_FALSE == handle->flag_debounce)
    {
      handle->debounce_start_time = HAL_GetTick();
      handle->flag_debounce = USBPD_TRUE;
      _timing = CAD_TPDDEBOUNCE_THRESHOLD;
    }
    else /* CAD_tDebounce already running */
    {
      /* Evaluate CAD_tDebounce */
      if (((HAL_GetTick() - handle->debounce_start_time) > CAD_TPDDEBOUNCE_THRESHOLD))
      {
        handle->flag_debounce = USBPD_FALSE;
        handle->current_state = USBPD_CAD_STATE_DETACHED;
        USBPD_PWR_VBUSDeInit(port_num);
#if defined(_ACCESSORY_SNK)
        if (USBPD_TRUE ==  handle->flag_accessory_snk)
        {
          handle->accessory_toggle_start_time = HAL_GetTick();
        }
#endif /* _ACCESSORY_SNK */
      }
    }
  }
  return _timing;
}

/**
  * @brief  Manage the attached state for sink role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Time in ms before next CAD event must be triggered.
  */
static uint32_t CAD_ManageStateAttached_SNK(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing;
  hal_ucpd_vstate_t cc_vstate;

#if defined(_LOW_POWER)
  /* Enable type C state machine */
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  HAL_UCPD_EnableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);

  for (int32_t index = 0U; index < USBPD_DELAY_READ_CC_STATUS; index++)
  {
    __DSB();
  };
#endif /* _LOW_POWER */

  if (h_phy_hw_if[port_num].active_cc == CC1)
  {
    cc_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  }
  else
  {
    cc_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
  }

  /* Check if Vbus is below disconnect threshold & Confirm that there is no RP */
  if ((USBPD_TRUE == USBPD_PWR_GetVBUSStatus(port_num, USBPD_PWR_SNKDETACH))
      && (cc_vstate == HAL_UCPD_VSTATE_SNK_VRA))
  {
    USBPD_PHY_HW_IF_SignalDetachment(port_num);
    /* Restart the toggle time */
    handle->current_hw_condition = (uint32_t)USBPD_CAD_HW_COND_Detachment;
    handle->current_state             = USBPD_CAD_STATE_DETACHED;
#if defined(_ACCESSORY_SNK)
    if (USBPD_TRUE ==  handle->flag_accessory_snk)
    {
      handle->accessory_toggle_start_time = HAL_GetTick();
    }
#endif /* _ACCESSORY_SNK */
    *p_event = USBPD_CAD_EVENT_DETACHED;
    *p_cc_pin = CCNONE;
    _timing = 0U;
  }
  else
  {
    _timing = CAD_VBUS_POLLING_TIME;
  }

#if defined(_LOW_POWER)
  /* Disable type C state machine */
  HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  HAL_UCPD_DisableTypeCDetectorCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#endif /* _LOW_POWER */

  return _timing;
}
#endif /* USBPD_SNK || USBPD_DRP */

/**
  * @brief  CAD state machine for sink role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
#if defined(USBPD_SNK)
static uint32_t CAD_StateMachine_SNK(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;
#if defined(_ACCESSORY_SNK)
  hal_ucpd_vstate_t cc1_vstate;
  hal_ucpd_vstate_t cc2_vstate;
#endif /* _ACCESSORY_SNK */

#if defined(_ACCESSORY_SNK)
  cc1_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC1);
  cc2_vstate = HAL_UCPD_GetTypeCVoltageLevelCCx(h_phy_hw_if[port_num].hucpd, HAL_UCPD_LINE_CC2);
#endif /* _ACCESSORY_SNK */

  /* Check CAD STATE */
  switch ((usbpd_cad_state_t)handle->current_state)
  {
    case USBPD_CAD_STATE_DETACHED:
    {
      _timing = CAD_ManageStateDetached_SNK(port_num);
      break;
    }

    case USBPD_CAD_STATE_ATTACHED_WAIT:
      _timing = CAD_ManageStateAttachedWait_SNK(port_num, p_event, p_cc_pin);
      break;

    case USBPD_CAD_STATE_ATTACHED:
      _timing = CAD_ManageStateAttached_SNK(port_num, p_event, p_cc_pin);
      break;

#if defined(_ACCESSORY_SNK)
    case USBPD_CAD_STATE_UNATTACHED_ACCESSORY:
    {
      handle->flag_debounce = USBPD_FALSE;

      if ((USBPD_TRUE == handle->flag_accessory_snk)
          && ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRA)
              && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRA)))
      {
        /* Get the time of this event */
        handle->debounce_start_time = HAL_GetTick();
        handle->current_state = USBPD_CAD_STATE_ATTACHED_ACCESSORY_WAIT;
      }
#if defined(USBPDCORE_VPD)
      else if ((USBPD_TRUE == handle->flag_vpd_snk)
               && (((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRPDFLT) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRA))
                   || ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRA) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRPDFLT))))
      {
        handle->debounce_start_time = HAL_GetTick();
        handle->current_state = USBPD_CAD_STATE_ATTACHED_ACCESSORY_WAIT;
      }
#endif /* USBPDCORE_VPD */
      else if (((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRPDFLT)  && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRA))
               || ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRA) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRPDFLT)))
      {
        if ((HAL_GetTick() - handle->accessory_toggle_start_time) > 200U)
        {
          handle->accessory_toggle_start_time = HAL_GetTick();
          handle->current_state = USBPD_CAD_STATE_DETACHED;
          USBPD_PHY_HW_IF_AssertRd(port_num);
        }
      }
      else
      {
        /* tOnePortToggleConnect between 0 and 80ms */
        if ((HAL_GetTick() - handle->accessory_toggle_start_time) > CAD_ACCESSORY_TOGGLE)
        {
          handle->accessory_toggle_start_time = HAL_GetTick();
          handle->current_state = USBPD_CAD_STATE_DETACHED;
          USBPD_PHY_HW_IF_AssertRd(port_num);
        }
      }
      break;
    }

    case USBPD_CAD_STATE_ATTACHED_ACCESSORY_WAIT :
    {
      handle->flag_debounce = USBPD_FALSE;

      if ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRA) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRA))
      {
        /* Check if the device is still connected after the debounce timing */
        if (HAL_GetTick() - handle->debounce_start_time > CAD_TCCDEBOUNCE_THRESHOLD)
        {
          handle->current_state = USBPD_CAD_STATE_AUDIO_ACCESSORY;
          *p_event = USBPD_CAD_EVENT_ACCESSORY;
        }
      }

#if defined(USBPDCORE_VPD)
      else if ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRA) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRPDFLT))
      {
        if (HAL_GetTick() - handle->debounce_start_time > CAD_TCCDEBOUNCE_THRESHOLD)
        {
          handle->current_state = USBPD_CAD_STATE_POWERED_ACCESSORY;
          *p_event = USPPD_CAD_EVENT_VPD;
          h_phy_hw_if[port_num].params->pe_vpd_status = VPD_UNKNOWN;
          h_phy_hw_if[port_num].params->cad_vpd_status = VPD_NONE;

          handle->cc = (uint32_t)CC2;

          *p_cc_pin = (usbpd_cc_line_t)handle->cc;

          USBPD_PHY_HW_IF_SignalAttachement(port_num, (usbpd_cc_line_t)handle->cc);
        }
      }
      else if ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRPDFLT) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRA))
      {
        if (HAL_GetTick() - handle->debounce_start_time > CAD_TCCDEBOUNCE_THRESHOLD)
        {
          handle->current_state = USBPD_CAD_STATE_POWERED_ACCESSORY;
          *p_event = USPPD_CAD_EVENT_VPD;
          h_phy_hw_if[port_num].params->pe_vpd_status = VPD_UNKNOWN;
          h_phy_hw_if[port_num].params->cad_vpd_status = VPD_NONE;

          handle->cc = (uint32_t)CC1;

          *p_cc_pin = (usbpd_cc_line_t)handle->cc;

          USBPD_PHY_HW_IF_SignalAttachement(port_num, (usbpd_cc_line_t)handle->cc);
        }
      }
#endif /* USBPDCORE_VPD */
      else
      {
        if ((HAL_GetTick() - handle->debounce_start_time) > CAD_TCCDEBOUNCE_THRESHOLD)
        {
          /* Get the time of this event */
          handle->accessory_toggle_start_time = HAL_GetTick();
          handle->current_state = USBPD_CAD_STATE_DETACHED;
          USBPD_PHY_HW_IF_AssertRd(port_num);
        }
      }

      break;
    }

    case USBPD_CAD_STATE_AUDIO_ACCESSORY:
    {
      /* Check if the device is still connected after the debounce timing */
      if ((cc1_vstate != HAL_UCPD_VSTATE_SNK_VRA) && (cc2_vstate != HAL_UCPD_VSTATE_SNK_VRA))
      {
        if (USBPD_FALSE == handle->flag_debounce)
        {
          handle->debounce_start_time = HAL_GetTick();
          handle->flag_debounce = USBPD_TRUE;
          _timing = CAD_TCCDEBOUNCE_THRESHOLD + 49U;
        }
        else
        {
          if ((HAL_GetTick() - handle->debounce_start_time) > (CAD_TCCDEBOUNCE_THRESHOLD + 50U))
          {
            handle->accessory_toggle_start_time = HAL_GetTick();
            handle->flag_debounce = USBPD_FALSE;
            handle->current_state = USBPD_CAD_STATE_DETACHED;
            handle->cc = (uint32_t)CCNONE;
            USBPD_PHY_HW_IF_AssertRd(port_num);
            *p_event = USBPD_CAD_EVENT_DETACHED;
            *p_cc_pin = CCNONE;
          }
        }
      }
      else
      {
        handle->flag_debounce = USBPD_FALSE;
        _timing = CAD_INFINITE_TIME;
      }
      break;
    }

#if defined(USBPDCORE_VPD)
    case USBPD_CAD_STATE_POWERED_ACCESSORY:
    {
      /* If SRC.Open detected on the monitored PIN switch to Unattached.SNK */
      if (((CC1 == (usbpd_cc_line_t)handle->cc) && (cc1_vstate == HAL_UCPD_VSTATE_SNK_VRP15A))
          || ((CC2 == (usbpd_cc_line_t)handle->cc) && (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRP15A)))
      {
        handle->accessory_toggle_start_time = HAL_GetTick();
        handle->current_state = USBPD_CAD_STATE_DETACHED;
        h_phy_hw_if[port_num].params->pe_vpd_status = VPD_NONE;
        *p_event = USBPD_CAD_EVENT_DETACHED;
        USBPD_PHY_HW_IF_AssertRd(port_num);
      }
      else
      {
        /*  if the port is not PD or a SRC it must switch to try.SNK */
        if (VPD_NOPD == h_phy_hw_if[port_num].params->cad_vpd_status)
        {
          handle->accessory_toggle_start_time = HAL_GetTick();
          handle->current_state = USBPD_CAD_STATE_DETACHED;
          *p_event = USBPD_CAD_EVENT_DETACHED;
          USBPD_PHY_HW_IF_AssertRd(port_num);
          h_phy_hw_if[port_num].params->pe_vpd_status = VPD_NONE;
        }
        /* If the port is PD but doesn't enter Alternate mode within tAMETimeout switch to Unsupported.Accessory */
        else if (VPD_FAILED_ENTER_ALTERNATE == h_phy_hw_if[port_num].params->cad_vpd_status)
        {
          handle->current_state = USBPD_CAD_STATE_UNSUPPORTED_ACCESSORY;
        }
        /* If Powered USB DEvice confirmed transition to CTUnattached.SNK */
        else if (VPD_DETECTED == h_phy_hw_if[port_num].params->cad_vpd_status)
        {
          handle->current_state = USBPD_CAD_STATE_CTVPD_UNATTACHED;
        }
      }
      break;
    }

    case USBPD_CAD_STATE_UNSUPPORTED_ACCESSORY:
    {
      /* If SRC.Open detected on the monitored PIN switch to Unattached.SNK */
      if ((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRP15A) || (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRP15A))
      {
        handle->accessory_toggle_start_time = HAL_GetTick();
        handle->current_state = USBPD_CAD_STATE_DETACHED;
        *p_event = USBPD_CAD_EVENT_DETACHED;
        USBPD_PHY_HW_IF_AssertRp(port_num);
      }
      break;
    }

    case USBPD_CAD_STATE_CTVPD_UNATTACHED:
    {
      if (USBPD_TRUE == USBPD_PWR_GetVBUSStatus(port_num, USBPD_PWR_ABOVE_VSAFE5V)) /* Check if Vbus is on */
      {
        /* The charger/SRC has been plug */
        handle->current_state = USBPD_CAD_STATE_CTVPD_ATTACHED;
      }
      else
      {
        /* If VBUS is VSafe0V and CC low for tVPDDetach = 10/20ms */
        if (((cc1_vstate == HAL_UCPD_VSTATE_SNK_VRP15A) || (cc2_vstate == HAL_UCPD_VSTATE_SNK_VRP15A))
            && (USBPD_TRUE == USBPD_PWR_GetVBUSStatus(port_num, USBPD_PWR_BELOW_VSAFE0V)))
        {
          if (USBPD_TRUE == handle->flag_debounce)
          {
            handle->debounce_start_time = HAL_GetTick();
          }
          else
          {
            if (HAL_GetTick() - handle->debounce_start_time > CAD_TVPDDETACH)
            {
              handle->accessory_toggle_start_time = HAL_GetTick();
              handle->current_state = USBPD_CAD_STATE_DETACHED;
              *p_event = USBPD_CAD_EVENT_DETACHED;
              USBPD_PHY_HW_IF_AssertRp(port_num);
            }
          }
        }
        else
        {
          handle->flag_debounce = USBPD_FALSE;
        }
      }
      break;
    }

    case USBPD_CAD_STATE_CTVPD_ATTACHED:
    {
      /* If VBUS removed */
      if (USBPD_TRUE == USBPD_PWR_GetVBUSStatus(port_num, USBPD_PWR_SNKDETACH))
      {
        /* The charger/SRC has been unplug */
        handle->current_state = USBPD_CAD_STATE_CTVPD_UNATTACHED;
      }
      break;
    }
#endif /* USBPDCORE_VPD */
#endif /* _ACCESSORY_SNK */

    default:
    {
      break;
    }
  }

  return _timing;
}
#endif /* USBPD_SNK */

#if defined(USBPD_SRC)
/**
  * @brief  CAD state machine for source role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
static uint32_t CAD_StateMachine_SRC(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  /*Check CAD STATE*/
  switch ((usbpd_cad_state_t)handle->current_state)
  {
    case USBPD_CAD_STATE_DETACH_SRC :
    {
      /* DeInitialize Vconn management */
      USBPD_PWR_VConnDeInit(port_num);
      /* DeInitialise VBUS power */
      USBPD_PWR_VBUSDeInit(port_num);
      /* Reset the resistor */
      USBPD_PHY_HW_IF_AssertRp(port_num);
      handle->current_state = USBPD_CAD_STATE_DETACHED;
      _timing = 0U;
      break;
    }

    case USBPD_CAD_STATE_SWITCH_TO_SNK :
    case USBPD_CAD_STATE_DETACHED:
    {
      _timing = CAD_ManageStateDetached_SRC(port_num);
      break;
    }

    case USBPD_CAD_STATE_ATTACHED_WAIT:
    {
      _timing = CAD_ManageStateAttachedWait_SRC(port_num, p_event, p_cc_pin);
      break;
    }

#if defined(_ACCESSORY_SRC)
    case USBPD_CAD_STATE_ACCESSORY:
    {
      _timing = CAD_INFINITE_TIME;
      CAD_Check_HW_SRC(port_num);
      if ((USBPD_CAD_HW_COND_AudioAdapter_Attachment != (usbpd_cad_hw_condition_t)handle->current_hw_condition)
          && (USBPD_CAD_HW_COND_PwrCable_Sink_Attachment != (usbpd_cad_hw_condition_t)handle->current_hw_condition))
      {
        handle->current_state = USBPD_CAD_STATE_DETACH_SRC;
        *p_cc_pin = CCNONE;
        handle->cc = (uint32_t)CCNONE;
        *p_event = USBPD_CAD_EVENT_DETACHED;
      }
      break;
    }

    case USBPD_CAD_STATE_DEBUG:
    {
      _timing = CAD_INFINITE_TIME;
      CAD_Check_HW_SRC(port_num);
      if ((usbpd_cad_hw_condition_t)handle->current_hw_condition != USBPD_CAD_HW_COND_Debug_Attachment)
      {
        handle->current_state = USBPD_CAD_STATE_DETACH_SRC;
        *p_cc_pin = CCNONE;
        handle->cc = (uint32_t)CCNONE;
        *p_event = USBPD_CAD_EVENT_DETACHED;
      }
      break;
    }
#endif /* _ACCESSORY_SRC */

    case USBPD_CAD_STATE_EMC :
    {
      _timing = CAD_ManageStateEMC(port_num, p_event, p_cc_pin);
      break;
    }

    /*CAD electronic cable with Sink ATTACHED*/
    case USBPD_CAD_STATE_ATTEMC:
    case USBPD_CAD_STATE_ATTACHED:
    {
      _timing = CAD_ManageStateAttached_SRC(port_num, p_event, p_cc_pin);
      break;
    }

    default :
    {
      break;
    }
  }

  return _timing;
}
#endif /* USBPD_SRC */

#if defined(USBPD_DRP)
/**
  * @brief  CAD state machine for dual role.
  * @param  port_num Port number.
  * @param  p_event Pointer on CAD event based on @ref usbpd_cad_event_t.
  * @param  p_cc_pin Pointer on CC Pin based on @ref usbpd_cc_line_t.
  * @retval Timeout value.
  */
static uint32_t CAD_StateMachine_DRP(uint8_t port_num, usbpd_cad_event_t *p_event, usbpd_cc_line_t *p_cc_pin)
{
  usbpd_cad_hw_if_handle_t *handle = &h_cad_hw_if[port_num];
  uint32_t _timing = CAD_DEFAULT_TIME;

  /*Check CAD STATE*/
  switch ((usbpd_cad_state_t)handle->current_state)
  {
    case USBPD_CAD_STATE_DETACH_SRC :
    {
      /* DeInitialize Vconn management */
      USBPD_PWR_VConnDeInit(port_num);
      /* DeInitialise VBUS power */
      USBPD_PWR_VBUSDeInit(port_num);
      _timing = 0U;
      handle->current_state = USBPD_CAD_STATE_SWITCH_TO_SNK;
      break;
    }

    case USBPD_CAD_STATE_SWITCH_TO_SRC:
    case USBPD_CAD_STATE_SWITCH_TO_SNK:
    {
      (void)HAL_UCPD_DisableRx(h_phy_hw_if[port_num].hucpd);
      if (USBPD_CAD_STATE_SWITCH_TO_SRC == (usbpd_cad_state_t)handle->current_state)
      {
        USBPD_PHY_HW_IF_AssertRp(port_num);
        h_phy_hw_if[port_num].params->pe_power_role = (uint32_t)USBPD_PORT_PWR_ROLE_SRC;
        h_phy_hw_if[port_num].params->pe_data_role = (uint32_t)USBPD_PORTDATAROLE_DFP;
        _timing = h_phy_hw_if[port_num].settings->cad_src_toggle_time_ms;
      }
      if (USBPD_CAD_STATE_SWITCH_TO_SNK == (usbpd_cad_state_t)handle->current_state)
      {
        USBPD_PHY_HW_IF_AssertRd(port_num);
        h_phy_hw_if[port_num].params->pe_power_role = (uint32_t)USBPD_PORT_PWR_ROLE_SNK;
        h_phy_hw_if[port_num].params->pe_data_role = (uint32_t)USBPD_PORTDATAROLE_UFP;
        _timing = h_phy_hw_if[port_num].settings->cad_snk_toggle_time_ms;
      }
      handle->accessory_toggle_start_time = HAL_GetTick();
      handle->current_state = USBPD_CAD_STATE_DETACHED;
      break;
    }

    case USBPD_CAD_STATE_DETACHED:
      _timing = CAD_ManageStateDetached_DRP(port_num);
      break;

    /*CAD STATE ATTACHED WAIT*/
    case USBPD_CAD_STATE_ATTACHED_WAIT:
      _timing = CAD_ManageStateAttachedWait_DRP(port_num, p_event, p_cc_pin);
      break;

#if defined(_ACCESSORY_SRC)
    case USBPD_CAD_STATE_ACCESSORY:
    {
      CAD_Check_HW_SRC(port_num);
      if ((USBPD_CAD_HW_COND_AudioAdapter_Attachment != (usbpd_cad_hw_condition_t)handle->current_hw_condition)
          && (USBPD_CAD_HW_COND_PwrCable_Sink_Attachment != (usbpd_cad_hw_condition_t)handle->current_hw_condition))
      {
        handle->current_state = USBPD_CAD_STATE_DETACH_SRC;
        *p_cc_pin = CCNONE;
        handle->cc = (uint32_t)CCNONE;
        *p_event = USBPD_CAD_EVENT_DETACHED;
      }
      else
      {
        _timing = CAD_INFINITE_TIME;
      }
      break;
    }

    case USBPD_CAD_STATE_DEBUG:
    {
      _timing = CAD_INFINITE_TIME;
      CAD_Check_HW_SRC(port_num);
      if ((usbpd_cad_hw_condition_t)handle->current_hw_condition != USBPD_CAD_HW_COND_Debug_Attachment)
      {
        handle->current_state = USBPD_CAD_STATE_DETACH_SRC;
        *p_cc_pin = CCNONE;
        handle->cc = (uint32_t)CCNONE;
        *p_event = USBPD_CAD_EVENT_DETACHED;
      }
      break;
    }
#endif /* _ACCESSORY_SRC */

    /* CAD ELECTRONIC CABLE ATTACHED */
    case USBPD_CAD_STATE_EMC:
      _timing = CAD_ManageStateEMC(port_num, p_event, p_cc_pin);
      break;

    /*CAD electronic cable with Sink ATTACHED*/
    case USBPD_CAD_STATE_ATTEMC:
    case USBPD_CAD_STATE_ATTACHED:
      _timing = CAD_ManageStateAttached_DRP(port_num, p_event, p_cc_pin);
      break;

    default:
      break;
  }

  return _timing;
}
#endif /* USBPD_DRP */

#if !defined(USBPDCORE_LIB_NO_PD)
/**
  * @brief  Get the Rx ordered sets supported by the selected port and update the PE SOP settings.
  * @param  port_num USBPD port number.
  * @param  p_settings Pointer to the @ref usbpd_settings_t structure to update.
  * @note   The field @ref pe_supported_sop is set according to the Rx ordered sets
  *         reported by the UCPD peripheral.
  */
void USBPD_CAD_HW_IF_GetRxOrderedSet(uint8_t port_num, usbpd_settings_t *p_settings)
{
  uint32_t rx_ordered_set;
  uint32_t supported_sop_mask = 0U;
  usbpd_port_handle_t *phy_handle = NULL;

  if ((port_num < (uint8_t)USBPD_PORT_COUNT) && (p_settings != NULL))
  {
#if (USBPD_PORT_COUNT == 1U)
    phy_handle = &h_phy_hw_if[USBPD_PORT_0];
#else
    phy_handle = &h_phy_hw_if[port_num];
#endif /* USBPD_PORT_COUNT == 1U */

    /* Retrieve the UCPD peripheral handle */
#if (USBPD_PORT_COUNT == 1U)
    phy_handle->hucpd = (hal_ucpd_handle_t *)usbpd_ucpd_0_gethandle();
#else
    if (port_num == USBPD_PORT_0)
    {
      phy_handle->hucpd = (hal_ucpd_handle_t *)usbpd_ucpd_0_gethandle();
    }
    else
    {
      phy_handle->hucpd = (hal_ucpd_handle_t *)usbpd_ucpd_1_gethandle();
    }
#endif /* USBPD_PORT_COUNT == 1U */

    rx_ordered_set = HAL_UCPD_GetRxOrderedSet(phy_handle->hucpd);

    p_settings->pe_supported_sop = USBPD_SUPPORTED_SOP_NONE;

    if ((rx_ordered_set & HAL_UCPD_RX_ORDERED_SET_SOP) != 0U)
    {
      supported_sop_mask |= (uint32_t)USBPD_SUPPORTED_SOP_SOP;
    }

    if ((rx_ordered_set & HAL_UCPD_RX_ORDERED_SET_SOP1) != 0U)
    {
      supported_sop_mask |= (uint32_t)USBPD_SUPPORTED_SOP_SOP1;
    }

    if ((rx_ordered_set & HAL_UCPD_RX_ORDERED_SET_SOP2) != 0U)
    {
      supported_sop_mask |= (uint32_t)USBPD_SUPPORTED_SOP_SOP2;
    }

    if ((rx_ordered_set & HAL_UCPD_RX_ORDERED_SET_SOP1_DEBUG) != 0U)
    {
      supported_sop_mask |= (uint32_t)USBPD_SUPPORTED_SOP_SOP1_DEBUG;
    }

    if ((rx_ordered_set & HAL_UCPD_RX_ORDERED_SET_SOP2_DEBUG) != 0U)
    {
      supported_sop_mask |= (uint32_t)USBPD_SUPPORTED_SOP_SOP2_DEBUG;
    }

    p_settings->pe_supported_sop = (usbpd_sop_support_t)supported_sop_mask;
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

/**
  ******************************************************************************
  * @file    usbpd_core.h
  * @brief   This file contains the core stack API.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#ifndef USBPD_CORE_H
#define USBPD_CORE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_def.h"
#if defined(USBPDCORE_TCPM_SUPPORT)
#include "tcpc.h"
#endif /* USBPDCORE_TCPM_SUPPORT */

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_CORE
  * @{
  */

/** @addtogroup USBPD_CORE_CAD
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup USBPD_CORE_CAD_Exported_Types USBPD CORE CAD Exported Types
  * @{
  */

/**
  * @brief  USBP CAD Status definition.
  */
typedef enum
{
  USBPD_CAD_OK              = 0U, /*!< USBPD CAD Status OK               */
  USBPD_CAD_INVALID_PORT    = 1U, /*!< USBPD CAD Status INVALID PORT     */
  USBPD_CAD_ERROR           = 2U, /*!< USBPD CAD Status ERROR            */
  USBPD_CAD_MALLOCERROR     = 3U, /*!< USBPD CAD Status ERROR MALLOC     */
  USBPD_CAD_INVALIDRESISTOR = 4U, /*!< USBPD CAD Status INVALID RESISTOR */
} usbpd_cad_status_t;

/**
  * @brief  USBPD CAD Activation values definition.
  */
typedef enum
{
  USBPD_CAD_DISABLE = 0U, /*!< USBPD CAD activation status Disable */
  USBPD_CAD_ENABLE  = 1U, /*!< USBPD CAD activation status Enable  */
} usbpd_cad_activation_t;

/**
  * @brief  USBPD CAD CallBacks exposed by CAD.
  */
typedef struct
{
  /**
    * @brief  CallBack used to report events to DPM.
    * @param  port_num The handle of the port.
    * @param  state CAD state.
    * @param  Cc The Communication Channel for the USBPD communication.
    */
  void (*USBPD_CAD_CallbackEvent)(uint8_t port_num, usbpd_cad_event_t state, usbpd_cc_line_t Cc);

  /**
    * @brief  CallBack to wakeup the CAD.
    */
  void (*USBPD_CAD_WakeUp)(void);
} usbpd_cad_cb_t;

/**
  * @}
  */

/** @defgroup USBPD_CORE_CAD_Exported_Functions_Grp1 USBPD CORE CAD Exported Functions
  * @{
  */

/**
  * @brief  Initialize the CAD module for a specified port.
  * @param  port_num Port number.
  * @param  cad_cb CAD port callback function.
  * @param  settings Pointer on @ref usbpd_settings_t structure.
  * @param  params Pointer on @ref usbpd_params_t structure.
  * @retval USBPD_CAD status.
  */
usbpd_cad_status_t USBPD_CAD_Init(uint8_t port_num, const usbpd_cad_cb_t *cad_cb, usbpd_settings_t *settings,
                                  usbpd_params_t *params);

/**
  * @brief  DeInitialize the CAD module for a specified port.
  * @param  port_num Port number.
  */
void USBPD_CAD_DeInit(uint8_t port_num);

/**
  * @brief  function used to process type C state machine detection.
  * @retval time (in ms) after which the process must be executed.
  */
uint32_t USBPD_CAD_Process(void);

/**
  * @brief  Enable CAD port.
  * @param  port_num Port number.
  */
void USBPD_CAD_PortEnable(uint8_t port_num);

/**
  * @brief  Disable CAD port.
  * @param  port_num Port number.
  */
void USBPD_CAD_PortDisable(uint8_t port_num);

/**
  * @brief  Set the resistor to present a SNK.
  * @param  port_num Port number.
  */
void USBPD_CAD_AssertRd(uint8_t port_num);

/**
  * @brief  Set the resistor to present a SRC.
  * @param  port_num Port number.
  */
void USBPD_CAD_AssertRp(uint8_t port_num);

/**
  * @brief  Force type C state machine to enter recovery state.
  * @param  port_num Port number.
  */
void USBPD_CAD_EnterErrorRecovery(uint8_t port_num);

#if defined(USBPDCORE_DRP) || defined(USBPDCORE_SRC)
/**
  * @brief  Set the default Rp resistor (applicable to SRC only).
  * @param  port_num Port number.
  * @param  rp_value Rp value.
  * @retval USBPD_CAD_OK or USBPD_CAD_INVALIDRESISTOR.
  */
usbpd_cad_status_t USBPD_CAD_SRC_SetRpResistor(uint8_t port_num, usbpd_cad_rp_value_t rp_value);

#endif /* USBPDCORE_DRP || USBPDCORE_SRC */

/**
  * @brief  Return a pointer to the DPM parameters associated with the given port.
  * @param  PortNum USBPD port number.
  * @retval Pointer to the corresponding @ref usbpd_params_t structure.
  * @retval NULL if PortNum is greater than or equal to @ref USBPD_PORT_COUNT.
  * @retval NULL if the CAD handle for the given port is not available.
  */
usbpd_params_t *USBPD_CAD_Get_DPM_Params(uint8_t PortNum);

/**
  * @}
  */

/** @addtogroup USBPD_CORE_TRACE
  * @{
  */

/** @defgroup USBPD_CORE_TRACE_Exported_Types USBPD CORE TRACE Exported Types
  * @{
  */
typedef enum
{
  USBPD_TRACE_FORMAT_TLV  = 0U,  /*!< Trace format TLV        */
  USBPD_TRACE_MESSAGE_IN  = 1U,  /*!< Trace message in        */
  USBPD_TRACE_MESSAGE_OUT = 2U,  /*!< Trace message out       */
  USBPD_TRACE_CADEVENT    = 3U,  /*!< Trace CAD event         */
  USBPD_TRACE_PE_STATE    = 4U,  /*!< Trace PE state          */
  USBPD_TRACE_CAD_LOW     = 5U,  /*!< Trace CAD low level     */
  USBPD_TRACE_DEBUG       = 6U,  /*!< Trace debug             */
  USBPD_TRACE_SRC         = 7U,  /*!< Trace SRC               */
  USBPD_TRACE_SNK         = 8U,  /*!< Trace SNK               */
  USBPD_TRACE_NOTIF       = 9U,  /*!< Trace notification      */
  USBPD_TRACE_POWER       = 10U, /*!< Trace power             */
  USBPD_TRACE_TCPM        = 11U, /*!< Trace TCPM              */
  USBPD_TRACE_PRL_STATE   = 12U, /*!< Trace PRL state         */
  USBPD_TRACE_PRL_EVENT   = 13U, /*!< Trace PRL event         */
  USBPD_TRACE_PHY_NOTFRWD = 14U, /*!< Trace PHY not forwarded */
  USBPD_TRACE_CPU         = 15U, /*!< Trace CPU               */
  USBPD_TRACE_TIMEOUT     = 16U, /*!< Trace timeout           */
  USBPD_TRACE_UCSI        = 18U, /*!< Trace UCSI              */
  USBPD_TRACE_MSG_MSC     = 19U, /*!< Trace MSC               */
} usbpd_trace_event_t;

/**
  * @brief  USBPD Trace entry point type definition.
  * @param  type  type of the trace
  * @param  port  port number
  * @param  sop   SOP type
  * @param  ptr   pointer on the data
  * @param  size  size of the data
  */
typedef void (*usbpd_trace_entry_point_t)(usbpd_trace_event_t type, uint8_t port,
                                          uint8_t sop, uint8_t *ptr, uint32_t size);

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup USBPD_CORE_PE
  * @{
  */
/** @defgroup USBPD_CORE_PE_Private_Defines USBPD CORE PE Private Defines
  * @brief  These defines are used in the stack library. Just provided for information.
  *         Those timers values are not to be changed by user (changing values on user side,
  *         will not have any impacts on lib behavior).
  * @{
  *
  */

/* Policy Engine Timers */
#ifdef USBPDCORE_PPS
#define PE_TPPSREQUEST                   (9000U)  /*!< tPPSRequest: Max value set to 10s                         */
#define PE_TPPSTIMEOUT                   (14000U) /*!< tPPSTimeout: min 12s to max 15s                           */
#endif /* USBPDCORE_PPS */
#define PE_TPSHARDRESET                  (27U)    /*!< tPSHardReset: min 25ms to max 30ms                        */
#define PE_SRC_TSRCRECOVER               (800U)   /*!< tSrcRecover for SRC: min 660 ms max 1s                    */
#define PE_SNK_TSRCRECOVER               (2000U)  /*!< tSrcRecover for SNK: 2s (less restrictive for SINK tests) */
#define PE_SRC_TSAFE0V_T2                (650U)   /*!< tSafe0V for SRC: 650 ms                                   */
#define PE_SNK_TSAFE0V_T2                (1000U)  /*!< tSafe0V for SNK: 1000 ms                                  */
#define PE_TSRCTURNON_T4                 (275U)   /*!< tSrcTurnOn: 275 ms                                        */
#define PE_TPSSOURCEOFF                  (920U)   /*!< tPSSourceOff: min 750ms to max 920ms (SPR)                */
#define PE_TPSSOURCEOFF_EPR              (1260U)  /*!< tPSSourceOff: min 1120ms to max 1400ms (EPR)              */
#define PE_TPSSOURCEON                   (400U)   /*!< tPSSourceOn: min 390ms to max 480ms                       */

#define PE_TPSTRANSITION                 (475U)   /*!< tPSTransition: min 450ms to max 550ms                     */
#define PE_TPSTRANSITION_EPR             (925U)   /*!< tPSTransition: min 830ms to max 1020ms                    */

#define PE_TSENDERRESPONSE               (28U)    /*!< tSenderResponse: min 27ms to max 33ms (PD3)               */
#define PE_TSENDERRESPONSE_PD2           (25U)    /*!< tSenderResponse: min 24ms to max 30ms (PD2)               */

#define PE_TTYPECSINKWAITCAP             (400U)   /*!< tTypeCSinkWaitCap: min 310ms to max 620ms                 */

#define PE_TTYPECSENDSOURCECAPA          (150U)   /*!< tTypeCSendSourceCap: min 100ms to max 200ms               */
#define PE_TSRCTRANSITION                (30U)    /*!< tSrcTransition: min 25ms to max 35ms                      */

#define PE_TSWAPSRCSTART_MIN             (20U)    /*!< tSwapSourceStart: 20 ms                                   */

#define PE_TBISTCONTMODE                 (35U)    /*!< tBISTContMode: min 30ms to 60 ms                          */
#if defined(USBPDCORE_VCONN_SUPPORT)
#define PE_TDISCOVERIDENTITY             (45U)    /*!< tDiscoverIdentity: min 40ms to max 50ms                   */
#endif /* USBPDCORE_VCONN_SUPPORT */

#define PE_TVDMSENDERRESPONSE            (30U)    /*!< tVDMSenderResponse: min 24ms to max 30ms                  */

#if defined(USBPDCORE_SVDM) || defined(USBPDCORE_ANSWER_DISCOIDENT)
#define PE_TVDMRECEIVERRESPONSE          (15U)    /*!< tVDMReceiverResponse: max 15ms                            */
#endif /* USBPDCORE_SVDM || USBPDCORE_ANSWER_DISCOIDENT */
#if defined(USBPDCORE_SVDM)
#define PE_TVDMENTERMODE                 (25U)    /*!< tVDMEnterMode: max 25ms                                   */
#define PE_TVDMEXITMODE                  (25U)    /*!< tVDMExitMode: max 25ms                                    */
#endif /* USBPDCORE_SVDM */

#if defined(USBPDCORE_SVDM) || defined(USBPDCORE_VCONN_SUPPORT)
#define PE_TVDMWAITMODEENTRY             (50U)    /*!< tVDMWaitModeEntry: max 40ms to 50ms                       */
#define PE_TVDMWAITMODEEXIT              (50U)    /*!< tVDMWaitModeExit: max 40ms to 50ms                        */
#endif /* USBPDCORE_SVDM || USBPDCORE_VCONN_SUPPORT */

#ifdef USBPDCORE_VCONN_SUPPORT
#define PE_TVCONNSOURCETIMEOUT           (150U)   /*!< tVCONNSourceTimeout: min 100ms to max 200ms               */
#endif /* USBPDCORE_VCONN_SUPPORT */


#define PE_TVCONNZERO                    (125U)   /*!< tVCONNZero          :max 125ms                            */
#define PE_TVCONNREAPPLIED                (10U)   /*!< tVCONNZero          :min 10 max 20ms                      */
#define PE_TDATARESETFAIL                (300U)   /*!< tDataResetFail      :min 300ms                            */
#define PE_TDATARESET                    (200U)   /*!< tDataReset          :min 200ms 225ms max 250ms            */

#define PE_AMETIMEOUT                   (1100U)   /*!< tAMETimeout         :min 1000ms                           */
/**
  * @}
  */

#if defined(USBPDCORE_SVDM) || defined(USBPDCORE_UVDM) || defined(USBPDCORE_VCONN_SUPPORT) \
    || defined(USBPDCORE_ANSWER_DISCOIDENT)
/** @defgroup USBPD_CORE_VDM_Exported_Callback USBPD CORE VDM Exported Callback
  * @{
  */

/**
  * @brief CallBacks exposed by the @ref USBPD_CORE_PE.
  * */
typedef struct
{
#if defined(USBPDCORE_SVDM) || defined(USBPDCORE_VCONN_SUPPORT) || defined(USBPDCORE_ANSWER_DISCOIDENT)
  /**
    * @brief  VDM discovery identity callback.
    * @param  port_num Current port number.
    * @param  p_identity Pointer on @ref usbpd_vdm_discovery_identity_t structure.
    * @note   Function is called to get Discovery identity information linked to the device and answer
    *         to SVDM Discovery identity init message sent by port partner
    * @retval USBPD status: @ref USBPD_ACK or @ref USBPD_BUSY.
    */
  usbpd_status_t(*USBPD_VDM_DiscoverIdentity)(uint8_t port_num, usbpd_vdm_discovery_identity_t *p_identity);
#endif /* USBPDCORE_SVDM || USBPDCORE_VCONN_SUPPORT || USBPDCORE_ANSWER_DISCOIDENT */

#if defined(USBPDCORE_SVDM) || defined(USBPDCORE_VCONN_SUPPORT)
  /**
    * @brief  VDM Discover SVID callback.
    * @param  port_num Current port number.
    * @param  p_SVID_Info Pointer on @ref usbpd_vdm_svid_info_t structure.
    * @param  p_svid_count Pointer on number of SVID.
    * @note   Function is called to retrieve SVID supported by device and answer
    *         to SVDM Discovery SVID init message sent by port partner
    * @retval USBPD status @ref USBPD_BUSY or @ref USBPD_ACK or @ref USBPD_NAK.
    */
  usbpd_status_t(*USBPD_VDM_DiscoverSVIDs)(uint8_t port_num, uint16_t **p_SVID_Info, uint8_t *p_svid_count);

  /**
    * @brief  VDM Discover Mode callback (report all the modes supported by SVID).
    * @param  port_num Current port number.
    * @param  svid SVID value.
    * @param  p_mode_tab Pointer on the mode value.
    * @param  mode_count Number of mode available.
    * @note   Function is called to report all the modes supported by selected SVID and answer
    *         to SVDM Discovery Mode init message sent by port partner
    * @retval USBPD status.
    */
  usbpd_status_t(*USBPD_VDM_DiscoverModes)(uint8_t port_num,
                                           uint16_t svid, uint32_t **p_mode_tab, uint8_t *mode_count);

  /**
    * @brief  VDM mode enter callback.
    * @param  port_num Current port number.
    * @param  svid SVID value.
    * @param  mode_index Index of the mode to be entered.
    * @note   Function is called to check if device can enter in the mode received for the selected SVID in the
    *         SVDM enter mode init message sent by port partner
    * @retval USBPD status @ref USBPD_ACK/@ref USBPD_NAK.
    */
  usbpd_status_t(*USBPD_VDM_ModeEnter)(uint8_t port_num, uint16_t svid, uint32_t mode_index);

  /**
    * @brief  VDM mode exit callback.
    * @param  port_num Current port number.
    * @param  svid SVID value.
    * @param  mode_index Index of the mode to be exited.
    * @note   Function is called to check if device can exit from the mode received for the selected SVID in the
    *         SVDM exit mode init message sent by port partner
    * @retval USBPD status @ref USBPD_ACK/@ref USBPD_NAK.
    */
  usbpd_status_t(*USBPD_VDM_ModeExit)(uint8_t port_num, uint16_t svid, uint32_t mode_index);

  /**
    * @brief  Inform identity callback.
    * @param  port_num Current port number.
    * @param  sop_type SOP type.
    * @param  command_status Command status based on @ref usbpd_vdm_user_command_type_t.
    * @param  p_identity Pointer on the discovery identity information based on @ref usbpd_vdm_discovery_identity_t.
    * @note   Function is called to save Identity information received in Discovery identity from port partner
              (answer to SVDM discovery identity sent by device)
    */
  void (*USBPD_VDM_InformIdentity)(uint8_t port_num, usbpd_sop_type_t sop_type,
                                   usbpd_vdm_user_command_type_t command_status,
                                   usbpd_vdm_discovery_identity_t *p_identity);

  /**
    * @brief  Inform SVID callback.
    * @param  port_num Current port number.
    * @param  sop_type SOP type.
    * @param  command_status Command status based on @ref usbpd_vdm_user_command_type_t.
    * @param  p_list_svid Pointer of list of SVID based on @ref usbpd_vdm_svid_info_t.
    * @note   Function is called to save list of SVID received in Discovery SVID from port partner
              (answer to SVDM discovery SVID sent by device)
    */
  void (*USBPD_VDM_InformSVID)(uint8_t port_num, usbpd_sop_type_t sop_type,
                               usbpd_vdm_user_command_type_t command_status, usbpd_vdm_svid_info_t *p_list_svid);

  /**
    * @brief  Inform Mode callback ( received in Discovery modes ACK).
    * @param  port_num Current port number.
    * @param  sop_type SOP type.
    * @param  command_status Command status based on @ref usbpd_vdm_user_command_type_t.
    * @param  pModesInfo Pointer of modes info based on @ref usbpd_vdm_mode_info_t.
    * @note   Function is called to save list of modes linked to SVID received in Discovery mode from port partner
              (answer to SVDM discovery mode sent by device)
    */
  void (*USBPD_VDM_InformMode)(uint8_t port_num, usbpd_sop_type_t sop_type,
                               usbpd_vdm_user_command_type_t command_status, usbpd_vdm_mode_info_t *pModesInfo);

  /**
    * @brief  Inform mode enter callback.
    * @param  port_num Current port number.
    * @param  sop_type SOP type.
    * @param  command_status Command status based on @ref usbpd_vdm_user_command_type_t.
    * @param  svid SVID ID.
    * @param  mode_index Index of the mode to be entered.
    * @note   Function is called to inform if port partner accepted or not to enter in the mode
    *         specified in the SVDM enter mode sent by the device
    */
  void (*USBPD_VDM_InformModeEnter)(uint8_t port_num, usbpd_sop_type_t sop_type,
                                    usbpd_vdm_user_command_type_t command_status, uint16_t svid, uint32_t mode_index);

  /**
    * @brief  Inform mode exit callback.
    * @param  port_num Current port number.
    * @param  sop_type SOP type.
    * @param  command_status Command status based on @ref usbpd_vdm_user_command_type_t.
    * @param  svid SVID ID.
    * @param  mode_index Index of the mode to be exited.
    * @note   Function is called to inform if port partner accepted or not to exit from the mode
    *         specified in the SVDM exit mode sent by the device
    */
  void (*USBPD_VDM_InformModeExit)(uint8_t port_num, usbpd_sop_type_t sop_type,
                                   usbpd_vdm_user_command_type_t command_status, uint16_t svid, uint32_t mode_index);

  /**
    * @brief  Send VDM Attention message callback.
    * @param  port_num Current port number.
    * @param  pdata_count Pointer of number of VDO to send.
    * @param  p_vdo Pointer of VDO to send.
    * @note   Function is called when device wants to send a SVDM attention message to port partner
    *         (for instance DP status can be filled through this function)
    */
  void (*USBPD_VDM_SendAttention)(uint8_t port_num, uint8_t *pdata_count, uint32_t *p_vdo);

  /**
    * @brief  Receive VDM Attention callback.
    * @param  port_num Current port number.
    * @param  data_count Number of received VDO.
    * @param  VDO Received VDO.
    * @note   Function is called when a SVDM attention init message has been received from port partner
    *         (for instance, save DP status data through this function)
    */
  void (*USBPD_VDM_ReceiveAttention)(uint8_t port_num, uint8_t data_count, uint32_t VDO);

  /**
    * @brief  VDM Send Specific message callback.
    * @param  port_num Current port number.
    * @param  sop_type SOP type.
    * @param  vdm_command VDM command based on @ref usbpd_vdm_svdm_command_t.
    * @param  pdata_count Pointer of number of VDO to send.
    * @param  p_vdo Pointer of VDO to send.
    * @note   Function is called when device wants to send a SVDM specific init message to port partner
    *         (for instance DP status or DP configure can be filled through this function)
    */
  void (*USBPD_VDM_SendSpecific)(uint8_t port_num, usbpd_sop_type_t sop_type,
                                 usbpd_vdm_svdm_command_t vdm_command, uint8_t *pdata_count, uint32_t *p_vdo);

  /**
    * @brief  VDM Receive Specific message callback.
    * @param  port_num Current port number.
    * @param  vdm_command VDM command based on @ref usbpd_vdm_svdm_command_t.
    * @param  pdata_count Pointer of number of received VDO and used for the answer.
    * @param  p_vdo Pointer of received VDO and use for the answer.
    * @note   Function is called to answer to a SVDM specific init message received by port partner.
    *         (for instance, retrieve DP status or DP configure data through this function)
    * @retval USBPD Status.
    */
  usbpd_status_t(*USBPD_VDM_ReceiveSpecific)(uint8_t port_num,
                                             usbpd_vdm_svdm_command_t vdm_command,
                                             uint8_t *pdata_count, uint32_t *p_vdo);

  /**
    * @brief  VDM Specific message callback to inform user of reception of VDM specific message.
    * @param  port_num Current port number.
    * @param  sop_type SOP type.
    * @param  vdm_command VDM command based on @ref usbpd_vdm_svdm_command_t.
    * @param  pdata_count Pointer of number of received VDO.
    * @param  p_vdo Pointer of received VDO.
    * @note   Function is called when answer from SVDM specific init message has been received by the device
    *         (for instance, save DP status and DP configure data through this function)
    */
  void (*USBPD_VDM_InformSpecific)(uint8_t port_num, usbpd_sop_type_t sop_type,
                                   usbpd_vdm_svdm_command_t vdm_command, uint8_t *pdata_count, uint32_t *p_vdo);

#endif /* USBPDCORE_SVDM || USBPDCORE_VCONN_SUPPORT */
#if defined(USBPDCORE_UVDM)
  /**
    * @brief  VDM Send Unstructured message callback.
    * @param  port_num Current port number.
    * @param  puvdm_header Pointer on UVDM header based on @ref usbpd_vdm_uvdm_header_t.
    * @param  pdata_count Pointer of number of VDO to send.
    * @param  p_vdo Pointer of VDO to send.
    */
  void (*USBPD_VDM_SendUVDM)(uint8_t port_num, usbpd_vdm_uvdm_header_t *puvdm_header,
                             uint8_t *pdata_count, uint32_t *p_vdo);

  /**
    * @brief  Unstructured VDM  message callback to inform user of reception of UVDM message.
    * @param  port_num Current port number.
    * @param  uvdm_header UVDM header based on @ref usbpd_vdm_uvdm_header_t.
    * @param  pdata_count Pointer of number of received VDO.
    * @param  p_vdo Pointer of received VDO.
    * @retval USBPD Status.
    */
  usbpd_status_t(*USBPD_VDM_ReceiveUVDM)(uint8_t port_num,
                                         usbpd_vdm_uvdm_header_t uvdm_header, uint8_t *pdata_count, uint32_t *p_vdo);
#endif /* USBPDCORE_UVDM */
} usbpd_vdm_cb_t;

/**
  * @}
  */

#endif /* USBPDCORE_SVDM || USBPDCORE_UVDM || USBPDCORE_VCONN_SUPPORT || USBPDCORE_ANSWER_DISCOIDENT */

/** @defgroup USBPD_CORE_PE_Exported_TypesDefinitions USBPD CORE PE Exported Types Definitions
  * @{
  */

/** @defgroup PE_CallBacks_structure_definition PE CallBacks structure definition
  * @brief  PE CallBacks exposed by the PE to the DPM.
  * @{
  */
typedef struct
{
  /**
    * @brief  Callback used to request DPM to setup the new power level.
    * @param  port_num Port number.
    * @note   this callback is mandatory for a SRC and DRP.
    * @retval Returned values are: @ref USBPD_ACCEPT, @ref USBPD_REJECT, @ref USBPD_WAIT.
    */
  usbpd_status_t(*USBPD_PE_RequestSetupNewPower)(uint8_t port_num);

  /**
    * @brief  Callback used to inform the DPM about the different HardReset step.
    * @param  port_num Port number.
    * @param  current_role Role of the board @ref usbpd_port_pwr_role_t.
    * @param  Status HR status @ref usbpd_hard_reset_status_t.
    */
  void (*USBPD_PE_HardReset)(uint8_t port_num, usbpd_port_pwr_role_t current_role, usbpd_hard_reset_status_t Status);

  /**
    * @brief  Callback used to ask application the reply status for a power role swap request.
    * @param  port_num Port number.
    * @note   if the callback is NULL, the request power role swap are automatically rejected.
    * @retval Returned values are: @ref USBPD_ACCEPT, @ref USBPD_WAIT, @ref USBPD_REJECT.
    */
  usbpd_status_t(*USBPD_PE_EvaluatPRSwap)(uint8_t port_num);

  /**
    * @brief  Callback used by the stack to notify an event.
    * @param  port_num Port number.
    * @param  event_val  @ref usbpd_notify_event_t.
    * @note   this callback is mandatory
    */
  void (*USBPD_PE_Notify)(uint8_t port_num, usbpd_notify_event_t event_val);

  /**
    * @brief  Callback used by the stack to inform DPM that an extended message has been received.
    * @param  port_num Port number.
    * @param  msg_id Extended message id @ref usbpd_extended_msg_t.
    * @param  p_data Pointer on the data.
    * @param  data_size Size of the data.
    */
  void (*USBPD_PE_ExtendedMessage)(uint8_t port_num,
                                   usbpd_extended_msg_t msg_id, uint8_t *p_data, uint16_t data_size);

  /**
    * @brief  Callback used by the stack to get information from DPM or PWR_IF.
    * @param  port_num Port number.
    * @param  data_id Type of data to be read from DPM based on @ref usbpd_core_datainfo_type_t.
    * @param  ptr Pointer on address where DPM data must be written (u8 pointer).
    * @param  Size Pointer on nb of bytes written by DPM.
    */
  void (*USBPD_PE_GetDataInfo)(uint8_t port_num, usbpd_core_datainfo_type_t data_id, uint8_t *ptr, uint32_t *size);

  /**
    * @brief  Callback used by the stack to set information inside DPM or PWR_IF.
    * @param  port_num Port number.
    * @param  data_id Type of data to be read from DPM based on @ref usbpd_core_datainfo_type_t.
    * @param  ptr Pointer on address where DPM data to be updated could be read (u8 pointer).
    * @param  size Nb of byte to be updated in DPM.
    */
  void (*USBPD_PE_SetDataInfo)(uint8_t port_num, usbpd_core_datainfo_type_t data_id, uint8_t *ptr, uint32_t size);

  /**
    * @brief  Callback used by a SOURCE to evaluate the SINK request.
    * @param  port_num Port number.
    * @param  p_pwr_object Pointer on the power data object.
    * @retval Returned values are: @ref USBPD_ACCEPT, @ref USBPD_REJECT, @ref USBPD_WAIT.
    */
  usbpd_status_t(*USBPD_PE_SRC_EvaluateRequest)(uint8_t port_num, usbpd_core_pdo_type_t *p_pwr_object);

  /**
    * @brief  Callback used by a sink to evaluate the Source Capabilities.
    * @param  port_num Port number.
    * @param  p_request_data Pointer on selected request data object.
    * @param  p_pwr_object Pointer on selected power data object.
    * @note   the callback is used in the SNK and DRP context.
    */
  void (*USBPD_PE_SNK_EvaluateCapabilities)(uint8_t port_num, uint32_t *RequestData,
                                            usbpd_core_pdo_type_t *p_pwr_object);

  /**
    * @brief  Callback used during the different step of the power role swap.
    * @param  port_num Port number.
    * @param  Role Of the board @ref usbpd_port_pwr_role_t.
    * @param  Status HR Status update @ref usbpd_prs_status_t.
    * @note   see MSC documentation to get more detail
    */
  void (*USBPD_PE_PowerRoleSwap)(uint8_t port_num, usbpd_port_pwr_role_t current_role,
                                 usbpd_prs_status_t Status);

  /**
    * @brief  Callback used to wakeup the current state machine.
    * @param  port_num Port number.
    * @note   this function is mandatory but has effect only in RTOS context
    */
  void (*USBPD_PE_WakeupCallback)(uint8_t port_num);

  /**
    * @brief  Callback used to ask application the reply status for a Vconn swap request.
    * @param  port_num Port number.
    * @retval Returned values are: @ref USBPD_ACCEPT, @ref USBPD_REJECT, @ref USBPD_WAIT.
    */
  usbpd_status_t(*USBPD_PE_EvaluateVconnSwap)(uint8_t port_num);

  /**
    * @brief  Callback used to enable/disable the vconn power.
    * @param  port_num Port number.
    * @param  state Enable or Disable VConn on CC lines.
    * @note   the function is mandatory to manage the VconnPower
    * @retval Returned values are: @ref USBPD_ACCEPT, @ref USBPD_REJECT.
    */
  usbpd_status_t(*USBPD_PE_VconnPwr)(uint8_t port_num, usbpd_functional_status_t state);

  /**
    * @brief  Callback used by PE to enter in error recovery state.
    * @param  port_num Port number.
    */
  void (*USBPD_PE_EnterErrorRecovery)(uint8_t port_num);

  /**
    * @brief  Callback used to ask application the reply status for a data_role_swap request.
    * @param  port_num Port number.
    * @note   if the callback is not set (ie NULL) the stack will automatically reject the request
    * @retval Returned values are:.
              @ref USBPD_ACCEPT if DRS can be accepted
              @ref USBPD_REJECT if DRS is not accepted in one data role (DFP or UFP) or in PD2.0 config
              @ref USBPD_NOTSUPPORTED if DRS is not supported at all by the application (in both data roles) - P3.0 only
    */
  usbpd_status_t(*USBPD_PE_DataRoleSwap)(uint8_t port_num);

  /**
    * @brief  Callback used to check the VBUS status.
    * @param  port_num Port number.
    * @param  vsafe Vsafe status based on @ref usbpd_vsafe_status_t.
    * @note   this function is mandatory for a SNK port and used inside the hardreset process
    * @retval Returned values are: @ref USBPD_DISABLE or @ref USBPD_ENABLE.
    */
  usbpd_functional_status_t(*USBPD_PE_IsPowerReady)(uint8_t port_num, usbpd_vsafe_status_t vsafe);

  /**
    * @brief  Callback used to update the PE user timers for the given port.
    * @param  port_num USBPD port number.
    */
  void (*USBPD_PE_UserTimerCounter)(uint8_t port_num);

#if  defined(USBPDCORE_EPR)
  /**
    * @brief  Callback used to request DPM what to do.
    * @param  port_num Port number.
    * @param  Id Of the action @ref usbpd_core_action_type_t.
    * @note   this function is a common function to ask DPM what the policy engine must do.
    * @retval Returned values are: @ref USBPD_ACCEPT, USBPD_REJECT, USBPD_WAIT, USBPD_DELAYANSWER, USBPD_NOTSUPPORTED.
    */
  uint32_t (*USBPD_PE_RequestDPMWhatToDo)(uint8_t port_num, uint32_t action_id);
#endif /* (USBPDCORE_USBDATA || USBPDCORE_EPR) */
} usbpd_pe_cb_t;

/**
  * @}
  */

/**
  * @}
  */

/** @defgroup USBPD_CORE_PE_Exported_Functions USBPD CORE PE Exported Functions
  * @{
  */

/** @defgroup USBPD_CORE_PE_Exported_Functions_Group1 USBPD CORE PE Exported Functions to DPM CORE
  * @{
  */

/**
  * @brief  Initialize Policy Engine layer for a Port.
  * @param  port_num Port number.
  * @param  p_settings Pointer on @ref usbpd_settings_t structure.
  * @param  p_params Pointer on @ref usbpd_params_t structure.
  * @param  p_pe_cb Callback function to be passed to PRL layer.
  * @note   the value p_settings and p_params must be presistante because the stack keep there memory location
  * @retval USBPD status Possible values are.
                          - @ref USBPD_OK
                          - @ref USBPD_ERROR
                          - @ref USPD_ERROR_CALLBACKMISSING
                          - @ref USBPD_INVALID_PORT_NUMBER
                          - @ref USBPD_MALLOCERROR
  */
usbpd_status_t USBPD_PE_Init(uint8_t port_num, usbpd_settings_t *p_settings, usbpd_params_t *p_params,
                             const usbpd_pe_cb_t *p_pe_cb);

/**
  * @brief  Execute the PE user timer counter callback for the given port.
  * @param  PortNum USBPD port number.
  * @note   The function returns without action if PortNum is out of range,
  *         if the PE handle is NULL, if the callback table is NULL,
  *         or if the user timer counter callback is not defined.
  */
void USBPD_PE_UserTimerCounter(uint8_t PortNum);

/**
  * @brief  Return a pointer to the DPM parameters associated with the given port.
  * @param  PortNum USBPD port number.
  * @retval Pointer to the corresponding @ref usbpd_params_t structure.
  * @retval NULL if PortNum is greater than or equal to @ref USBPD_PORT_COUNT.
  * @retval NULL if the PE handle for the given port is NULL.
  */
usbpd_params_t *USBPD_PE_Get_DPM_Params(uint8_t PortNum);

/**
  * @brief  Return a pointer to the DPM settings associated with the given port.
  * @param  PortNum USBPD port number.
  * @retval Pointer to the corresponding @ref usbpd_settings_t structure.
  * @retval NULL if PortNum is greater than or equal to @ref USBPD_PORT_COUNT.
  * @retval NULL if the PE handle for the given port is NULL.
  */
usbpd_settings_t *USBPD_PE_Get_DPM_Settings(uint8_t PortNum);

/**
  * @brief  Check coherence between lib selected and the lib include inside the project.
  * @param  lib_id Value fix defined with the switch lib.
  * @note   This function can be used only during development process to avoid mismatch
  * @retval if the check is OK @ref USBPD_TRUE else @ref USBPD_FALSE.
  */
uint32_t            USBPD_PE_CheckLIB(uint32_t lib_id);

/**
  * @brief  Return the need of the stack in terms of dynamique allocation.
  * @note   the value returned corresponds to the allocation need for 2 ports so if application manages
  *         only one port the need is be divided by 2
  * @retval Memory size.
  */
uint32_t            USBPD_PE_GetMemoryConsumption(void);

uint32_t            USBPD_PRL_ManageChunkRx(uint8_t port_num);

/**
  * @brief  Set the trace pointer and the debug level.
  * @param  ptr Function on trace.
  * @param  debug Trace debug level.
  * @note   the recommended value for debug trace level is 0x3
  *              0x1 : Policy engine state
  *              0x2 : Notification
  *              0x3 : Policy engine state + Notification
  */
void                USBPD_PE_SetTrace(usbpd_trace_entry_point_t ptr, uint8_t debug);

/**
  * @brief  Notification function.
  * @param  port_num Port number.
  * @param  event_val Event based on @ref usbpd_notify_event_t.
  * @note this function is used by DPM_CORE or the stack to inform the user DPM application
  */
void                USBPD_PE_Notification(uint8_t port_num, usbpd_notify_event_t event_val);


#ifdef USBPDCORE_DRP
/**
  * @brief  Policy Engine DRP state machine.
  * @param  port_num Port number.
  * @retval Timing in ms.
  */
uint32_t            USBPD_PE_StateMachine_DRP(uint8_t port_num);
#endif /* USBPDCORE_DRP */

#if defined(USBPDCORE_VPD) && defined(USBPDCORE_SNK)
/**
  * @brief  Policy Engine SNK state machine supporting VPD.
  * @param  port_num Port number.
  * @retval Timing in ms.
  */
uint32_t USBPD_PE_StateMachine_SNKwVPD(uint8_t port_num);
#endif /* USBPDCORE_VPD && USBPDCORE_SNK */

/**
  * @brief  Function called by DPM to before calling the state machine.
  * @param  port_num Port number.
  */
void USBPD_PE_StateMachine_Reset(uint8_t port_num);

/**
  * @brief  Function called by DPM to exit the state machine.
  * @param  port_num Port number.
  */
void USBPD_PE_StateMachine_Stop(uint8_t port_num);

/**
  * @brief  Function called by DPM to set the cable status connected or disconnected.
  * @param  port_num Port number.
  * @param  flag_is_connected Cable connection status: 1 if connected and 0 is disconnected.
  * @retval USBPD status @ref USBPD_OK.
  * @note this function is obsolete the connection information are managed at cad level
  */
void USBPD_PE_IsCableConnected(uint8_t port_num, uint8_t flag_is_connected);

/**
  * @brief  Increment PE Timers tick.
  * @param  port_num Port number.
  * @note   This function must be called each elapsed ms
  */
void USBPD_PE_TimerCounter(uint8_t port_num);

/**
  * @brief  Update PE Timers tick.
  * @param  port_num Port number.
  * @param  tick Value in ms (can't execed the value 0x... else the value will not be take into account).
  * @note   This function must be called when exited the low power mode
  */
void USBPD_PE_TimerCounteUpdate(uint8_t port_num, uint16_t tick);

/**
  * @}
  */

#if defined(USBPDCORE_SRC) || defined(USBPDCORE_DRP)
/** @defgroup USBPD_CORE_SRC_Exported_Functions_Group1 USBPD CORE PE SRC Exported Functions to DPM CORE
  * @{
  */
/**
  * @brief  Policy Engine Source state machine.
  * @param  port_num Port number.
  * @retval Timing in ms.
  */
uint32_t            USBPD_PE_StateMachine_SRC(uint8_t port_num);

/**
  * @}
  */
#endif /* USBPDCORE_SRC || USBPDCORE_DRP */

#if defined(USBPDCORE_SNK) || defined(USBPDCORE_DRP)
/** @defgroup USBPD_CORE_PE_SNK_Exported_Functions_Group1 USBPD CORE PE SNK Exported Functions to DPM CORE
  * @{
  */
/**
  * @brief  Policy Engine Sink state machine.
  * @param  port_num Port number.
  * @retval Timing in ms.
  */
uint32_t            USBPD_PE_StateMachine_SNK(uint8_t port_num);

/**
  * @}
  */
#endif /* USBPDCORE_SNK || USBPDCORE_DRP */

#if defined(USBPDCORE_SVDM) || defined(USBPDCORE_UVDM) || defined(USBPDCORE_VCONN_SUPPORT) \
    || defined(USBPDCORE_ANSWER_DISCOIDENT)
/** @defgroup USBPD_CORE_PE_Exported_Functions_Group2 USBPD CORE PE Exported Functions to VDM USER
  * @{
  */
/**
  * @brief  Initialize VDM callback functions in PE.
  * @param  port_num Port number.
  * @param  vdm_callbacks Pointer on VDM callbacks based on @ref usbpd_vdm_cb_t.
  */
void USBPD_PE_InitVDM_Callback(uint8_t port_num, usbpd_vdm_cb_t *vdm_callbacks);
/**
  * @}
  */
#endif /* USBPDCORE_SVDM || USBPDCORE_UVDM || USBPDCORE_VCONN_SUPPORT || USBPDCORE_ANSWER_DISCOIDENT */

/** @defgroup USBPD_CORE_PE_Exported_Functions_Group3 USBPD CORE PE Exported Functions to DPM USER
  * @{
  */

/**
  * @brief  This generic function to send a control message.
  * @param  port_num Port number.
  * @param  ctrl_msg Control message id @ref usbpd_control_msg_t.
  * @param  sop_type SOP Type based on @ref usbpd_sop_type_t.
  * @retval status    @ref USBPD_OK, @ref USBPD_BUSY, @ref USBPD_ERROR or @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_Request_CtrlMessage(uint8_t port_num, usbpd_control_msg_t ctrl_msg,
                                            usbpd_sop_type_t sop_type);

/**
  * @brief  This generic function is used to send data message.
  * @param  port_num Port number.
  * @param  DataMsg Data message id based on @ref usbpd_data_msg_t.
  * @param  p_data Pointer on the data to send.
  * @note   the parameter p_data is used only if DataMsg is equal
            to USBPD_DATAMSG_ALERT or USBPD_DATAMSG_GET_COUNTRY_INFO
  * @retval status    @ref USBPD_OK, @ref USBPD_BUSY, @ref USBPD_ERROR or @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_Request_DataMessage(uint8_t port_num, usbpd_data_msg_t DataMsg, uint32_t *p_data);

/**
  * @brief  This function is used to force PE to perform an Hard Reset.
  * @param  port_num Port number.
  * @retval status @ref USBPD_OK.
  */
usbpd_status_t USBPD_PE_Request_HardReset(uint8_t port_num);

/**
  * @brief  Request the PE to send a cable reset.
  * @param  port_num The current port number.
  * @note   Only a DFP must generate Cable Reset Signaling.
            A DFP must only generate Cable Reset Signaling within an Explicit Contract.
            The DFP has to be supplying VCONN prior to a Cable Reset
  * @retval USBPD Status.
  */
usbpd_status_t USBPD_PE_Request_CableReset(uint8_t port_num);

#if defined(USBPDCORE_SNK) || defined(USBPDCORE_DRP)
/**
  * @brief  This function request PE to send a request message.
  * @param  port_num Port number.
  * @param  rdo Requested data object.
  * @param  pwr_object Power Object.
  * @retval status    @ref USBPD_OK, @ref USBPD_BUSY, @ref USBPD_ERROR or @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_Send_Request(uint8_t port_num, uint32_t rdo, usbpd_core_pdo_type_t pwr_object);
#endif /* USBPDCORE_SNK || USBPDCORE_DRP */

#if defined(USBPDCORE_BATTERY) || defined(USBPDCORE_MANU_INFO) \
    || defined(USBPDCORE_SECURITY_MSG) || defined(USBPDCORE_FWUPD)
/**
  * @brief  This function send an extended message.
  * @param  port_num Port number.
  * @param  sop_type Message type based on @ref usbpd_sop_type_t.
  * @param  message_type Extended Msg type @ref usbpd_extended_msg_t.
  * @param  p_data Data pointer to PRL layer.
  * @param  data_size Size of the data (<= 260).
  * @note   The management of chunk or unchunked message is manage inside the
  * @retval status @ref USBPD_OK, @ref USBPD_BUSY, @ref USBPD_ERROR or @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_SendExtendedMessage(uint8_t port_num, usbpd_sop_type_t sop_type,
                                            usbpd_extended_msg_t message_type,
                                            uint8_t *p_data, uint16_t data_size);
#endif /* USBPDCORE_BATTERY || USBPDCORE_MANU_INFO || USBPDCORE_SECURITY_MSG || USBPDCORE_FWUPD */

#if defined(USBPDCORE_FASTROLESWAP)
/**
  * @brief  this function request PE to perform an FRS signalling.
  * @param  port_num.
  */
void USBPD_PE_ExecFastRoleSwapSignalling(uint8_t port_num);
#endif /* USBPDCORE_FASTROLESWAP */

#if defined(USBPDCORE_SVDM) || defined(USBPDCORE_VCONN_SUPPORT)
/**
  * @brief  Called by DPM to request the PE to perform a VDM identity request.
  * @param  port_num Port number.
  * @param  sop_type SOP Type (Only SOP or SOP').
  * @note   On PD2.0, SVDM Identity cannot be sent by UFP.
  * @retval USBPD status.
  */
usbpd_status_t USBPD_PE_SVDM_RequestIdentity(uint8_t port_num, usbpd_sop_type_t sop_type);

/**
  * @brief  Called by DPM to request the PE to perform a VDM SVID request.
  * @param  port_num Port number.
  * @param  sop_type SOP Type (only valid for SOP or SOP').
  * @retval USBPD status: @ref USBPD_BUSY, @ref USBPD_OK, @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_SVDM_RequestSVID(uint8_t port_num, usbpd_sop_type_t sop_type);

/**
  * @brief  Called by DPM to request the PE to perform a VDM Discovery mode message on one SVID.
  * @param  port_num Port number.
  * @param  sop_type SOP Type.
  * @param  svid SVID used for discovery mode message.
  * @retval USBPD status: @ref USBPD_BUSY, @ref USBPD_OK, @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_SVDM_RequestMode(uint8_t port_num, usbpd_sop_type_t sop_type, uint16_t svid);

/**
  * @brief  Called by DPM to request the PE to perform a VDM Discovery mode message on one SVID.
  * @param  port_num Port number.
  * @param  sop_type SOP Type.
  * @param  svid SVID used for discovery mode message.
  * @param  mode_index Index of the mode to be entered.
  * @retval USBPD status: @ref USBPD_BUSY, @ref USBPD_OK, @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_SVDM_RequestModeEnter(uint8_t port_num, usbpd_sop_type_t sop_type, uint16_t svid,
                                              uint8_t mode_index);

/**
  * @brief  Called by DPM to request the PE to perform a VDM mode exit.
  * @param  port_num Port number.
  * @param  sop_type SOP Type.
  * @param  svid SVID used for discovery mode message.
  * @param  mode_index Index of the mode to be exit.
  * @retval USBPD status: @ref USBPD_BUSY, @ref USBPD_OK, @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_SVDM_RequestModeExit(uint8_t port_num, usbpd_sop_type_t sop_type, uint16_t svid,
                                             uint8_t mode_index);

/**
  * @brief  Called by DPM to request the PE to send a specific SVDM message.
  * @param  port_num Port number.
  * @param  sop_type Received message type based on @ref usbpd_sop_type_t.
  * @param  Command VDM command based on @ref usbpd_vdm_svdm_command_t.
  * @param  SVID Used SVID.
  * @retval USBPD status: @ref USBPD_BUSY, @ref USBPD_OK, @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_SVDM_RequestSpecific(uint8_t port_num, usbpd_sop_type_t sop_type,
                                             usbpd_vdm_svdm_command_t Command, uint16_t svid);
#endif /* USBPDCORE_SVDM || USBPDCORE_VCONN_SUPPORT */

#if defined(USBPDCORE_SVDM)
/**
  * @brief  Called by DPM to request the PE to perform a VDM Attention.
  * @param  port_num Port number.
  * @param  sop_type Received message type based on @ref usbpd_sop_type_t (valid with SOP only).
  * @param  svid Used SVID.
  * @retval USBPD status: @ref USBPD_BUSY, @ref USBPD_OK, @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_SVDM_RequestAttention(uint8_t port_num, usbpd_sop_type_t sop_type, uint16_t svid);
#endif /* USBPDCORE_SVDM */

#if defined(USBPDCORE_UVDM)
/**
  * @brief  Called by DPM to request the PE to send a UVDM message.
  * @param  port_num Port number.
  * @param  sop_type Received message type based on @ref usbpd_sop_type_t.
  * @retval USBPD status: @ref USBPD_BUSY, @ref USBPD_OK, @ref USBPD_FAIL.
  */
usbpd_status_t USBPD_PE_UVDM_RequestMessage(uint8_t port_num, usbpd_sop_type_t sop_type);
#endif /* USBPDCORE_UVDM */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup USBPD_CORE_PRL
  * @{
  */

/** @defgroup USBPD_CORE_PRL_Exported_Functions USBPD CORE PRL Exported Functions
  * @{
  */

/** @defgroup USBPD_CORE_PRL_Exported_Functions_Group1 USBPD CORE PRL Interface function for timer
  * @{
  */

/**
  * @brief  Decrement The PRL Timers values.
  * @param  port_num Port number.
  */
void USBPD_PRL_TimerCounter(uint8_t port_num);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#if defined(USBPDCORE_TCPM_SUPPORT)
/** @addtogroup USBPD_CORE_TCPM
  * @{
  */

/** @defgroup USBPD_CORE_TCPM_Exported_Functions USBPD CORE TCPM Exported Functions
  * @{
  */

/** @defgroup USBPD_CORE_TCPM_Exported_Functions_Grp1 USBPD CORE TCPM Exported Functions to DPM and PWR_IF applications
  * @{
  */

/**
  * @brief  Initialize TCPC devices.
  * @param  port_num Port number.
  * @param  TCPC_ToggleRole Indication if TCPC must perform a role toggle or not.
  * @param  Params Pointer on PE parameters based on @ref usbpd_params_t.
  * @param  CallbackFunctions Pointer on CAD callbacks.
  * @param  TCPC_Driver Pointer on TCPC drivers based on @ref TCPC_DrvTypeDef.
  * @retval USBPD status.
  */
usbpd_status_t  USBPD_TCPM_HWInit(uint8_t port_num, uint8_t TCPC_ToggleRole, usbpd_params_t *Params,
                                  usbpd_cad_cb_t *CallbackFunctions, TCPC_DrvTypeDef *TCPC_Driver);

/**
  * @brief  Set CC line for PD connection.
  * @param  port_num Port number.
  * @param  Pull Power role can be one of the following values:.
  *         @arg @ref TYPEC_CC_RA
  *         @arg @ref TYPEC_CC_RP
  *         @arg @ref TYPEC_CC_RD
  *         @arg @ref TYPEC_CC_OPEN
  * @param  state Activation or deactivation of connection.
  * @retval USBPD status.
  */
usbpd_status_t  USBPD_TCPM_set_cc(uint32_t port_num, tcpc_cc_pull_t Pull, usbpd_functional_status_t state);

/**
  * @brief  Enable or disable VCONN.
  * @param  port_num Port number.
  * @param  state Activation or deactivation of VCONN.
  * @retval USBPD status.
  */
usbpd_status_t  USBPD_TCPM_set_vconn(uint32_t port_num, usbpd_functional_status_t state);

/**
  * @brief  Enable VBUS.
  * @param  port_num The port handle.
  * @retval USBPD status.
  */
usbpd_status_t  USBPD_TCPM_VBUS_Enable(uint32_t port_num);

/**
  * @brief  Disable VBUS.
  * @param  port_num The port handle.
  * @retval USBPD status.
  */
usbpd_status_t  USBPD_TCPM_VBUS_Disable(uint32_t port_num);

/**
  * @brief  Get VBUS voltage.
  * @param  port_num The port handle.
  * @retval VBUS value in mV.
  */
uint16_t USBPD_TCPM_VBUS_GetVoltage(uint32_t port_num);

/**
  * @brief  Retrieve the VBUS VSafe0 status for a specified port.
  * @param  port_num The port handle.
  * @retval usbpd_status_t.
  */
usbpd_status_t  USBPD_TCPM_VBUS_IsVsafe0V(uint32_t port_num);

/**
  * @brief  Retrieve the VBUS Vsafe5V status for a specified port.
  * @param  port_num The port handle.
  * @retval usbpd_status_t.
  */
usbpd_status_t  USBPD_TCPM_VBUS_IsVsafe5V(uint32_t port_num);

/**
  * @brief  Management of ALERT.
  * @param  Event Event reported by application.
  * @retval USBPD status.
  */
usbpd_status_t  USBPD_TCPM_alert(uint32_t Event);

/**
  * @brief  Force type C state machine to enter recovery state.
  * @param  port_num Port number.
  * @retval USBPD status.
  */
usbpd_status_t   USBPD_TCPM_EnterErrorRecovery(uint32_t port_num);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
#endif /* USBPDCORE_TCPM_SUPPORT */

#if defined(USBPDCORE_EPR)

/**
  * @brief  This function is used to send an extended control message.
  * @param  port_num Port number.
  * @param  message_type Extended control Msg type @ref usbpd_extended_control_msg_epr_t.
  * @retval USBPD status.
  *       USBPD_BUSY  : the request can be execute because an action is ongoing
  *       USBPD_ERROR : the PD state doesn't allow this operation
  *       USBPD_FAIL  : the operation is not allowed in this context
  *       USBPD_OK    : the request has been post to the stack
  */
usbpd_status_t USBPD_PE_Send_ExtendeControlMessage(uint8_t port_num,
                                                   usbpd_extended_control_msg_epr_t message_type);

#if defined(USBPDCORE_EPR) && (defined(USBPDCORE_SNK) || defined(USBPDCORE_DRP))
/**
  * @brief  This function request PE to send an EPR_Mode enter.
  * @param  port_num Port number.
  * @retval status    @ref USBPD_OK, @ref USBPD_BUSY, @ref USBPD_ERROR or @ref USBPD_FAIL.
  *
  * @note : this function can be only called by a sink with an explicit contract
  */
usbpd_status_t USBPD_PE_Request_EPRModeEnter(uint8_t port_num);

/**
  * @brief  This function request PE to send an EPR_Mode exit.
  * @param  port_num Port number.
  * @retval status    @ref USBPD_OK, @ref USBPD_BUSY, @ref USBPD_ERROR or @ref USBPD_FAIL.
  *
  * @note : this function can be only called by a sink with an explicit contract
  */
usbpd_status_t USBPD_PE_Request_EPRModeExit(uint8_t port_num);
#endif /* USBPDCORE_EPR && (USBPDCORE_SNK || USBPDCORE_DRP) */
#endif /* (USBPDCORE_EPR || USBPDCORE_USBDATA) */
/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* USBPD_CORE_H */

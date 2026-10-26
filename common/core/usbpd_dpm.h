/**
  ******************************************************************************
  * @file    usbpd_dpm.h
  * @brief   This file contains the structures required to manage the DPM layer.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef USBPD_DPM_H
#define USBPD_DPM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct
{
  uint32_t requested_voltage_1mv_units;     /*!< Sink request operating voltage in mV units     */
  uint32_t operating_current_max_1ma_units; /*!< Sink request Max operating current in mA units */
  uint32_t operating_current_1ma_units;     /*!< Sink request operating current in mA units     */
  uint32_t operating_pwr_max_1mw_units;     /*!< Sink request Max operating power in mW units   */
  uint32_t operating_pwr_1mw_units;         /*!< Sink request operating power in mW units       */
} usbpd_dpm_snk_pwr_request_details_t;

/**
  * @brief  USBPD DPM User Settings definition.
  */
typedef struct
{
  uint32_t                             pe_data_swap_support      : 1U;  /*!< Support data swap                       */
  uint32_t                             pe_vconn_swap_support     : 1U;  /*!< Support VCONN swap                      */
  uint32_t                             pe_dr_swap_to_dfp_support : 1U;  /*!< If supported, DR Swap to DFP can be
                                                                             accepted or not by the user else
                                                                             directly rejected                       */
  uint32_t                             pe_dr_swap_to_ufp_support : 1U;  /*!< If supported, DR Swap to UFP can be
                                                                             accepted or not by the user else
                                                                             directly rejected                       */
  uint32_t                             reserved1                 : 28U; /*!< Reserved bits */
  usbpd_snk_pwr_request_t              dpm_snk_requested_pwr;           /*!< Requested Power by the sink board       */
  usbpd_midb_t                         dpm_manu_info_port;              /*!< Manufacturer information used for the
                                                                             port                                    */
  uint16_t                             reserved_manu;                   /*!< Reserved bits to match with
                                                                             Manufacturer information                */
  usbpd_scedb_t                        dpm_src_ext_capa;                /*!< SRC Extended Capability                 */
#if defined(USBPDCORE_SNK_CAPA_EXT)
  usbpd_skedb_t                        dpm_snk_ext_capa;                /*!< SNK Extended Capability                 */
#endif /* USBPDCORE_SNK_CAPA_EXT */
  uint8_t                              reserved_snk_capa[3U];           /*!< Reserved bits to match with SnkCapaExt
                                                                             information                             */
  uint32_t                             pwr_accessory_detection   : 1U;  /*!< It enables or disables powered
                                                                             accessory detection                     */
  uint32_t                             pwr_accessory_transition  : 1U;  /*!< It enables or disables transition from
                                                                             Powered.accessory to Try.SNK            */
  uint32_t                             pwr_unconstrained         : 1U;  /*!< UUT has an external power source
                                                                             available that is sufficient to
                                                                             adequately power the system while
                                                                             charging external devices or the UUT
                                                                             primary function is to charge external
                                                                             devices.                                */
  uint32_t                             pwr_rp_value              : 2U;  /*!< RP resistor value based on
                                                                             @ref usbpd_cad_source_current_adv_t     */
  uint32_t                             usb_support               : 1U;  /*!< USB_Comms_Capable, is the UUT capable
                                                                             of enumerating as a USB host or device? */
  uint32_t                             usb_device_support        : 1U;  /*!< Type_C_Can_Act_As_Device, Indicates
                                                                             whether the UUT can communicate with
                                                                             USB 2.0 or USB 3.1 as a device or as
                                                                             the Upstream Facing Port of a hub.      */
  uint32_t                             usb_host_support          : 1U;  /*!< Type_C_Can_Act_As_Host, Indicates
                                                                             whether the UUT can communicate with
                                                                             USB 2.0 or USB 3.1 as a host or as the
                                                                             Downstream Facing Port of a hub         */
  uint32_t                             usb_suspend_support       : 1U;  /*!< USB Suspend support values in PDO
                                                                             definition (Source)                     */
  uint32_t                             cad_tdrp                  : 7U;  /*!< The period that DRP must complete a
                                                                             Source to Sink and back advertisement   */
  uint32_t                             cad_dcsrc_rp              : 7U;  /*!< The percent of time that a DRP must
                                                                             advertise Source during tDRP (in %)     */
  uint32_t                             reserved2                 : 31U; /*!< Reserved bits                           */
} usbpd_dpm_user_settings_t;

/**
  * @brief  USBPD DPM ID Settings definition.
  */
typedef struct
{
  uint32_t xid; /*!< Value provided by the USB-IF assigned to the product */
  uint16_t vid; /*!< Vendor ID (assigned by the USB-IF)                   */
  uint16_t pid; /*!< Product ID (assigned by the manufacturer)            */
} usbpd_dpm_id_settings_t;

/**
  * @brief  USBPD DPM PDO Selection method definition.
  */
typedef enum
{
  USBPD_DPM_PDO_SEL_METHOD_MAX_PWR  = 0U, /*!< Select the PDO with the maximum power   */
  USBPD_DPM_PDO_SEL_METHOD_MIN_PWR  = 1U, /*!< Select the PDO with the minimum power   */
  USBPD_DPM_PDO_SEL_METHOD_MAX_VOLT = 2U, /*!< Select the PDO with the maximum voltage */
  USBPD_DPM_PDO_SEL_METHOD_MIN_VOLT = 3U, /*!< Select the PDO with the minimum voltage */
  USBPD_DPM_PDO_SEL_METHOD_MAX_CUR  = 4U, /*!< Select the PDO with the maximum current */
  USBPD_DPM_PDO_SEL_METHOD_MIN_CUR  = 5U, /*!< Select the PDO with the minimum current */
} usbpd_dpm_pdo_sel_method_t;

/**
  * @brief  USBPD DPM handle Structure definition
  */
typedef struct
{
  uint32_t          p_list_rcv_snk_pdo[USBPD_MAX_NB_PDO]; /*!< The list of received Sink Power Data Objects from Port
                                                               partner (when Port partner is a Sink or a DRP port).   */
  uint32_t          rcv_snk_pdo_count;                    /*!< The number of received Sink Power Data Objects from
                                                               port Partner (when Port partner is a Sink or a DRP
                                                               port).                                                 */
  uint32_t          p_list_rcv_src_pdo[USBPD_MAX_NB_PDO]; /*!< The list of received Source Power Data Objects from
                                                               Port partner                                           */
  uint32_t          rcv_src_pdo_count;                    /*!< The number of received Source Power Data Objects from
                                                               port Partner  (when Port partner is a Source or a DRP
                                                               port).                                                 */
  usbpd_dpm_pdo_sel_method_t usbpd_dpm_pdo_select_method; /*<! PDO selection method                                   */
  uint32_t          rcv_request_do_msg;                   /*!< Received request Power Data Object message from the
                                                               port Partner                                           */
  uint32_t          rcv_request_do_msg_previous;          /*!< Previous Request Power Data Object message to be sent  */
#if defined(USBPDCORE_PPS)
  usbpd_ppsdb_t     rcv_pps_status;                       /*!< PPS Status received by port partner                    */
#endif /* USBPDCORE_PPS */
#if defined(USBPDCORE_SNK_CAPA_EXT)
  usbpd_skedb_t     rcv_snk_extended_capa;                /*!< SNK Extended Capability received by port partner       */
#endif /* USBPDCORE_SNK_CAPA_EXT */
  uint32_t          rdo_msg;                              /*!< Request Power Data Object message to be sent           */
  uint32_t          rdo_position;                         /*!< RDO Position of requested DO in Source list of
                                                               capabilities                                           */
  uint32_t          rdo_position_previous;                /*!< RDO Position of previous requested DO in Source list
                                                               of capabilities                                        */
  uint32_t          requested_voltage_1mv_units;          /*!< Value of requested voltage                             */
  uint32_t          requested_current_1ma_units;          /*!< Value of requested current                             */

  int16_t           measured_current_1ma_units;           /*!< Value of measured current                              */
  volatile uint32_t error_code;                           /*!< USB PD Error code                                      */
  volatile uint8_t  flag_is_connected;                    /*!< USB PD connection state                                */
  uint16_t          flag_cable_pd_capable : 1U;           /*!< Flag to keep information that Cable is be PD capable   */
  uint16_t          flag_cable_reset_ongoing : 1U;        /*!< Flag to manage a cable reset on going                  */

  uint16_t          reserved1 : 14U;                      /*!< Reserved bytes                                         */
#if defined(USBPDCORE_BATTERY)
  usbpd_bsdo_t      battery_status;                       /*!< Battery status                                         */
#endif /* USBPDCORE_BATTERY */
#if defined(_MONITOR_INTERFACE)
  volatile uint16_t timer_meas_report;                    /*!< Timer used to send measurement report                  */
#endif /* _MONITOR_INTERFACE */

  /**
    * @brief  Ask DPM for a delay (OS specific).
    * @param  port_num The current port number.
    * @param  time_ms The time in ms.
    * @note   When using an OS this delay must be preemptible.
    */
  void (*DPM_Delay)(uint8_t port_num, uint32_t time_ms);
} usbpd_dpm_handle_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* USBPD_DPM_H */


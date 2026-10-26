/**
  ******************************************************************************
  * @file    usbpd_utils_monitor.c
  * @brief   Monitor control module.
  *          This file provides a set of firmware functions to manage the usbpd
  *          monitor utility for communication with a computer.
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
#include "usbpd_utils_monitor.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS_MONITOR
  * @brief   This file contains PHY hardware interface functions.
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_Private_Types USBPD_UTILS_MONITOR Private Types
  * @{
  */

/**
  * @brief  USBPD MONITOR REJECT REASON.
  */
typedef enum
{
  MONITOR_REJ_DPM_REJECT                     = 0x00U, /*<! Reject: DPM reject                                         */
  MONITOR_REJ_DPM_BUSY                       = 0x01U, /*<! Reject: Busy                                               */
  MONITOR_REJ_DPM_TIMEOUT                    = 0x02U, /*<! Reject: Timeout                                            */
  MONITOR_REJ_DPM_INVALID_MESSAGE            = 0x03U, /*<! Reject: Invalid message                                    */
  MONITOR_REJ_DPM_OUT_OF_RANGE               = 0x04U, /*<! Reject: Out of range                                       */
  MONITOR_REJ_DPM_INVALID_PORT_NUMBER        = 0x05U, /*<! Reject: Invalid port number                                */
  MONITOR_REJ_DPM_NOT_READY                  = 0x06U, /*<! Reject: Not ready                                          */
} utils_monitor_reject_reason_t;

/**
  * @brief  USBPD MONITOR REGISTER TCPC.
  */
typedef enum
{
  MONITOR_REG_VENDOR_ID                      = 0x00U, /*<! VENDOR_ID                                                  */
  MONITOR_REG_PRODUCT_ID                     = 0x02U, /*<! PRODUCT_ID                                                 */
  MONITOR_REG_DEVICE_ID                      = 0x04U, /*<! DEVICE_ID                                                  */
  MONITOR_REG_USBTYPEC_REV                   = 0x06U, /*<! USBTYPEC_REV                                               */
  MONITOR_REG_USBPD_REV_VER                  = 0x08U, /*<! USBPD_REV_VER                                              */
  MONITOR_REG_USBPD_INTERFACE_REV            = 0x0AU, /*<! USBPD_INTERFACE_REV                                        */
  MONITOR_REG_ALERT                          = 0x10U, /*<! ALERT                                                      */
  MONITOR_REG_ALERT_MASK                     = 0x12U, /*<! ALERT_MASK                                                 */
  MONITOR_REG_POWER_STATUS_MASK              = 0x14U, /*<! POWER_STATUS_MASK                                          */
  MONITOR_REG_FAULT_STATUS_MASK              = 0x15U, /*<! FAULT_STATUS_MASK                                          */
  MONITOR_REG_CONFIG_STANDARD_OUTPUT         = 0x18U, /*<! CONFIG_STANDARD_OUTPUT                                     */
  MONITOR_REG_TCPC_CONTROL                   = 0x19U, /*<! TCPC_CONTROL                                               */
  MONITOR_REG_ROLE_CONTROL                   = 0x1AU, /*<! ROLE_CONTROL                                               */
  MONITOR_REG_FAULT_CONTROL                  = 0x1BU, /*<! FAULT_CONTROL                                              */
  MONITOR_REG_POWER_CONTROL                  = 0x1CU, /*<! POWER_CONTROL                                              */
  MONITOR_REG_CC_STATUS                      = 0x1DU, /*<! CC_STATUS                                                  */
  MONITOR_REG_POWER_STATUS                   = 0x1EU, /*<! POWER_STATUS                                               */
  MONITOR_REG_FAULT_STATUS                   = 0x1FU, /*<! FAULT_STATUS                                               */
  MONITOR_REG_COMMAND                        = 0x23U, /*<! COMMAND                                                    */
  MONITOR_REG_DECIVE_CAP1                    = 0x24U, /*<! DECIVE_CAP1                                                */
  MONITOR_REG_DECIVE_CAP2                    = 0x26U, /*<! DECIVE_CAP2                                                */
  MONITOR_REG_STD_INPUT_CAP                  = 0x28U, /*<! STD_INPUT_CAP                                              */
  MONITOR_REG_STD_OUTPUT_CAP                 = 0x29U, /*<! STD_OUTPUT_CAP                                             */
  MONITOR_REG_MSG_HEADER_INFO                = 0x2EU, /*<! MSG_HEADER_INFO                                            */
  MONITOR_REG_RX_DETECT                      = 0x2FU, /*<! RX_DETECT                                                  */
  MONITOR_REG_RX_BYTE_COUNT                  = 0x30U, /*<! RX_BYTE_COUNT                                              */
  MONITOR_REG_RX_BUFFER_FRAME_TYPE           = 0x31U, /*<! RX_BUFFER_FRAME_TYPE                                       */
  MONITOR_REG_RX_HEADER                      = 0x32U, /*<! RX_HEADER                                                  */
  MONITOR_REG_RX_DATA                        = 0x34U, /*<! RX_DATA                                                    */
  MONITOR_REG_TRANSMIT                       = 0x50U, /*<! TRANSMIT                                                   */
  MONITOR_REG_TX_BYTE_COUNT                  = 0x51U, /*<! TX_BYTE_COUNT                                              */
  MONITOR_REG_TX_HEADER                      = 0x52U, /*<! TX_HEADER                                                  */
  MONITOR_REG_TX_DATA                        = 0x54U, /*<! TX_DATA                                                    */
  MONITOR_REG_VBUS_VOLTAGE                   = 0x70U, /*<! VBUS_VOLTAGE                                               */
  MONITOR_REG_VBUS_SINK_DISCONNECT_THRESHOLD = 0x72U, /*<! VBUS_SINK_DISCONNECT_THRESHOLD                             */
  MONITOR_REG_VBUS_STOP_DISCHARGE_THRESHOLD  = 0x74U, /*<! VBUS_STOP_DISCHARGE_THRESHOLD                              */
  MONITOR_REG_VBUS_VOLTAGE_ALARM_HI_CFG      = 0x76U, /*<! VBUS_VOLTAGE_ALARM_HI_CFG                                  */
  MONITOR_REG_VBUS_VOLTAGE_ALARM_LO_CFG      = 0x78U, /*<! VBUS_VOLTAGE_ALARM_LO_CFG                                  */
  MONITOR_REG_VENDOR_DATA                    = 0x80U, /*<! VENDOR_DATA                                                */
} utils_monitor_register_tcpc_t;

/**
  * @brief  USBPD MONITOR TAG PARAM.
  */
typedef enum
{
  MONITOR_PARAM_SOP                              = 0x00U, /*<! SOP support                                            */
  MONITOR_PARAM_SOP1                             = 0x00U, /*<! SOP prime support                                      */
  MONITOR_PARAM_SOP2                             = 0x00U, /*<! SOP second support                                     */
  MONITOR_PARAM_FASTROLESWAP                     = 0x02U, /*<! It enables the FRS                                     */
  MONITOR_PARAM_DATAROLESWAP_TO_UFP              = 0x03U, /*<! Support of Data role swap to UFP                       */
  MONITOR_PARAM_DEFAULTPOWERROLE                 = 0x04U, /*<! DRP on PE side but no toggle on CAD
                                                              (dependency with CADRoleToggle)                         */
  MONITOR_PARAM_DRP_SUPPORT                      = 0x05U, /*<! DRP support (Power Role swap support)                  */
  MONITOR_PARAM_CADROLETOGGLE                    = 0x06U, /*<! CAD Role Toggle                                        */
  MONITOR_PARAM_PE_SCAP_HR                       = 0x07U, /*<! Hard reset after N_SOURCE_CAPS message sending
                                                              tentatives                                              */
  MONITOR_PARAM_VCONNSWAP                        = 0x08U, /*<! It enables or disables the VCONN swap capability       */
  MONITOR_PARAM_VDM_SUPPORT                      = 0x09U, /*<! Indicate if VDM messages support is enabled or not     */
  MONITOR_PARAM_PING_SUPPORT                     = 0x0AU, /*<! Indicate if Ping message support is enabled or not     */
  MONITOR_PARAM_PPS_SUPPORT                      = 0x0BU, /*<! PPS message supported by PE                            */
  MONITOR_PARAM_SRCCAPAEXTSUPPORT                = 0x0BU, /*<! Source_Capabilities_Extended message supported by PE   */
  MONITOR_PARAM_ALERTSUPPORT                     = 0x0BU, /*<! Alert message supported by PE                          */
  MONITOR_PARAM_STATUSSUPPORT                    = 0x0BU, /*<! Status message supported by PE
                                                              (is_alert_supported must be enabled)                    */
  MONITOR_PARAM_BATTERYSTATUSSUPPORT             = 0x0BU, /*<! Battery_Status message supported by PE                 */
  MONITOR_PARAM_BATTERYCAPASUPPORT               = 0x0BU, /*<! Battery_Capabilities message supported by PE           */
  MONITOR_PARAM_MANUINFOSUPPORT                  = 0x0BU, /*<! Manufacturer_Info message supported by PE              */
  MONITOR_PARAM_COUNTRYCODESSUPPORT              = 0x0BU, /*<! Country_Codes message supported by PE                  */
  MONITOR_PARAM_COUNTRYINFOSUPPORT               = 0x0BU, /*<! Country_Info message supported by PE                   */
  MONITOR_PARAM_SECURESPONSESUPPORT              = 0x0BU, /*<! Security_Response message supported by PE              */
  MONITOR_PARAM_SNK_PDO                          = 0x0CU, /*<! maximum 7 * 32bit                                      */
  MONITOR_PARAM_SRC_PDO                          = 0x0DU, /*<! maximum 7 * 32bit                                      */
  MONITOR_PARAM_TDRP                             = 0x0EU, /*<! The period a DRP must complete a Source to Sink
                                                              and back advertisement                                  */
  MONITOR_PARAM_DCSRC_DRP                        = 0x0FU, /*<! The percent of time that a DRP must advertise
                                                              Source during tDRP (in %)                               */
  MONITOR_PARAM_RESPONDS_TO_DISCOV_SOP           = 0x10U, /*<! UUT can respond successfully to a Discover Identity
                                                              command from its port partner using SOP.                */
  MONITOR_PARAM_ATTEMPTS_DISCOV_SOP              = 0x11U, /*<! UUT can send a Discover Identity command to
                                                              its port partner using SOP                              */
  MONITOR_PARAM_XID_SOP                          = 0x12U, /*<! A decimal number assigned by USB-IF before
                                                              certification                                           */
  MONITOR_PARAM_DATA_CAPABLE_AS_USB_HOST_SOP     = 0x13U, /*<! Indicates whether the UUT is capable of
                                                              enumerating USB Host                                    */
  MONITOR_PARAM_DATA_CAPABLE_AS_USB_DEVICE_SOP   = 0x14U, /*<! Indicates whether the UUT is capable of
                                                              enumerating as a USB Device                             */
  MONITOR_PARAM_PRODUCT_TYPE_SOP                 = 0x15U, /*<! Product type of the UUT                                */
  MONITOR_PARAM_USBPD_VDM_MODAL_OPERATION_SUPPORTED_SOP    = 0x16U, /*<! Modal operation support                      */
  MONITOR_PARAM_USB_VID_SOP                      = 0x17U, /*<! A unique 16-bit number, assigned to the Vendor
                                                              by USB-IF.                                              */
  MONITOR_PARAM_PID_SOP                          = 0x18U, /*<! A unique number assigned by the Vendor ID holder
                                                              identifying the product                                 */
  MONITOR_PARAM_BCDDEVICE_SOP                    = 0x19U, /*<! A unique number assigned by the Vendor ID holder
                                                              containing identity information relevant to the
                                                              release version of the product.                         */
  MONITOR_PARAM_MEASUREREPORTING                 = 0x1AU, /*<! 0x00: Disable Measure reporting
                                                              (1 << 8) | (tr between 0 & 127): Enable Measure reporting
                                                              every tr x 40 ms. (if xx = 0, only 1 indication
                                                              to report to MONITOR)                                   */
  MONITOR_PARAM_MANUINFOPORT_VID                 = 0x1BU, /*<! Vendor ID (assigned by the USB-IF))
                                                              used by Manufacturer info  set for port                 */
  MONITOR_PARAM_MANUINFOPORT_PID                 = 0x1BU, /*<! Product ID (assigned by the manufacturer)
                                                              used by Manufacturer info  set for port                 */
  MONITOR_PARAM_MANUINFOPORT_STRING              = 0x1BU, /*<! Vendor string used by Manufacturer info  set for port  */
  MONITOR_PARAM_DATAROLESWAP_TO_DFP              = 0x1CU, /*<! Support of Data role swap to DFP                       */
  MONITOR_PARAM_NB_TAG                           = (MONITOR_PARAM_DATAROLESWAP_TO_DFP + 1U), /*<! Max number of param */
  MONITOR_PARAM_ALL                              = 0xFFU, /*<! All parameters                                         */
} utils_monitor_tag_param_t;

/**
  * @brief  USBPD MONITOR TAG INDICATION.
  */
typedef enum
{
  MONITOR_IND_NUMBEROFRCVSNKPDO                  = 0x00U, /*<! Number of received Sink Power Data Objects from
                                                              the port Partner (when the Port partner is a Sink or a
                                                              DRP port)                                               */
  MONITOR_IND_RDOPOSITION                        = 0x01U, /*<! RDO Position of the requested Data Object in the Source
                                                              list of capabilities                                    */
  MONITOR_IND_LISTOFRCVSRCPDO                    = 0x02U, /*<! List of received Source Power Data Objects from
                                                              the Port partner (when the Port partner is a Source or a
                                                              DRP port)                                               */
  MONITOR_IND_NUMBEROFRCVSRCPDO                  = 0x03U, /*<! Number of received Source Power Data Objects from
                                                              the Port Partner (when the Port partner is a Source or a
                                                              DRP port)                                               */
  MONITOR_IND_LISTOFRCVSNKPDO                    = 0x04U, /*<! List of received Sink Power Data Objects from
                                                              the Port partner (when the Port partner is a Sink or a
                                                              DRP port)                                               */
  MONITOR_IND_ISCONNECTED                        = 0x05U, /*<! USBPD connection state                                 */
  MONITOR_IND_CC                                 = 0x06U, /*<! CC side                                                */
  MONITOR_IND_DATAROLE                           = 0x07U, /*<! Initial data role                                      */
  MONITOR_IND_POWERROLE                          = 0x08U, /*<! Power role                                             */
  MONITOR_IND_CCDEFAULTCURRENTADVERTISED         = 0x09U, /*<! Advertised current capability                          */
  MONITOR_IND_VCONNON                            = 0x0AU, /*<! Vconn Status                                           */
  MONITOR_IND_VBUS_LEVEL                         = 0x0CU, /*<! VBUS level (in mV)                                     */
  MONITOR_IND_IBUS_LEVEL                         = 0x0CU, /*<! IBUS level (in mA)                                     */
  MONITOR_IND_PD_SPECREVISION                    = 0x0DU, /*<! Selected Specification revision                        */
  MONITOR_IND_PD_MESSAGENOTIF                    = 0x0EU, /*<! Send notifications of PD messages                      */
  MONITOR_IND_NBBATTERIES                        = 0x0FU, /*<! Number of batteries supported by the devices           */
  MONITOR_IND_COUNTRYCODES                       = 0x10U, /*<! List of country codes received in the COUNTRY_CODES
                                                              message                                                 */
  MONITOR_IND_SVDM_SVIDS                         = 0x11U, /*<! List of SVDM SVIDs received in the SVDM Discovery SVID
                                                              message                                                 */
  MONITOR_IND_SVDM_MODES                         = 0x12U, /*<! List of modes received in the SVDM Discovery SVID
                                                              message                                                 */
  MONITOR_IND_TIMESTAMP                          = 0x13U, /*<! Timestamp used for VBUS and IBUS values                */
  MONITOR_IND_PPS                                = 0x14U, /*<! PPS value based on usbpd_ppsdb_t                       */
  MONITOR_IND_STATUS                             = 0x15U, /*<! Status value based on @ref usbpd_sdb_t                 */
  MONITOR_IND_VDM_IDENTITY                       = 0x16U, /*<! VDM Identity based on @ref usbpd_vdm_id_header_vdo_t   */
  MONITOR_IND_CABLE_VDO                          = 0x19U, /*<! VDM Cable Object based on usbpd_vdm_cable_vdo_t        */
  MONITOR_IND_ALL                                = (MONITOR_IND_CABLE_VDO + 1U), /*<! Maximum number of indications   */
} utils_monitor_tag_indication_t;

/**
  * @brief  USBPD MONITOR TAG PARAM_MSG.
  */
typedef enum
{
  MONITOR_PARAM_MSG_SOPTYPE                      = 0x00U, /*<! SOP type used for DPM_MESSAGE_REQ                      */
  MONITOR_PARAM_MSG_RDOPOSITION                  = 0x01U, /*<! RDO position of Received SRC PDO (used for RESET_REQ)  */
  MONITOR_PARAM_MSG_REQUESTEDVOLTAGE             = 0x02U, /*<! Requested voltage in REQUEST message (in mV)           */
  MONITOR_PARAM_MSG_ALERTMSG                     = 0x04U, /*<! Alert message used in ALERT message                    */
  MONITOR_PARAM_MSG_COUNTRYCODE                  = 0x05U, /*<! Coutry code used in GET_COUNTRY_INFO message           */
  MONITOR_PARAM_MSG_SVDM_SVID                    = 0x06U, /*<! SVID used in SVDM disco mode, enter mode & exit mode   */
  MONITOR_PARAM_MSG_SVDM_MODEINDEX               = 0x07U, /*<! Mode index used in SVDM enter and exit mode messages   */
  MONITOR_PARAM_MSG_UVDM_DATA                    = 0x08U, /*<! Data used in UVDM message                              */
  MONITOR_PARAM_MSG_DP_STATUS                    = 0x09U, /*<! Data used in DP status message                         */
  MONITOR_PARAM_MSG_DP_CONFIGURE                 = 0x0AU, /*<! Data used in DP config message                         */
  MONITOR_PARAM_MSG_DP_ATTENTION                 = 0x0BU, /*<! Data used in DP attention message                      */
  MONITOR_PARAM_MSG_BATTERYREF                   = 0x0CU, /*<! Battery reference used in Get battery capa & status
                                                              message                                                 */
  MONITOR_PARAM_MSG_MANUINFODATA                 = 0x0DU, /*<! Manufacturer_Info data used in Get Manufacturer info
                                                              message                                                 */
  MONITOR_PARAM_MSG_FREE_TEXT                    = 0x0EU, /*<! Free text                                              */
  MONITOR_PARAM_MSG_ALL                          = (MONITOR_PARAM_MSG_FREE_TEXT + 1U), /*<! Number max of msg param   */
} utils_monitor_tag_param_msg_t;

/**
  * @brief  USBPD MONITOR MESSAGES.
  */
typedef enum
{
  MONITOR_MSG_GOTOMIN                            = 0x00U, /*<! Note: Refer to Control message in USB_PD specv3.0      */
  MONITOR_MSG_PING                               = 0x01U, /*<! Note: Refer to Control message in USB_PD specv3.0      */
  MONITOR_MSG_GET_SRC_CAPA                       = 0x03U, /*<! Note: Refer to Control message in USB_PD specv3.0      */
  MONITOR_MSG_GET_SNK_CAPA                       = 0x04U, /*<! Message sent only if port partner is DRP.
                                                              Answer to this message is a SNK_CAPA
                                                              Note: Refer to Control message in USB_PD specv3.0       */
  MONITOR_MSG_DR_SWAP                            = 0x05U, /*<! Note: Refer to Control message in USB_PD specv3.0      */
  MONITOR_MSG_PR_SWAP                            = 0x06U, /*<! Note: Refer to Control message in USB_PD specv3.0      */
  MONITOR_MSG_VCONN_SWAP                         = 0x07U, /*<! Note: Refer to Control message in USB_PD specv3.0      */
  MONITOR_MSG_SOFT_RESET                         = 0x08U, /*<! Note: Refer to Control message in USB_PD specv3.0      */
  MONITOR_MSG_GET_SOURCE_CAPA_EXTENDED           = 0x09U, /*<! Specific to PD3.0
                                                              Note: Refer to Control message in USB_PD specv3.0       */
  MONITOR_MSG_GET_STATUS                         = 0x0AU, /*<! Specific to PD3.0. Return a STATUS message
                                                              Note: Refer to Control message in USB_PD specv3.0       */
  MONITOR_MSG_FR_SWAP                            = 0x0BU, /*<! Specific to PD3.0
                                                              Note: Refer to Control message in USB_PD specv3.0       */
  MONITOR_MSG_GET_PPS_STATUS                     = 0x0CU, /*<! Specific to PD3.0. Wait for PPS_STATUS message
                                                              Note: Refer to Control message in USB_PD specv3.0       */
  MONITOR_MSG_GET_COUNTRY_CODES                  = 0x0DU, /*<! Specific to PD3.0. Return a COUNTRY_CODES message
                                                              Note: Refer to Control message in USB_PD specv3.0       */
  MONITOR_MSG_SOURCE_CAPA                        = 0x0EU, /*<! DPM_CONFIG_SET_REQ to be sent before sending this
                                                               message.
                                                              Note: Refer to data message in USB_PD specv3.0          */
  MONITOR_MSG_REQUEST                            = 0x0FU, /*<! Select one of the capability available.
                                                              Note: Refer to data message in USB_PD specv3.0          */
  MONITOR_MSG_ALERT                              = 0x11U, /*<! Specific to PD3.0
                                                              Note: Refer to data message in USB_PD specv3.0          */
  MONITOR_MSG_GET_COUNTRY_INFO                   = 0x12U, /*<! Return COUNTRY_INFO message.
                                                              Note: Refer to data message in USB_PD specv3.0          */
  MONITOR_MSG_VDM_DISCO_IDENT                    = 0x13U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_VDM_DISCO_SVID                     = 0x14U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_VDM_DISCO_MODE                     = 0x15U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_VDM_ENTER_MODE                     = 0x16U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_VDM_EXIT_MODE                      = 0x17U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_VDM_ATTENTION                      = 0x18U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_VDM_UNSTRUCTURED                   = 0x19U, /*<! 1..7 x 32bit
                                                              Note: Refer to VDM message in USB_PD specv3.0           */
  MONITOR_MSG_FREE_TEXT                          = 0x1AU, /*<! Free text                                              */
  MONITOR_MSG_DISPLAY_PORT_STATUS                = 0x20U, /*<! 1..7 x 32bit
                                                              Note: Refer to VDM message in USB_PD specv3.0           */
  MONITOR_MSG_DISPLAY_PORT_CONFIG                = 0x21U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_DISPLAY_PORT_ATTENTION             = 0x22U, /*<! Note: Refer to VDM message in USB_PD specv3.0          */
  MONITOR_MSG_HARD_RESET                         = 0x24U, /*<! Note: refer to Reset messages in USB_PD specv3.0       */
  MONITOR_MSG_CABLE_RESET                        = 0x25U, /*<! Note: refer to Reset messages in USB_PD specv3.0       */
  MONITOR_MSG_GET_BAT_CAPA                       = 0x26U, /*<! Return BAT_CAPA message
                                                              Note: Refer to extended message in USB_PD specv3.0      */
  MONITOR_MSG_GET_BAT_STATUS                     = 0x27U, /*<! Return BAT_STATUS message
                                                              Note: Refer to extended message in USB_PD specv3.0      */
  MONITOR_MSG_GET_MANU_INFO                      = 0x28U, /*<! Return MANU_INFO message
                                                              Note: Refer to extended message in USB_PD specv3.0      */
  MONITOR_MSG_SECU_REQUEST                       = 0x29U, /*<! Note: Refer to extended message in USB_PD specv3.0     */
  MONITOR_MSG_FIRM_UPDATE_REQUEST                = 0x2AU, /*<! Note: Refer to extended message in USB_PD specv3.0     */
  MONITOR_MSG_GET_SINK_CAPA_EXTENDED             = 0x2BU, /*<! Specific to PD3.0
                                                              Note: Refer to Control message in USB_PD specv3.0       */
} utils_monitor_messages_t;

/**
  * @brief  USBPD MONITOR INIT.
  */
typedef enum
{
  MONITOR_INIT_HWBOARDVERSION                    = 0x00U, /*<! ASCII stream to indicate STM32 name like STM32F032xB   */
  MONITOR_INIT_HWPDTYPE                          = 0x01U, /*<! ASCII stream to indicate HW PD type used by the device */
  MONITOR_INIT_NBPORTMAX                         = 0x02U, /*<! Indicate maximum number of ports which can be
                                                              configured in the device (value 1, 2 , 3)               */
  MONITOR_INIT_FWVERSION                         = 0x03U, /*<! 4 bytes for FW version + 4 bytes for Stack version     */
  MONITOR_INIT_TYPECSPECREVISION                 = 0x04U, /*<! Type-C revision supported by the port                  */
  MONITOR_INIT_PDSPECREVISION                    = 0x04U, /*<! PD spec revision supported by the port                 */
  MONITOR_INIT_EXTENDEDMESSAGESUNCKUNKED         = 0x06U, /*<! Supports extended messages                             */
  MONITOR_INIT_ACCESSORYSUPP                     = 0x07U, /*<! Accessory supported or not                             */
  MONITOR_INIT_POWERACCESSORYDETECTION           = 0x08U, /*<! It enables or disables powered accessory detection     */
  MONITOR_INIT_POWERACCESSORYTRANSITION          = 0x09U, /*<! It enables or disables transition from
                                                              Powered.accessory to Try.SNK                            */
  MONITOR_INIT_ISCABLE                           = 0x0BU, /*<! Cable (asserts Ra on VConn, communicates using SOP'
                                                              and SOP''), when this mode is selected the stack will
                                                              run in cable mode so all parameters like pwr_role are
                                                              not used                                                */
  MONITOR_INIT_TRYFEATURE                        = 0x10U, /*<! Try Feature                                            */
  MONITOR_INIT_RPRESISTORVALUE                   = 0x12U, /*<! RP resistor value                                      */
  MONITOR_INIT_USBSUPPORT                        = 0x13U, /*<! USB_Comms_Capable, is the UUT capable of enumerating
                                                              as a USB host or device?                                */
  MONITOR_INIT_USBDEVICE                         = 0x14U, /*<! Type_C_Can_Act_As_Device, Indicates whether the UUT can
                                                              communicate with USB 2.0 or USB 3.1 as a device or as
                                                              the Upstream Facing Port of a hub.                      */
  MONITOR_INIT_USBHOST                           = 0x15U, /*<! Type_C_Can_Act_As_Host, Indicates whether the UUT can
                                                              communicate with USB 2.0 or USB 3.1 as a host or
                                                              as the Downstream Facing Port of a hub                  */
  MONITOR_INIT_UNCONSTRAINED_POWERED             = 0x16U, /*<! UUT has an external power source available that is
                                                              sufficient to adequately power the system while charging
                                                              external devices or the UUTs primary function is to
                                                              charge external devices.                                */
  MONITOR_INIT_USBSUSPENDSUPPORT                 = 0x17U, /*<! USB Suspend support                                    */
  MONITOR_INIT_VCONNDISCHARGE                    = 0x18U, /*<! It enables or disables the VCONN discharge on CC pin   */
  MONITOR_INIT_VCONNILIM                         = 0x19U, /*<! enables or not VconnLim                                */
  MONITOR_INIT_VCONNILIMVALUE                    = 0x1AU, /*<! It allows changing the default current limit supplying
                                                              VCONN on the CC pins                                    */
  MONITOR_INIT_VCONNMONITORING                   = 0x1BU, /*<! It enables or disables UVLO threshold detection
                                                              on VCONN pin                                            */
  MONITOR_INIT_VCONNTHRESHOLDUVLO                = 0x1CU, /*<! High UVLO threshold of 4.65 V; 1b: Low UVLO threshold
                                                              of 2.65 V (case of VCONN-powered accessories
                                                              operating down to 2.7 V)                                */
  MONITOR_INIT_VCONNSUPPLY                       = 0x1DU, /*<! It enables or disables the VCONN supply
                                                              capability on CC pin                                    */
  MONITOR_INIT_NB_PORT_START                     = 0x1EU, /*<! Start Port number                                      */
  MONITOR_INIT_ORIGINAL_SETTINGS                 = 0x1FU, /*<! This flag allows to know if the user settings read by
                                                              the FW are from the original FW or a previous
                                                              configuration saved by the user.                        */
} utils_monitor_init_t;
/**
  * @brief USBPD MONITOR User Event.
  */
typedef enum
{
  MONITOR_USER_EVENT_TIMER                       = 0U,   /*<! TIMER EVENT                                             */
  MONITOR_USER_EVENT_MONITOR                     = 1U,   /*<! MONITOR EVENT                                           */
  MONITOR_USER_EVENT_NONE                        = 2U,   /*<! NO EVENT                                                */
} utils_monitor_user_event_t;

/**
  * @brief  USBPD MONITOR Handle definition.
  * @{
  */
typedef struct
{
  uint32_t              ListOfRcvSRCPDO[USBPD_MAX_NB_PDO];   /*!< The list of received Source Power Data Objects
                                                                  from Port partner (when Port partner is a Source
                                                                  or a DRP port).                                     */
  uint32_t              NumberOfRcvSRCPDO;                   /*!< The number of received Source Power Data Objects
                                                                  from port Partner (when Port partner is a Source
                                                                  or a DRP port). This parameter must be set
                                                                  to a value lower than USBPD_MAX_NB_PDO              */
  uint32_t              ListOfRcvSNKPDO[USBPD_MAX_NB_PDO];   /*!< The list of received Sink Power Data Objects
                                                                  from Port partner (when Port partner is a Sink
                                                                  or a DRP port).                                     */
  uint32_t              NumberOfRcvSNKPDO;                   /*!< The number of received Sink Power Data Objects
                                                                  from port Partner(when Port partner is a Sink
                                                                  or a DRP port). This parameter must be set to
                                                                  a value lower than USBPD_MAX_NB_PDO                 */
  uint32_t              RDOPosition;                         /*!< RDO Position of requested DO in Source list
                                                                  of capabilities */
  uint32_t              voltage_requested;                    /*!< Value of requested voltage                          */
  uint32_t              RequestedCurrent;                    /*!< Value of requested current                          */
  uint32_t              RDOPositionPrevious;                 /*!< RDO Position of previous requested DO
                                                                  in Source list of capabilities                      */
  uint32_t              RcvRequestDOMsg;                     /*!< Received request Power Data Object message
                                                                  from the port Partner                               */
#if _STATUS
  usbpd_sdb_t     RcvStatus;                                 /*!< Status received by port partner                     */
#endif /* _STATUS */
#if _PPS
  usbpd_ppsdb_t  RcvPPSStatus;                               /*!< PPS Status received by port partner                 */
#endif /* _PPS */
#if _SRC_CAPA_EXT
  usbpd_scedb_t   RcvSRCExtendedCapa;                        /*!< SRC Extended Capability received by port partner    */
#endif /* _SRC_CAPA_EXT */
#if defined(USBPDCORE_SNK_CAPA_EXT)
  usbpd_skedb_t   RcvSNKExtendedCapa;                        /*!< SNK Extended Capability received by port partner    */
#endif /* USBPDCORE_SNK_CAPA_EXT */
#if _MANU_INFO
  usbpd_gmidb_t   GetManufacturerInfo;                       /*!< Get Manufacturer Info                               */
#endif /* _MANU_INFO */
#if _BATTERY
  usbpd_gbsdb_t   GetBatteryStatus;                          /*!< Get Battery status                                  */
  usbpd_gbcdb_t   GetBatteryCapability;                      /*!< Get Battery Capability                              */
  usbpd_bsdo_t    BatteryStatus;                             /*!< Battery status                                      */
#endif /* _BATTERY */
  usbpd_alert_data_object_t     RcvAlert;                    /*!< Save the Alert received by port partner             */
#ifdef _VCONN_SUPPORT
  usbpd_vdm_discovery_identity_t VDM_DiscoCableIdentify;     /*!< VDM Cable Discovery Identify                        */
#endif /* _VCONN_SUPPORT */
#ifdef _VDM
  usbpd_vdm_discovery_identity_t   VDM_DiscoIdentify;        /*!< VDM Discovery Identify                              */
  USBPD_SVIDPortPartnerInfo_TypeDef VDM_SVIDPortPartner;     /*!< VDM SVID list                                       */
  usbpd_vdm_mode_info_t            VDM_ModesPortPartner;     /*!< VDM modes list                                      */
#endif /* _VDM */
} utils_monitor_handle_t;

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_Private_Constants USBPD_UTILS_MONITOR Private Constants
  * @{
  */

#define MONITOR_DEFAULT_WAIT         (0xFFFFFFFFU) /*!< Default value for waiting time */

#if !defined(USBPD_FW_VERSION)
#define USBPD_FW_VERSION             (0xFFFFFFFFU) /*!< Default value for FW version */
#endif /* !USBPD_FW_VERSION */

#if !defined(USBPD_START_PORT_NUMBER)
#define USBPD_START_PORT_NUMBER      (0U) /*!< Default value for start port number */
#endif /* !USBPD_START_PORT_NUMBER */

#define MONITOR_PORT_BIT_POSITION    (5U) /*!< Bit position of port number in TAG id */

#define MONITOR_BOX_MESSAGES_MAX     (30U) /*!< Maximum number of messages stored in the box */

#define MONITOR_TIMER_ENABLE_MSK     ((uint16_t)0x8000U) /*!< Enable Timer Mask */
#define MONITOR_TIMER_READ_MSK       ((uint16_t)0x7FFFU) /*!< Read Timer Mask   */

#if defined(USBPDCORE_SNK_CAPA_EXT)
#define MONITOR_VDM_CABLE_INFO      (USBPD_CORE_DATAINFO_SNK_EXTENDED_CAPA + 1U) /*!< VDM Cable Info */
#else
#define MONITOR_VDM_CABLE_INFO      (USBPD_CORE_DATAINFO_UNSTRUCTURED_VDM + 1U) /*!< VDM Cable Info */
#endif /* USBPDCORE_SNK_CAPA_EXT */

#if defined(_RTOS)
#if (osCMSIS < 0x20000U)
#define MONITOR_STACK_SIZE_ADDON_FOR_CMSIS (1U) /*<! Addon stack size for CMSIS < 2.0.0 */
#else
#define MONITOR_STACK_SIZE_ADDON_FOR_CMSIS (5U) /*<! Addon stack size for CMSIS >= 2.0.0 */
#endif /* osCMSIS < 0x20000U */
#define OS_MONITOR_PRIORITY                osPriorityLow /*<! Priority */
#define OS_MONITOR_STACK_SIZE              (240U * MONITOR_STACK_SIZE_ADDON_FOR_CMSIS) /*<! Stack size */
#elif defined(USBPD_THREADX)
#define OS_MONITOR_PRIORITY                (1U) /*<! Priority */
#define OS_MONITOR_STACK_SIZE              (2048U) /*<! Stack size */
#endif /*_RTOS*/

#define MONITOR_REG_NUMBER_OF_REGISTERS    (39U) /*<! Number of registers */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_Private_Macros USBPD_UTILS_MONITOR Private Macros
  * @{
  */

/**
  * @brief  Set the tag ID for a specific port and tag.
  * @param  PORT The port number.
  * @param  TAG The tag value.
  * @retval The tag ID.
  */
#define MONITOR_SET_TAG_ID(PORT, TAG) (((PORT) << MONITOR_PORT_BIT_POSITION) | (TAG))

#if defined(_RTOS) || defined(USBPD_THREADX)
/**
  * @brief  Start a timer for a specific port with a given timeout.
  * @param  PORT The port number.
  * @param  TIMER The timer array.
  * @param  TIMEOUT The timeout value.
  */
#define MONITOR_START_TIMER(PORT, TIMER, TIMEOUT)                                          \
  do                                                                                       \
  {                                                                                        \
    TIMER[PORT] = (TIMEOUT) |  MONITOR_TIMER_ENABLE_MSK;                                   \
    MONITOROS_PUT_MESSAGE_QUEUE(usbpd_utils_monitor_msgbox, MONITOR_USER_EVENT_TIMER, 0U); \
  } while(0U);
#else
/**
  * @brief  Start a timer for a specific port with a given timeout.
  * @param  PORT The port number.
  * @param  TIMER The timer array.
  * @param  TIMEOUT The timeout value.
  */
#define MONITOR_START_TIMER(PORT, TIMER, TIMEOUT) TIMER[PORT] = (TIMEOUT) |  MONITOR_TIMER_ENABLE_MSK;
#endif /* _RTOS || USBPD_THREADX */

/**
  * @brief  Check if a timer is running for a specific port.
  * @param  PORT The port number.
  * @param  TIMER The timer array.
  * @retval True if the timer is running, false otherwise.
  */
#define IS_MONITOR_TIMER_RUNNING(PORT, TIMER) ((uint16_t)((TIMER)[(PORT)] & MONITOR_TIMER_READ_MSK) > 0U)

/**
  * @brief  Check if a timer has expired for a specific port.
  * @param  PORT The port number.
  * @param  TIMER The timer array.
  * @retval True if the timer has expired, false otherwise.
  */
#define IS_MONITOR_TIMER_EXPIRED(PORT, TIMER) (MONITOR_TIMER_ENABLE_MSK == (TIMER)[(PORT)])

#if defined(USBPD_SNK) || defined(USBPD_DRP)

/**
  * @brief  Update the minimum voltage value.
  * @param  PDO_VOLT The PDO voltage value.
  * @param  SNK_VOLT The current minimum voltage value.
  */
#define MONITOR_UPDATE_VOLTAGE_MIN(PDO_VOLT, SNK_VOLT)             \
  (SNK_VOLT) = ((PDO_VOLT) < (SNK_VOLT)) ? (PDO_VOLT): (SNK_VOLT);

/**
  * @brief  Update the maximum voltage value.
  * @param  PDO_VOLT The PDO voltage value.
  * @param  SNK_VOLT The current maximum voltage value.
  */
#define MONITOR_UPDATE_VOLTAGE_MAX(PDO_VOLT, SNK_VOLT)             \
  (SNK_VOLT) = ((PDO_VOLT) > (SNK_VOLT)) ? (PDO_VOLT): (SNK_VOLT);

/**
  * @brief  Update the maximum current value.
  * @param  PDO_CURRENT The PDO current value.
  * @param  SNK_CURRENT The current maximum current value.
  */
#define MONITOR_UPDATE_CURRENT_MAX(PDO_CURRENT, SNK_CURRENT)                      \
  (SNK_CURRENT) = ((PDO_CURRENT) > (SNK_CURRENT)) ? (PDO_CURRENT): (SNK_CURRENT);

/**
  * @brief  Update the maximum power value.
  * @param  PDO_POWER The PDO power value.
  * @param  SNK_POWER The current maximum power value.
  */
#define MONITOR_UPDATE_POWER_MAX(PDO_POWER, SNK_POWER)              \
  SNK_POWER = (PDO_POWER) > (SNK_POWER) ? (PDO_POWER): (SNK_POWER);

/**
  * @brief  Check and update the minimum voltage value.
  * @param  PDO_VOLT The PDO voltage value.
  * @param  SNK_VOLT The current minimum voltage value.
  */
#define MONITOR_CHECK_VOLTAGE_MIN(PDO_VOLT, SNK_VOLT) (SNK_VOLT) = (PDO_VOLT);

/**
  * @brief  Check and update the maximum voltage value.
  * @param  PDO_VOLT The PDO voltage value.
  * @param  SNK_VOLT The current maximum voltage value.
  */
#define MONITOR_CHECK_VOLTAGE_MAX(PDO_VOLT, SNK_VOLT) (SNK_VOLT) = (PDO_VOLT);

/**
  * @brief  Check and update the maximum current value.
  * @param  PDO_CURRENT The PDO current value.
  * @param  SNK_CURRENT The current maximum current value.
  */
#define MONITOR_CHECK_CURRENT_MAX(PDO_CURRENT, SNK_CURRENT) (SNK_CURRENT) = (PDO_CURRENT);

/**
  * @brief  Check and update the maximum power value.
  * @param  PDO_POWER The PDO power value.
  * @param  SNK_POWER The current maximum power value.
  */
#define MONITOR_CHECK_POWER_MAX(PDO_POWER, SNK_POWER) (SNK_POWER) = (PDO_POWER);

/**
  * @brief  Decode a value from 50mV multiples to mV.
  * @param  VALUE The value in 50mV multiples.
  * @retval The decoded value in mV.
  */
#define MONITOR_DECODE_50MV(VALUE)  ((uint16_t)(((VALUE) * 50U)))

/**
  * @brief  Decode a value from 100mV multiples to mV.
  * @param  VALUE The value in 100mV multiples.
  * @retval The decoded value in mV.
  */
#define MONITOR_DECODE_100MV(VALUE) ((uint16_t)(((VALUE) * 100U)))

/**
  * @brief  Decode a value from 10mA multiples to mA.
  * @param  VALUE The value in 10mA multiples.
  * @retval The decoded value in mA.
  */
#define MONITOR_DECODE_10MA(VALUE)  ((uint16_t)(((VALUE) * 10U)))

/**
  * @brief  Decode a value from 50mA multiples to mA.
  * @param  VALUE The value in 50mA multiples.
  * @retval The decoded value in mA.
  */
#define MONITOR_DECODE_50MA(VALUE)  ((uint16_t)(((VALUE) * 50U)))

/**
  * @brief  Decode a value from 250mW multiples to mW.
  * @param  VALUE The value in 250mW multiples.
  * @retval The decoded value in mW.
  */
#define MONITOR_DECODE_250MW(VALUE) ((uint16_t)(((VALUE) * 250U)))

#endif /* USBPD_SNK) || USBPD_DRP */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_Private_Functions USBPD_UTILS_MONITOR Private Functions
  * @{
  */

extern uint32_t HAL_GetTick(void);
extern void HAL_NVIC_SystemReset(void);

#if defined(_RTOS) || defined(USBPD_THREADX)
MONITORDEF_TASK_FUNCTION(UTILS_MONITOR_Task);
static uint32_t            UTILS_MONITOR_CheckTimers(void);
#endif /* _RTOS || USBPD_THREADX */

static void                UTILS_MONITOR_RxCallback(uint8_t Character, uint8_t Error);
static void                UTILS_MONITOR_SendDPMInitCnf(uint8_t port_num, uint8_t *string);
static void                UTILS_MONITOR_RequestMessageReq(uint8_t port_num, uint8_t *instruction,
                                                           uint8_t *pEncodedMsg);
static void                UTILS_MONITOR_SendDpmConfigSetCnf(uint8_t port_num, uint8_t *instruction,
                                                             uint8_t *pEncodedMsg);
static void                UTILS_MONITOR_SendDpmConfigGetCnf(uint8_t port_num, uint8_t *instruction,
                                                             uint8_t *pEncodedMsg);
static void                UTILS_MONITOR_SendDpmConfigGetRej(uint8_t port_num, uint8_t *pEncodedMsg,
                                                             utils_monitor_reject_reason_t RejecReason);
static void                UTILS_MONITOR_SendDpmMessageRej(uint8_t port_num, uint8_t *pEncodedMsg,
                                                           utils_monitor_reject_reason_t RejecReason);
static void                UTILS_MONITOR_SendDpmRegisterReadCnf(uint8_t port_num, uint8_t *string, uint8_t param);
static void                UTILS_MONITOR_SendDpmRegisterWriteCnf(uint8_t port_num, uint8_t *string, uint8_t *toProcess);
static usbpd_status_t UTILS_MONITOR_ManageFreeText(uint8_t port_num, uint8_t *pPayload, uint16_t size);
#if defined(USBPD_SNK) || defined(USBPD_DRP)
static void                UTILS_MONITOR_UpdateSNKPowerPort0(void);
#if USBPD_PORT_COUNT==2
static void                UTILS_MONITOR_UpdateSNKPowerPort1(void);
#endif /* USBPD_PORT_COUNT==2 */
#endif /* USBPD_SNK) || USBPD_DRP */
#if defined(USE_STM32_UTILITY_OS)
void                       UTILS_MONITOR_Execute(void);
#endif /* USE_STM32_UTILITY_OS */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_Private_Variables USBPD_UTILS_MONITOR Private Variables
  * @{
  */

volatile uint16_t utils_monitor_timer_meas_report[USBPD_PORT_COUNT]; /*!< Timer used to send measurement report*/

usbpd_utils_monitor_user_params_t usbpd_utils_monitor_user_params[USBPD_PORT_COUNT]; /*!< Monitor user parameters */

uint8_t p_data_rx[USBPD_UTILS_MONITOR_TLV_SIZE_MAX]; /*!< Buffer used to store the received data */
uint8_t p_processed_data[USBPD_UTILS_MONITOR_TLV_SIZE_MAX]; /*!< Buffer used to store the processed data */

usbpd_utils_monitor_state_t usbpd_utils_monitor_state = MONITOR_STATE_INIT;

uint8_t flag_monitor_original_settings; /*!< Flag to indicate that user settings is from original FW (not
                                             saved into external data) */

#if defined(_RTOS) || defined(USBPD_THREADX)
MONITOROS_QUEUE_ID  usbpd_utils_monitor_msgbox; /*!< Queue used to store the messages to be processed */
MONITOROS_TASK_ID usbpd_utils_monitor_thread_id; /*!< Thread used to manage the monitor */
#else
volatile utils_monitor_user_event_t usbpd_utils_monitor_eventflag = MONITOR_USER_EVENT_NONE; /*!< Event flag used to
                                                                                              manage the monitor */
#endif /* _RTOS || USBPD_THREADX */

const uint8_t *(*pp_hw_board_version_cb)(void)  = NULL; /*!< Callback function to retrieve the hardware board version */
const uint8_t *(*pp_hw_pd_type_cb)(void)        = NULL; /*!< Callback function to retrieve the hardware PD type */
uint16_t (*pp_get_voltage_cb)(uint8_t)         = NULL;  /*!< Callback function to retrieve the VBUS voltage */
int16_t (*pp_get_current_cb)(uint8_t)         = NULL;   /*!< Callback function to retrieve the IBUS current */
usbpd_status_t(*pp_free_text_cb)(uint8_t, uint8_t *, uint16_t) = NULL; /*!< Callback function to manage FreeText */

utils_monitor_handle_t hmonitor[USBPD_PORT_COUNT]; /*!< Monitor handle */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_UTILS_MONITOR_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes the USBPD monitor.
  * @param  p_hw_board_version_cb Callback function to retrieve the hardware board version.
  * @param  p_hw_pd_type_cb Callback function to retrieve the hardware PD type.
  * @param  p_get_voltage_cb Callback function to retrieve the VBUS voltage.
  * @param  p_get_current_cb Callback function to retrieve the IBUS current.
  * @return The functional state of the monitor after initialization of type @ref usbpd_functional_status_t.
  */
usbpd_functional_status_t USBPD_UTILS_MONITOR_Init(const uint8_t *(*p_hw_board_version_cb)(void),
                                                   const uint8_t *(*p_hw_pd_type_cb)(void),
                                                   uint16_t (*p_get_voltage_cb)(uint8_t),
                                                   int16_t (*p_get_current_cb)(uint8_t))
{
  usbpd_functional_status_t _status = USBPD_ENABLE;

  /* Retrieve data from FLASH if needed */
  flag_monitor_original_settings = ((USBPD_MONITOR_HW_IF_OK == USBPD_UTILS_MONITOR_HW_IF_LoadExternalData()) ?
                                    USBPD_FALSE : USBPD_TRUE);

  /* Need to update cad_tdrp & cad_dcsrc_rp if cad_snk_toggle_time_ms not initialized */
  if ((0U == usbpd_dpm_settings[USBPD_PORT_0].cad_snk_toggle_time_ms)
      || (0U == usbpd_dpm_settings[USBPD_PORT_0].cad_src_toggle_time_ms))
  {
    DPM_USER_Settings[USBPD_PORT_0].cad_tdrp      = 80U;
    DPM_USER_Settings[USBPD_PORT_0].cad_dcsrc_rp = 50U;
  }
  else
  {
    DPM_USER_Settings[USBPD_PORT_0].cad_dcsrc_rp = (usbpd_dpm_settings[USBPD_PORT_0].cad_src_toggle_time_ms * 100U) /
                                                   (usbpd_dpm_settings[USBPD_PORT_0].cad_src_toggle_time_ms +
                                                    usbpd_dpm_settings[USBPD_PORT_0].cad_snk_toggle_time_ms);
    DPM_USER_Settings[USBPD_PORT_0].cad_tdrp = (usbpd_dpm_settings[USBPD_PORT_0].cad_src_toggle_time_ms * 100U) /
                                               DPM_USER_Settings[USBPD_PORT_0].cad_dcsrc_rp;
  }
#if USBPD_PORT_COUNT==2
  if ((0U == usbpd_dpm_settings[USBPD_PORT_1].cad_snk_toggle_time_ms)
      || (0U == usbpd_dpm_settings[USBPD_PORT_1].cad_src_toggle_time_ms))
  {
    DPM_USER_Settings[USBPD_PORT_1].cad_tdrp      = 80U;
    DPM_USER_Settings[USBPD_PORT_1].cad_dcsrc_rp = 50U;
  }
  else
  {
    DPM_USER_Settings[USBPD_PORT_1].cad_dcsrc_rp = (usbpd_dpm_settings[USBPD_PORT_1].cad_src_toggle_time_ms * 100U) /
                                                   (usbpd_dpm_settings[USBPD_PORT_1].cad_src_toggle_time_ms +
                                                    usbpd_dpm_settings[USBPD_PORT_1].cad_snk_toggle_time_ms);
    DPM_USER_Settings[USBPD_PORT_1].cad_tdrp = (usbpd_dpm_settings[USBPD_PORT_1].cad_src_toggle_time_ms * 100U) /
                                               DPM_USER_Settings[USBPD_PORT_1].cad_dcsrc_rp;
  }
#endif /* USBPD_PORT_COUNT==2 */


  pp_hw_board_version_cb  = p_hw_board_version_cb;
  pp_hw_pd_type_cb        = p_hw_pd_type_cb;
  pp_get_voltage_cb      = p_get_voltage_cb;
  pp_get_current_cb      = p_get_current_cb;

  /* Register 2 callbacks for notification in DPM */
  USBPD_DPM_SetNotification_MONITOR(USBPD_UTILS_MONITOR_FormatAndSendNotification,
                                    USBPD_UTILS_MONITOR_PostNotificationMessage, USBPD_UTILS_MONITOR_SaveInfo);

#if !defined(USBPD_THREADX)
  _status = (usbpd_functional_status_t) USBPD_UTILS_MONITOR_InitOS(NULL);
#endif /* !USBPD_THREADX */
  return _status;
}

uint32_t USBPD_UTILS_MONITOR_InitOS(void *p_memory)
{
  static uint8_t _init = 0U;
  MONITOROS_INIT();

  /* This init check to keep the compatibility with the previous version of the MONITOR */
  /* Only threadX required to separate the OS object init with the init code.       */
  if (_init == 0U)
  {
    _init = 1U;
#if defined(_RTOS) || defined(USBPD_THREADX)
    MONITOROS_CREATE_QUEUE(usbpd_utils_monitor_msgbox, "MONITORBOX", MONITOR_BOX_MESSAGES_MAX, MONITOROS_ELEMENT_SIZE);
    MONITOROS_CREATE_TASK(usbpd_utils_monitor_thread_id, MONITOR, UTILS_MONITOR_Task, OS_MONITOR_PRIORITY,
                          OS_MONITOR_STACK_SIZE, &usbpd_utils_monitor_msgbox);
#else
    USBPD_UTILS_MONITOR_Start();
#if defined(USE_STM32_UTILITY_OS)
    UTIL_SEQ_RegTask(TASK_MONITOR, 0U, UTILS_MONITOR_Execute);
    UTIL_SEQ_SetTask(TASK_MONITOR, 0U);
#endif /* USE_STM32_UTILITY_OS */
#endif /* _RTOS || USBPD_THREADX */
  }

#if defined(_RTOS) || defined(USBPD_THREADX)
error:
  return (_status);
#else
  return (USBPD_ENABLE);
#endif /* _RTOS || USBPD_THREADX */
}

void USBPD_UTILS_MONITOR_Reset(void)
{
  USBPD_UTILS_MONITOR_HW_IF_ResetExternalData();
}

void USBPD_UTILS_MONITOR_Start(void)
{
  /* Register Rx complete callback */
  USBPD_UTILS_TRACER_Register_Rx_Cplt_Callback(UTILS_MONITOR_RxCallback);

  /* register the MONITOR callback to manage UART reception */
  USBPD_UTILS_TRACER_StartRX();
}

/**
  * @brief  MONITOR task.
  * @param  pEvent MONITOR User event.
  */
#if defined(_RTOS) || defined(USBPD_THREADX)
MONITORDEF_TASK_FUNCTION(UTILS_MONITOR_Task)
#else
static void UTILS_MONITOR_Execute(void)
#endif /* _RTOS || USBPD_THREADX */
{
#if defined(_RTOS) || defined(USBPD_THREADX)
  uint32_t _timing = MONITOR_DEFAULT_WAIT;
  USBPD_UTILS_MONITOR_Start();
  do
  {
    MONITOROS_QUEUE_EVENT event = 0U;
    MONITOROS_GETMESSAGE_QUEUE(usbpd_utils_monitor_msgbox, _timing, event);
    switch ((utils_monitor_user_event_t)(event & 0xFU))
    {
      case MONITOR_USER_EVENT_MONITOR:
      {
        (void)USBPD_UTILS_MONITOR_RXProcess((uint32_t)event);
        /* Sent an event to check if measurement report has been requested */
        MONITOROS_PUT_MESSAGE_QUEUE(usbpd_utils_monitor_msgbox, MONITOR_USER_EVENT_TIMER, 0U);
        break;
      }
      case MONITOR_USER_EVENT_TIMER:
      {
#endif /* _RTOS || USBPD_THREADX */
#if !(defined(_RTOS) || defined(USBPD_THREADX))
        switch (usbpd_utils_monitor_eventflag & 0xFU)
        {
          case MONITOR_USER_EVENT_NONE:
            break;
          case MONITOR_USER_EVENT_MONITOR:
            USBPD_UTILS_MONITOR_RXProcess(usbpd_utils_monitor_eventflag);
          case MONITOR_USER_EVENT_TIMER:
            break;
          default:
            break;
        }
        usbpd_utils_monitor_eventflag = MONITOR_USER_EVENT_NONE;
#endif /* _RTOS || USBPD_THREADX */
        for (uint8_t _instance = 0U; _instance < (uint8_t)USBPD_PORT_COUNT; _instance++)
        {
          /* -------------------------------------------------  */
          /* Check if timeout related to Measurement reporting  */
          /* -------------------------------------------------  */
          /* - Send a MONITOR Event only if PE is connected
             and Measurement report has been enabled          */
          if ((USBPD_TRUE == usbpd_dpm_params[_instance].pe_flag_is_connected)
              && (1U == usbpd_utils_monitor_user_params[_instance].u.b.measurement_report_enable)
              && (0U != usbpd_utils_monitor_user_params[_instance].u.b.measurement_report_value))
          {
            /* Check if timer has expired */
            if (IS_MONITOR_TIMER_EXPIRED(_instance, utils_monitor_timer_meas_report))
            {
              uint32_t event_mr = MONITOR_USER_EVENT_MONITOR
                                  | (_instance << MONITOR_PE_PORT_NUM_POS)
                                  | (MONITOR_NOTIF_MEASUREMENT << MONITOR_PE_NOTIF_POS);
              (void)USBPD_UTILS_MONITOR_RXProcess(event_mr);
            }
            /* Start or Restart Measurement report timer */
            if (!(IS_MONITOR_TIMER_RUNNING(_instance, utils_monitor_timer_meas_report)))
            {
              MONITOR_START_TIMER(_instance, utils_monitor_timer_meas_report,
                                  (uint16_t)(usbpd_utils_monitor_user_params[_instance].u.b.measurement_report_value) *
                                  MONITOR_NOTIF_MEASUREMENT_STEP);
            }
          }
          else
          {
            /* Stop measurement report timer */
            utils_monitor_timer_meas_report[_instance] = 0U;
          }
        }
#if defined(_RTOS) || defined(USBPD_THREADX)
        break;
      }

      default:
        break;
    }
    _timing = UTILS_MONITOR_CheckTimers();
  } while (1U);
#endif /* _RTOS || USBPD_THREADX */
}

/**
  * @}
  */

/** @addtogroup USBPD_UTILS_MONITOR_Private_Functions USBPD_UTILS_MONITOR Private Functions
  * @{
  */

#if defined(_RTOS) || defined(USBPD_THREADX)
static uint32_t UTILS_MONITOR_CheckTimers(void)
{
  uint32_t _timing = MONITOR_DEFAULT_WAIT;
  uint32_t _current_timing;

  /* Calculate the minimum timers to wake-up MONITOR task */
  for (uint8_t instance = 0U; instance < (uint8_t)USBPD_PORT_COUNT; instance++)
  {
    /* Check if Measurement reporting has been enabled by the MONITOR */
    _current_timing = (uint32_t)(utils_monitor_timer_meas_report[instance]) & MONITOR_TIMER_READ_MSK;
    if (_current_timing > 0U)
    {
      if (_current_timing < _timing)
      {
        _timing = _current_timing;
      }
    }
  }
  return _timing;
}
#endif /* RTOS || USBPD_THREADX */

void USBPD_UTILS_MONITOR_TimerCounter(void)
{
  /* Increment MONITOR timer only if Measurement report has been activated */
  if (1U == usbpd_utils_monitor_user_params[USBPD_PORT_0].u.b.measurement_report_enable)
  {
    if ((utils_monitor_timer_meas_report[USBPD_PORT_0] & MONITOR_TIMER_READ_MSK) > 0U)
    {
      utils_monitor_timer_meas_report[USBPD_PORT_0]--;
    }
#if !(defined(_RTOS) || defined(USBPD_THREADX)) && defined(USE_STM32_UTILITY_OS)
    else
    {
      UTIL_SEQ_SetTask(TASK_MONITOR, 0U);
    }
#endif /* !(_RTOS || USBPD_THREADX) && USE_STM32_UTILITY_OS) */
  }
#if USBPD_PORT_COUNT==2
  if (1U == usbpd_utils_monitor_user_params[USBPD_PORT_1].u.b.measurement_report_enable)
  {
    if ((utils_monitor_timer_meas_report[USBPD_PORT_1] & MONITOR_TIMER_READ_MSK) > 0U)
    {
      utils_monitor_timer_meas_report[USBPD_PORT_1]--;
    }
#if !(defined(_RTOS) || defined(USBPD_THREADX)) && defined(USE_STM32_UTILITY_OS)
    else
    {
      UTIL_SEQ_SetTask(TASK_MONITOR, 0U);
    }
#endif /* !(_RTOS || USBPD_THREADX) && USE_STM32_UTILITY_OS) */
  }
#endif /* USBPD_PORT_COUNT == 2 */
}

/**
  * @brief  callback called to end a transfer.
  * @param  Character Byte received by the device.
  * @param  Error Error detected in the reception.
  * @retval 1 If message to send to DPM application.
  */
static void UTILS_MONITOR_RxCallback(uint8_t Character, uint8_t Error)
{
  uint32_t event;
  event = USBPD_UTILS_MONITOR_GetMessage(Character, Error);
  /* register the MONITOR callback to manage next UART reception */
  USBPD_UTILS_TRACER_StartRX();
  if (event == 1U)
  {
#if defined(_RTOS) || defined(USBPD_THREADX)
    MONITOROS_PUT_MESSAGE_QUEUE(usbpd_utils_monitor_msgbox, MONITOR_USER_EVENT_MONITOR, 0U);
#else
    usbpd_utils_monitor_eventflag = MONITOR_USER_EVENT_MONITOR;
#if defined(USE_STM32_UTILITY_OS)
    USBPD_UTILS_MONITOR_RXProcess(usbpd_utils_monitor_eventflag);
    usbpd_utils_monitor_eventflag = MONITOR_USER_EVENT_NONE;
#endif /* USE_STM32_UTILITY_OS */
#endif /* _RTOS */
  }
}

/**
  * @brief  Main Trace RX process to push data on the media.
  * @param  Event MONITOR trace event.
  * @retval TiminG.
  */
uint32_t USBPD_UTILS_MONITOR_RXProcess(uint32_t Event)
{
  uint8_t *msg;
  usbpd_utils_monitor_state_t state;
  uint8_t size;

  if (0U == (Event & MONITOR_PE_NOTIF_MSK))
  {
    /* Message have been received by MONITOR */
    state = USBPD_UTILS_MONITOR_SendAnswer(&msg, &size);

    switch (state)
    {
      case MONITOR_STATE_RESET:
        /* DPM_RESET_REQ received by the device */
        /* Save parameters in FLASH */
        (void)USBPD_UTILS_MONITOR_HW_IF_SaveExternalData();
        HAL_NVIC_SystemReset();
        break;
      case MONITOR_STATE_INIT:
        /* DPM_INIT_REQ received by the device */
        break;
      default:
        /* Another MONITOR messages received by the device */
        USBPD_UTILS_TRACER_Add(msg, size);
        break;
    }
  }
  else
  {
    uint16_t type_event = ((uint16_t)Event & MONITOR_PE_NOTIF_MSK) >> MONITOR_PE_NOTIF_POS;
    if (MONITOR_NOTIF_MEASUREMENT == type_event)
    {
      /* Notification related to timeout for measure reporting */
      /* Send a notification to associated port */
      USBPD_UTILS_MONITOR_FormatAndSendNotification(((Event & MONITOR_PE_PORT_NUM_MSK) >> MONITOR_PE_PORT_NUM_POS),
                                                    MONITOR_NOTIF_MEASUREREPORTING,
                                                    HAL_GetTick());
    }
    else
    {
      /* Message have been received by PE */
      /* Send a notification to associated port */
      USBPD_UTILS_MONITOR_FormatAndSendNotification(((Event & MONITOR_PE_PORT_NUM_MSK) >> MONITOR_PE_PORT_NUM_POS),
                                                    MONITOR_NOTIF_PE_EVENT,
                                                    type_event);
    }
  }
  return 0U;
}

/**
  * @brief  TRACE function to send a notification.
  * @param  port_num Index of current used port.
  * @param  notification_type Type of requested notification is a combination
  *         of @ref USBPD_TRACE_TRACE_TYPE_NOTIFICATION.
  * @param  value Value depending of notification_type.
  * @note   If notification_type == MONITOR_NOTIF_ISCONNECTED, Value must be
  *         equal to 0 (Not connected) or 1 (connected).
  * @retval USBPD Status.
  */
uint32_t USBPD_UTILS_MONITOR_FormatAndSendNotification(uint8_t port_num, uint32_t notification_type, uint32_t value)
{
  uint8_t *msg;
  uint8_t size;
  /* Only send notification if MONITOR is connected */
  if (MONITOR_STATE_INIT != USBPD_UTILS_MONITOR_SendNotification((uint8_t)port_num, &msg, &size,
                                                                 notification_type, value))
  {
    USBPD_UTILS_TRACER_Add(msg, size);
  }
  return 0U;
}


/**
  * @brief  Function Called through UART IT RX to fill the MONITOR RX buffer.
  * @param  Character Received byte.
  * @param  Error Error if issue during reception.
  * @retval 1 Indicates that a complete MONITOR message have been received.
  */
uint32_t USBPD_UTILS_MONITOR_GetMessage(uint8_t Character, uint8_t Error)
{
  uint32_t status = 0U;
  static uint16_t counter = 0U;   /* Allows for example to count the number of SOF or EOF we received consecutively */
  static uint16_t cursor = 0U;    /* Our position in the buffer */
  static uint16_t currentSize;    /* How many bytes we expect */
  static enum receptionStateMachine
  {
    Rstart,
    RonGoing,
    Rsize,
    Rend,
    Rerror
  } Rstate = Rstart;

  if (cursor < USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
  {
    p_data_rx[cursor] = Character;
  }
  else /* buffer too small, overflow */
  {
    cursor = 0U;
    counter = 0U;
    Rstate = Rstart;
    return 0U;
  }

  if (Error != 0U)
  {
    Rstate = Rerror;
  }

  switch (Rstate)
  {
    case Rstart:
      if (p_data_rx[cursor] == USBPD_UTILS_MONITOR_TLV_SOF)
      {
        counter++;
      }
      else
      {
        counter = 0U;
        /* cursor is unsigned, but it will inevitably increments at the end of function,
           and we need a 0 at the next pass here */
        cursor = 0xFFFFU;
      }

      if (counter == 4U) /* The whole SOF is received */
      {
        counter = 0U;
        Rstate = Rsize;
      }
      break;
    case Rsize:
      counter++;

      if (counter == 3U) /* We have received the size */
      {
        currentSize = (uint16_t)((p_data_rx[cursor - 1U]) << 8U) + (uint16_t)(p_data_rx[cursor]);
        if (0U == currentSize)
        {
          Rstate = Rend;
        }
        else
        {
          Rstate = RonGoing;
        }
        counter = 0U;
      }
      break;
    case RonGoing:
      counter++;

      if (counter == currentSize)
      {
        /* When there is no value, the first EOF is handled by Rongoing, before Rend takes control */
        counter = 0U;
        Rstate = Rend;
      }
      break;
    case Rend:
      counter++;
      if ((p_data_rx[cursor] == USBPD_UTILS_MONITOR_TLV_EOF)
          && (p_data_rx[cursor - 1U] == USBPD_UTILS_MONITOR_TLV_EOF)
          && (p_data_rx[cursor - 2U] == USBPD_UTILS_MONITOR_TLV_EOF)
          && (p_data_rx[cursor - 3U] == USBPD_UTILS_MONITOR_TLV_EOF))
      {
        /* The semaphore must be given only in this case, because otherwise
           it means we didn't receive the correct size of bytes */
        if (counter == 4U)
        {
          status = 1U;
        }
        counter = 0U;
        cursor = 0xFFFFU;
        Rstate = Rstart;
      }

      if (cursor == (currentSize + 11U))
      {
        /* No complete EOF arrived. We reset the buffer for safety even if the instruction might be complete. */
        counter = 0U;
        cursor = 0xFFFFU;
        Rstate = Rstart;
      }
      break;
    case Rerror:
      counter = 0U;
      cursor = 0xFFFFU;
      Rstate = Rstart;
      break;
    default:
      break;
  }

  cursor++;
  return status;
}

/**
  * @brief  Answer To a message received by the MONITOR.
  * @param  pMsgToSend Pointer on the message to send.
  * @param  pSizeMsg Pointer on the size of the message to send.
  * @retval MONITOR state.
  */
usbpd_utils_monitor_state_t USBPD_UTILS_MONITOR_SendAnswer(uint8_t **pMsgToSend, uint8_t *pSizeMsg)
{
  /* Extract the port from the tag*/
  uint8_t port = p_data_rx[USBPD_UTILS_MONITOR_TLV_TAG_POSITION] >> MONITOR_PORT_BIT_POSITION;

  /* Do the appropriate treatment in response to what we have received */
  switch (p_data_rx[USBPD_UTILS_MONITOR_TLV_TAG_POSITION] & 0x1FU)
  {
    case DPM_RESET_REQ:
      /* Reset*/
      return MONITOR_STATE_RESET;

    case DPM_INIT_REQ:
    {
      UTILS_MONITOR_SendDPMInitCnf(port, p_processed_data);
      *pMsgToSend = p_processed_data;
      *pSizeMsg = (uint8_t)(USBPD_UTILS_MONITOR_TLV_GetStringLength(p_processed_data) + 8U);
      USBPD_UTILS_TRACER_Add(*pMsgToSend, *pSizeMsg);
      usbpd_utils_monitor_state = MONITOR_STATE_RUNNING;
      if (0U == port)
      {
        /* Send a notification all the port */
        (void)USBPD_UTILS_MONITOR_FormatAndSendNotification(USBPD_PORT_0,
                                                            MONITOR_NOTIF_ISCONNECTED | MONITOR_NOTIF_PE_EVENT
                                                            | MONITOR_NOTIF_TIMESTAMP, USBPD_NOTIFY_ALL);
#if USBPD_PORT_COUNT==2
        (void)USBPD_UTILS_MONITOR_FormatAndSendNotification(USBPD_PORT_1,
                                                            MONITOR_NOTIF_ISCONNECTED | MONITOR_NOTIF_PE_EVENT
                                                            | MONITOR_NOTIF_TIMESTAMP, USBPD_NOTIFY_ALL);
#endif /* USBPD_PORT_COUNT == 2 */
      }
      return MONITOR_STATE_INIT;
    }

    case DPM_CONFIG_GET_REQ:
      if (0U != port)
      {
        UTILS_MONITOR_SendDpmConfigGetCnf((port - 1U), p_data_rx, p_processed_data);
      }
      else
      {
        UTILS_MONITOR_SendDpmConfigGetRej(port, p_processed_data, MONITOR_REJ_DPM_INVALID_PORT_NUMBER);
      }
      break;
    case DPM_CONFIG_SET_REQ:
      if (0U != port)
      {
        UTILS_MONITOR_SendDpmConfigSetCnf((port - 1U), p_data_rx, p_processed_data);
      }
      else
      {
        UTILS_MONITOR_SendDpmConfigGetRej(port, p_processed_data, MONITOR_REJ_DPM_INVALID_PORT_NUMBER);
      }
      break;
    case DPM_MESSAGE_REQ:
      if (0U != port)
      {
        UTILS_MONITOR_RequestMessageReq((port - 1U), p_data_rx, p_processed_data);
      }
      else
      {
        UTILS_MONITOR_SendDpmMessageRej(port, p_processed_data, MONITOR_REJ_DPM_INVALID_PORT_NUMBER);
      }
      break;
    case DPM_REGISTER_READ_REQ:
      if (0U != port)
      {
        /* If size is 0 */
        if ((p_data_rx[USBPD_UTILS_MONITOR_TLV_LENGTH_HIGH_POSITION] == 0U)
            && (p_data_rx[USBPD_UTILS_MONITOR_TLV_LENGTH_LOW_POSITION] == 0U))
        {
          UTILS_MONITOR_SendDpmRegisterReadCnf((port - 1U), p_processed_data, 0xFFU);
        }
        else
        {
          /* p_data_rx[USBPD_UTILS_MONITOR_TLV_VALUE_POSITION] is the tag in parameter,
             when only one parameter is needed (cf USBPD MONITOR specification)*/
          UTILS_MONITOR_SendDpmRegisterReadCnf((port - 1U), p_processed_data,
                                               p_data_rx[USBPD_UTILS_MONITOR_TLV_VALUE_POSITION]);
        }
      }
      else
      {
        UTILS_MONITOR_SendDpmConfigGetRej(port, p_processed_data, MONITOR_REJ_DPM_INVALID_PORT_NUMBER);
      }
      break;
    case DPM_REGISTER_WRITE_REQ:
      if (0U != port)
      {
        UTILS_MONITOR_SendDpmRegisterWriteCnf((port - 1U), p_processed_data, p_data_rx);
      }
      else
      {
        UTILS_MONITOR_SendDpmConfigGetRej(port, p_processed_data, MONITOR_REJ_DPM_INVALID_PORT_NUMBER);
      }
      break;
    default:
      break;
  }

  *pMsgToSend = p_processed_data;
  *pSizeMsg = (uint8_t)(USBPD_UTILS_MONITOR_TLV_GetStringLength(p_processed_data) + 8U);
  return usbpd_utils_monitor_state;
}

/**
  * @brief  Answer To a message received by the MONITOR.
  * @param  port_num Port number.
  * @param  pMsgToSend Pointer on the message to send.
  * @param  pSizeMsg Pointer on the size of the message to send.
  * @param  notification_type Type of the notification.
  * @param  value Value of the notification.
  * @retval MONITOR state.
  */
usbpd_utils_monitor_state_t USBPD_UTILS_MONITOR_SendNotification(uint8_t port_num, uint8_t **pMsgToSend,
                                                                 uint8_t *pSizeMsg, uint32_t notification_type,
                                                                 uint32_t value)
{
  usbpd_utils_monitor_tlv_tx_data_t send_tlv;
  usbpd_utils_monitor_state_t monitor_state = MONITOR_STATE_INIT;

  /* Send a notitification only if MONITOR is running */
  if (usbpd_utils_monitor_state == MONITOR_STATE_RUNNING)
  {
    (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, MONITOR_SET_TAG_ID((port_num + 1U), DPM_MESSAGE_IND),
                                             USBPD_UTILS_MONITOR_TLV_SIZE_MAX, p_processed_data);

    /* Check PD connection */
    if ((notification_type & MONITOR_NOTIF_ISCONNECTED) == MONITOR_NOTIF_ISCONNECTED)
    {
      /* Is Connected*/
      (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_ISCONNECTED, 1U,
                                        (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_pwr_status });
      if (USBPD_TRUE == usbpd_dpm_params[port_num].pe_flag_is_connected)
      {
        uint8_t rp_value;
        /* CC line */
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_CC, 1U,
                                          (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].active_cc });
        /* Power Role*/
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_POWERROLE, 1U,
                                          (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_power_role });
        /* CC Default Current Advertised */
        rp_value = 3U; /* (uint8_t)CAD_GetRPValue(port_num); */
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_CCDEFAULTCURRENTADVERTISED, 1U, &rp_value);
        if (USBPD_PWR_STATE_EXPLICITCONTRACT == (usbpd_pwr_state_t)usbpd_dpm_params[port_num].pe_pwr_status)
        {
          /* Data Role*/
          (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_DATAROLE, 1U,
                                            (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_data_role });
          /* Vconn ON*/
          (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_VCONNON, 1U,
                                            (uint8_t[]) { usbpd_dpm_params[port_num].vconn_status });
          /* PD_Spec Revision */
          (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_PD_SPECREVISION, 1U,
                                            (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_spec_revision });
        }
      }
    }

    /* Check PD message event */
    if ((notification_type & MONITOR_NOTIF_PE_EVENT) == MONITOR_NOTIF_PE_EVENT)
    {
      if ((uint32_t)USBPD_NOTIFY_ALL != value)
      {
        /* PD_MessageNotif */
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_PD_MESSAGENOTIF, 1U, (uint8_t *)&value);

        switch (value)
        {
          case USBPD_NOTIFY_POWER_STATE_CHANGE:
            /* Is Connected*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_ISCONNECTED, 1U,
                                              (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_pwr_status });
            if (USBPD_PWR_STATE_NO == (usbpd_pwr_state_t)usbpd_dpm_params[port_num].pe_pwr_status)
            {
              /* CC line */
              (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_CC, 1U,
                                                (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].active_cc });
              /* pwr_role*/
              (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_POWERROLE, 1U,
                                                (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_power_role });
            }
            break;
          case USBPD_NOTIFY_GETSNKCAP_ACCEPTED:
            /* NumberOfRcvSNKPDO */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_NUMBEROFRCVSNKPDO, 1U,
                                              (uint8_t[])
            { hmonitor[port_num].NumberOfRcvSNKPDO });
            /* ListOfRcvSNKPDO*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_LISTOFRCVSNKPDO,
                                              (uint16_t)(hmonitor[port_num].NumberOfRcvSNKPDO
                                                         * 4U),
                                              (uint8_t *) hmonitor[port_num].ListOfRcvSNKPDO);
            break;
          case USBPD_NOTIFY_GETSRCCAP_ACCEPTED:
            /* NumberOfRcvSRCPDO*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_NUMBEROFRCVSRCPDO, 1U,
                                              (uint8_t[])
            { hmonitor[port_num].NumberOfRcvSRCPDO });
            /* ListOfRcvSRCPDO*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_LISTOFRCVSRCPDO,
                                              (uint16_t)(hmonitor[port_num].NumberOfRcvSRCPDO
                                                         * 4U),
                                              (uint8_t *) hmonitor[port_num].ListOfRcvSRCPDO);
            break;
          case USBPD_NOTIFY_POWER_EXPLICIT_CONTRACT:
            if (USBPD_PORT_PWR_ROLE_SNK == (usbpd_port_pwr_role_t)usbpd_dpm_params[port_num].pe_power_role)
            {
              /* NumberOfRcvSRCPDO*/
              (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_NUMBEROFRCVSRCPDO, 1U,
                                                (uint8_t[])
              { hmonitor[port_num].NumberOfRcvSRCPDO });
              /* ListOfRcvSRCPDO*/
              (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_LISTOFRCVSRCPDO,
                                                (uint16_t)(hmonitor[port_num].NumberOfRcvSRCPDO
                                                           * 4U),
                                                (uint8_t *) hmonitor[port_num].ListOfRcvSRCPDO);
            }
            /* RDOPosition */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_RDOPOSITION, 1U,
                                              (uint8_t[]) { hmonitor[port_num].RDOPosition });
            /* DataRole*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_DATAROLE, 1U,
                                              (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_data_role });
            /* pwr_role*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_POWERROLE, 1U,
                                              (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_power_role });
            /* VconnON*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_VCONNON, 1U,
                                              (uint8_t[]) { usbpd_dpm_params[port_num].vconn_status });
            /* PD_SpecRevision */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_PD_SPECREVISION, 1U,
                                              (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_spec_revision });
            break;
          case USBPD_NOTIFY_POWER_SWAP_TO_SNK_DONE:
          case USBPD_NOTIFY_POWER_SWAP_TO_SRC_DONE:
            /* pwr_role*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_POWERROLE, 1U,
                                              (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_power_role });
            break;
          case USBPD_NOTIFY_DATAROLESWAP_UFP:
          case USBPD_NOTIFY_DATAROLESWAP_DFP:
            /* DataRole*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_DATAROLE, 1U,
                                              (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_data_role });
            break;
          case USBPD_NOTIFY_PD_SPECIFICATION_CHANGE:
            /* PD_SpecRevision */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_PD_SPECREVISION, 1U,
                                              (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_spec_revision });
            break;
          case USBPD_NOTIFY_VCONN_SWAP_COMPLETE:
            /* VconnON */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_VCONNON, 1U,
                                              (uint8_t[]) { usbpd_dpm_params[port_num].vconn_status });
            break;
#if _PPS
          case USBPD_NOTIFY_PPS_STATUS_RECEIVED:
            /* PPS value */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_PPS, 4U,
                                              (uint8_t *)&hmonitor[port_num].RcvPPSStatus.d32);
            break;
#endif /* _PPS */
#if _STATUS
          case USBPD_NOTIFY_STATUS_RECEIVED:
          {
            uint8_t tab[5U] =
            {
              hmonitor[port_num].RcvStatus.internal_temp,
              hmonitor[port_num].RcvStatus.present_input,
              hmonitor[port_num].RcvStatus.present_battery_input,
              hmonitor[port_num].RcvStatus.event_flags,
              hmonitor[port_num].RcvStatus.temperature_status
            };
            /* Status value */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_STATUS, 5U, tab);
            break;
          }
#endif /* _STATUS */
#ifdef _VDM
          case USBPD_NOTIFY_VDM_IDENTIFY_RECEIVED:
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_VDM_IDENTITY, 4U,
                                              (uint8_t *)
                                              &hmonitor[port_num].VDM_DiscoIdentify.id_header.d32);
            break;
          case USBPD_NOTIFY_VDM_SVID_RECEIVED:
          {
            uint32_t index;
            uint16_t size;
            /* List of received SVDM SVID */
            if (0U != hmonitor[port_num].VDM_SVIDPortPartner.svid_count)
            {
              (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_SVDM_SVIDS, 2U,
                                                (uint8_t *)
                                                &hmonitor[port_num].VDM_SVIDPortPartner.svids[0U]);
              size = 2U;
              for (index = 1U; index < hmonitor[port_num].VDM_SVIDPortPartner.svid_count; index++)
              {
                (void)USBPD_UTILS_MONITOR_TLV_AddValue(&send_tlv,
                                                       (uint8_t *)
                                                       &hmonitor[port_num].VDM_SVIDPortPartner.svids[index],
                                                       2U);
                size += 2U;
              }
              USBPD_UTILS_MONITOR_TLV_UpdateSizeTag(&send_tlv, size);
            }
          }
          break;
          case USBPD_NOTIFY_VDM_MODE_RECEIVED:
          {
            uint32_t index;
            uint16_t size;
            /* List of received SVDM MODES */
            if (0U != hmonitor[port_num].VDM_ModesPortPartner.mode_count)
            {
              (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_SVDM_MODES, 4U,
                                                (uint8_t *)&hmonitor[port_num].VDM_ModesPortPartner.modes[0U]);
              size = 4U;
              for (index = 1U; index < hmonitor[port_num].VDM_ModesPortPartner.mode_count; index++)
              {
                (void)USBPD_UTILS_MONITOR_TLV_AddValue(&send_tlv,
                                                       (uint8_t *)&hmonitor[port_num].VDM_ModesPortPartner.modes[index],
                                                       4U);
                size += 4U;
              }
              USBPD_UTILS_MONITOR_TLV_UpdateSizeTag(&send_tlv, size);
            }
          }
          break;
#endif /* _VDM */
#if defined(_VCONN_SUPPORT)
          case USBPD_NOTIFY_VDM_CABLE_IDENT_RECEIVED:
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_CABLE_VDO, 4U,
                                              (uint8_t *)&hmonitor[port_num].VDM_DiscoCableIdentify.cable_vdo.d32);
            break;
#endif /* _VCONN_SUPPORT */
          default:
            break;
        }
      }
      else
      {
        /* Send all the indication parameters if connected */
        if (USBPD_TRUE == usbpd_dpm_params[port_num].pe_flag_is_connected)
        {
#if defined(_VDM)
          uint32_t index;
#endif /* _VDM */

          if (0U != hmonitor[port_num].NumberOfRcvSNKPDO)
          {
            /* NumberOfRcvSNKPDO */
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_NUMBEROFRCVSNKPDO, 1U,
                                              (uint8_t[]) { hmonitor[port_num].NumberOfRcvSNKPDO });
            /* ListOfRcvSNKPDO*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_LISTOFRCVSNKPDO,
                                              (uint16_t)(hmonitor[port_num].NumberOfRcvSNKPDO * 4U),
                                              (uint8_t *)hmonitor[port_num].ListOfRcvSNKPDO);
          }
          /* RDOPosition */
          (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_RDOPOSITION, 1U,
                                            (uint8_t[]) { hmonitor[port_num].RDOPosition });
          if (USBPD_PORT_PWR_ROLE_SNK == (usbpd_port_pwr_role_t)usbpd_dpm_params[port_num].pe_power_role)
          {
            /* NumberOfRcvSRCPDO*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_NUMBEROFRCVSRCPDO, 1U,
                                              (uint8_t[]) { hmonitor[port_num].NumberOfRcvSRCPDO });
            /* ListOfRcvSRCPDO*/
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_LISTOFRCVSRCPDO,
                                              (uint16_t)hmonitor[port_num].NumberOfRcvSRCPDO * 4U,
                                              (uint8_t *)hmonitor[port_num].ListOfRcvSRCPDO);
          }
          /* PD_SpecRevision */
          (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_PD_SPECREVISION, 1U,
                                            (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_spec_revision });

#if _SRC_CAPA_EXT
          /* Number of batteries supported by the port partner */
          (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_NBBATTERIES, 1U,
                                            (uint8_t[]) { hmonitor[port_num].RcvSRCExtendedCapa.batteries_count });
#endif /* _SRC_CAPA_EXT */

#if defined(_VDM)
          if (0U != hmonitor[port_num].VDM_DiscoIdentify.id_header.d32)
          {
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_VDM_IDENTITY, 4U,
                                              (uint8_t *)&hmonitor[port_num].VDM_DiscoIdentify.id_header.d32);
          }
#if defined(_VCONN_SUPPORT)
          if (1U == hmonitor[port_num].VDM_DiscoCableIdentify.cable_vdo_presence)
          {
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_CABLE_VDO, 4U,
                                              (uint8_t *)&hmonitor[port_num].VDM_DiscoCableIdentify.cable_vdo.d32);
          }
#endif /* _VCONN_SUPPORT */
          /* List of received SVDM SVID */
          if (0U != hmonitor[port_num].VDM_SVIDPortPartner.svid_count)
          {
            uint16_t size;
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_SVDM_SVIDS, 2U,
                                              (uint8_t *)&hmonitor[port_num].VDM_SVIDPortPartner.svids[0U]);
            size = 2U;
            for (index = 1U; index < hmonitor[port_num].VDM_SVIDPortPartner.svid_count; index++)
            {
              (void)USBPD_UTILS_MONITOR_TLV_AddValue(&send_tlv,
                                                     (uint8_t *)&hmonitor[port_num].VDM_SVIDPortPartner.svids[index],
                                                     2U);
              size += 2U;
            }
            USBPD_UTILS_MONITOR_TLV_UpdateSizeTag(&send_tlv, size);
          }

          /* List of received SVDM MODES */
          if (0U != hmonitor[port_num].VDM_ModesPortPartner.mode_count)
          {
            uint16_t size;
            (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_SVDM_MODES, 4U,
                                              (uint8_t *)&hmonitor[port_num].VDM_ModesPortPartner.modes[0U]);
            size = 4U;
            for (index = 1U; index < hmonitor[port_num].VDM_ModesPortPartner.mode_count; index++)
            {
              (void)USBPD_UTILS_MONITOR_TLV_AddValue(&send_tlv,
                                                     (uint8_t *)&hmonitor[port_num].VDM_ModesPortPartner.modes[index],
                                                     4U);
              size += 4U;
            }
            USBPD_UTILS_MONITOR_TLV_UpdateSizeTag(&send_tlv, size);
          }
#endif /* _VDM */
        }
      }
    }

    /* Check if notification linked to a measure report */
    if ((notification_type & MONITOR_NOTIF_MEASUREREPORTING) == MONITOR_NOTIF_MEASUREREPORTING)
    {
      uint32_t vsense = 0U;
      int32_t isense = 0;

      (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_TIMESTAMP, 4U, (uint8_t *)&value);

      if (NULL != pp_get_voltage_cb)
      {
        vsense = pp_get_voltage_cb(port_num);
      }
      if (NULL != pp_get_voltage_cb)
      {
        isense = pp_get_current_cb(port_num);
        if (isense < 0)
        {
          isense = -isense;
        }
      }

      /* VBUS & IBUS level */
      vsense = ((uint32_t)(isense) << 16U) | vsense;
      (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_VBUS_LEVEL, 4U, (uint8_t *)&vsense);
    }

    /* Check if timestamp */
    if ((notification_type & MONITOR_NOTIF_TIMESTAMP) == MONITOR_NOTIF_TIMESTAMP)
    {
      (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_TIMESTAMP, 4U, (uint8_t *)&value);
    }

    /* Check if power change */
    if ((notification_type & MONITOR_NOTIF_POWER_EVENT) == MONITOR_NOTIF_POWER_EVENT)
    {
      /* Is Connected*/
      (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_ISCONNECTED, 1U,
                                        (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_pwr_status });
      if (USBPD_PWR_STATE_NO == (usbpd_pwr_state_t)usbpd_dpm_params[port_num].pe_pwr_status)
      {
        /* CC line */
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_CC, 1U,
                                          (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].active_cc });
        /* pwr_role*/
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_IND_POWERROLE, 1U,
                                          (uint8_t[]) { (uint8_t)usbpd_dpm_params[port_num].pe_power_role });
      }
    }

    USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);

    if (0U != USBPD_UTILS_MONITOR_TLV_GetStringLength(p_processed_data))
    {
      *pMsgToSend = p_processed_data;
      *pSizeMsg = (uint8_t)USBPD_UTILS_MONITOR_TLV_GetStringLength(p_processed_data) + 8U;
      monitor_state = MONITOR_STATE_RUNNING;

    }

  }

  return monitor_state;
}

/**
  * @brief  Callback function called by PE to inform MONITOR about PE event.
  * @param  port_num Port number.
  * @param  event_val Event vale @ref usbpd_notify_event_t.
  */
void USBPD_UTILS_MONITOR_PostNotificationMessage(uint8_t port_num, uint16_t event_val)
{
#if defined(USBPD_SNK) || defined(USBPD_DRP)
  if ((uint16_t)USBPD_NOTIFY_REQUEST_ACCEPTED == event_val)
  {
    DPM_USER_Settings[port_num].dpm_snk_requested_pwr.operating_voltage_1mv_units
      = hmonitor[port_num].voltage_requested;
    DPM_USER_Settings[port_num].dpm_snk_requested_pwr.operating_pwr_1mw_units
      = (hmonitor[port_num].voltage_requested *
         hmonitor[port_num].RequestedCurrent) / 1000U;
  }
#endif /* USBPD_SNK || USBPD_DRP */

  /* Send a notitification only if MONITOR is running */
  if (usbpd_utils_monitor_state == MONITOR_STATE_RUNNING)
  {
    uint32_t event = (uint32_t)MONITOR_USER_EVENT_MONITOR
                     | ((uint32_t)port_num << MONITOR_PE_PORT_NUM_POS)
                     | ((uint32_t)event_val << MONITOR_PE_NOTIF_POS);
#if defined(_RTOS) || defined(USBPD_THREADX)
    MONITOROS_PUT_MESSAGE_QUEUE(usbpd_utils_monitor_msgbox, event, 0U);
#else
    usbpd_utils_monitor_eventflag = event;
#if defined(USE_STM32_UTILITY_OS)
    USBPD_UTILS_MONITOR_RXProcess(usbpd_utils_monitor_eventflag);
    usbpd_utils_monitor_eventflag = MONITOR_USER_EVENT_NONE;
#endif /* USE_STM32_UTILITY_OS */
#endif /* _RTOS */
  }
}

/**
  * @brief  Check Is MONITOR is connected to the board or not.
  * @retval USBPD state.
  */
usbpd_functional_status_t USBPD_UTILS_MONITOR_IsRunning(void)
{
  return ((usbpd_utils_monitor_state == MONITOR_STATE_RUNNING) ? USBPD_ENABLE : USBPD_DISABLE);
}

/**
  * @brief  MONITOR Callback to save information coming from PortPartner.
  * @param  port_num Port number.
  * @param  data_id Type of data to be updated in DPM based on @ref usbpd_core_datainfo_type_t.
  * @param  ptr Pointer on the data.
  * @param  size Nb of bytes to be updated in MONITOR.
  */
void USBPD_UTILS_MONITOR_SaveInfo(uint8_t port_num, uint8_t data_id, uint8_t *ptr, uint32_t size)
{
  uint32_t index;

  /* Check type of information targeted by request */
  switch (data_id)
  {
    /* Case requested DO position Data information:
    */
    case USBPD_CORE_DATAINFO_DATATYPE_RDO_POSITION:
      if (size == 4U)
      {
        uint8_t *temp;
        temp = (uint8_t *)&hmonitor[port_num].RDOPosition;
        (void)memcpy(temp, ptr, size);
        hmonitor[port_num].RDOPositionPrevious = *ptr;
        temp = (uint8_t *)&hmonitor[port_num].RDOPositionPrevious;
        (void)memcpy(temp, ptr, size);
      }
      break;

    /* Case Received Source PDO values Data information:
    */
    case USBPD_CORE_DATAINFO_DATATYPE_RCV_SRC_PDO:
      if (size <= (USBPD_MAX_NB_PDO * 4U))
      {
        uint8_t *rdo;
        hmonitor[port_num].NumberOfRcvSRCPDO = (size / 4U);
        /* Copy PDO data in DPM Handle field */
        for (index = 0U; index < (size / 4U); index++)
        {
          rdo = (uint8_t *)&hmonitor[port_num].ListOfRcvSRCPDO[index];
          (void)memcpy(rdo, (ptr + (index * 4U)), (4U * sizeof(uint8_t)));
        }
      }
      break;

    /* Case Received Sink PDO values Data information:
    */
    case USBPD_CORE_DATAINFO_DATATYPE_RCV_SNK_PDO:
      if (size <= (USBPD_MAX_NB_PDO * 4U))
      {
        uint8_t *rdo;
        hmonitor[port_num].NumberOfRcvSNKPDO = (size / 4U);
        /* Copy PDO data in DPM Handle field */
        for (index = 0U; index < (size / 4U); index++)
        {
          rdo = (uint8_t *)&hmonitor[port_num].ListOfRcvSNKPDO[index];
          (void)memcpy(rdo, (ptr + (index * 4U)), (4U * sizeof(uint8_t)));
        }
      }
      break;

    /* Case Received Request PDO Data information:
    */
    case USBPD_CORE_DATAINFO_DATATYPE_RCV_REQ_PDO:
      if (size == 4U)
      {
        uint8_t *rdo;
        rdo = (uint8_t *)&hmonitor[port_num].RcvRequestDOMsg;
        (void)memcpy(rdo, ptr, size);
      }
      break;

#if _STATUS
    case USBPD_CORE_DATAINFO_INFO_STATUS:
    {
      uint8_t *info_status;
      info_status = (uint8_t *)&hmonitor[port_num].RcvStatus;
      (void)memcpy(info_status, ptr, size);
      break;
    }
#endif /* _STATUS */
#if _PPS
    case USBPD_CORE_DATAINFO_PPS_STATUS:
    {
      uint8_t  *ext_capa;
      ext_capa = (uint8_t *)&hmonitor[port_num].RcvPPSStatus;
      (void)memcpy(ext_capa, ptr, size);
      break;
    }
#endif /* _PPS */
#if _SRC_CAPA_EXT
    case USBPD_CORE_DATAINFO_EXTENDED_CAPA:
    {
      uint8_t  *ext_capa;
      ext_capa = (uint8_t *)&hmonitor[port_num].RcvSRCExtendedCapa;
      (void)memcpy(ext_capa, ptr, size);
      break;
    }
#endif /* _SRC_CAPA_EXT */
#if defined(USBPDCORE_SNK_CAPA_EXT)
    case USBPD_CORE_DATAINFO_SNK_EXTENDED_CAPA:
    {
      uint8_t  *_snk_ext_capa;
      _snk_ext_capa = (uint8_t *)&hmonitor[port_num].RcvSNKExtendedCapa;
      (void)memcpy(_snk_ext_capa, ptr, size);
      break;
    }
#endif /* USBPDCORE_SNK_CAPA_EXT */
#if _MANU_INFO
    case USBPD_CORE_DATAINFO_GET_MANUFACTURER_INFO:
    {
      uint8_t *temp = (uint8_t *)ptr;
      hmonitor[port_num].GetManufacturerInfo.manufacturer_info_target = *temp;
      temp++;
      hmonitor[port_num].GetManufacturerInfo.manufacturer_info_ref    = *temp;
      break;
    }
#endif /* _MANU_INFO */
#if _BATTERY
    case USBPD_CORE_DATAINFO_BATTERY_STATUS:
      hmonitor[port_num].BatteryStatus.d32 = *ptr;
      break;

    case USBPD_CORE_DATAINFO_GET_BATTERY_STATUS:
      hmonitor[port_num].GetBatteryStatus.battery_status_ref = *(uint8_t *)ptr;
      break;

    case USBPD_CORE_DATAINFO_GET_BATTERY_CAPABILITY:
    {
      uint8_t *temp = (uint8_t *)ptr;
      hmonitor[port_num].GetBatteryCapability.battey_cap_ref = *temp;
      break;
    }
#endif /* _BATTERY */
#if _ALERT
    case USBPD_CORE_DATAINFO_ALERT:
    {
      uint8_t  *alert;
      alert = (uint8_t *)&hmonitor[port_num].RcvAlert.d32;
      (void)memcpy(alert, ptr, size);
      break;
    }
#endif /* _ALERT */
#if defined(_VCONN_SUPPORT)
    case MONITOR_VDM_CABLE_INFO:
    {
      uint8_t  *disco_ident;
      disco_ident = (uint8_t *)&hmonitor[port_num].VDM_DiscoCableIdentify;
      (void)memcpy(disco_ident, ptr, size);
      break;
    }
#endif /* _VCONN_SUPPORT */

    /* In case of unexpected data type (Set request could not be fulfilled):
     */
    default:
      break;
  }
}

/**
  * @brief  Register Callback function to be used with Free Text feature.
  * @param  p_free_text_cb Callback function to register (port number, payload and size).
  */
void USBPD_UTILS_MONITOR_RegisterCallbackFreeText(usbpd_status_t(*p_free_text_cb)(uint8_t, uint8_t *, uint16_t))
{
  pp_free_text_cb = p_free_text_cb;
}

/**
  * @}
  */

/** @defgroup USBPD_MONITOR_API_Private_Functions MONITOR API Private Functions
  * @{
  */
/**
  * @brief  Send DPM_INIT_CNF message to MONITOR.
  * @param  port_num Port number.
  * @param  pEncodedMsg Pointer on the message to be encoded.
  */
static void UTILS_MONITOR_SendDPMInitCnf(uint8_t port_num, uint8_t *pEncodedMsg)
{
  usbpd_utils_monitor_tlv_tx_data_t ToSendTLV;
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&ToSendTLV, MONITOR_SET_TAG_ID(port_num, DPM_INIT_CNF),
                                           USBPD_UTILS_MONITOR_TLV_SIZE_MAX, pEncodedMsg);

  /* Information by board */
  if (0U == port_num)
  {
    /* HWBoardVersion */
    const uint8_t *hwversion;
    if (NULL != pp_hw_board_version_cb)
    {
      hwversion = pp_hw_board_version_cb();
    }
    else
    {
      hwversion = (uint8_t *) "UNKNOWN";
    }
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_HWBOARDVERSION,
                                      (uint16_t)(strlen((const char *)hwversion)), hwversion);

    /* HWPDType */
    const uint8_t *hwpdtype;
    if (NULL != pp_hw_board_version_cb)
    {
      hwpdtype  = pp_hw_pd_type_cb();
    }
    else
    {
      hwpdtype = (uint8_t *) "UNKNOWN";
    }
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_HWPDTYPE,
                                      (uint16_t)(strlen((const char *)hwpdtype)), hwpdtype);

    /* NbPortMax */
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_NBPORTMAX, 1U,
                                      (uint8_t[]) { USBPD_PORT_COUNT });
    /* FW Version */
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_FWVERSION, 8U, (uint8_t[])
    {
      (USBPD_FW_VERSION & 0x000000FFU),
      (USBPD_FW_VERSION & 0x0000FF00U) >> 8U,
      (USBPD_FW_VERSION & 0x00FF0000U) >> 16U,
      (USBPD_FW_VERSION & 0xFF000000U) >> 24U,
      (LIB_ID & 0x000000FFU),
      (LIB_ID & 0x0000FF00U) >> 8U,
      (LIB_ID & 0x00FF0000U) >> 16U,
      (LIB_ID & 0xFF000000U) >> 24U
    }
                                     );
    /* Start Port number */
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_NB_PORT_START, 1U,
                                      (uint8_t[]) { USBPD_START_PORT_NUMBER });
    /* Start Port number */
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_NB_PORT_START, 1U,
                                      (uint8_t[]) { USBPD_START_PORT_NUMBER });

    /* Original settings */
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_ORIGINAL_SETTINGS, 1U,
                                      (uint8_t *)&flag_monitor_original_settings);

    USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&ToSendTLV);
  }
  else
  {
    uint16_t settings;
    uint8_t port = port_num - 1U;
#if defined(MB1303)
    uint16_t fake = 0xADDEU;
#endif /*MB1303*/
    /* TypeCSpecVersion & PDSpecRevision */
    settings = (uint16_t)((usbpd_spec_rev_t)usbpd_dpm_settings[port].pe_spec_revision);
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_TYPECSPECREVISION, 2U,
                                      (uint8_t[]) { 0x12U, settings });
    /* Extended Messages UnCkunked */
    settings = (uint16_t)usbpd_dpm_settings[port].pe_pd3_support.b.pe_unchunk_support;
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_EXTENDEDMESSAGESUNCKUNKED, 1U,
                                      (uint8_t *)&settings);
    /* AccessorySupp */
    settings = (uint16_t)usbpd_dpm_settings[port].cad_accessory_support;
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_ACCESSORYSUPP, 1U, (uint8_t *)&settings);
    /*PowerAccessoryDetection*/
    settings = (uint16_t)DPM_USER_Settings[port].pwr_accessory_detection;
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_POWERACCESSORYDETECTION, 1U,
                                      (uint8_t *)&settings);
    /*PowerAccessoryTransition*/
    settings = (uint16_t)DPM_USER_Settings[port].pwr_accessory_transition;
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_POWERACCESSORYTRANSITION, 1U,
                                      (uint8_t *)&settings);
    /*IsCable*/
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_ISCABLE, 1U, (uint8_t[]) { 0x00U });
    /*TryFeature*/
    settings = (uint16_t)usbpd_dpm_settings[port].cad_try_feature_support;
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_TRYFEATURE, 1U, (uint8_t *)&settings);
    /*RpResistorValue*/
    settings = (uint16_t)((usbpd_cad_source_current_adv_t)DPM_USER_Settings[port].pwr_rp_value);
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_RPRESISTORVALUE, 1U, (uint8_t *)&settings);
    /*USBSupport*/
    settings = (uint16_t)((usbpd_core_pdo_usb_com_support_t)DPM_USER_Settings[port].usb_support);
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_USBSUPPORT, 1U, (uint8_t *)&settings);
    /*USBDevice*/
    settings = (uint16_t)DPM_USER_Settings[port].usb_device_support;
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_USBDEVICE, 1U, (uint8_t *)&settings);
    /*USBHost*/
    settings = (uint16_t)DPM_USER_Settings[port].usb_host_support;
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_USBHOST, 1U, (uint8_t *)&settings);
    /*Externally_Powered*/
    settings = (uint16_t)((usbpd_core_pdo_extpowered_support_t)DPM_USER_Settings[port].pwr_unconstrained);
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_UNCONSTRAINED_POWERED, 1U,
                                      (uint8_t *)&settings);
    /*USBSuspendSupport*/
    settings = (uint16_t)((usbpd_core_pdo_usb_suspend_support_t)DPM_USER_Settings[port].usb_suspend_support);
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_USBSUSPENDSUPPORT, 1U, (uint8_t *)&settings);
#if defined(MB1303)
    /*VConnDischarge*/
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_VCONNDISCHARGE, 2U, (uint8_t *)&fake);
    /*VConnLim*/
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_VCONNILIM, 2U, (uint8_t *)&fake);
    /*VConnLimValue*/
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_VCONNILIMVALUE, 2U, (uint8_t *)&fake);
    /*VConnMonitoring*/
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_VCONNMONITORING, 2U, (uint8_t *)&fake);
    /*VConnThresholdUVLO*/
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_VCONNTHRESHOLDUVLO, 2U, (uint8_t *)&fake);
    /*VConnSupply*/
    (void)USBPD_UTILS_MONITOR_TLV_Add(&ToSendTLV, (uint8_t)MONITOR_INIT_VCONNSUPPLY, 2U, (uint8_t *)&fake);
#endif /*MB1303*/

    USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&ToSendTLV);
  }
}

/**
  * @brief  Execution of the message received.
  * @note   Generation of a TLV instruction to send in the context of DPM_MESSAGE
  */
static void UTILS_MONITOR_RequestMessageReq(uint8_t port_num, uint8_t *instruction, uint8_t *pEncodedMsg)
{
  usbpd_status_t status = USBPD_ERROR;
  usbpd_utils_monitor_tlv_rx_data_t process_tlv;
  usbpd_utils_monitor_tlv_tx_data_t send_tlv;

  (void)USBPD_UTILS_MONITOR_TLV_DecodeInit(&process_tlv, instruction);
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, MONITOR_SET_TAG_ID((port_num + 1U), DPM_MESSAGE_CNF),
                                           USBPD_UTILS_MONITOR_TLV_SIZE_MAX, pEncodedMsg);

  /*
    In case there is an error, we use this variable to know when to send a DPM_MESSAGE_REJ
    instead of a DPM_MESSAGE_CNF. This variable contains the cause if there is one.
  */
  utils_monitor_reject_reason_t error;

  uint8_t tag;
  uint16_t length;
  uint8_t *value;
  (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
  switch (tag)
    /*
      Each case must:
      - Apply his specific request in the stack. If parameters are needed, they can be found in the value pointer
      - If there is an error, set the error variable according to it (cf USBPD_MONITOR_Specification)
      - Otherwise in case of success, do nothing else
    */
  {
    case MONITOR_MSG_GOTOMIN:
      status = USBPD_DPM_RequestGotoMin(port_num);
      break;
    case MONITOR_MSG_PING:
      status = USBPD_DPM_RequestPing(port_num);
      break;
    case MONITOR_MSG_GET_SRC_CAPA:
      status = USBPD_DPM_RequestGetSourceCapability(port_num);
      break;
    case MONITOR_MSG_GET_SNK_CAPA:
      status = USBPD_DPM_RequestGetSinkCapability(port_num);
      break;
    case MONITOR_MSG_DR_SWAP:
      status = USBPD_DPM_RequestDataRoleSwap(port_num);
      break;
    case MONITOR_MSG_PR_SWAP:
      status = USBPD_DPM_RequestPowerRoleSwap(port_num);
      break;
    case MONITOR_MSG_VCONN_SWAP:
      status = USBPD_DPM_RequestVconnSwap(port_num);
      break;
    case MONITOR_MSG_SOURCE_CAPA:
      status = USBPD_DPM_RequestSourceCapability(port_num);
      break;
    case MONITOR_MSG_REQUEST:
    {
      uint16_t voltage = 0U;
      uint8_t nb_expected_tag = 0U;
      uint8_t index_pdo = 0U;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 2U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_RDOPOSITION == (utils_monitor_tag_param_msg_t)tag)
        {
          index_pdo = value[0U];
        }
        else if (MONITOR_PARAM_MSG_REQUESTEDVOLTAGE == (utils_monitor_tag_param_msg_t)tag)
        {
          voltage = USBPD_LE16(&value[0U]);
        }
        else
        {
          /* Nothing to do */
        }

        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (2U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestMessageRequest(port_num, index_pdo, voltage);
      }
      break;
    }
    case MONITOR_MSG_SOFT_RESET:
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
      {
        status = USBPD_DPM_RequestSoftReset(port_num, (usbpd_sop_type_t)value[0U]);
      }
      break;
    case MONITOR_MSG_HARD_RESET:
      status = USBPD_DPM_RequestHardReset(port_num);
      break;
    case MONITOR_MSG_CABLE_RESET:
      status = USBPD_DPM_RequestCableReset(port_num);
      break;
#if defined(_VDM) || defined(_VCONN_SUPPORT)
    case MONITOR_MSG_VDM_DISCO_IDENT:
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
      {
        status = USBPD_DPM_RequestVDM_DiscoveryIdentify(port_num, (usbpd_sop_type_t)value[0U]);
      }
      break;
    case MONITOR_MSG_VDM_DISCO_SVID:
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
      {
        status = USBPD_DPM_RequestVDM_DiscoverySVID(port_num, (usbpd_sop_type_t)value[0U]);
      }
      break;
    case MONITOR_MSG_VDM_DISCO_MODE:
    {
      uint16_t svid = 0U;
      uint8_t nb_expected_tag = 0U;
      uint8_t sop = USBPD_SOPTYPE_SOP;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 2U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
        {
          sop = value[0U];
        }
        else if (MONITOR_PARAM_MSG_SVDM_SVID == (utils_monitor_tag_param_msg_t)tag)
        {
          svid = USBPD_LE16(&value[0U]);
        }
        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (2U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestVDM_DiscoveryMode(port_num, (usbpd_sop_type_t)sop, svid);
      }
    }
    break;
    case MONITOR_MSG_VDM_ENTER_MODE:
    {
      uint16_t mode = 0U;
      uint16_t svid = 0U;
      uint8_t nb_expected_tag = 0U;
      uint8_t sop = USBPD_SOPTYPE_SOP;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 3U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
        {
          sop = value[0U];
        }
        else if (MONITOR_PARAM_MSG_SVDM_SVID == (utils_monitor_tag_param_msg_t)tag)
        {
          svid = USBPD_LE16(&value[0U]);
        }
        else if (MONITOR_PARAM_MSG_SVDM_MODEINDEX == (utils_monitor_tag_param_msg_t)tag)
        {
          mode = USBPD_LE16(&value[0U]);
        }
        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (3U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestVDM_EnterMode(port_num, (usbpd_sop_type_t)sop, svid, mode);
      }
    }
    break;
    case MONITOR_MSG_VDM_EXIT_MODE:
    {
      uint16_t svid = 0U;
      uint16_t mode = 0U;
      uint8_t nb_expected_tag = 0U;
      uint8_t sop = USBPD_SOPTYPE_SOP;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 3U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
        {
          sop = value[0U];
        }
        else if (MONITOR_PARAM_MSG_SVDM_SVID == (utils_monitor_tag_param_msg_t)tag)
        {
          svid = USBPD_LE16(&value[0U]);
        }
        else if (MONITOR_PARAM_MSG_SVDM_MODEINDEX == (utils_monitor_tag_param_msg_t)tag)
        {
          mode = value[0U];
        }
        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (3U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestVDM_ExitMode(port_num, (usbpd_sop_type_t)sop, svid, mode);
      }
    }
    break;
    case MONITOR_MSG_VDM_UNSTRUCTURED:
      break;
#endif /* _VDM || _VCONN_SUPPORT */
#if defined(_VDM)
    case MONITOR_MSG_DISPLAY_PORT_STATUS:
    {
      uint32_t dp_status;
      uint16_t svid = 0U;
      uint8_t nb_expected_tag = 0U;
      uint8_t sop = USBPD_SOPTYPE_SOP;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 3U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
        {
          sop = value[0U];
        }
        else if (MONITOR_PARAM_MSG_SVDM_SVID == (utils_monitor_tag_param_msg_t)tag)
        {
          svid = USBPD_LE16(&value[0U]);
        }
        else if (MONITOR_PARAM_MSG_DP_STATUS == (utils_monitor_tag_param_msg_t)tag)
        {
          dp_status = USBPD_LE32(&value[0U]);
        }
        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (3U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestDisplayPortStatus(port_num, (usbpd_sop_type_t)sop, svid, &dp_status);
      }
    }
    break;
    case MONITOR_MSG_DISPLAY_PORT_CONFIG:
    {
      uint32_t dp_config;
      uint16_t svid = 0U;
      uint8_t nb_expected_tag = 0U;
      uint8_t sop = USBPD_SOPTYPE_SOP;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 3U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
        {
          sop = value[0U];
        }
        else if (MONITOR_PARAM_MSG_SVDM_SVID == (utils_monitor_tag_param_msg_t)tag)
        {
          svid = USBPD_LE16(&value[0U]);
        }
        else if (MONITOR_PARAM_MSG_DP_CONFIGURE == (utils_monitor_tag_param_msg_t)tag)
        {
          dp_config = USBPD_LE32(&value[0U]);
        }
        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (3U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestDisplayPortConfig(port_num, (usbpd_sop_type_t)sop, svid, &dp_config);
      }
    }
    break;
    case MONITOR_MSG_DISPLAY_PORT_ATTENTION:
      break;
    case MONITOR_MSG_VDM_ATTENTION:
    {
      uint16_t svid = 0U;
      uint8_t nb_expected_tag = 0U;
      uint8_t sop = USBPD_SOPTYPE_SOP;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 2U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
        {
          sop = value[0U];
        }
        else if (MONITOR_PARAM_MSG_SVDM_SVID == (utils_monitor_tag_param_msg_t)tag)
        {
          svid = USBPD_LE16(&value[0U]);
        }
        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (2U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestAttention(port_num, (usbpd_sop_type_t)sop, svid);
      }
    }
    break;
#endif /* _VDM */
    case MONITOR_MSG_FREE_TEXT:
    {
      uint8_t nb_expected_tag = 0U;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 1U))
      {
        nb_expected_tag++;
      }
      if (1U == nb_expected_tag)
      {
        status = UTILS_MONITOR_ManageFreeText(port_num, value, length);
      }
      break;
    }
    case MONITOR_MSG_FR_SWAP:
      status = USBPD_DPM_RequestFastRoleSwap(port_num);
      break;
    case MONITOR_MSG_GET_PPS_STATUS:
      status = USBPD_DPM_RequestGetPPS_Status(port_num);
      break;
    case MONITOR_MSG_GET_COUNTRY_CODES:
      status = USBPD_DPM_RequestGetCountryCodes(port_num);
      break;
    case MONITOR_MSG_GET_STATUS:
      status = USBPD_DPM_RequestGetStatus(port_num);
      break;
    case MONITOR_MSG_GET_SOURCE_CAPA_EXTENDED:
      status = USBPD_DPM_RequestGetSourceCapabilityExt(port_num);
      break;
    case MONITOR_MSG_GET_SINK_CAPA_EXTENDED:
      status = USBPD_DPM_RequestGetSinkCapabilityExt(port_num);
      break;
    case MONITOR_MSG_ALERT:
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      if (MONITOR_PARAM_MSG_ALERTMSG == (utils_monitor_tag_param_msg_t)tag)
      {
        usbpd_alert_data_object_t alert;
        alert.d32 = USBPD_LE32(&value[0U]);
        status = USBPD_DPM_RequestAlert(port_num, alert);
      }
      break;
    case MONITOR_MSG_GET_COUNTRY_INFO:
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      if (MONITOR_PARAM_MSG_COUNTRYCODE == (utils_monitor_tag_param_msg_t)tag)
      {
        uint16_t country_code;
        country_code = USBPD_LE16(&value[0U]);
        status = USBPD_DPM_RequestGetCountryInfo(port_num, country_code);
      }
      break;
    case MONITOR_MSG_GET_BAT_CAPA:
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      if (MONITOR_PARAM_MSG_BATTERYREF == (utils_monitor_tag_param_msg_t)tag)
      {
        status = USBPD_DPM_RequestGetBatteryCapability(port_num, (uint8_t *)&value[0U]);
      }
      break;
    case MONITOR_MSG_GET_BAT_STATUS:
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      if (MONITOR_PARAM_MSG_BATTERYREF == (utils_monitor_tag_param_msg_t)tag)
      {
        status = USBPD_DPM_RequestGetBatteryStatus(port_num, (uint8_t *)&value[0U]);
      }
      break;
    case MONITOR_MSG_GET_MANU_INFO:
    {
      uint8_t manu_info[2U];
      uint8_t nb_expected_tag = 0U;
      uint8_t sop = (uint8_t)USBPD_SOPTYPE_SOP;
      if (length > USBPD_UTILS_MONITOR_TLV_SIZE_MAX)
      {
        break;
      }
      (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      while ((0U != length) && (USBPD_UTILS_MONITOR_TLV_SIZE_MAX > length) && (nb_expected_tag < 3U))
      {
        nb_expected_tag++;
        if (MONITOR_PARAM_MSG_SOPTYPE == (utils_monitor_tag_param_msg_t)tag)
        {
          sop = value[0U];
        }
        else if (MONITOR_PARAM_MSG_BATTERYREF == (utils_monitor_tag_param_msg_t)tag)
        {
          manu_info[1U] = value[0U];
        }
        else if (MONITOR_PARAM_MSG_MANUINFODATA == (utils_monitor_tag_param_msg_t)tag)
        {
          manu_info[0U] = value[0U];
        }
        else
        {
          /* Nothing to do */
        }

        (void)USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &length, &value);
      }
      if (3U == nb_expected_tag)
      {
        status = USBPD_DPM_RequestGetManufacturerInfo(port_num, (usbpd_sop_type_t)sop, (uint8_t *)&manu_info);
      }
      break;
    }
    case MONITOR_MSG_SECU_REQUEST:
      status = USBPD_DPM_RequestSecurityRequest(port_num);
      break;
    case MONITOR_MSG_FIRM_UPDATE_REQUEST:
      break;
    default:
      break;
  }

  /* Only applies if an error was specified (= if the case couldn't success)*/
  if (USBPD_OK != status)
  {
    switch (status)
    {
      case USBPD_BUSY:
        error = MONITOR_REJ_DPM_NOT_READY;
        break;
      case USBPD_TIMEOUT:
        error = MONITOR_REJ_DPM_TIMEOUT;
        break;
      case USBPD_NOTSUPPORTED:
      case USBPD_ERROR:
      default:
        error = MONITOR_REJ_DPM_REJECT;
        break;
    }
    USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
    /* Turn the tag 0x0A into 0x0B to signal an error*/
    (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, MONITOR_SET_TAG_ID((port_num + 1U), DPM_MESSAGE_REJ),
                                             USBPD_UTILS_MONITOR_TLV_SIZE_MAX, pEncodedMsg);
    (void)USBPD_UTILS_MONITOR_TLV_AddValue(&send_tlv, (uint8_t *)&error, 1U);
  }

  USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
  USBPD_UTILS_MONITOR_TLV_DecodeDeInit(&process_tlv);
}

/**
  * @brief  Application of the received configuration.
  * @note   Generation of a TLV instruction to send in the context of DPM_CONFIG_SET
  */
static void UTILS_MONITOR_SendDpmConfigSetCnf(uint8_t port_num, uint8_t *instruction, uint8_t *pEncodedMsg)
{
  usbpd_utils_monitor_tlv_tx_data_t send_tlv;
  usbpd_utils_monitor_tlv_rx_data_t process_tlv;
  /*
    In case there is an error, we use this variable to know when to send a DPM_CONFIG_SET instead
    of a DPM_MESSAGE_CNF. This variable contains the cause if there is one.
  */
  uint8_t *value;
  uint16_t size;
  uint16_t dcdrp = 0U;
  uint8_t error = 0xFFU;
  uint8_t param_not_applicated[MONITOR_PARAM_ALL]; /* List of parameters who could not be applied. */
  uint8_t counter_param_not_applicated = 0U;  /* Counter of not applicated parameters */
  uint8_t tag;
  uint8_t flag_drp = 0U;
  uint8_t tdrp = 0U;

  (void)USBPD_UTILS_MONITOR_TLV_DecodeInit(&process_tlv, instruction);
  /* pEncodedMsg can be sent as it is at this point, if there isn't any error during application of settings*/
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, MONITOR_SET_TAG_ID((port_num + 1U), DPM_CONFIG_SET_CNF),
                                           USBPD_UTILS_MONITOR_TLV_SIZE_MAX, pEncodedMsg);

  /* Do while loop, used to parse and apply each parameter */
  while ((uint8_t)(0xFFU) != USBPD_UTILS_MONITOR_TLV_Get(&process_tlv, &tag, &size, &value))
    /*
      As long as we haven't read all the parameters. If error is set, we continue to loop
      to apply the other parameters if it's because of a DPM_REJECT
      While loop, used to parse and apply each parameter
    */
  {
    switch ((utils_monitor_tag_param_t)tag)
      /*
        Each case must:
        - Apply his specific parameter. If data is needed, it can be found in the value pointer
        - In case of success, do nothing else
        - Otherwise if there is an error, set the error variable according to it (cf USBPD_MONITOR_Specification)
        - If the error is DPM_REJECT (0x00), add the parameter tag to param_not_applicated, using the
          counter counter_param_not_applicated. It allows to list the rejected parameters in the DPM_CONFIG_REJ
      */
    {
      case MONITOR_PARAM_SOP:
#if !defined(USBPDCORE_LIB_NO_PD)
        /* SOP & SOP1 & SOP2 */
        /* SOP1_Debug & SOP2_Debug not implemented */
        usbpd_dpm_settings[port_num].pe_supported_sop = (usbpd_sop_support_t)value[0U];
#endif /* !USBPDCORE_LIB_NO_PD */
        break;
      case MONITOR_PARAM_FASTROLESWAP:
        /*FastRoleSwap*/
        usbpd_dpm_settings[port_num].pe_pd3_support.b.pe_fast_role_swap_support = value[0U];
        break;
      case MONITOR_PARAM_DATAROLESWAP_TO_UFP:
        /*data_role_swap to UFP */
        DPM_USER_Settings[port_num].pe_dr_swap_to_ufp_support = value[0U];
        break;
      case MONITOR_PARAM_DATAROLESWAP_TO_DFP:
        /*data_role_swap to DFP */
        DPM_USER_Settings[port_num].pe_dr_swap_to_dfp_support = value[0U];
        break;
      case MONITOR_PARAM_DEFAULTPOWERROLE:
        /*DefaultPowerRole*/
        usbpd_dpm_settings[port_num].pe_default_role = (usbpd_port_pwr_role_t)value[0U];
        break;
#if !defined(USBPDCORE_LIB_NO_PD)
      case MONITOR_PARAM_DRP_SUPPORT:
        /*DRP_Support*/
        usbpd_dpm_settings[port_num].pe_pwr_role_swap_support = value[0U];
        break;
      case MONITOR_PARAM_CADROLETOGGLE:
        /*CADRoleToggle*/
        usbpd_dpm_settings[port_num].cad_role_toggle_support = value[0U];
        break;
      case MONITOR_PARAM_PE_SCAP_HR:
        /*PE_SCAP_HR*/
        usbpd_dpm_settings[port_num].pe_caps_counter_support = value[0U];
        break;
      case MONITOR_PARAM_VCONNSWAP:
        /*VConnSwap*/
        DPM_USER_Settings[port_num].pe_vconn_swap_support = value[0U];
        break;
#if defined(_VDM)
      case MONITOR_PARAM_VDM_SUPPORT:
        /* DRP Support */
        usbpd_dpm_settings[port_num].pe_vdm_support = value[0U];
        break;
#endif /*_VDM*/
      case MONITOR_PARAM_PING_SUPPORT:
        /* Ping Support */
        usbpd_dpm_settings[port_num].pe_ping_support = value[0U];
        break;
      case MONITOR_PARAM_PPS_SUPPORT:
        usbpd_dpm_settings[port_num].pe_pd3_support.d16 = USBPD_LE16(&value[0U]);
        break;
#endif /* !USBPDCORE_LIB_NO_PD */
#if defined(USBPD_SNK) || defined(USBPD_DRP)
      case MONITOR_PARAM_SNK_PDO:
      {
        uint32_t index_pdo;
        uint32_t index_monitor;
        uint32_t pdo;
#if USBPD_PORT_COUNT==2
        if (USBPD_PORT_0 == port_num)
#endif /* USBPD_PORT_COUNT==2 */
        {
          (void)memset(USBPD_PWR_Port_PDO_Storage[0U].sink_pdos.pdo_list, 0x00U,
                       sizeof(USBPD_PWR_Port_PDO_Storage[0U].sink_pdos.pdo_list[USBPD_MAX_NB_PDO]));
          index_pdo = 0U;
          for (index_monitor = 0U; index_monitor < size; index_monitor = index_monitor + 4U)
          {
            pdo = USBPD_LE32(&value[index_monitor]);
            USBPD_PWR_Port_PDO_Storage[0U].sink_pdos.pdo_list[index_pdo] = pdo;
            index_pdo++;
          }
          *USBPD_PWR_Port_PDO_Storage[0U].sink_pdos.pdo_count = (uint8_t)(size / 4U);
          UTILS_MONITOR_UpdateSNKPowerPort0();
        }
#if USBPD_PORT_COUNT==2
        else
        {
          (void)memset(USBPD_PWR_Port_PDO_Storage[1U].sink_pdos.pdo_list, 0x00U,
                       sizeof(USBPD_PWR_Port_PDO_Storage[1U].sink_pdos.pdo_list[USBPD_MAX_NB_PDO]));
          index_pdo = 0U;
          for (index_monitor = 0U; index_monitor < size; index_monitor = index_monitor + 4U)
          {
            pdo = USBPD_LE32(&value[index_monitor]);
            USBPD_PWR_Port_PDO_Storage[1U].sink_pdos.pdo_list[index_pdo] = pdo;
            index_pdo++;
          }
          *USBPD_PWR_Port_PDO_Storage[1U].sink_pdos.pdo_count = (uint8_t)(size / 4U);
          UTILS_MONITOR_UpdateSNKPowerPort1();
        }
#endif /* USBPD_PORT_COUNT==2 */
        break;
      }
#endif /* (USBPD_SNK) || (USBPD_DRP) */
#if defined(USBPD_SRC) || defined(USBPD_DRP)
      case MONITOR_PARAM_SRC_PDO:
      {
        uint32_t index_pdo;
        uint32_t index_monitor;
        uint32_t pdo;
#if USBPD_PORT_COUNT==2
        if (USBPD_PORT_0 == port_num)
#endif /* USBPD_PORT_COUNT==2 */
        {
          (void)memset(USBPD_PWR_Port_PDO_Storage[0U].source_pdos.pdo_list, 0x00U,
                       sizeof(USBPD_PWR_Port_PDO_Storage[0U].source_pdos.pdo_list[USBPD_MAX_NB_PDO]));
          index_pdo = 0U;
          for (index_monitor = 0U; index_monitor < size; index_monitor = index_monitor + 4U)
          {
            pdo = USBPD_LE32(&value[index_monitor]);
            USBPD_PWR_Port_PDO_Storage[0U].source_pdos.pdo_list[index_pdo] = pdo;
            index_pdo++;
          }
          *USBPD_PWR_Port_PDO_Storage[0U].source_pdos.pdo_count = (uint8_t)(size / 4U);
        }
#if USBPD_PORT_COUNT==2
        else
        {
          (void)memset(USBPD_PWR_Port_PDO_Storage[1U].source_pdos.pdo_list, 0x00U,
                       sizeof(USBPD_PWR_Port_PDO_Storage[1U].source_pdos.pdo_list[USBPD_MAX_NB_PDO]));
          index_pdo = 0U;
          for (index_monitor = 0U; index_monitor < size; index_monitor = index_monitor + 4U)
          {
            pdo = USBPD_LE32(&value[index_monitor]);
            USBPD_PWR_Port_PDO_Storage[1U].source_pdos.pdo_list[index_pdo] = pdo;
            index_pdo++;
          }
          *USBPD_PWR_Port_PDO_Storage[1U].source_pdos.pdo_count = (uint8_t)(size / 4U);
        }
#endif /* USBPD_PORT_COUNT==2 */
        break;
      }
#endif /* (USBPD_SRC) || (USBPD_DRP) */
      case MONITOR_PARAM_TDRP:
        tdrp = value[0U];
        DPM_USER_Settings[port_num].cad_tdrp = tdrp;
        flag_drp = 1U;
        break;
      case MONITOR_PARAM_DCSRC_DRP:
        dcdrp = value[0U];
        DPM_USER_Settings[port_num].cad_dcsrc_rp = dcdrp;
        flag_drp = 1U;
        break;
      case MONITOR_PARAM_RESPONDS_TO_DISCOV_SOP:
        usbpd_dpm_settings[port_num].pe_respond_to_discov_sop_support = value[0U];
        break;
      case MONITOR_PARAM_ATTEMPTS_DISCOV_SOP:
        usbpd_dpm_settings[port_num].pe_attempts_to_discov_sop_support = value[0U];
        break;
      case MONITOR_PARAM_XID_SOP:
        DPM_ID_Settings[port_num].xid       = USBPD_LE32(&value[0U]);
        break;
      case MONITOR_PARAM_USB_VID_SOP:
        DPM_ID_Settings[port_num].vid       = USBPD_LE16(&value[0U]);
        break;
      case MONITOR_PARAM_PID_SOP:
        DPM_ID_Settings[port_num].pid       = USBPD_LE16(&value[0U]);
        break;
#if defined(_VDM)
      case MONITOR_PARAM_DATA_CAPABLE_AS_USB_HOST_SOP:
        dpm_vdm_settings[port_num].vdm_usb_host_support    = (usbpd_vdm_usb_capa_t)value[0U];
        break;
      case MONITOR_PARAM_DATA_CAPABLE_AS_USB_DEVICE_SOP:
        dpm_vdm_settings[port_num].vdm_usb_device_support  = (usbpd_vdm_usb_capa_t)value[0U];
        break;
      case MONITOR_PARAM_PRODUCT_TYPE_SOP:
        dpm_vdm_settings[port_num].vdm_product_type_ufp_cp = (usbpd_vdm_product_type_t)value[0U];
        break;
      case MONITOR_PARAM_USBPD_VDM_MODAL_OPERATION_SUPPORTED_SOP:
        dpm_vdm_settings[port_num].vdm_modal_operation    = (usbpd_vdm_modal_op_support_t)value[0U];
        break;
      case MONITOR_PARAM_BCDDEVICE_SOP:
        dpm_vdm_settings[port_num].vdm_bcd_device_sop     = USBPD_LE16(&value[0U]);
        break;
#endif /* _VDM */
      case MONITOR_PARAM_MEASUREREPORTING:
        /* measurement_report */
        usbpd_utils_monitor_user_params[port_num].u.measurement_report = value[0U];
        break;
#if _MANU_INFO
      case MONITOR_PARAM_MANUINFOPORT_VID:
        DPM_USER_Settings[port_num].dpm_manu_info_port.vid = USBPD_LE16(&value[0U]);
        DPM_USER_Settings[port_num].dpm_manu_info_port.pid = USBPD_LE16(&value[2U]);
        size -= 4U;
        (void)memcpy(DPM_USER_Settings[port_num].dpm_manu_info_port.manu_string, &value[4U], size);
        break;
#endif /* _MANU_INFO */
      default:
        error = (uint8_t)MONITOR_REJ_DPM_REJECT;
        param_not_applicated[counter_param_not_applicated] = tag;
        counter_param_not_applicated++;
        break;
    }
  }

#if !defined(USBPDCORE_LIB_NO_PD)
  /* Updtate cad_snk_toggle_time_ms & cad_src_toggle_time_ms only if TDRP and/or DC_SRC_DRP have been received */
  if (1U == flag_drp)
  {
    uint16_t calcul;
    if (0U == tdrp)
    {
      tdrp = (uint8_t)(usbpd_dpm_settings[port_num].cad_snk_toggle_time_ms +
                       usbpd_dpm_settings[port_num].cad_src_toggle_time_ms);
    }
    if (0U == dcdrp)
    {
      dcdrp = (uint16_t)
              ((usbpd_dpm_settings[port_num].cad_src_toggle_time_ms * 100U)
               / (usbpd_dpm_settings[port_num].cad_snk_toggle_time_ms +
                  usbpd_dpm_settings[port_num].cad_src_toggle_time_ms));
    }
    calcul = (tdrp * dcdrp) / 100U;
    usbpd_dpm_settings[port_num].cad_src_toggle_time_ms = (uint8_t)calcul;
    calcul = tdrp * (100U - dcdrp) / 100U;
    usbpd_dpm_settings[port_num].cad_snk_toggle_time_ms = (uint8_t)calcul;
  }
#endif /* !USBPDCORE_LIB_NO_PD */

  /* Only applies if an error was specified. Will send a DPM_CONFIG_REJ instead of DPM_CONFIG_SET_CNF*/
  if (error != 0xFFU)
  {
    USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
    (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, MONITOR_SET_TAG_ID((port_num + 1U), DPM_CONFIG_REJ),
                                             USBPD_UTILS_MONITOR_TLV_SIZE_MAX, pEncodedMsg);
    (void)USBPD_UTILS_MONITOR_TLV_AddValue(&send_tlv, &error, 1U);

    /* If the cause of error is DPM_REJECT, we have to list the tag of parameters which didn't pass */
    if (error != 0U)
    {
      uint32_t index;
      for (index = 0U; index < counter_param_not_applicated; index++)
      {
        (void)USBPD_UTILS_MONITOR_TLV_AddValue(&send_tlv, &param_not_applicated[index], 1U);
      }
    }
    USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
  }
}

static void UTILS_MONITOR_SendDpmConfigGetCnf(uint8_t port_num, uint8_t *instruction, uint8_t *pEncodedMsg)
{
  usbpd_utils_monitor_tlv_tx_data_t send_tlv;
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, MONITOR_SET_TAG_ID((port_num + 1U), DPM_CONFIG_GET_CNF),
                                           USBPD_UTILS_MONITOR_TLV_SIZE_MAX, pEncodedMsg);
  uint16_t length = USBPD_UTILS_MONITOR_TLV_GetStringLength(instruction) - USBPD_UTILS_MONITOR_TLV_HEADER_SIZE;
  uint8_t index = 0U;
  uint8_t param;

  /* This is a state machine. */
  do
  {
    /* If there is no parameters, we go through each case of the state machine in one pass. (conditional breaks) */
    if (0U == length)
    {
      param = (uint8_t)MONITOR_PARAM_ALL;
    }
    else
    {
      /* If there are, we loop the state machine and go through one case each time. */
      param = instruction[USBPD_UTILS_MONITOR_TLV_VALUE_POSITION + index];
    }

    /*
      Each case must simply use the USBPD_UTILS_MONITOR_TLV_Add function to insert his corresponding data,
      as suggested in comment.
    */
    switch ((utils_monitor_tag_param_t)param)
    {
      case MONITOR_PARAM_ALL:
#if !defined(USBPDCORE_LIB_NO_PD)
      case MONITOR_PARAM_SOP:
      {
        /* SOP & SOP1 & SOP2 */
        /* SOP1_Debug & SOP2_Debug not implemented */
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_supported_sop;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_SOP, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
      case MONITOR_PARAM_FASTROLESWAP:
      {
        /*FastRoleSwap*/
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_pd3_support.b.pe_fast_role_swap_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_FASTROLESWAP, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
#endif /* !USBPDCORE_LIB_NO_PD */
      case MONITOR_PARAM_DATAROLESWAP_TO_UFP:
      {
        /* data_role_swap to UFP */
        uint8_t settings = (uint8_t)DPM_USER_Settings[port_num].pe_dr_swap_to_ufp_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_DATAROLESWAP_TO_UFP, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
      case MONITOR_PARAM_DATAROLESWAP_TO_DFP:
      {
        /* data_role_swap to DFP */
        uint8_t settings = (uint8_t)DPM_USER_Settings[port_num].pe_dr_swap_to_dfp_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_DATAROLESWAP_TO_DFP, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
      case MONITOR_PARAM_DEFAULTPOWERROLE:
      {
        /*DefaultPowerRole*/
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_default_role;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_DEFAULTPOWERROLE, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
#if !defined(USBPDCORE_LIB_NO_PD)
      case MONITOR_PARAM_DRP_SUPPORT:
      {
        /*DRP_Support*/
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_pwr_role_swap_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_DRP_SUPPORT, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
      case MONITOR_PARAM_CADROLETOGGLE:
      {
        /*CADRoleToggle*/
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].cad_role_toggle_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_CADROLETOGGLE, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
      case MONITOR_PARAM_PE_SCAP_HR:
      {
        /*PE_SCAP_HR*/
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_caps_counter_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_PE_SCAP_HR,  1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
#if defined(_VDM)
      case MONITOR_PARAM_VDM_SUPPORT:
      {
        /* DRP Support */
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_vdm_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_VDM_SUPPORT, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
#endif /*_VDM*/
      case MONITOR_PARAM_PING_SUPPORT:
      {
        /* Ping Support */
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_ping_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_PING_SUPPORT, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
      case MONITOR_PARAM_PPS_SUPPORT:
      {
        /* PD3 Support */
        uint16_t settings = (uint16_t)usbpd_dpm_settings[port_num].pe_pd3_support.d16;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_PPS_SUPPORT, 2U, (uint8_t *)&settings);
        if (0U != length)
        {
          break;
        }
      }
#endif /* !USBPDCORE_LIB_NO_PD */
      case MONITOR_PARAM_VCONNSWAP:
      {
        /*VConnSwap*/
        uint8_t settings = (uint8_t)DPM_USER_Settings[port_num].pe_vconn_swap_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_VCONNSWAP, 1U, &settings);
        if (0U != length)
        {
          break;
        }
      }
#if defined(USBPD_SNK) || defined(USBPD_DRP)
      case MONITOR_PARAM_SNK_PDO:
      {
        uint8_t *list_pdo;
        uint8_t nb_pdo;
        nb_pdo   =  *USBPD_PWR_Port_PDO_Storage[port_num].sink_pdos.pdo_count;
        list_pdo = (uint8_t *)USBPD_PWR_Port_PDO_Storage[port_num].sink_pdos.pdo_list;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_SNK_PDO,
                                          ((uint16_t)(nb_pdo) * 4U), list_pdo);
      }
      if (0U != length)
      {
        break;
      }
#endif /* USBPD_SNK || USBPD_DRP */
#if defined(USBPD_SRC) || defined(USBPD_DRP)
      case MONITOR_PARAM_SRC_PDO:
      {
        uint8_t *list_pdo;
        uint8_t nb_pdo;
        nb_pdo   =  *USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_count;
        list_pdo = (uint8_t *)USBPD_PWR_Port_PDO_Storage[port_num].source_pdos.pdo_list;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_SRC_PDO,
                                          ((uint16_t)(nb_pdo) * 4U), list_pdo);
      }
      if (0U != length)
      {
        break;
      }
#endif /* USBPD_SRC || USBPD_DRP */
      case MONITOR_PARAM_TDRP:
      {
        /* Calculate the current value of tDRP (value between 50ms and 100ms) */
        uint8_t value = (uint8_t)DPM_USER_Settings[port_num].cad_tdrp;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_TDRP, 1U, &value);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_DCSRC_DRP:
      {
        /* Calculate the current value of tDRP (value between 50ms and 100ms) */
        uint8_t value = (uint8_t)DPM_USER_Settings[port_num].cad_dcsrc_rp;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_DCSRC_DRP, 1U, &value);
      }
      if (0U != length)
      {
        break;
      }
#if !defined(USBPDCORE_LIB_NO_PD)
      case MONITOR_PARAM_RESPONDS_TO_DISCOV_SOP:
      {
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_respond_to_discov_sop_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_RESPONDS_TO_DISCOV_SOP, 1U, &settings);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_ATTEMPTS_DISCOV_SOP:
      {
        uint8_t settings = (uint8_t)usbpd_dpm_settings[port_num].pe_attempts_to_discov_sop_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_ATTEMPTS_DISCOV_SOP, 1U, &settings);
      }
      if (0U != length)
      {
        break;
      }
#endif /* !USBPDCORE_LIB_NO_PD */
      case MONITOR_PARAM_XID_SOP:
      {
        uint32_t value = DPM_ID_Settings[port_num].xid;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_XID_SOP, 4U, (uint8_t *)&value);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_USB_VID_SOP:
      {
        uint16_t value = (uint16_t)DPM_ID_Settings[port_num].vid;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_USB_VID_SOP, 2U, (uint8_t *)&value);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_PID_SOP:
      {
        uint16_t value = (uint16_t)DPM_ID_Settings[port_num].pid;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_PID_SOP, 2U, (uint8_t *)&value);
      }
      if (0U != length)
      {
        break;
      }
#if defined(_VDM)
      case MONITOR_PARAM_DATA_CAPABLE_AS_USB_HOST_SOP:
      {
        uint8_t settings = (uint8_t)dpm_vdm_settings[port_num].vdm_usb_host_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_DATA_CAPABLE_AS_USB_HOST_SOP,
                                          1U, &settings);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_DATA_CAPABLE_AS_USB_DEVICE_SOP:
      {
        uint8_t settings = (uint8_t)dpm_vdm_settings[port_num].vdm_usb_device_support;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_DATA_CAPABLE_AS_USB_DEVICE_SOP, 1U,
                                          &settings);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_PRODUCT_TYPE_SOP:
      {
        uint8_t settings = (uint8_t)dpm_vdm_settings[port_num].vdm_product_type_ufp_cp;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_PRODUCT_TYPE_SOP, 1U, &settings);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_USBPD_VDM_MODAL_OPERATION_SUPPORTED_SOP:
      {
        uint8_t settings = (uint8_t)dpm_vdm_settings[port_num].vdm_modal_operation;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_USBPD_VDM_MODAL_OPERATION_SUPPORTED_SOP, 1U,
                                          &settings);
      }
      if (0U != length)
      {
        break;
      }
      case MONITOR_PARAM_BCDDEVICE_SOP:
      {
        uint16_t value = (uint16_t)dpm_vdm_settings[port_num].vdm_bcd_device_sop;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_BCDDEVICE_SOP, 2U, (uint8_t *)&value);
      }
      if (0U != length)
      {
        break;
      }
#endif /* _VDM */
      case MONITOR_PARAM_MEASUREREPORTING:
      {
        uint8_t settings = (uint8_t)usbpd_utils_monitor_user_params[port_num].u.measurement_report;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_MEASUREREPORTING, 1U, &settings);
      }
      if (0U != length)
      {
        break;
      }
#if _MANU_INFO
      case MONITOR_PARAM_MANUINFOPORT_VID:
      {
        uint8_t size = (uint8_t)sizeof(DPM_USER_Settings[port_num].dpm_manu_info_port.manu_string) + 4U;
        (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)MONITOR_PARAM_MANUINFOPORT_VID, size,
                                          (uint8_t *)&DPM_USER_Settings[port_num].dpm_manu_info_port);
      }
      if (0U != length)
      {
        break;
      }
#endif /* _MANU_INFO */
      default:
        if (0U != length)
        {
          /* At least one parameter does not exist */
          USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
          UTILS_MONITOR_SendDpmConfigGetRej(port_num, pEncodedMsg, MONITOR_REJ_DPM_INVALID_MESSAGE);
          goto __end;
        }
        break;
    }
    index++;
    /*
    Loop until we haven't processed each specified parameter.
    If there weren't any (length = 0), we exit the loop after one execution.
    */
  } while (index < length);

__end:
  USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
}

/**
  * @brief  Send A DPM_CONFIG_REJ message.
  * @param  port_num Port number.
  * @param  pEncodedMsg Pointer on the message to send.
  * @param  RejectReason Reason of rejection.
  */
static void UTILS_MONITOR_SendDpmConfigGetRej(uint8_t port_num, uint8_t *pEncodedMsg,
                                              utils_monitor_reject_reason_t RejectReason)
{
  UNUSED(port_num);
  usbpd_utils_monitor_tlv_tx_data_t send_tlv;
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, (uint8_t)DPM_CONFIG_REJ, USBPD_UTILS_MONITOR_TLV_SIZE_MAX,
                                           pEncodedMsg);
  (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)RejectReason, 0U, NULL);
  USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
}

/**
  * @brief  Send A DPM_MESSAGE_REJ message.
  * @param  port_num Port number.
  * @param  pEncodedMsg Pointer on the message to send.
  * @param  RejectReason Reason of rejection.
  */
static void UTILS_MONITOR_SendDpmMessageRej(uint8_t port_num, uint8_t *pEncodedMsg,
                                            utils_monitor_reject_reason_t RejectReason)
{
  UNUSED(port_num);
  usbpd_utils_monitor_tlv_tx_data_t send_tlv;
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&send_tlv, (uint8_t)DPM_MESSAGE_REJ, USBPD_UTILS_MONITOR_TLV_SIZE_MAX,
                                           pEncodedMsg);
  (void)USBPD_UTILS_MONITOR_TLV_Add(&send_tlv, (uint8_t)RejectReason, 0U, NULL);
  USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&send_tlv);
}

/**
  * @brief  Send A DPM_MESSAGE_REJ message.
  * @param  port_num Port number.
  * @param  pEncodedMsg Pointer on the message to send.
  * @param  RejectReason Reason of rejection.
  */
static void UTILS_MONITOR_SendDpmRegisterReadCnf(uint8_t port_num, uint8_t *pEncodedMsg, uint8_t param)
{
  usbpd_utils_monitor_tlv_tx_data_t ToSendTLV;
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&ToSendTLV, MONITOR_SET_TAG_ID((port_num + 1U), DPM_REGISTER_READ_CNF),
                                           USBPD_UTILS_MONITOR_TLV_SIZE_MAX, pEncodedMsg);

  uint8_t stateMachineArray[MONITOR_REG_NUMBER_OF_REGISTERS] =
  {
    (uint8_t)MONITOR_REG_VENDOR_ID, (uint8_t)MONITOR_REG_PRODUCT_ID, (uint8_t)MONITOR_REG_DEVICE_ID,
    (uint8_t)MONITOR_REG_USBTYPEC_REV, (uint8_t)MONITOR_REG_USBPD_REV_VER, (uint8_t)MONITOR_REG_USBPD_INTERFACE_REV,
    (uint8_t)MONITOR_REG_ALERT, (uint8_t)MONITOR_REG_ALERT_MASK, (uint8_t)MONITOR_REG_POWER_STATUS_MASK,
    (uint8_t)MONITOR_REG_FAULT_STATUS_MASK, (uint8_t)MONITOR_REG_CONFIG_STANDARD_OUTPUT,
    (uint8_t)MONITOR_REG_TCPC_CONTROL, (uint8_t)MONITOR_REG_ROLE_CONTROL, (uint8_t)MONITOR_REG_FAULT_CONTROL,
    (uint8_t)MONITOR_REG_POWER_CONTROL, (uint8_t)MONITOR_REG_CC_STATUS, (uint8_t)MONITOR_REG_POWER_STATUS,
    (uint8_t)MONITOR_REG_FAULT_STATUS, (uint8_t)MONITOR_REG_COMMAND, (uint8_t)MONITOR_REG_DECIVE_CAP1,
    (uint8_t)MONITOR_REG_DECIVE_CAP2, (uint8_t)MONITOR_REG_STD_INPUT_CAP, (uint8_t)MONITOR_REG_STD_OUTPUT_CAP,
    (uint8_t)MONITOR_REG_MSG_HEADER_INFO, (uint8_t)MONITOR_REG_RX_DETECT, (uint8_t)MONITOR_REG_RX_BYTE_COUNT,
    (uint8_t)MONITOR_REG_RX_BUFFER_FRAME_TYPE, (uint8_t)MONITOR_REG_RX_HEADER, (uint8_t)MONITOR_REG_RX_DATA,
    (uint8_t)MONITOR_REG_TRANSMIT, (uint8_t)MONITOR_REG_TX_BYTE_COUNT, (uint8_t)MONITOR_REG_TX_HEADER,
    (uint8_t)MONITOR_REG_TX_DATA, (uint8_t)MONITOR_REG_VBUS_VOLTAGE,
    (uint8_t)MONITOR_REG_VBUS_SINK_DISCONNECT_THRESHOLD, (uint8_t)MONITOR_REG_VBUS_STOP_DISCHARGE_THRESHOLD,
    (uint8_t)MONITOR_REG_VBUS_VOLTAGE_ALARM_HI_CFG, (uint8_t)MONITOR_REG_VBUS_VOLTAGE_ALARM_LO_CFG,
    (uint8_t)MONITOR_REG_VENDOR_DATA
  };
  uint8_t counter = 0U;

  /*  If param is not at 0xFF, then it means we only want one parameter.
      We then have to get out of the state machine after the first state */
  if (param != 0xFFU)
  {
    *stateMachineArray = param;
  }

  do
  {
    switch (stateMachineArray[counter])
      /*
        In each case, the register needs to be read. Inclusion in the TLV which
        is going to be sent has to be done with USBPD_UTILS_MONITOR_TLV_Add().
      */
    {
      case MONITOR_REG_VENDOR_ID:

        break;
      case MONITOR_REG_PRODUCT_ID:

        break;
      case MONITOR_REG_DEVICE_ID:

        break;
      case MONITOR_REG_USBTYPEC_REV:

        break;
      case MONITOR_REG_USBPD_REV_VER:

        break;
      case MONITOR_REG_USBPD_INTERFACE_REV:

        break;
      case MONITOR_REG_ALERT:

        break;
      case MONITOR_REG_ALERT_MASK:

        break;
      case MONITOR_REG_POWER_STATUS_MASK:

        break;
      case MONITOR_REG_FAULT_STATUS_MASK:

        break;
      case MONITOR_REG_CONFIG_STANDARD_OUTPUT:

        break;
      case MONITOR_REG_TCPC_CONTROL:

        break;
      case MONITOR_REG_ROLE_CONTROL:

        break;
      case MONITOR_REG_FAULT_CONTROL:

        break;
      case MONITOR_REG_POWER_CONTROL:

        break;
      case MONITOR_REG_CC_STATUS:

        break;
      case MONITOR_REG_POWER_STATUS:

        break;
      case MONITOR_REG_FAULT_STATUS:

        break;
      case MONITOR_REG_COMMAND:

        break;
      case MONITOR_REG_DECIVE_CAP1:

        break;
      case MONITOR_REG_DECIVE_CAP2:

        break;
      case MONITOR_REG_STD_INPUT_CAP:

        break;
      case MONITOR_REG_STD_OUTPUT_CAP:

        break;
      case MONITOR_REG_MSG_HEADER_INFO:

        break;
      case MONITOR_REG_RX_DETECT:

        break;
      case MONITOR_REG_RX_BYTE_COUNT:

        break;
      case MONITOR_REG_RX_BUFFER_FRAME_TYPE:

        break;
      case MONITOR_REG_RX_HEADER:

        break;
      case MONITOR_REG_RX_DATA:

        break;
      case MONITOR_REG_TRANSMIT:

        break;
      case MONITOR_REG_TX_BYTE_COUNT:

        break;
      case MONITOR_REG_TX_HEADER:

        break;
      case MONITOR_REG_TX_DATA:

        break;
      case MONITOR_REG_VBUS_VOLTAGE:

        break;
      case MONITOR_REG_VBUS_SINK_DISCONNECT_THRESHOLD:

        break;
      case MONITOR_REG_VBUS_STOP_DISCHARGE_THRESHOLD:

        break;
      case MONITOR_REG_VBUS_VOLTAGE_ALARM_HI_CFG:

        break;
      case MONITOR_REG_VBUS_VOLTAGE_ALARM_LO_CFG:

        break;
      case MONITOR_REG_VENDOR_DATA:

        break;
      default:
        break;
    }
    counter++;
  } while ((counter < MONITOR_REG_NUMBER_OF_REGISTERS) && (param != 0xFFU));

  USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&ToSendTLV);
}

static void UTILS_MONITOR_SendDpmRegisterWriteCnf(uint8_t port_num, uint8_t *pEncodedMsg, uint8_t *toProcess)
{
  usbpd_utils_monitor_tlv_rx_data_t ToProcessTLV;
  (void)USBPD_UTILS_MONITOR_TLV_DecodeInit(&ToProcessTLV, toProcess);

  uint8_t registerName;
  uint16_t registerSize;
  uint8_t *registerValue;

  (void)USBPD_UTILS_MONITOR_TLV_Get(&ToProcessTLV, &registerName, &registerSize, &registerValue);

  switch (registerName)
    /*
      Each case must write the data pointed by registerValue in the appropriate register,
      while taking into account the size of the data with registerSize.
    */
  {
    case MONITOR_REG_VENDOR_ID:

      break;
    case MONITOR_REG_PRODUCT_ID:

      break;
    case MONITOR_REG_DEVICE_ID:

      break;
    case MONITOR_REG_USBTYPEC_REV:

      break;
    case MONITOR_REG_USBPD_REV_VER:

      break;
    case MONITOR_REG_USBPD_INTERFACE_REV:

      break;
    case MONITOR_REG_ALERT:

      break;
    case MONITOR_REG_ALERT_MASK:

      break;
    case MONITOR_REG_POWER_STATUS_MASK:

      break;
    case MONITOR_REG_FAULT_STATUS_MASK:

      break;
    case MONITOR_REG_CONFIG_STANDARD_OUTPUT:

      break;
    case MONITOR_REG_TCPC_CONTROL:

      break;
    case MONITOR_REG_ROLE_CONTROL:

      break;
    case MONITOR_REG_FAULT_CONTROL:

      break;
    case MONITOR_REG_POWER_CONTROL:

      break;
    case MONITOR_REG_CC_STATUS:

      break;
    case MONITOR_REG_POWER_STATUS:

      break;
    case MONITOR_REG_FAULT_STATUS:

      break;
    case MONITOR_REG_COMMAND:

      break;
    case MONITOR_REG_DECIVE_CAP1:

      break;
    case MONITOR_REG_DECIVE_CAP2:

      break;
    case MONITOR_REG_STD_INPUT_CAP:

      break;
    case MONITOR_REG_STD_OUTPUT_CAP:

      break;
    case MONITOR_REG_MSG_HEADER_INFO:

      break;
    case MONITOR_REG_RX_DETECT:

      break;
    case MONITOR_REG_RX_BYTE_COUNT:

      break;
    case MONITOR_REG_RX_BUFFER_FRAME_TYPE:

      break;
    case MONITOR_REG_RX_HEADER:

      break;
    case MONITOR_REG_RX_DATA:

      break;
    case MONITOR_REG_TRANSMIT:

      break;
    case MONITOR_REG_TX_BYTE_COUNT:

      break;
    case MONITOR_REG_TX_HEADER:

      break;
    case MONITOR_REG_TX_DATA:

      break;
    case MONITOR_REG_VBUS_VOLTAGE:

      break;
    case MONITOR_REG_VBUS_SINK_DISCONNECT_THRESHOLD:

      break;
    case MONITOR_REG_VBUS_STOP_DISCHARGE_THRESHOLD:

      break;
    case MONITOR_REG_VBUS_VOLTAGE_ALARM_HI_CFG:

      break;
    case MONITOR_REG_VBUS_VOLTAGE_ALARM_LO_CFG:

      break;
    case MONITOR_REG_VENDOR_DATA:

      break;
    default:
      break;
  }
  USBPD_UTILS_MONITOR_TLV_DecodeDeInit(&ToProcessTLV);

  usbpd_utils_monitor_tlv_tx_data_t ToSendTLV;
  /* Generation of the empty response, as it must be done*/
  (void)USBPD_UTILS_MONITOR_TLV_EncodeInit(&ToSendTLV, MONITOR_SET_TAG_ID((port_num + 1U),
                                                                          DPM_REGISTER_WRITE_CNF), 12U, pEncodedMsg);
  USBPD_UTILS_MONITOR_TLV_EncodeDeInit(&ToSendTLV);
}

/**
  * @brief  Manage Free text sent by the MONITOR.
  * @param  port_num The current port number.
  * @param  pPayload Pointer of the text to manage.
  * @param  size size of the text.
  * @retval USBPD Status.
  */
static usbpd_status_t UTILS_MONITOR_ManageFreeText(uint8_t port_num, uint8_t *pPayload, uint16_t size)
{
  usbpd_status_t _status = USBPD_FAIL;

  if (NULL != pp_free_text_cb)
  {
    pp_free_text_cb(port_num, pPayload, size);
    _status = USBPD_OK;
  }

  return _status;
}

#if defined(USBPD_SNK) || defined(USBPD_DRP)
/**
  * @brief  Function To update SNK PDO and power user settings on Port0.
  */
static void UTILS_MONITOR_UpdateSNKPowerPort0(void)
{
  usbpd_pdo_t pdo;
  uint32_t _max_power = 0U;
  uint16_t _voltage;
  uint16_t _current;
  uint16_t _power;
  uint16_t _min_voltage = 0xFFFFU;
  uint16_t _max_voltage = 0U;
  uint16_t _max_current = 0U;

  for (uint32_t _index = 0U; _index < *USBPD_PWR_Port_PDO_Storage[0U].sink_pdos.pdo_count; _index++)
  {
    pdo.d32 = USBPD_PWR_Port_PDO_Storage[0U].sink_pdos.pdo_list[_index];
    switch ((usbpd_core_pdo_type_t)pdo.pdo_generic.pdo_type)
    {
      case USBPD_CORE_PDO_TYPE_FIXED:    /*!< Fixed Supply PDO */
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_fixed.voltage_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = MONITOR_DECODE_10MA(pdo.pdo_snk_fixed.operational_current_10ma_units);
        MONITOR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
      case USBPD_CORE_PDO_TYPE_BATTERY:  /*!< Battery Supply PDO */
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_battery.voltage_min_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_battery.voltage_max_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _power = MONITOR_DECODE_250MW(pdo.pdo_snk_battery.operational_pwr_250mw_units);
        MONITOR_UPDATE_POWER_MAX(_power, _max_power);
        break;
      case USBPD_CORE_PDO_TYPE_VARIABLE: /*!< Variable Supply (non-battery) PDO */
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_variable.voltage_min_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_variable.voltage_max_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = MONITOR_DECODE_10MA(pdo.pdo_snk_variable.operational_current_10ma_units);
        MONITOR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
#if _PPS
      case USBPD_CORE_PDO_TYPE_APDO:     /*!< Augmented Power Data Object (APDO) */
        _voltage = MONITOR_DECODE_100MV(pdo.pdo_src_snk_apdo.voltage_min_100mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = MONITOR_DECODE_100MV(pdo.pdo_src_snk_apdo.voltage_max_100mv_units);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = MONITOR_DECODE_50MA(pdo.pdo_src_snk_apdo.current_max_50ma_units);
        MONITOR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
#endif /* PPS */
      default:
        break;
    }
  }

  MONITOR_CHECK_VOLTAGE_MIN(_min_voltage,
                            DPM_USER_Settings[USBPD_PORT_0].dpm_snk_requested_pwr.operating_voltage_min_1mv_units);
  MONITOR_CHECK_VOLTAGE_MAX(_max_voltage,
                            DPM_USER_Settings[USBPD_PORT_0].dpm_snk_requested_pwr.operating_voltage_max_1mv_units);
  MONITOR_CHECK_CURRENT_MAX(_max_current,
                            DPM_USER_Settings[USBPD_PORT_0].dpm_snk_requested_pwr.operating_current_max_1ma_units);
  _max_power = ((uint32_t)(_max_voltage) * (uint32_t)(_max_current)) / 1000U;
  MONITOR_CHECK_POWER_MAX(_max_power,
                          DPM_USER_Settings[USBPD_PORT_0].dpm_snk_requested_pwr.operating_pwr_maw_1mw_units);
}

#if USBPD_PORT_COUNT==2
/**
  * @brief  Function To update SNK PDO and power user settings on Port1.
  */
static void UTILS_MONITOR_UpdateSNKPowerPort1(void)
{
  usbpd_pdo_t pdo;
  uint32_t _max_power = 0U;
  uint16_t _voltage = 0U;
  uint16_t _current = 0U;
  uint16_t _power = 0U;
  uint16_t _min_voltage = 0xFFFFU;
  uint16_t _max_voltage = 0U;
  uint16_t _max_current = 0U;

  for (uint32_t _index = 0U; _index < *USBPD_PWR_Port_PDO_Storage[1U].sink_pdos.pdo_count; _index++)
  {
    pdo.d32 = USBPD_PWR_Port_PDO_Storage[1U].sink_pdos.pdo_list[_index];
    switch ((usbpd_core_pdo_type_t)pdo.pdo_generic.pdo_type)
    {
      case USBPD_CORE_PDO_TYPE_FIXED:    /*!< Fixed Supply PDO */
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_fixed.voltage_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = MONITOR_DECODE_10MA(pdo.pdo_snk_fixed.operational_current_10ma_units);
        MONITOR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
      case USBPD_CORE_PDO_TYPE_BATTERY:  /*!< Battery Supply PDO */
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_battery.voltage_min_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_battery.voltage_max_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _power = MONITOR_DECODE_250MW(pdo.pdo_snk_battery.operational_pwr_250mw_units);
        MONITOR_UPDATE_POWER_MAX(_power, _max_power);
        break;
      case USBPD_CORE_PDO_TYPE_VARIABLE: /*!< Variable Supply (non-battery) PDO */
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_variable.voltage_min_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = MONITOR_DECODE_50MV(pdo.pdo_snk_variable.voltage_max_50mv_units);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = MONITOR_DECODE_10MA(pdo.pdo_snk_variable.operational_current_10ma_units);
        MONITOR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
#if _PPS
      case USBPD_CORE_PDO_TYPE_APDO:     /*!< Augmented Power Data Object (APDO) */
        _voltage = MONITOR_DECODE_100MV(pdo.pdo_src_snk_apdo.voltage_min_100mv_units);
        MONITOR_UPDATE_VOLTAGE_MIN(_voltage, _min_voltage);
        _voltage = MONITOR_DECODE_100MV(pdo.pdo_src_snk_apdo.voltage_max_100mv_units);
        MONITOR_UPDATE_VOLTAGE_MAX(_voltage, _max_voltage);
        _current = MONITOR_DECODE_50MA(pdo.pdo_src_snk_apdo.current_max_50ma_units);
        MONITOR_UPDATE_CURRENT_MAX(_current, _max_current);
        break;
#endif /* PPS */
      default:
        break;
    }
  }

  MONITOR_CHECK_VOLTAGE_MIN(_min_voltage,
                            DPM_USER_Settings[USBPD_PORT_1].dpm_snk_requested_pwr.operating_voltage_min_1mv_units);
  MONITOR_CHECK_VOLTAGE_MAX(_max_voltage,
                            DPM_USER_Settings[USBPD_PORT_1].dpm_snk_requested_pwr.operating_voltage_max_1mv_units);
  MONITOR_CHECK_CURRENT_MAX(_max_current,
                            DPM_USER_Settings[USBPD_PORT_1].dpm_snk_requested_pwr.operating_current_max_1ma_units);
  _max_power = (_max_voltage * _max_current) / 1000U;
  MONITOR_CHECK_POWER_MAX(_max_power,
                          DPM_USER_Settings[USBPD_PORT_1].dpm_snk_requested_pwr.operating_pwr_maw_1mw_units);
}
#endif /* USBPD_PORT_COUNT==2 */
#endif /* USBPD_SNK) || USBPD_DRP */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  ******************************************************************************
  * @file    usbpd_utils_monitor.h
  * @brief   This file contains all the functions prototypes for monitor utility.
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
#ifndef USBPD_UTILS_MONITOR_H
#define USBPD_UTILS_MONITOR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "usbpd_utils_monitor_hw_if.h"
#include "usbpd_utils_monitor_tlv.h"
#include "usbpd_utils_monitor_os.h"
#include "usbpd_utils_tracer.h"

#include "usbpd_dpm_conf.h" /* To access DPM configuration */
#include "usbpd_dpm_user.h" /* To access DPM configuration */
#include "usbpd_vdm_user.h" /* To access VDM configuration */
#include "usbpd_pwr.h"      /* To access PDO storage configuration */

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS
  * @{
  */

/** @addtogroup USBPD_UTILS_MONITOR
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_Exported_Constants USBPD_UTILS_MONITOR Exported Constants
  * @{
  */

#define MONITOR_PE_PORT_NUM_POS (24U)                             /*!< Position of the port number in the notif.      */
#define MONITOR_PE_PORT_NUM_MSK (0x3U << MONITOR_PE_PORT_NUM_POS) /*!< Mask of the port number in the notification    */
#define MONITOR_PE_PORT_NUM_0   (0x0U << MONITOR_PE_PORT_NUM_POS) /*!< Port 0                                         */
#define MONITOR_PE_PORT_NUM_1   (0x1U << MONITOR_PE_PORT_NUM_POS) /*!< Port 1                                         */

/** @defgroup USBPD_UTILS_MONITOR_NOTIFICATION USBPD_UTILS_MONITOR NOTIFICATION
  * @{
  */

#define MONITOR_PE_NOTIF_POS                     (4U)
#define MONITOR_PE_NOTIF_MSK                     (0xFFU << MONITOR_PE_NOTIF_POS) /*!< Notification type mask         */

#define MONITOR_NOTIF_MEASUREMENT                (USBPD_NOTIFY_ALL + 1U)         /*!< Measurement notification       */
#define MONITOR_NOTIF_MEASUREMENT_STEP           (40U)                           /*!< Measurement step in ms         */

#define MONITOR_NOTIF_NUMBEROFRCVSNKPDO          (1UL << 0U)                     /*<! Number of received sink PDOs   */
#define MONITOR_NOTIF_RDOPOSITION                (1UL << 1U)                     /*<! RDO position                   */
#define MONITOR_NOTIF_LISTOFRCVSRCPDO            (1UL << 2U)                     /*<! List of received source PDOs   */
#define MONITOR_NOTIF_NUMBEROFRCVSRCPDO          (1UL << 3U)                     /*<! Number of received source PDOs */
#define MONITOR_NOTIF_LISTOFRCVSNKPDO            (1UL << 4U)                     /*<! List of received sink PDOs     */
#define MONITOR_NOTIF_ISCONNECTED                (1UL << 5U)                     /*<! Is connected                   */
#define MONITOR_NOTIF_DATAROLE                   (1UL << 6U)                     /*<! Data role                      */
#define MONITOR_NOTIF_POWERROLE                  (1UL << 7U)                     /*<! Power role                     */
#define MONITOR_NOTIF_CCDEFAULTCURRENTADVERTISED (1UL << 8U)                     /*<! CC default current advertised  */
#define MONITOR_NOTIF_VCONNON                    (1UL << 9U)                     /*<! VCONN ON                       */
#define MONITOR_NOTIF_VCONNSWAPPED               (1UL << 10U)                    /*<! VCONN SWAPPED                  */
#define MONITOR_NOTIF_MEASUREREPORTING           (1UL << 11U)                    /*<! Measure reporting              */
#define MONITOR_NOTIF_CC                         (1UL << 12U)                    /*<! CC                             */
#define MONITOR_NOTIF_PE_EVENT                   (1UL << 13U)                    /*<! PE event                       */
#define MONITOR_NOTIF_TIMESTAMP                  (1UL << 14U)                    /*<! Timestamp                      */
#define MONITOR_NOTIF_POWER_EVENT                (1UL << 15U)                    /*<! Power event                    */

/**
  * @}
  */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_UTILS_MONITOR_Exported_Types USBPD_UTILS_MONITOR Exported Types
  * @{
  */

/**
  * @brief  USBPD MONITOR STATE MACHINE STATE.
  */
typedef enum
{
  MONITOR_STATE_INIT    = 0U, /*!< MONITOR init state    */
  MONITOR_STATE_RUNNING = 1U, /*!< MONITOR running state */
  MONITOR_STATE_RESET   = 2U, /*!< MONITOR reset state   */
} usbpd_utils_monitor_state_t;

/**
  * @brief  USBPD MONITOR TAG TYPE.
  */
typedef enum
{
  DPM_RESET_REQ            = 0x01U, /*!< Reset request          */
  DPM_INIT_REQ             = 0x02U, /*!< Init request           */
  DPM_INIT_CNF             = 0x03U, /*!< Init confirm           */
  DPM_CONFIG_GET_REQ       = 0x04U, /*!< Config get request     */
  DPM_CONFIG_GET_CNF       = 0x05U, /*!< Config get confirm     */
  DPM_CONFIG_SET_REQ       = 0x06U, /*!< Config set request     */
  DPM_CONFIG_SET_CNF       = 0x07U, /*!< Config set confirm     */
  DPM_CONFIG_REJ           = 0x08U, /*!< Config reject          */
  DPM_MESSAGE_REQ          = 0x09U, /*!< Message request        */
  DPM_MESSAGE_CNF          = 0x0AU, /*!< Message confirm        */
  DPM_MESSAGE_REJ          = 0x0BU, /*!< Message reject         */
  DPM_MESSAGE_IND          = 0x0CU, /*!< Message indication     */
  DPM_MESSAGE_RSP          = 0x0DU, /*!< Message response       */
  DPM_REGISTER_READ_REQ    = 0x0EU, /*!< Register read request  */
  DPM_REGISTER_READ_CNF    = 0x0FU, /*!< Register read confirm  */
  DPM_REGISTER_WRITE_REQ   = 0x10U, /*!< Register write request */
  DPM_REGISTER_WRITE_CNF   = 0x11U, /*!< Register write confirm */
  DEBUG_STACK_MESSAGE      = 0x12U, /*!< Debug stack message    */
} usbpd_utils_monitor_tag_t;

/**
  * @brief USBPD utils monitor user parameters.
  */
typedef struct
{
  uint16_t vbus_level                   :16U;  /*!< VBUS Level                                 */
  uint16_t ibus_level                   :16U;  /*!< IBUS Level                                 */

  union
  {
    uint8_t measurement_report;                /*<! Measurement Reporting                      */
    struct
    {
      uint8_t measurement_report_value  :7U;   /*!< Enable Measure reporting every tr x 40 ms  */
      uint8_t measurement_report_enable :1U;   /*!< Enable or Disable Measure reporting        */
    } b;
  } u;
  uint8_t reserved                      :8U;   /*!< Reserved bits                              */
} usbpd_utils_monitor_user_params_t;

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_Exported_Functions USBPD_UTILS_MONITOR Exported Functions
  * @{
  */

usbpd_functional_status_t  USBPD_UTILS_MONITOR_Init(const uint8_t *(*p_hw_board_version_cb)(void),
                                                    const uint8_t *(*p_hw_pd_type_cb)(void),
                                                    uint16_t (*p_get_voltage_cb)(uint8_t),
                                                    int16_t (*p_get_current_cb)(uint8_t));
uint32_t                    USBPD_UTILS_MONITOR_InitOS(void *p_memory);
void                        USBPD_UTILS_MONITOR_Reset(void);
void                        USBPD_UTILS_MONITOR_Start(void);
void                        USBPD_UTILS_MONITOR_TimerCounter(void);
uint32_t                    USBPD_UTILS_MONITOR_RXProcess(uint32_t Event);
uint32_t                    USBPD_UTILS_MONITOR_FormatAndSendNotification(uint8_t port_num, uint32_t notification_type,
                                                                          uint32_t value);
uint32_t                    USBPD_UTILS_MONITOR_GetMessage(uint8_t Character, uint8_t Error);
usbpd_utils_monitor_state_t USBPD_UTILS_MONITOR_SendAnswer(uint8_t **pMsgToSend, uint8_t *pSizeMsg);
usbpd_utils_monitor_state_t USBPD_UTILS_MONITOR_SendNotification(uint8_t port_num, uint8_t **pMsgToSend,
                                                                 uint8_t *pSizeMsg, uint32_t TypeNotifcation,
                                                                 uint32_t value);
void                        USBPD_UTILS_MONITOR_PostNotificationMessage(uint8_t port_num, uint16_t event_val);
void                        USBPD_UTILS_MONITOR_SaveInfo(uint8_t port_num, uint8_t data_id, uint8_t *ptr,
                                                         uint32_t size);
usbpd_functional_status_t   USBPD_UTILS_MONITOR_IsRunning(void);
#if !defined(USE_STM32_UTILITY_OS)
void                        USBPD_UTILS_MONITOR_Execute(void);
#endif /* !USE_STM32_UTILITY_OS */
void                        USBPD_UTILS_MONITOR_RegisterCallbackFreeText(usbpd_status_t(*CB_FreeText)(uint8_t,
                                                                         uint8_t *, uint16_t));

/**
  * @}
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

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* USBPD_UTILS_MONITOR_H */

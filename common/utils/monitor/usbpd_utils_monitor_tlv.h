/**
  ******************************************************************************
  * @file    usbpd_utils_monitor_tlv.h
  * @brief   Header file for usbpd_utils_monitor_tlv.c.
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
#ifndef USBPD_UTILS_MONITOR_TLV_H
#define USBPD_UTILS_MONITOR_TLV_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdlib.h>

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS
  * @{
  */

/** @addtogroup USBPD_UTILS_MONITOR_TLV
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_TLV_Exported_Constants USBPD_UTILS_MONITOR_TLV Exported Constants
  * @{
  */

#define USBPD_UTILS_MONITOR_TLV_SOF                   (0xFDU) /*!< Start of Frame value                */
#define USBPD_UTILS_MONITOR_TLV_EOF                   (0xA5U) /*!< End of Frame value                  */
#define USBPD_UTILS_MONITOR_TLV_SIZE_MAX              (256U)  /*!< Max size of a TLV                   */

#define USBPD_UTILS_MONITOR_TLV_TAG_POSITION          (4U) /*<! Byte position of TAG                   */
#define USBPD_UTILS_MONITOR_TLV_LENGTH_HIGH_POSITION  (5U) /*<! Byte position of LENGTH HIGH           */
#define USBPD_UTILS_MONITOR_TLV_LENGTH_LOW_POSITION   (6U) /*<! Byte position of LENGTH LOW            */
#define USBPD_UTILS_MONITOR_TLV_VALUE_POSITION        (7U) /*<! Byte position of VALUE                 */
#define USBPD_UTILS_MONITOR_TLV_HEADER_SIZE           (3U) /*<! Size of TLV header (TAG(1) + LENGTH(2) */
#define USBPD_UTILS_MONITOR_TLV_SOF_SIZE              (4U) /*<! USBPD_UTILS_MONITOR_TLV_SOF * 4        */
#define USBPD_UTILS_MONITOR_TLV_EOF_SIZE              (4U) /*<! USBPD_UTILS_MONITOR_TLV_EOF * 4        */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  USBPD_UTILS_MONITOR_TLV_Exported_Types USBPD_UTILS_MONITOR_TLV Exported Types
  * @{
  */

/**
  * @brief Transmit TLV structure.
  */
typedef struct
{
  uint8_t  *p_data;         /*!< Pointer to TLV instruction data */
  uint16_t  max_size;       /*!< Max size of the instruction     */
  uint16_t  eof_position;   /*!< Position of the EOF in data     */
  uint16_t  size_position;  /*!< Position of the size tag        */
} usbpd_utils_monitor_tlv_tx_data_t;

/**
  * @brief Received TLV structure.
  */
typedef struct
{
  uint8_t  *p_data;         /*!< Pointer to TLV instruction data */
  uint16_t  read_position;  /*!< Read position                   */
} usbpd_utils_monitor_tlv_rx_data_t;

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_TLV_Exported_Functions USBPD_UTILS_MONITOR_TLV Exported Functions
  * @{
  */

uint8_t  USBPD_UTILS_MONITOR_TLV_EncodeInit(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, uint8_t tag, uint16_t size_max,
                                            uint8_t *ptr);
void     USBPD_UTILS_MONITOR_TLV_EncodeDeInit(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx);

uint16_t USBPD_UTILS_MONITOR_TLV_Add(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, uint8_t tag, uint16_t size,
                                     const uint8_t *value);
uint16_t USBPD_UTILS_MONITOR_TLV_AddValue(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, const uint8_t *value,
                                          uint16_t size);
void     USBPD_UTILS_MONITOR_TLV_UpdateSizeTag(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, uint16_t size);

uint8_t  USBPD_UTILS_MONITOR_TLV_DecodeInit(usbpd_utils_monitor_tlv_rx_data_t *p_tlv_rx, uint8_t *p_received_string);
void     USBPD_UTILS_MONITOR_TLV_DecodeDeInit(usbpd_utils_monitor_tlv_rx_data_t *p_tlv_rx);

uint8_t  USBPD_UTILS_MONITOR_TLV_Get(usbpd_utils_monitor_tlv_rx_data_t *p_tlv_rx, uint8_t *tag, uint16_t *length,
                                     uint8_t **value);

uint16_t USBPD_UTILS_MONITOR_TLV_GetStringLength(const uint8_t *string);

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

#endif /* USBPD_UTILS_MONITOR_TLV_H */

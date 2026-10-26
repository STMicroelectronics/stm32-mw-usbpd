/**
  ******************************************************************************
  * @file    usbpd_utils_monitor_tlv.c
  * @brief   Provides functions to manipulate TLV structures and/or strings.
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
#include "usbpd_utils_monitor_tlv.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS
  * @{
  */

/** @addtogroup USBPD_UTILS_MONITOR_TLV
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_TLV_Private_Types USBPD_UTILS_MONITOR_TLV Private Types
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_TLV_Private_Constants USBPD_UTILS_MONITOR_TLV Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_TLV_Private_Macros USBPD_UTILS_MONITOR_TLV Private Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_TLV_Private_Functions USBPD_UTILS_MONITOR_TLV Private Functions
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_MONITOR_TLV_Private_Variables USBPD_UTILS_MONITOR_TLV Private Variables
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_UTILS_MONITOR_TLV_Exported_Functions
  * @{
  */

/**
  * @brief  Initializes a @ref usbpd_utils_monitor_tlv_tx_data_t structure, preparing it to be used with
  *         USBPD_UTILS_MONITOR_TLV_Add().
  * @param  p_tlv_tx The @ref usbpd_utils_monitor_tlv_tx_data_t structure to be initialized.
  * @param  tag The tag to be used as the master tag in this instruction.
  * @param  size_max The maximum size of the instruction. This is used to prevent overflow.
  * @param  ptr A pointer to the buffer where the TLV data will be stored.
  * @note   This function must be called before using USBPD_UTILS_MONITOR_TLV_Add() or
  *         USBPD_UTILS_MONITOR_TLV_EncodeDeInit().
  *         It creates the Start of Frame (SOF) and End of Frame (EOF) markers.
  * @note   This function does not handle memory allocation; it is the user's responsibility to
  *         allocate memory for the value.
  * @retval 0 If the initialization is successful.
  * @retval 0xFF If the initialization fails (e.g., buffer is NULL or size_max is too small).
  */
uint8_t USBPD_UTILS_MONITOR_TLV_EncodeInit(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, uint8_t tag, uint16_t size_max,
                                           uint8_t *ptr)
{
  if (ptr == NULL)
  {
    return 0xFFU;  /* Buffer is NULL*/
  }
  if (size_max < 11U)
  {
    return 0xFFU;  /* Because of his small max size, the buffer can't even receive one empty TLV*/
  }

  p_tlv_tx->p_data = ptr;
  p_tlv_tx->max_size = size_max;

  p_tlv_tx->p_data[0U] = USBPD_UTILS_MONITOR_TLV_SOF;
  p_tlv_tx->p_data[1U] = USBPD_UTILS_MONITOR_TLV_SOF;
  p_tlv_tx->p_data[2U] = USBPD_UTILS_MONITOR_TLV_SOF;
  p_tlv_tx->p_data[3U] = USBPD_UTILS_MONITOR_TLV_SOF; /* SOF*/

  p_tlv_tx->p_data[4U] = tag; /* tag*/

  p_tlv_tx->p_data[5U] = 0U;
  p_tlv_tx->p_data[6U] = 0U; /* size*/

  p_tlv_tx->p_data[7U] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[8U] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[9U] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[10U] = USBPD_UTILS_MONITOR_TLV_EOF; /* EOF*/

  p_tlv_tx->eof_position = 7U;

  return 0U;
}

/**
  * @brief  Deinitializes a @ref usbpd_utils_monitor_tlv_tx_data_t structure.
  * @param  p_tlv_tx The @ref usbpd_utils_monitor_tlv_tx_data_t structure to deinitialize.
  * @note   After this, the USBPD_UTILS_MONITOR_TLV_Add() function can no longer be used with this structure
  *         (unless it is reinitialized with USBPD_UTILS_MONITOR_TLV_EncodeInit() ).
  *         If a @ref usbpd_utils_monitor_tlv_tx_data_t structure is used without deinitialization, the associated
  *         string can be altered. Once deinitialization is done, the string used can be directly sent.
  */
void USBPD_UTILS_MONITOR_TLV_EncodeDeInit(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx)
{
  p_tlv_tx->p_data        = NULL;
  p_tlv_tx->eof_position = 0xFFFFU;
  p_tlv_tx->max_size     = 0U;
}

/**
  * @brief  Inserts data in the TLV format inside the value of a host TLV instruction.
  * @param  p_tlv_tx The @ref usbpd_utils_monitor_tlv_tx_data_t structure in which we add data.
  * @param  tag The byte to put as the tag of the data we add.
  * @param  size The size of the data, in bytes.
  * @param  value A pointer to the value to add.
  * @note   Refer to the USBPD MONITOR INTERFACE SPECIFICATION DOCUMENT for more information about this
  *         practice (part 5.3.1).
  *         Before calling this function, USBPD_UTILS_MONITOR_TLV_EncodeInit() must be called with the same
  *         @ref usbpd_utils_monitor_tlv_tx_data_t structure. This function can add data to the same
  *         @ref usbpd_utils_monitor_tlv_tx_data_t structure indefinitely, as long as the maximum size defined
  *         during initialization of the structure isn't reached. SOF and EOF are handled as data is added.
  * @retval The number of bytes written if successful.
  * @retval 0xFFFF If an error occurs (e.g., buffer overflow, NULL data pointer, or uninitialized structure).
  *
  */
uint16_t USBPD_UTILS_MONITOR_TLV_Add(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, uint8_t tag, uint16_t size,
                                     const uint8_t *value)
{
  if (p_tlv_tx->p_data[p_tlv_tx->eof_position] != USBPD_UTILS_MONITOR_TLV_EOF)
  {
    return 0xFFFFU;  /* EOF has been lost. Has any TLV operation failed, or went interrupted ? */
  }
  if ((p_tlv_tx->eof_position + 4U + 3U + size) > p_tlv_tx->max_size)
  {
    return 0xFFFFU;  /* Can't add this TLV, because it will overflow the provided buffer.*/
  }
  if (p_tlv_tx->p_data == NULL)
  {
    return 0xFFFFU;  /* Data points to NULL. Has the encoding been initialized with
                        USBPD_UTILS_MONITOR_TLV_EncodeInit() ? */
  }
  if (p_tlv_tx->eof_position == 0xFFFFU)
  {
    return 0xFFFFU;  /* EOF at -1.  Has the decoding been initialized with USBPD_UTILS_MONITOR_TLV_DecodeInit() ? */
  }

  p_tlv_tx->p_data[p_tlv_tx->eof_position] = tag; /* tag*/
  p_tlv_tx->eof_position++;
  p_tlv_tx->size_position = p_tlv_tx->eof_position;

  p_tlv_tx->p_data[p_tlv_tx->eof_position] = (uint8_t)((size >> 8U) & 0x00FFU); /* size*/
  p_tlv_tx->p_data[p_tlv_tx->eof_position + 1U] = (uint8_t)((size) & 0x00FFU);
  p_tlv_tx->eof_position += 2U;

  for (uint32_t index = 0U; index < size; index++) /* value*/
  {
    p_tlv_tx->p_data[p_tlv_tx->eof_position] = value[index];
    p_tlv_tx->eof_position++;
  }

  p_tlv_tx->p_data[p_tlv_tx->eof_position] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[p_tlv_tx->eof_position + 1U] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[p_tlv_tx->eof_position + 2U] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[p_tlv_tx->eof_position + 3U] = USBPD_UTILS_MONITOR_TLV_EOF; /* EOF*/

  /* Update of the top level size marker*/
  uint16_t total_lgth = ((uint16_t)(p_tlv_tx->p_data[5U]) << 8U) + (uint16_t)(p_tlv_tx->p_data[6U]) + 3U + size;
  p_tlv_tx->p_data[5U] = (uint8_t)(total_lgth >> 8U);
  p_tlv_tx->p_data[6U] = (uint8_t)total_lgth;

  return (3U + size);
}

/**
  * @brief  Inserts a value into the TLV packet.
  * @param  p_tlv_tx The usbpd_utils_monitor_tlv_tx_data_t structure in which the data will be added.
  * @param  size The size of the data, in bytes.
  * @param  value A pointer to the value that will be added.
  * @retval The position of the cursor if the initialization is successful.
  * @retval 0xFF If the initialization fails (e.g., received string is NULL).
  */
uint16_t USBPD_UTILS_MONITOR_TLV_AddValue(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, const uint8_t *value,
                                          uint16_t size)
{
  if (p_tlv_tx->p_data[p_tlv_tx->eof_position] != USBPD_UTILS_MONITOR_TLV_EOF)
  {
    return 0xFFFFU;  /* EOF has been lost. Has any TLV operation failed, or went interrupted ? */
  }
  if ((p_tlv_tx->eof_position + 4U + 3U + size) > p_tlv_tx->max_size)
  {
    return 0xFFFFU;  /* Can't add this TLV, because it will overflow the provided buffer.*/
  }
  if (p_tlv_tx->p_data == NULL)
  {
    return 0xFFFFU;  /* Data points to NULL. Has the encoding been initialized with
                        USBPD_UTILS_MONITOR_TLV_EncodeInit() ? */
  }
  if (p_tlv_tx->eof_position == 0xFFFFU)
  {
    return 0xFFFFU;  /* EOF at -1.  Has the decoding been initialized with USBPD_UTILS_MONITOR_TLV_DecodeInit() ? */
  }

  for (uint32_t index = 0U; index < size; index++)                          /* value*/
  {
    p_tlv_tx->p_data[p_tlv_tx->eof_position] = value[index];
    p_tlv_tx->eof_position++;
  }

  p_tlv_tx->p_data[p_tlv_tx->eof_position] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[p_tlv_tx->eof_position + 1U] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[p_tlv_tx->eof_position + 2U] = USBPD_UTILS_MONITOR_TLV_EOF;
  p_tlv_tx->p_data[p_tlv_tx->eof_position + 3U] = USBPD_UTILS_MONITOR_TLV_EOF;                   /* EOF*/

  /* Update of the top level size marker*/
  uint16_t total_lgth = ((uint16_t)(p_tlv_tx->p_data[5U]) << 8U) + (uint16_t)(p_tlv_tx->p_data[6U]) + size;
  p_tlv_tx->p_data[5U] = (uint8_t)(total_lgth >> 8U);
  p_tlv_tx->p_data[6U] = (uint8_t)(total_lgth);

  return 3U + size;
}

/**
  * @brief  Updates the size of the previously inserted TAG inside the TLV packet.
  * @param  p_tlv_tx The structure @ref usbpd_utils_monitor_tlv_tx_data_t in which the data is added.
  * @param  size The size of the data in bytes.
  */
void USBPD_UTILS_MONITOR_TLV_UpdateSizeTag(usbpd_utils_monitor_tlv_tx_data_t *p_tlv_tx, uint16_t size)
{
  p_tlv_tx->p_data[p_tlv_tx->size_position]      = (uint8_t)(size >> 8U);    /* size*/
  p_tlv_tx->p_data[p_tlv_tx->size_position + 1U] = (uint8_t)(size & 0x00FFU);
}

/**
  * @brief  Initializes a @ref usbpd_utils_monitor_tlv_rx_data_t structure for use with USBPD_UTILS_MONITOR_TLV_Get().
  *         This function sets up the structure and positions the cursor on the first parameter.
  *         This function must be called before calling USBPD_UTILS_MONITOR_TLV_Get() and
  *         USBPD_UTILS_MONITOR_TLV_DecodeDeInit().
  * @param  p_tlv_rx The @ref usbpd_utils_monitor_tlv_rx_data_t structure to be initialized.
  * @param  p_received_string The string to be decoded.
  * @note   The cursor is positioned at the start of the first parameter.
  * @return The position of the cursor if the initialization is successful, 0xFF otherwise.
  */
uint8_t USBPD_UTILS_MONITOR_TLV_DecodeInit(usbpd_utils_monitor_tlv_rx_data_t *p_tlv_rx, uint8_t *p_received_string)
{
  if (p_received_string == NULL)
  {
    return 0xFFU;  /* Received string is NULL*/
  }
  if ((p_received_string[0U] != USBPD_UTILS_MONITOR_TLV_SOF)
      || (p_received_string[1U] != USBPD_UTILS_MONITOR_TLV_SOF)
      || (p_received_string[2U] != USBPD_UTILS_MONITOR_TLV_SOF)
      || (p_received_string[3U] != USBPD_UTILS_MONITOR_TLV_SOF))
  {
    return 0xFFU;  /* Incorrect SOF*/
  }

  p_tlv_rx->p_data = p_received_string;
  p_tlv_rx->read_position = 7; /* Cursor at start of value*/

  return ((uint8_t)(p_tlv_rx->read_position));
}

/**
  * @brief  Deinitializes a @ref usbpd_utils_monitor_tlv_rx_data_t structure.
  * @param  p_tlv_rx The @ref usbpd_utils_monitor_tlv_rx_data_t structure to deinitialize.
  * @note   After calling this function, the USBPD_UTILS_MONITOR_TLV_Get() function can no longer be used with
  *         this structure unless it is reinitialized with USBPD_UTILS_MONITOR_TLV_DecodeInit().
  *         This function is used to deinitialize the @ref usbpd_utils_monitor_tlv_rx_data_t structure and does not
  *         handle any deallocation.
  *         It is typically called when the cursor reaches the end of the TLV data and there is no further
  *         processing to be done.
  */
void USBPD_UTILS_MONITOR_TLV_DecodeDeInit(usbpd_utils_monitor_tlv_rx_data_t *p_tlv_rx)
{
  p_tlv_rx->p_data = NULL;
  p_tlv_rx->read_position = 0U;
}

/**
  * @brief  Retrieves TLV-formatted data from the value of a host TLV instruction.
  * @param  p_tlv_rx The @ref usbpd_utils_monitor_tlv_rx_data_t structure that contains the TLV data structure.
  * @param  tag A pointer to store the decoded tag.
  * @param  length A pointer to store the decoded size.
  * @param  value A double pointer that will contain a pointer directly to the data in the reception buffer.
  * @note   Please refer to the USBPD MONITOR INTERFACE SPECIFICATION DOCUMENT for more information about this
  *         practice (part 5.3.1).
  *         Before calling this function, USBPD_UTILS_MONITOR_TLV_DecodeInit() must be called with the same
  *         @ref usbpd_utils_monitor_tlv_rx_data_t structure.
  *         After reading a parameter, the cursor is positioned at the beginning of the next one.
  *         The return value indicates whether there is another parameter after the current one.
  * @retval 0 if the reading was successful and there is another parameter after.
  * @retval 1 if the reading was successful and it was the last parameter, 0xFF otherwise.
  */
uint8_t USBPD_UTILS_MONITOR_TLV_Get(usbpd_utils_monitor_tlv_rx_data_t *p_tlv_rx, uint8_t *tag, uint16_t *length,
                                    uint8_t **value)
{
  if (p_tlv_rx->p_data == NULL)
  {
    return 0xFFU;  /* Data points to NULL. Has the decoding been initialized with
                      USBPD_UTILS_MONITOR_TLV_DecodeInit() ? */
  }
  if (p_tlv_rx->read_position == 0U)
  {
    return 0xFFU;  /* The cursor is not positioned. Has the decoding been initialized with
                      USBPD_UTILS_MONITOR_TLV_DecodeInit() ? */
  }
  if (p_tlv_rx->p_data[p_tlv_rx->read_position] == USBPD_UTILS_MONITOR_TLV_EOF)
  {
    return 0xFFU;  /* EOF reached. There is not any parameter left to read.*/
  }

  /* tag*/
  *tag = p_tlv_rx->p_data[p_tlv_rx->read_position];
  p_tlv_rx->read_position++;
  /* length*/
  *length = ((uint16_t)(p_tlv_rx->p_data[p_tlv_rx->read_position]) << 8U)
            + (uint16_t)(p_tlv_rx->p_data[p_tlv_rx->read_position + 1U]);
  p_tlv_rx->read_position += 2U;
  /* value*/
  *value = &p_tlv_rx->p_data[p_tlv_rx->read_position];
  p_tlv_rx->read_position += *length;

  return 0U;
}

/**
  * @brief  Returns the length of an 8-bit TLV string in bytes.
  * @param  pString A pointer to a uint8_t array representing the TLV string, with or without SOF and EOF markers.
  * @note   This function calculates the size of the string by relying on the length marker in the TLV format.
  *         It includes the tag, length, and value, but excludes the Start of Frame (SOF) and End of Frame (EOF)
  *         markers, if present.
  * @return The size of the TLV string in bytes, including the tag, length, and value, but excluding the SOF and
  *         EOF markers, if applicable.
  */
uint16_t USBPD_UTILS_MONITOR_TLV_GetStringLength(const uint8_t *pString)
{
  uint16_t length;   /* Variable to be return.*/
  uint8_t start = 0U; /* To indicate the start of the real pString, in case there is a EOF. */

  while (pString[start] == USBPD_UTILS_MONITOR_TLV_SOF)
  {
    start++;
  } /* start variable is now after the SOF if there is one.*/

  length = ((uint16_t)pString[start + 1U] << 8U) | (uint16_t)pString[start + 2U];
  length += 3U;

  return length;
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

/**
  * @}
  */

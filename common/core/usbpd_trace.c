/**
  ******************************************************************************
  * @file    usbpd_trace.c
  * @brief   This file contains trace control functions.
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

/* Includes ------------------------------------------------------------------*/
#include "usbpd_trace.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_TRACE
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_TRACE_Private_Types USBPD_TRACE Private Types
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_TRACE_Private_Constants USBPD_TRACE Private Constants
  * @{
  */

#define TRACE_SIZE_HEADER_TRACE   (9U)              /*!< Type + Time x 2 + portnum + Sop + size  */
#define TRACE_PORT_BIT_POSITION   (5U)              /*!< Bit position of port number in TAG ID   */

#define TLV_SOF                   ((uint8_t)0xFDU)  /*!< Start of Frame                          */
#define TLV_EOF                   ((uint8_t)0xA5U)  /*!< End of Frame                            */
#define TLV_SIZE_MAX              (256U)            /*!< Maximum size of TLV data                */
#define TLV_HEADER_SIZE           (3U)              /*!< Size of TLV header (TAG(1) + LENGTH(2)  */
#define TLV_SOF_SIZE              (4U)              /*!< Size of TLV Start of Frame              */
#define TLV_EOF_SIZE              (4U)              /*!< Size of TLV End of Frame                */

#define DEBUG_STACK_MESSAGE       (0x12U)           /*!< Tag ID for debug stack message          */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_TRACE_Private_Macros USBPD_TRACE Private Macros
  * @{
  */

/**
  * @brief  Set the TAG ID.
  * @param  port Port number.
  * @param  tag_id Tag ID.
  * @retval Tag ID.
  */
#define TRACE_SET_TAG_ID(port, tag_id)  (((port) << TRACE_PORT_BIT_POSITION) | (tag_id))

/**
  * @brief  Write data in the buffer.
  * @param  position Position in the buffer.
  * @param  data Data to write.
  */
#define USBPD_UTILS_TRACER_WRITE_DATA(position, data) \
  do                                                  \
  {                                                   \
    USBPD_UTILS_TRACER_WriteData((position),(data)); \
    (position) = ((position) + 1U);                  \
  } while(0U);

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_TRACE_Private_Functions USBPD_TRACE Private Functions
  * @{
  */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_TRACE_Private_Variables USBPD_TRACE Private Variables
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_TRACE_Exported_Functions
  * @{
  */

/**
  * @brief  Initialize the trace module.
  * @param  None.
  */
void USBPD_TRACE_Init(void)
{
#if defined(USBPD_TRACE)
  static const uint8_t overflow_s[] =
  {
    TLV_SOF, TLV_SOF, TLV_SOF, TLV_SOF,                                        /* Buffer header */
    0x32U,                                                                     /* Tag id        */
    0x0U, 0x18U,                                                               /* Length        */
    0x6U,                                                                      /* Type          */
    0x0U, 0x0U, 0x0U, 0x0U,                                                    /* Time          */
    0x0U,                                                                      /* portnum       */
    0x0U,                                                                      /* SOP           */
    0x0U, 0x0FU,                                                               /* Size          */
    'T', 'R', 'A', 'C', 'E', ' ', 'O', 'V', 'E', 'R', '_', 'F', 'L', 'O', 'W', /* Data          */
    TLV_EOF, TLV_EOF, TLV_EOF, TLV_EOF                                         /* Buffer end    */
  };

  /* initialize tracer module */
  USBPD_UTILS_TRACER_Init();

  /* Initialize PE trace */
  USBPD_PE_SetTrace(USBPD_TRACE_Add, 3U);

  /* Initialize the overflow detection */
  (void)USBPD_UTILS_TRACER_EnableOverFlow(overflow_s, (uint8_t)sizeof(overflow_s));

  /* Print an init message */
  USBPD_TRACE_Add(USBPD_TRACE_DEBUG, USBPD_PORT_0, TRACE_TYPE_INFO, NULL, 0U);

#else
  return;
#endif /* USBPD_TRACE */
}

/**
  * @brief  DeInitialize the trace module.
  * @param  None.
  */
void USBPD_TRACE_DeInit(void)
{
#if defined(USBPD_TRACE)
  USBPD_UTILS_TRACER_DeInit();
#endif /* USBPD_TRACE */
}

/**
  * @brief  Add a trace in the trace buffer.
  * @param  type Type of the trace.
  * @param  portnum Port number.
  * @param  sop SOP type.
  * @param  ptr Pointer on the data to trace.
  * @param  size Size of the data to trace.
  */
void USBPD_TRACE_Add(usbpd_trace_event_t type, uint8_t portnum, uint8_t sop, uint8_t *ptr, uint32_t size)
{
#if defined(USBPD_TRACE)
  uint32_t time;
  int32_t  allocation;
  uint32_t index;

  /*  Get trace timing */
  time = USBPD_UTILS_TRACER_GetTick();

  /* Lock the access to the trace buffer */
  USBPD_UTILS_TRACER_Lock();

  /* Data are encapsulated inside a TLV string */
  /* Allocate buffer size */
  allocation = USBPD_UTILS_TRACER_AllocateBufer(size + TRACE_SIZE_HEADER_TRACE +
                                                TLV_HEADER_SIZE + TLV_SOF_SIZE + TLV_EOF_SIZE);

  /* Check allocation */
  if (allocation  != -1)
  {
    uint16_t writepos = (uint16_t)allocation;

    /* Copy SOF bytes */
    for (index = 0U; index < TLV_SOF_SIZE; index++)
    {
      USBPD_UTILS_TRACER_WRITE_DATA(writepos, TLV_SOF);
    }

    /* Copy the TAG */
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, TRACE_SET_TAG_ID((portnum + 1u), DEBUG_STACK_MESSAGE));
    /* Copy the LENGTH */
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)((size + TRACE_SIZE_HEADER_TRACE) >> 8U));
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)(size + TRACE_SIZE_HEADER_TRACE));

    /* Trace type */
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)type);

    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)time);
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)(time >> 8U));
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)(time >> 16U));
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)(time >> 24U));

    USBPD_UTILS_TRACER_WRITE_DATA(writepos, portnum);
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, sop);

    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)(size >> 8U));
    USBPD_UTILS_TRACER_WRITE_DATA(writepos, (uint8_t)size);

    /* initialize the ptr for Read/Write */
    for (index = 0U; index < size; index++)
    {
      USBPD_UTILS_TRACER_WRITE_DATA(writepos, ptr[index]);
    }

    /* Copy EOF bytes */
    for (index = 0U; index < TLV_EOF_SIZE; index++)
    {
      USBPD_UTILS_TRACER_WRITE_DATA(writepos, TLV_EOF);
    }
  }

  /* Unlock the access to the trace buffer */
  USBPD_UTILS_TRACER_UnLock();

  /* Send the data */
  USBPD_UTILS_TRACER_SendData();
#else
  return;
#endif /* USBPD_TRACE */
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

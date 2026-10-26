/**
  ******************************************************************************
  * @file    usbpd_trace.h
  * @brief   Header file for usbpd_trace.c.
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
#ifndef USBPD_TRACE_H
#define USBPD_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbpd_core.h"
#if defined(USBPD_TRACE)
#include "usbpd_utils_tracer.h"
#include <string.h>
extern int32_t snprintf(char *str, size_t size, const char *format, ...);
#endif /* USBPD_TRACE */

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_TRACE
  * @{
  */

/* Exported constants ---------------------------------------------------------*/
/** @defgroup UCPD_Exported_Constants UCPD Exported Constants
  * @{
  */

#define USBPD_TRACE_VERBOSE_LEVEL_NONE    (0U) /*<! No trace                             */
#define USBPD_TRACE_VERBOSE_LEVEL_ERROR   (1U) /*<! Error trace only                     */
#define USBPD_TRACE_VERBOSE_LEVEL_WARNING (2U) /*<! Error and warning trace              */
#define USBPD_TRACE_VERBOSE_LEVEL_INFO    (3U) /*<! Error, warning and info trace        */
#define USBPD_TRACE_VERBOSE_LEVEL_DEBUG   (4U) /*<! Error, warning, info and debug trace */

/* Default verbosity level if not defined in the source file */
#ifndef USBPD_TRACE_VERBOSE_LEVEL
#define USBPD_TRACE_VERBOSE_LEVEL  USBPD_TRACE_VERBOSE_LEVEL_DEBUG /*<! Default verbosity level */
#endif /* USBPD_TRACE_VERBOSE_LEVEL */

#define TRACE_DEBUG_MAX_SIZE (256U) /*!< Maximum size of debug message in bytes */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup  UCPD_Exported_Types UCPD Exported Types
  * @{
  */

#define TRACE_TYPE_ERROR   0U /*!< Error trace       */
#define TRACE_TYPE_WARNING 1U /*!< Warning trace     */
#define TRACE_TYPE_INFO    2U /*!< Information trace */
#define TRACE_TYPE_DEBUG   3U /*!< Debug trace       */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup UCPD_Exported_Functions UCPD Exported Functions
  * @{
  */

#if !defined(USBPDCORE_LIB_NO_PD)
extern usbpd_trace_entry_point_t USBPD_Trace;
#endif /* !USBPDCORE_LIB_NO_PD */

void USBPD_TRACE_Init(void);
void USBPD_TRACE_DeInit(void);
void USBPD_TRACE_Add(usbpd_trace_event_t type, uint8_t portnum, uint8_t sop, uint8_t *ptr, uint32_t size);

#if defined(USBPD_TRACE)
#define USBPD_TRACE_GENERIC_PRINTF(port_num, type, prefix, prefix_length, ...)       \
  do {                                                                               \
    uint8_t strprint[TRACE_DEBUG_MAX_SIZE];                                          \
    uint8_t *p_ptr = &strprint[prefix_length];                                       \
    (void)memcpy(strprint, (const uint8_t *)(prefix), (prefix_length));              \
    int32_t size_s = snprintf((char*) p_ptr, TRACE_DEBUG_MAX_SIZE - (prefix_length), \
                              __VA_ARGS__);                                          \
    if (size_s > 0)                                                                  \
    {                                                                                \
      USBPD_TRACE_Add(USBPD_TRACE_DEBUG, (port_num), 0, (uint8_t *)strprint,         \
                      ((uint32_t)size_s + (uint32_t)prefix_length));                 \
    }                                                                                \
  } while (0)

#if ((USBPD_TRACE_VERBOSE_LEVEL) >= (USBPD_TRACE_VERBOSE_LEVEL_DEBUG))
#define USBPD_TRACE_DEBUG_PRINTF(port_num, ...) \
  USBPD_TRACE_GENERIC_PRINTF(port_num, TRACE_TYPE_DEBUG, "DEBUG: ", 7U, __VA_ARGS__)
#else
#define USBPD_TRACE_DEBUG_PRINTF(port_num, ...)
#endif /* USBPD_TRACE_VERBOSE_LEVEL >= USBPD_TRACE_VERBOSE_LEVEL_DEBUG */

#if ((USBPD_TRACE_VERBOSE_LEVEL) >= (USBPD_TRACE_VERBOSE_LEVEL_INFO))
#define USBPD_TRACE_INFO_PRINTF(port_num, ...) \
  USBPD_TRACE_GENERIC_PRINTF(port_num, TRACE_TYPE_INFO, "INFO: ", 6U, __VA_ARGS__)
#else
#define USBPD_TRACE_INFO_PRINTF(port_num, ...)
#endif /* USBPD_TRACE_VERBOSE_LEVEL >= USBPD_TRACE_VERBOSE_LEVEL_INFO */

#if ((USBPD_TRACE_VERBOSE_LEVEL) >= (USBPD_TRACE_VERBOSE_LEVEL_WARNING))
#define USBPD_TRACE_WARNING_PRINTF(port_num, ...) \
  USBPD_TRACE_GENERIC_PRINTF(port_num, TRACE_TYPE_WARNING, "WARNING: ", 9U, __VA_ARGS__)
#else
#define USBPD_TRACE_WARNING_PRINTF(port_num, ...)
#endif /* USBPD_TRACE_VERBOSE_LEVEL >= USBPD_TRACE_VERBOSE_LEVEL_WARNING */

#if ((USBPD_TRACE_VERBOSE_LEVEL) >= (USBPD_TRACE_VERBOSE_LEVEL_ERROR))
#define USBPD_TRACE_ERROR_PRINTF(port_num, ...) \
  USBPD_TRACE_GENERIC_PRINTF(port_num, TRACE_TYPE_ERROR, "ERROR: ", 7U, __VA_ARGS__)
#else
#define USBPD_TRACE_ERROR_PRINTF(port_num, ...)
#endif /* USBPD_TRACE_VERBOSE_LEVEL >= USBPD_TRACE_VERBOSE_LEVEL_ERROR */

#else
#define USBPD_TRACE_DEBUG_PRINTF(port_num, ...)
#define USBPD_TRACE_INFO_PRINTF(port_num, ...)
#define USBPD_TRACE_WARNING_PRINTF(port_num, ...)
#define USBPD_TRACE_ERROR_PRINTF(port_num, ...)
#endif /* USBPD_TRACE */

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
#endif

#endif /* USBPD_CAD_H */

/**
  ******************************************************************************
  * @file    usbpd_utils_monitor_os.h
  * @brief   This file contains the monitor os portability macro definition.
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
#ifndef USBPD_UTILS_MONITOR_OS_H
#define USBPD_UTILS_MONITOR_OS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#if defined(USBPD_THREADX)
#include "tx_api.h"
#elif defined(_RTOS)
#if (osCMSIS >= 0x20000U)
#include "FreeRTOS.h"
#include "task.h"
#else
#include "cmsis_os.h"
#endif /* osCMSIS >= 0x20000U */
#elif defined(USE_STM32_UTILITY_OS)
#include "utilities_conf.h"
#endif /* USBPD_THREADX */

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_CORE_OS
  * @{
  */

/** @addtogroup USBPD_CORE_OS_Macro
  * @{
  */
/* Exported define -----------------------------------------------------------*/
/**
  * @brief macro definition used to define the task function.
  */
#if defined(USBPD_THREADX)

#define MONITORDEF_TASK_FUNCTION(__FUNCTION__)   void (__FUNCTION__)(ULONG argument)
#else

#if (osCMSIS < 0x20000U)

#define MONITORDEF_TASK_FUNCTION(__FUNCTION__)   void (__FUNCTION__)(void const *argument)

#else

#define MONITORDEF_TASK_FUNCTION(__FUNCTION__)   void (__FUNCTION__)(void *argument)

#endif /* (osCMSIS < 0x20000U)*/
#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to initialize the OS environment.
  */
#if defined(USBPD_THREADX)
#define MONITOROS_INIT()                                 \
  TX_BYTE_POOL *monitor_pool = (TX_BYTE_POOL*)p_memory; \
  char *ptr;                                         \
  uint32_t _status = TX_SUCCESS;
#elif defined(_RTOS)
#define MONITOROS_INIT()                                 \
  (void)(p_memory);                                 \
  uint32_t _status = USBPD_ENABLE;
#else
#define MONITOROS_INIT()                                 \
  (void)(p_memory);
#endif /* USBPD_THREADX */

/**
  * @brief macro definition the define a queue type.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_QUEUE_ID TX_QUEUE

#else

#define MONITOROS_QUEUE_ID osMessageQueueId_t

#endif /* USBPD_THREADX */

/**
  * @brief macro definition the define a queue type.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_ELEMENT_SIZE TX_1_ULONG

#else

#define MONITOROS_ELEMENT_SIZE sizeof(uint32_t)

#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to define a queue.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_CREATE_QUEUE(_ID_,_NAME_, _ELT_,_ELTSIZE_)                                              \
  do{                                                                                                 \
    _status = tx_byte_allocate(monitor_pool, (VOID **) &ptr,(_ELT_)*sizeof(ULONG)*(_ELTSIZE_),TX_NO_WAIT);\
    if (_status != TX_SUCCESS)                                                                        \
    {                                                                                                 \
      goto error;                                                                                     \
    }                                                                                                 \
    _status = tx_queue_create(&(_ID_),(_NAME_), (_ELTSIZE_), ptr ,(_ELT_)*sizeof(ULONG)*(_ELTSIZE_)); \
    if (_status != TX_SUCCESS)                                                                        \
    {                                                                                                 \
      goto error;                                                                                     \
    }                                                                                                 \
  } while(0);

#else

#if (osCMSIS < 0x20000U)

#define MONITOROS_CREATE_QUEUE(_ID_,_NAME_,_ELT_,_ELTSIZE_)    \
  do                                                       \
  {                                                        \
    osMessageQDef(queuetmp, (_ELT_), (_ELTSIZE_));         \
    (_ID_) = osMessageCreate(osMessageQ(queuetmp), NULL);  \
    if((_ID_) == 0)                                        \
    {                                                      \
      _status = USBPD_DISABLE;                             \
      goto error;                                          \
    }                                                      \
  } while(0);
#else

#define MONITOROS_CREATE_QUEUE(_ID_,_NAME_,_ELT_,_ELTSIZE_)    \
  do                                                       \
  {                                                        \
    (_ID_) = osMessageQueueNew((_ELT_),(_ELTSIZE_), NULL); \
    if((_ID_) == 0)                                        \
    {                                                      \
      _status = USBPD_DISABLE;                             \
      goto error;                                          \
    }                                                      \
  }while(0);

#endif /* osCMSIS < 0x20000U */
#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to read a queue message.
  */
#if defined(USBPD_THREADX)
#define MONITOROS_QUEUE_EVENT ULONG
#else
#define MONITOROS_QUEUE_EVENT uint32_t
#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to read a queue message.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_GETMESSAGE_QUEUE(_ID_, _TIME_,_VALUE_)        \
  do                                                        \
  {                                                         \
    tx_queue_receive(&(_ID_), (void*)&(_VALUE_), (_TIME_)); \
  } while(0);

#else
#if (osCMSIS < 0x20000U)
#define MONITOROS_GETMESSAGE_QUEUE(_ID_, _TIME_,_VALUE_)  \
  do                                                  \
  {                                                   \
    osEvent evt = osMessageGet((_ID_),(_TIME_));      \
    (_VALUE_) = evt.value.v;                          \
  } while(0);
#else
#define MONITOROS_GETMESSAGE_QUEUE(_ID_, _TIME_,_VALUE_)          \
  do                                                          \
  {                                                           \
    (void)osMessageQueueGet((_ID_),&(_VALUE_),NULL,(_TIME_)); \
  } while(0);

#endif /* (osCMSIS < 0x20000U) */
#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to define put a message inside the queue.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_PUT_MESSAGE_QUEUE(_ID_,_MSG_,_TIMEOUT_) \
  do                                                  \
  {                                                   \
    ULONG _msg = _MSG_;                               \
    (void)tx_queue_send(&(_ID_), &_msg,(_TIMEOUT_));  \
  }while(0);
#else

#if (osCMSIS < 0x20000U)
#define MONITOROS_PUT_MESSAGE_QUEUE(_ID_,_MSG_,_TIMEOUT_) \
  do                                                  \
  {                                                   \
    (void)osMessagePut((_ID_),(_MSG_),(_TIMEOUT_));   \
  } while(0);
#else
#define MONITOROS_PUT_MESSAGE_QUEUE(_ID_,_MSG_,_TIMEOUT_)          \
  do                                                           \
  {                                                            \
    uint32_t eventmsg = (_MSG_);                               \
    (void)osMessageQueuePut((_ID_), &eventmsg, 0U,(_TIMEOUT_));\
  } while(0);
#endif /* osCMSIS < 0x20000U */
#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to define a task.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_DEFINE_TASK(_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)
#else
#if (osCMSIS < 0x20000U)

#define MONITOROS_DEFINE_TASK(_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)

#else

#define MONITOROS_DEFINE_TASK(_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)
#endif /* osCMSIS < 0x20000U */
#endif /* USBPD_THREADX */

/**
  * @brief macro definition of the TASK id.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_TASK_ID   TX_THREAD
#else

#define MONITOROS_TASK_ID   osThreadId_t
#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to create a task.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_CREATE_TASK(_ID_,_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_)      \
  do                                                                                \
  {                                                                                 \
    _status = tx_byte_allocate(monitor_pool, (VOID **)&ptr,(_STACK_SIZE_),TX_NO_WAIT);  \
    if(_status != TX_SUCCESS)                                                       \
    {                                                                               \
      goto error;                                                                   \
    }                                                                               \
    _status = tx_thread_create(&(_ID_),#_NAME_,(_FUNC_), 0,                         \
                               ptr,(_STACK_SIZE_),                                  \
                               _PRIORITY_, 1, TX_NO_TIME_SLICE, TX_AUTO_START);     \
    if(_status != TX_SUCCESS)                                                       \
    {                                                                               \
      goto error;                                                                   \
    }                                                                               \
  } while(0);

#else
#if (osCMSIS < 0x20000U)

#define MONITOROS_CREATE_TASK(_ID_,_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_) \
  do                                                                           \
  {                                                                            \
    osThreadDef(_NAME_, _FUNC_, _PRIORITY_, 0, _STACK_SIZE_);                  \
    (_ID_) = osThreadCreate(osThread(_NAME_), (void *)(_PARAM_));              \
    if (NULL == (_ID_))                                                        \
    {                                                                          \
      _status = USBPD_DISABLE;                                                 \
      goto error;                                                              \
    }                                                                          \
  } while(0);
#else

#define MONITOROS_CREATE_TASK(_ID_,_NAME_,_FUNC_,_PRIORITY_,_STACK_SIZE_, _PARAM_) \
  do                                                                           \
  {                                                                            \
    osThreadAttr_t Thread_Atrr = {.name = #_NAME_,.priority = (_PRIORITY_), .stack_size = (_STACK_SIZE_)}; \
    (_ID_) = osThreadNew(_FUNC_, (void *)(_PARAM_),                            \
                         &Thread_Atrr);                                        \
    if (NULL == (_ID_))                                                        \
    {                                                                          \
      _status = USBPD_DISABLE;                                                 \
      goto error;                                                              \
    }                                                                          \
  } while(0);
#endif /* osCMSIS < 0x20000U */
#endif /* USBPD_THREADX */

/**
  * @brief macro definition used to manage the delay.
  */
#if defined(USBPD_THREADX)

#define MONITOROS_DELAY(_TIME_)   tx_thread_sleep(_TIME_)
#else

#define MONITOROS_DELAY(_TIME_)   osDelay(_TIME_)
#endif /* USBPD_THREADX */

/* Exported types ------------------------------------------------------------*/
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

#endif /* USBPD_UTILS_MONITOR_OS_H */

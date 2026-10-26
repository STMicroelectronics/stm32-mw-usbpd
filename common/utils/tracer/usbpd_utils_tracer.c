/**
  ******************************************************************************
  * @file    usbpd_utils_tracer.c
  * @brief   Tracer control module.
  *          This file provides a set of firmware functions to manage the USBPD
  *          tracer utility layer.
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
#include "usbpd_utils_tracer.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS
  * @{
  */

/** @addtogroup USBPD_UTILS_TRACER
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_Private_Types USBPD_UTILS_TRACER Private Types
  * @{
  */

/**
  * @brief  Tracer overflow status.
  */
typedef enum
{
  TRACER_OVERFLOW_NONE,     /*<! No overflow detected */
  TRACER_OVERFLOW_DETECTED, /*<! Overflow detected    */
  TRACER_OVERFLOW_SENT      /*<! Overflow sent        */
} usbpd_tracer_overflow_t;

/**
  * @brief  Tracer context structure.
  */
typedef struct
{
  uint32_t                pTxRead;                                 /*!< Running pointer to the buffer to read         */
  uint32_t                pTxWrite;                                /*!< Running pointer to the buffer to write        */
  uint8_t                 pTxData[USBPD_TRACER_HW_IF_BUFFER_SIZE]; /*!< Buffer to store the Tx data                   */
  uint32_t                size_sent;                               /*!< Size of the data sent                         */
  uint32_t                lowpower_counter;                        /*!< Counter to manage low power mode              */
  volatile uint8_t        counter;                                 /*!< Counter to manage the lock                    */
  const uint8_t          *pOverflowData;                           /*!< Pointer to the overflow data                  */
  uint8_t                 overflowSize;                            /*!< Size of the overflow data                     */
  usbpd_tracer_overflow_t overflowStatus;                          /*!< Overflow status                               */
  uint8_t                 flag_discontinue;                        /*!< Flag to indicate if the data is discontinuous */
} usbpd_tracer_context_t;

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_Private_Constants USBPD_UTILS_TRACER Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_Private_Macros USBPD_UTILS_TRACER Private Macros
  * @{
  */

/**
  * @brief  Init trace emb critical section.
  */
#define TRACE_EMB_INIT_CRITICAL_SECTION() \
  uint32_t primask; /*!< primask for critical section */

/**
  * @brief  Enter trace emb critical section by disabling interrupts.
  */
#define TRACE_EMB_ENTER_CRITICAL_SECTION() \
  primask = __get_PRIMASK();               \
  __disable_irq();

/**
  * @brief  Exit trace emb critical section by enabling interrupts.
  */
#define TRACE_EMB_LEAVE_CRITICAL_SECTION() \
  __set_PRIMASK(primask);

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_Private_Functions USBPD_UTILS_TRACER Private Functions
  * @{
  */

static void USBPD_UTILS_TRACER_TxCpltCallback(void);

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_Private_Variables USBPD_UTILS_TRACER Private Variables
  * @{
  */

static usbpd_tracer_context_t tracer_context; /*!< Tracer context structure */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_UTILS_TRACER_Exported_Functions
  * @{
  */

/**
  * @brief Initialize the USBPD tracer module.
  */
void USBPD_UTILS_TRACER_Init(void)
{
  /* Initialize the ptr for Read/Write */
  (void)memset(&tracer_context, 0, sizeof(usbpd_tracer_context_t));

  /* Initialize tracer hardware interface */
  USBPD_UTILS_TRACER_HW_IF_Init();

  /* Initialize the lowpower aspect */
  USBPD_UTILS_TRACER_LowPowerInit();

  /* Register Tx complete function */
  (void)USBPD_UTILS_TRACER_HW_IF_RegTxCpltCb(USBPD_UTILS_TRACER_TxCpltCallback);
}

/**
  * @brief DeInitialize the USBPD tracer module.
  */
void USBPD_UTILS_TRACER_DeInit(void)
{
  /* Reset context */
  (void)memset(&tracer_context, 0, sizeof(usbpd_tracer_context_t));

  /* DeInitialize tracer hardware interface */
  USBPD_UTILS_TRACER_HW_IF_DeInit();

  /* DeInitialize the lowpower aspect */
  USBPD_UTILS_TRACER_LowPowerDeInit();
}

/**
  * @brief  Add data to the trace buffer.
  * @param  p_data Pointer on the data to send.
  * @param  size size of the data to send.
  */
void USBPD_UTILS_TRACER_Add(const uint8_t *p_data, uint32_t size)
{
  int32_t writepos;
  const uint8_t *data_to_write = p_data;
  uint32_t index;

  /* Data processing */
  USBPD_UTILS_TRACER_Lock();
  writepos = USBPD_UTILS_TRACER_AllocateBufer(size);

  /* If allocation failed, unlock and return */
  if (((uint32_t)writepos) >= USBPD_TRACER_HW_IF_BUFFER_SIZE)
  {
    USBPD_UTILS_TRACER_UnLock();
    return;
  }

  /* Initialize the ptr for Read/Write */
  for (index = 0U; index < size; index++)
  {
    /* Write data into the buffer */
    tracer_context.pTxData[writepos % ((int32_t)USBPD_TRACER_HW_IF_BUFFER_SIZE)] = data_to_write[index];
    /* Update the write position */
    writepos = (writepos + 1) % ((int32_t)USBPD_TRACER_HW_IF_BUFFER_SIZE);
  }
  USBPD_UTILS_TRACER_UnLock();

  /* Tx processing */
  USBPD_UTILS_TRACER_SendData();
}

/**
  * @brief  Add one data into the trace buffer.
  * @param  pos Position in the buffer.
  * @param  data Data value.
  */
void USBPD_UTILS_TRACER_WriteData(uint16_t pos, uint8_t data)
{
  /* Write data into the buffer */
  tracer_context.pTxData[pos % USBPD_TRACER_HW_IF_BUFFER_SIZE] = data;
}

/**
  * @brief Send tracer data.
  */
void USBPD_UTILS_TRACER_SendData(void)
{
  uint32_t pBegin;
  uint32_t pEnd;
  TRACE_EMB_INIT_CRITICAL_SECTION();

  TRACE_EMB_ENTER_CRITICAL_SECTION();

  if (0U == tracer_context.counter)
  {
    pBegin = tracer_context.pTxRead;
    pEnd = tracer_context.pTxWrite;

    if (pBegin != pEnd)
    {
      USBPD_UTILS_TRACER_Lock();

      if (pEnd > pBegin)
      {
        tracer_context.size_sent = pEnd - pBegin;
        tracer_context.flag_discontinue = 0U;
      }
      else  /* pBegin > pEnd */
      {
        /* Wrap around */
        tracer_context.size_sent = USBPD_TRACER_HW_IF_BUFFER_SIZE - pBegin;
        tracer_context.flag_discontinue = 1U;
      }
      USBPD_UTILS_TRACER_LowPowerSendData();

      tracer_context.lowpower_counter++;

      TRACE_EMB_LEAVE_CRITICAL_SECTION();
      USBPD_UTILS_TRACER_HW_IF_SendData((const uint8_t *)(&(tracer_context.pTxData[pBegin])), tracer_context.size_sent);
      TRACE_EMB_ENTER_CRITICAL_SECTION();
    }
  }

  /* Low power processing */
  switch (tracer_context.lowpower_counter)
  {
    case 1U:
      USBPD_UTILS_TRACER_LowPowerSendData();
      break;

    case 0U:
      USBPD_UTILS_TRACER_LowPowerSendDataComplete();
      break;

    default:
      break;
  }

  TRACE_EMB_LEAVE_CRITICAL_SECTION();
}

/**
  * @brief Lock the trace buffer.
  */
void USBPD_UTILS_TRACER_Lock(void)
{
  TRACE_EMB_INIT_CRITICAL_SECTION();
  TRACE_EMB_ENTER_CRITICAL_SECTION();
  tracer_context.counter++;
  TRACE_EMB_LEAVE_CRITICAL_SECTION();
}

/**
  * @brief UnLock the trace buffer.
  */
void USBPD_UTILS_TRACER_UnLock(void)
{
  TRACE_EMB_INIT_CRITICAL_SECTION();
  TRACE_EMB_ENTER_CRITICAL_SECTION();
  tracer_context.counter--;
  TRACE_EMB_LEAVE_CRITICAL_SECTION();
}

/**
  * @brief  Register the callback function to call when a data is received.
  * @param  p_callback Pointer to the trace interface Rx callback function.
  */
void USBPD_UTILS_TRACER_Register_Rx_Cplt_Callback(usbpd_tracer_rx_cb_t p_callback)
{
  /* Register the callback function */
  (void)USBPD_UTILS_TRACER_HW_IF_RegisterRxCallback(p_callback);
}

/**
  * @brief Start the data reception for the monitor module.
  */
void USBPD_UTILS_TRACER_StartRX(void)
{
  /* Start the reception */
  (void)USBPD_UTILS_TRACER_HW_IF_StartRX();
}

/**
  * @brief  Allocate space inside the tracer buffer.
  * @param  size size of the data to allocate.
  * @returns Write position inside the buffer, or -1 if no space available.
  */
int32_t USBPD_UTILS_TRACER_AllocateBufer(uint32_t size)
{
  uint32_t freesize;
  int32_t pos = -1;
  TRACE_EMB_INIT_CRITICAL_SECTION();

  TRACE_EMB_ENTER_CRITICAL_SECTION();

  /* Compute the free size */
  if (tracer_context.pTxWrite == tracer_context.pTxRead)
  {
    freesize = USBPD_TRACER_HW_IF_BUFFER_SIZE;
  }
  else
  {
    /* Check if the buffer is not full */
    if (tracer_context.pTxWrite > tracer_context.pTxRead)
    {
      /* Wrap around */
      freesize = USBPD_TRACER_HW_IF_BUFFER_SIZE - tracer_context.pTxWrite + tracer_context.pTxRead;
    }
    else
    {
      /* No wrap around */
      freesize = tracer_context.pTxRead - tracer_context.pTxWrite;
    }
  }

  /* Check if the size to allocate is available */
  if (freesize > size)
  {
    /* Allocate the buffer */
    pos = (int32_t)tracer_context.pTxWrite;
    tracer_context.pTxWrite = (tracer_context.pTxWrite + size) % USBPD_TRACER_HW_IF_BUFFER_SIZE;
    if (TRACER_OVERFLOW_SENT == tracer_context.overflowStatus)
    {
      tracer_context.overflowStatus = TRACER_OVERFLOW_NONE;
    }
  }
  else
  {
    /* Overflow detected */
    if (TRACER_OVERFLOW_NONE == tracer_context.overflowStatus)
    {
      tracer_context.overflowStatus = TRACER_OVERFLOW_DETECTED;
    }
  }

  TRACE_EMB_LEAVE_CRITICAL_SECTION();

  return pos;
}

/**
  * @brief  Enable the overflow detection.
  * @param  Data Pointer on the overflow string.
  * @param  size Of the overflow string.
  * @retval USBPD_UTILS_TRACER_OK operation completed successfully.
  * @retval USBPD_UTILS_TRACER_ERROR size is null.
  */
usbpd_tracer_status_t USBPD_UTILS_TRACER_EnableOverFlow(const uint8_t *p_data, uint8_t size)
{
  /* Check if the size is not null */
  if (size != 0U)
  {
    /* Enable the overflow detection */
    tracer_context.pOverflowData = p_data;
    tracer_context.overflowSize = size;

    return USBPD_UTILS_TRACER_OK;
  }

  return USBPD_UTILS_TRACER_ERROR;
}


/**
  * @brief Initialize the low power mode for the USBPD tracer.
  */
__WEAK void USBPD_UTILS_TRACER_LowPowerInit(void)
{
  /** @warning This function must not be modified, when the callback is needed,
               the USBPD_UTILS_TRACER_LowPowerInit() can be implemented in the user file
   */
}

/**
  * @brief DeInitialize the low power mode for the USBPD tracer.
  */
__WEAK void USBPD_UTILS_TRACER_LowPowerDeInit(void)
{
  /** @warning This function must not be modified, when the callback is needed,
               the USBPD_UTILS_TRACER_LowPowerDeInit() can be implemented in the user file
   */
}

/**
  * @brief Send data in low power mode.
  */
__WEAK void USBPD_UTILS_TRACER_LowPowerSendData(void)
{
  /** @warning This function must not be modified, when the callback is needed,
               the USBPD_UTILS_TRACER_LowPowerSendData() can be implemented in the user file
   */
}

/**
  * @brief Send data complete in low power mode.
  */
__WEAK void USBPD_UTILS_TRACER_LowPowerSendDataComplete(void)
{
  /** @warning This function must not be modified, when the callback is needed,
               the USBPD_UTILS_TRACER_LowPowerSendDataComplete() can be implemented in the user file
   */
}

/**
  * @brief Get the current time tick for trace timestamping.
  */
uint32_t USBPD_UTILS_TRACER_GetTick(void)
{
  return USBPD_UTILS_TRACER_HW_IF_GetTick();
}


/**
  * @}
  */

/** @addtogroup USBPD_UTILS_TRACER_Private_Functions USBPD_UTILS_TRACER Private Functions
  * @{
  */

/**
  * @brief Tracer Tx complete callback.
  */
static void USBPD_UTILS_TRACER_TxCpltCallback(void)
{
  TRACE_EMB_INIT_CRITICAL_SECTION();
  TRACE_EMB_ENTER_CRITICAL_SECTION();

  /* Update the read pointer */
  tracer_context.pTxRead = (tracer_context.pTxRead + tracer_context.size_sent) % USBPD_TRACER_HW_IF_BUFFER_SIZE;

  /* If overflow detected, send the overflow data */
  if ((tracer_context.pOverflowData != NULL) && (tracer_context.overflowStatus == TRACER_OVERFLOW_DETECTED)
      && (tracer_context.flag_discontinue == 0U))
  {
    tracer_context.overflowStatus = TRACER_OVERFLOW_SENT;
    TRACE_EMB_LEAVE_CRITICAL_SECTION();

    USBPD_UTILS_TRACER_HW_IF_SendData(tracer_context.pOverflowData, tracer_context.overflowSize);
  }
  else
  {
    /* Else, send the next data */
    tracer_context.lowpower_counter--;
    TRACE_EMB_LEAVE_CRITICAL_SECTION();

    USBPD_UTILS_TRACER_UnLock();
    USBPD_UTILS_TRACER_SendData();
  }
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

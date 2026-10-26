/**
  ******************************************************************************
  * @file    usbpd_utils_tracer_hw_if.c
  * @brief   Trace hardware interface control module.
  *          This file provides a set of firmware functions to manage the usbpd
  *          trace utility layer hardware interface.
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
#include "usbpd_utils_tracer_hw_if.h"
#include "usbpd_config.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_UTILS
  * @{
  */

/** @addtogroup USBPD_UTILS_TRACER_HW_IF
  * @{
  */

#if !defined (USE_HAL_UART_REGISTER_CALLBACKS) || (USE_HAL_UART_REGISTER_CALLBACKS != 1)
#error USE_HAL_UART_REGISTER_CALLBACKS is required for this interface
#endif /* !USE_HAL_UART_REGISTER_CALLBACKS */

#if !defined (USE_HAL_UART_GET_LAST_ERRORS) || (USE_HAL_UART_GET_LAST_ERRORS != 1)
#error USE_HAL_UART_GET_LAST_ERRORS is required for this interface
#endif /* !USE_HAL_UART_GET_LAST_ERRORS */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_HW_IF_Private_Types USBPD_UTILS_TRACER_HW_IF Private Types
  * @{
  */

/**
  * @brief USBPD tracer hardware interface handle structure definition.
  */
typedef struct
{
  hal_uart_handle_t         *huart;     /*!< HAL UART Handle                         */
  usbpd_trace_if_callbacks_t cbs;       /*!< USBPD_TRACER_HW_IF callbacks            */
  uint8_t                    flag_init; /*!< Tracks the init status of the interface */
  uint8_t                    rx_buff;   /*!< Buffer for data reception               */
} usbpd_tracer_hw_if_handle_t;

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_HW_IF_Private_Constants USBPD_UTILS_TRACER_HW_IF Private Constants
  * @{
  */

#define RX_TRANSFER_SIZE (0x1U) /*!< Size of reception transfer in byte */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_HW_IF_Private_Macros USBPD_UTILS_TRACER_HW_IF Private Macros
  * @{
  */

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_HW_IF_Private_Functions USBPD_UTILS_TRACER_HW_IF Private Functions
  * @{
  */

static void TRACER_HW_IF_TxCpltCallback(hal_uart_handle_t *huart);
static void TRACER_HW_IF_RxCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte,
                                        hal_uart_rx_event_types_t rx_event);

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_UTILS_TRACER_HW_IF_Private_Variables USBPD_UTILS_TRACER_HW_IF Private Variables
  * @{
  */

static usbpd_tracer_hw_if_handle_t h_tracer_hw_if; /*!< USBPD tracer hardware interface handle */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_UTILS_TRACER_HW_IF_Exported_Functions
  * @{
  */

/**
  * @brief  USBPD tracer hardware interface init.
  * @param  huart Pointer to a @ref hal_uart_handle_t structure which contains the UART instance.
  */
void USBPD_UTILS_TRACER_HW_IF_Init(void)
{
  /* Reset handle */
  h_tracer_hw_if.rx_buff = 0U;
  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_RxCplt = NULL;
  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_TxCplt = NULL;

  h_tracer_hw_if.huart = (hal_uart_handle_t *)usbpd_trace_gethandle();

  /* Register the trace TxCplt callback */
  (void)HAL_UART_RegisterTxCpltCallback(h_tracer_hw_if.huart, TRACER_HW_IF_TxCpltCallback);

  /* Register the trace RxCplt callback */
  (void)HAL_UART_RegisterRxCpltCallback(h_tracer_hw_if.huart, TRACER_HW_IF_RxCpltCallback);

  /* Disable UART clock to save on power when not used */
  usbpd_trace_disable_clk();

  h_tracer_hw_if.flag_init = 1U;
}

/**
  * @brief USBPD tracer hardware interface Deinit.
  */
void USBPD_UTILS_TRACER_HW_IF_DeInit(void)
{
  /* Un-Register the trace TxCplt callback */
  (void)HAL_UART_RegisterTxCpltCallback(h_tracer_hw_if.huart, HAL_UART_TxCpltCallback);

  usbpd_trace_disable_clk();

  /* Reset handle */
  h_tracer_hw_if.huart = NULL;
  h_tracer_hw_if.rx_buff = 0U;
  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_RxCplt = NULL;
  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_TxCplt = NULL;

  h_tracer_hw_if.flag_init = 0U;
}

/**
  * @brief  Register the USBPD tracer hardware interface Rx Callback.
  * @param  p_callback Pointer to the trace interface Rx callback function.
  * @retval HAL_OK The function has been registered.
  * @retval HAL_INVALID_PARAM p_callback is NULL.
  */
hal_status_t USBPD_UTILS_TRACER_HW_IF_RegisterRxCallback(usbpd_trace_hw_if_rx_cb_t p_callback)
{
#if defined(USE_HAL_CHECK_PARAM) && (USE_HAL_CHECK_PARAM == 1)
  /* Check input parameter */
  if (p_callback == NULL)
  {
    return HAL_INVALID_PARAM;
  }
#endif /* USE_HAL_CHECK_PARAM */

  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_RxCplt = p_callback;

  return HAL_OK;
}

/**
  * @brief  Register the USBPD tracer hardware interface Tx Complete Callback.
  * @param  p_callback Pointer to the trace interface Tx complete callback function.
  * @retval HAL_OK The function has been registered.
  * @retval HAL_INVALID_PARAM p_callback is NULL.
  */
hal_status_t USBPD_UTILS_TRACER_HW_IF_RegTxCpltCb(usbpd_trace_hw_if_tx_cb_t p_callback)
{
#if defined(USE_HAL_CHECK_PARAM) && (USE_HAL_CHECK_PARAM == 1)
  /* Check input parameter */
  if (p_callback == NULL)
  {
    return HAL_INVALID_PARAM;
  }
#endif /* USE_HAL_CHECK_PARAM */

  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_TxCplt = p_callback;

  return HAL_OK;
}

/**
  * @brief  Start USBPD tracer hardware interface Rx reception.
  * @retval HAL_OK The reception has been correctly started.
  * @retval HAL_ERROR The reception has failed to start.
  */
hal_status_t USBPD_UTILS_TRACER_HW_IF_StartRX(void)
{
  /* Enable UART clock for reception */
  usbpd_trace_enable_clk();

  /* Receive one byte at a time */
  if (HAL_UART_Receive_IT(h_tracer_hw_if.huart, &(h_tracer_hw_if.rx_buff), RX_TRANSFER_SIZE) != HAL_OK)
  {
    /* Error handling */
    return HAL_ERROR;
  }

  return HAL_OK;
}

/**
  * @brief  USBPD tracer hardware interface send data.
  * @param  pdata Data pointer.
  * @param  size Size to send in byte.
  */
void USBPD_UTILS_TRACER_HW_IF_SendData(const uint8_t *pdata, uint32_t size)
{
  if (h_tracer_hw_if.flag_init != 1U)
  {
    /* Forbid sending data in case trace is not initialized */
    return;
  }

  /* Enable UART clock for transmission */
  usbpd_trace_enable_clk();

  /* Transmit data */
  (void)HAL_UART_Transmit_DMA(h_tracer_hw_if.huart, pdata, size);
}


/**
  * @brief Get the current time tick for trace timestamping.
  */
uint32_t USBPD_UTILS_TRACER_HW_IF_GetTick(void)
{
  /* Return HAL tick */
  return HAL_GetTick();
}

/**
  * @}
  */

/** @addtogroup USBPD_UTILS_TRACER_HW_IF_Private_Functions USBPD_UTILS_TRACER_HW_IF Private Functions
  * @{
  */

/**
  * @brief  Tx transfer completed callback.
  * @param  huart Pointer to a @ref hal_uart_handle_t structure which contains the UART instance.
  */
static void TRACER_HW_IF_TxCpltCallback(hal_uart_handle_t *huart)
{
  const hal_uart_handle_t *huart_cst = huart;
  STM32_UNUSED(huart_cst);

  /* Disable UART clock to save on power when not used */
  if (h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_RxCplt == NULL)
  {
    usbpd_trace_disable_clk();
  }

  /* Call the tracer Tx complete callback */
  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_TxCplt();
}

/**
  * @brief  Rx transfer completed callback.
  * @param  huart Pointer to a @ref hal_uart_handle_t structure which contains the UART instance.
  */
static void TRACER_HW_IF_RxCpltCallback(hal_uart_handle_t *huart, uint32_t size_byte,
                                        hal_uart_rx_event_types_t rx_event)
{
  (void)size_byte; /* Unused parameter */
  (void)rx_event; /* Unused parameter */
  uint8_t flag_error;
  flag_error = (HAL_UART_GetLastErrorCodes(huart) == HAL_UART_RECEIVE_ERROR_NONE) ? 0U : 1U;

  /* Call the tracer Tx complete callback */
  h_tracer_hw_if.cbs.USBPD_TRACER_HW_IF_RxCplt(h_tracer_hw_if.rx_buff, flag_error);
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

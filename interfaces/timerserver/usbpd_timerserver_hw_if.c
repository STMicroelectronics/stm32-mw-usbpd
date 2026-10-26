/**
  ******************************************************************************
  * @file    usbpd_timerserver_hw_if.c
  * @brief   Timer server hardware interface module.
  *          This file provides a set of functions to manage the timer server
  *          interface.
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
#include "usbpd_timerserver_hw_if.h"

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_TIMESERVER_HW_IF
  * @{
  */

/* Private types -----------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_HW_IF_Private_Types USBPD_TIMESERVER_HW_IF Private Types
  * @{
  */

/**
  * @}
  */

/* Private constants ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_HW_IF_Private_Constants USBPD_TIMESERVER_HW_IF Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros -------------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_HW_IF_Private_Macros USBPD_TIMESERVER_HW_IF Private Macros
  * @{
  */

/**
  * @brief Retrieve TIM instance from handle
  */
#define TIM_GET_INSTANCE(handle) \
  ((TIM_TypeDef *)((uint32_t)((handle)->instance)))

/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_HW_IF_Private_Functions USBPD_TIMESERVER_HW_IF Private Functions
  * @{
  */
#if defined(USBPD_TIMERSERVER_TIM_MS)
#if defined(USE_HAL_TIM_REGISTER_CALLBACKS) && (USE_HAL_TIM_REGISTER_CALLBACKS == 1U)
void USBPD_ms_TIM_UpdateCallback(hal_tim_handle_t *htim);
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
#endif /* USBPD_TIMERSERVER_TIM_MS */

/**
  * @}
  */

/* Private variables ---------------------------------------------------------*/
/** @defgroup USBPD_TIMESERVER_HW_IF_Private_Variables USBPD_TIMESERVER_HW_IF Private Variables
  * @{
  */

static int8_t timer_initcounter = 0; /*!< Counter managing the number of timer initialization */
static hal_tim_handle_t *h_timserv; /*!< Handle to the timer server */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup USBPD_TIMESERVER_HW_IF_Exported_Functions
  * @{
  */
/**
  * @brief  Protocol timing management in ms.
  */
void USBPD_TIMERSERVER_TimerCounter(void)
{
  const usbpd_params_t *dpm_params_p0;

  dpm_params_p0 = USBPD_CAD_Get_DPM_Params(USBPD_PORT_0);

  if (dpm_params_p0 == NULL)
  {
    return;
  }

  /* Call PE/PRL timers functions only if DPM is initialized */
  if (USBPD_TRUE == dpm_params_p0->dpm_flag_initialized)
  {
    USBPD_PE_UserTimerCounter(USBPD_PORT_0);
    USBPD_PE_TimerCounter(USBPD_PORT_0);
    USBPD_PRL_TimerCounter(USBPD_PORT_0);
  }
#if USBPD_PORT_COUNT==2
  const usbpd_params_t *dpm_params_p1;

  dpm_params_p1 = USBPD_CAD_Get_DPM_Params(USBPD_PORT_1);

  if (dpm_params_p1 == NULL)
  {
    return;
  }

  if (USBPD_TRUE == dpm_params_p1->dpm_flag_initialized)
  {
    USBPD_PE_UserTimerCounter(USBPD_PORT_1);
    USBPD_PE_TimerCounter(USBPD_PORT_1);
    USBPD_PRL_TimerCounter(USBPD_PORT_1);
  }
#endif /* USBPD_PORT_COUNT == 2 */
}

#if defined(USBPD_TIMERSERVER_TIM_MS)
/**
  * @brief  Initialize timer to manage protocol timing in ms.
  */
usbpd_status_t USBPD_TIMERSERVER_HW_IF_Ms_Init()
{
#if defined(USE_HAL_TIM_REGISTER_CALLBACKS) && (USE_HAL_TIM_REGISTER_CALLBACKS == 1U)
  if (HAL_TIM_RegisterUpdateCallback(usbpd_tim_ms_gethandle(), USBPD_ms_TIM_UpdateCallback) != HAL_OK)
  {
    return USBPD_ERROR;
  }
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */

  HAL_CORTEX_NVIC_SetPriority(USBPD_TIM_MS_IRQ, (hal_cortex_nvic_preemp_priority_t)15,
                              HAL_CORTEX_NVIC_SUB_PRIORITY_0);
  if (HAL_TIM_Start_IT(usbpd_tim_ms_gethandle()) != HAL_OK)
  {
    return USBPD_ERROR;
  }
  return USBPD_OK;
}

/**
  * @brief Timer update interrupt callback.
  * @note  Depending on the configuration, it can be registered as a callback or used directly.
  */
#if defined(USE_HAL_TIM_REGISTER_CALLBACKS) && (USE_HAL_TIM_REGISTER_CALLBACKS == 1U)
void USBPD_ms_TIM_UpdateCallback(hal_tim_handle_t *htim)
#else
void HAL_TIM_UpdateCallback(hal_tim_handle_t *htim)
#endif /* USE_HAL_TIM_REGISTER_CALLBACKS */
{
  const hal_tim_handle_t *htim_cst = htim;
  STM32_UNUSED(htim_cst);
  USBPD_TIMERSERVER_TimerCounter();
}
#endif /* USBPD_TIMERSERVER_TIM_MS */

/**
  * @brief  Initialize timer to manage protocol timing in us.
  */
hal_status_t USBPD_TIMERSERVER_HW_IF_Init()
{
  h_timserv = usbpd_tim_us_gethandle();

  /* If the timer is not already configured */
  if (0 == timer_initcounter)
  {
    /* Enable the timer clock */
    usbpd_tim_us_enable_clk();

    /* Start the timer */
    if (HAL_TIM_Start(h_timserv) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Start TIM output channel */
    if (HAL_TIM_OC_StartChannel(h_timserv, HAL_TIM_CHANNEL_1) != HAL_OK)
    {
      return HAL_ERROR;
    }
    if (HAL_TIM_OC_StartChannel(h_timserv, HAL_TIM_CHANNEL_2) != HAL_OK)
    {
      return HAL_ERROR;
    }
#if defined(LL_TIM_CHANNEL_CH3)
    if (h_timserv->channel_states[2U] == HAL_TIM_OC_CHANNEL_STATE_IDLE)
    {
      if (HAL_TIM_OC_StartChannel(h_timserv, HAL_TIM_CHANNEL_3) != HAL_OK)
      {
        return HAL_ERROR;
      }
    }
#endif /* LL_TIM_CHANNEL_CH3 */
#if defined(LL_TIM_CHANNEL_CH4)
    if (h_timserv->channel_states[3U] == HAL_TIM_OC_CHANNEL_STATE_IDLE)
    {
      if (HAL_TIM_OC_StartChannel(h_timserv, HAL_TIM_CHANNEL_4) != HAL_OK)
      {
        return HAL_ERROR;
      }
    }
#endif /* LL_TIM_CHANNEL_CH4 */
#if defined(LL_TIM_CHANNEL_CH5)
    if (h_timserv->channel_states[4U] == HAL_TIM_OC_CHANNEL_STATE_IDLE)
    {
      if (HAL_TIM_OC_StartChannel(h_timserv, HAL_TIM_CHANNEL_5) != HAL_OK)
      {
        return HAL_ERROR;
      }
    }
#endif /* LL_TIM_CHANNEL_CH5 */
#if defined(LL_TIM_CHANNEL_CH6)
    if (h_timserv->channel_states[5U] == HAL_TIM_OC_CHANNEL_STATE_IDLE)
    {
      if (HAL_TIM_OC_StartChannel(h_timserv, HAL_TIM_CHANNEL_6) != HAL_OK)
      {
        return HAL_ERROR;
      }
    }
#endif /* LL_TIM_CHANNEL_CH6 */
  }

  /* Increment the counter variable, to avoid deinitialing the timer if another port is still connected */
  timer_initcounter++;

  return HAL_OK;
}

/**
  * @brief  DeInitialize timer.
  */
void USBPD_TIMERSERVER_HW_IF_DeInit(void)
{
  /* Decrement the counter variable, to avoid deinitialing the timer if another port is still connected */
  timer_initcounter--;

  /* If no port is using the timer */
  if (0 == timer_initcounter)
  {
    /* Stop The timer */
    (void)HAL_TIM_Stop(h_timserv);

    /* Stop TIM output channel */
    (void)HAL_TIM_OC_StopChannel(h_timserv, HAL_TIM_CHANNEL_1);
    (void)HAL_TIM_OC_StopChannel(h_timserv, HAL_TIM_CHANNEL_2);
#if defined(LL_TIM_CHANNEL_CH3)
    if (h_timserv->channel_states[2U] == HAL_TIM_OC_CHANNEL_STATE_ACTIVE)
    {
      (void)HAL_TIM_OC_StopChannel(h_timserv, HAL_TIM_CHANNEL_3);
    }
#endif /* LL_TIM_CHANNEL_CH3 */
#if defined(LL_TIM_CHANNEL_CH4)
    if (h_timserv->channel_states[3U] == HAL_TIM_OC_CHANNEL_STATE_ACTIVE)
    {
      (void)HAL_TIM_OC_StopChannel(h_timserv, HAL_TIM_CHANNEL_4);
    }
#endif /* LL_TIM_CHANNEL_CH4 */
#if defined(LL_TIM_CHANNEL_CH5)
    if (h_timserv->channel_states[4U] == HAL_TIM_OC_CHANNEL_STATE_ACTIVE)
    {
      (void)HAL_TIM_OC_StopChannel(h_timserv, HAL_TIM_CHANNEL_5);
    }
#endif /* LL_TIM_CHANNEL_CH5 */
#if defined(LL_TIM_CHANNEL_CH6)
    if (h_timserv->channel_states[5U] == HAL_TIM_OC_CHANNEL_STATE_ACTIVE)
    {
      (void)HAL_TIM_OC_StopChannel(h_timserv, HAL_TIM_CHANNEL_6);
    }
#endif /* LL_TIM_CHANNEL_CH6 */

    /* Disable the timer clock */
    usbpd_us_tim_disable_clk();
  }
}

/**
  * @brief  Start the timer counting.
  * @param  id Timer ID.
  * @param  time_us Time in us.
  */
hal_status_t USBPD_TIMERSERVER_HW_IF_Start(uint8_t timer_id, uint32_t time_us)
{
  uint32_t pulse = (time_us + HAL_TIM_GetCounter(h_timserv)) % USBPD_TIMERSERVER_HW_IF_MAX_TIME;

  static const hal_tim_oc_compare_unit_t oc_compare_unit_lut[] =
  {
    HAL_TIM_OC_COMPARE_UNIT_1,
    HAL_TIM_OC_COMPARE_UNIT_2,
#if defined(HAL_TIM_OC_COMPARE_UNIT_3)
    HAL_TIM_OC_COMPARE_UNIT_3,
#endif /* HAL_TIM_OC_COMPARE_UNIT_3 */
#if defined(HAL_TIM_OC_COMPARE_UNIT_4)
    HAL_TIM_OC_COMPARE_UNIT_4,
#endif /* HAL_TIM_OC_COMPARE_UNIT_4 */
#if defined(HAL_TIM_OC_COMPARE_UNIT_5)
    HAL_TIM_OC_COMPARE_UNIT_5,
#endif /* HAL_TIM_OC_COMPARE_UNIT_5 */
#if defined(HAL_TIM_OC_COMPARE_UNIT_6)
    HAL_TIM_OC_COMPARE_UNIT_6,
#endif /* HAL_TIM_OC_COMPARE_UNIT_6 */
  };

  static void (*p_funct_clear_cc_flag[])(TIM_TypeDef * timx) =
  {
    LL_TIM_ClearFlag_CC1,
    LL_TIM_ClearFlag_CC2,
#if defined(LL_TIM_CHANNEL_CH3)
    LL_TIM_ClearFlag_CC3,
#endif /* LL_TIM_CHANNEL_CH3 */
#if defined(LL_TIM_CHANNEL_CH4)
    LL_TIM_ClearFlag_CC4,
#endif /* LL_TIM_CHANNEL_CH4 */
#if defined(LL_TIM_CHANNEL_CH5)
    LL_TIM_ClearFlag_CC5,
#endif /* LL_TIM_CHANNEL_CH5 */
#if defined(LL_TIM_CHANNEL_CH6)
    LL_TIM_ClearFlag_CC6,
#endif /* LL_TIM_CHANNEL_CH6 */
  };

  size_t lut_size = sizeof(oc_compare_unit_lut) / sizeof(oc_compare_unit_lut[0]);

  if (timer_id < lut_size)
  {
    if (h_timserv->channel_states[timer_id] == HAL_TIM_OC_CHANNEL_STATE_ACTIVE)
    {
      (void)HAL_TIM_OC_SetCompareUnitPulse(h_timserv, oc_compare_unit_lut[timer_id], pulse);
      p_funct_clear_cc_flag[timer_id](TIM_GET_INSTANCE(h_timserv));
      return HAL_OK;
    }
    else
    {
      /* Unknown timer ID */
      return HAL_ERROR;
    }
  }
  else
  {
    /* Unknown timer ID */
    return HAL_ERROR;
  }
}

/**
  * @brief  Check timer expiration.
  * @param  if Timer ID.
  */
uint32_t USBPD_TIMERSERVER_HW_IF_IsExpired(uint8_t timer_id)
{
  static uint32_t (*p_funct_isactive_cc_flag[])(const TIM_TypeDef * timx) =
  {
    LL_TIM_IsActiveFlag_CC1,
    LL_TIM_IsActiveFlag_CC2,
#if defined(LL_TIM_CHANNEL_CH3)
    LL_TIM_IsActiveFlag_CC3,
#endif /* LL_TIM_CHANNEL_CH3 */
#if defined(LL_TIM_CHANNEL_CH4)
    LL_TIM_IsActiveFlag_CC4,
#endif /* LL_TIM_CHANNEL_CH4 */
#if defined(LL_TIM_CHANNEL_CH5)
    LL_TIM_IsActiveFlag_CC5,
#endif /* LL_TIM_CHANNEL_CH5 */
#if defined(LL_TIM_CHANNEL_CH6)
    LL_TIM_IsActiveFlag_CC6,
#endif /* LL_TIM_CHANNEL_CH6 */
  };

  size_t lut_size = sizeof(p_funct_isactive_cc_flag) / sizeof(p_funct_isactive_cc_flag[0]);

  if (timer_id < lut_size)
  {
    return p_funct_isactive_cc_flag[timer_id](TIM_GET_INSTANCE(h_timserv));
  }
  else
  {
    /* Unknown timer ID */
    return 0U;
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

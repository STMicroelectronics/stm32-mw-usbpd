/**
  ******************************************************************************
  * @file    usbpd_def.h
  * @brief   Global defines for USBPD library.
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
#ifndef USBPD_DEF_H
#define USBPD_DEF_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Includes ------------------------------------------------------------------*/
#include "stm32_hal.h"

#include "cmsis_compiler.h"

#include <stdint.h>
#include <stddef.h>

/** @addtogroup STM32_USBPD_LIBRARY
  * @{
  */

/** @addtogroup USBPD_CORE
  * @{
  */

/** @addtogroup USBPD_CORE_DEF
  * @{
  */

/* Exported macro ------------------------------------------------------------*/
/** @defgroup USBPD_CORE_DEF_Exported_Macros USBPD CORE DEF Exported Macros
  * @{
  */

/**
  * @brief  Compare two variables and return the smallest.
  * @param  VAR1 First variable to be compared.
  * @param  VAR2 Second variable to be compared.
  * @return The smallest variable.
  */
#define USBPD_MIN(VAR1, VAR2) (((VAR1) > (VAR2)) ? (VAR2) : (VAR1))

/**
  * @brief  Compare two variables and return the biggest.
  * @param  VAR1 First variable to be compared.
  * @param  VAR2 Second variable to be compared.
  * @return The biggest variable.
  */
#define USBPD_MAX(VAR1, VAR2) (((VAR1) < (VAR2)) ? (VAR2) : (VAR1))

/**
  * @brief  Check if the requested voltage is valid.
  * @param  MV Requested voltage in mV units.
  * @param  MV_MAX Max Requested voltage in mV units.
  * @param  MV_MIN Min Requested voltage in mV units.
  * @retval 1 if the voltage is valid voltage, else 0.
  */
#define USBPD_IS_VALID_VOLTAGE(MV, MV_MAX, MV_MIN) ((((MV) <= (MV_MAX)) && ((MV) >= (MV_MIN))) ? 1U : 0U)

/**
  * @brief  Divide X by Y and round up.
  * @param  X Dividend.
  * @param  Y Divisor.
  * @retval Result of the division rounded up.
  */
#define DIV_ROUND_UP(X, Y) (((X) + ((Y) - 1U)) / (Y))

/**
  * @brief  Convert millivolts to ADC units.
  * @param  X Voltage in millivolts.
  * @retval Corresponding ADC value.
  */
#define MV2ADC(X) ((X * 4095U) / 3300U)

/**
  * @brief  Convert ADC units to millivolts.
  * @param  X ADC value.
  * @retval Corresponding voltage in millivolts.
  */
#define ADC2MV(X) ((X * 3300U) / 4095U)

/* Macros for integer division with various rounding variants default integer
   division rounds down. */
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/**
  * @brief  Write a 32-bits value in a buffer in little endian.
  * @param  ADDR Address where the value must be written.
  * @param  DATA Value to be written.
  */
#define USBPD_WRITE32(ADDR, DATA)                                                   \
  do                                                                                \
  {                                                                                 \
    uint8_t bindex;                                                                 \
    for(bindex = 0U; bindex < 4U; bindex++)                                         \
    {                                                                               \
      ((uint8_t *)ADDR)[bindex] = ((uint8_t)(DATA >> (8U * bindex)) & 0x000000FFU); \
    }                                                                               \
  } while(0U);

/**
  * @brief  Write a 16-bits value in a buffer in little endian.
  * @param  ADDR Address where the value must be written.
  * @param  DATA Value to be written.
  */
#define USBPD_WRITE16(ADDR, DATA)                                                \
  do                                                                             \
  {                                                                              \
    uint8_t bindex;                                                              \
    for(bindex = 0U; bindex < 2U; bindex++)                                      \
    {                                                                            \
      ((uint8_t *)ADDR)[bindex] = ((uint8_t)(DATA >> (8U * bindex)) & 0x000FFU); \
    }                                                                            \
  } while(0U);

/**
  * @brief  Read a 16-bits value from a buffer in little endian.
  * @param  ADDR Address from where the value must be read.
  * @retval Value read.
  */
#define USBPD_LE16(ADDR) (((uint16_t)(*((uint8_t *)(ADDR))))\
                          + (((uint16_t)(*(((uint8_t *)(ADDR)) + 1U))) << 8U))

/**
  * @brief  Read a 32-bits value from a buffer in little endian.
  * @param  ADDR Address from where the value must be read.
  * @retval Value read.
  */
#define USBPD_LE32(ADDR) ((((uint32_t)(*(((const uint8_t *)(ADDR)) + 0U))) +          \
                           (((uint32_t)(*(((const uint8_t *)(ADDR)) + 1U))) << 8U) +  \
                           (((uint32_t)(*(((const uint8_t *)(ADDR)) + 2U))) << 16U)+  \
                           (((uint32_t)(*(((const uint8_t *)(ADDR)) + 3U))) << 24U)))

/**
  * @}
  */

/* Exported defines   --------------------------------------------------------*/
/** @defgroup USBPD_CORE_DEF_Exported_Defines USBPD CORE DEF Exported Defines
  * @{
  */

#define USBPD_FALSE (0U) /*!< Define used to set a logical state to false */
#define USBPD_TRUE  (1U) /*!< Define used to set a logical state to true */

/** @defgroup USBPD_CORE_DEF_Exported_Defines_Swiches USBPD Compilations switches
  * @brief List of compilation switches which can be used to reduce size of the CORE library.
  * @{
  */

/* ---------------  FULL  --------------- */
#if defined(USBPDCORE_LIB_PD3_FULL)
/* #define USBPDCORE_NOPD */
#define USBPDCORE_EPR
#define USBPDCORE_PPS
#define USBPDCORE_SPR_AVS
#define USBPDCORE_UNCHUNCKED_MODE
#define USBPDCORE_USBDATA
#define USBPDCORE_DATA_SWAP
#define USBPDCORE_FASTROLESWAP
#define USBPDCORE_SRC
#define USBPDCORE_SNK
#define USBPDCORE_DRP
#define USBPDCORE_SRC_CAPA_EXT
#define USBPDCORE_SNK_CAPA_EXT
#define USBPDCORE_SVDM
#define USBPDCORE_UVDM
#define USBPDCORE_VCONN_SUPPORT
#define USBPDCORE_ANSWER_DISCOIDENT
#define USBPDCORE_GIVESNKCAP
#define USBPDCORE_GETSRCCAP
#define USBPDCORE_GETSNKCAP
#define USBPDCORE_ALERT
#define USBPDCORE_STATUS
#define USBPDCORE_BATTERY
#define USBPDCORE_MANU_INFO
#define USBPDCORE_SECURITY_MSG
#define USBPDCORE_FWUPD
#define USBPDCORE_COUNTRY_MSG
/* #define USBPDCORE_PING_SUPPORT */
#define USBPDCORE_BIST
#define USBPDCORE_ERROR_RECOVERY
/* #define USBPDCORE_PECABLE */
/* #define USBPDCORE_GOTOMIN */
/* #define USBPDCORE_TCPM_SUPPORT */
/* -------------------------------------- */

/* ---------------  SPR  ---------------- */
#elif defined(USBPDCORE_LIB_PD3_CONFIG_SPR)
/* #define USBPDCORE_NOPD */
/* #define USBPDCORE_EPR */
#define USBPDCORE_PPS
#define USBPDCORE_SPR_AVS
#define USBPDCORE_UNCHUNCKED_MODE
/* #define USBPDCORE_USBDATA */
#define USBPDCORE_DATA_SWAP
#define USBPDCORE_FASTROLESWAP
#define USBPDCORE_SRC
#define USBPDCORE_SNK
#define USBPDCORE_DRP
#define USBPDCORE_SRC_CAPA_EXT
#define USBPDCORE_SNK_CAPA_EXT
#define USBPDCORE_SVDM
#define USBPDCORE_UVDM
#define USBPDCORE_VCONN_SUPPORT
#define USBPDCORE_ANSWER_DISCOIDENT
#define USBPDCORE_GIVESNKCAP
#define USBPDCORE_GETSRCCAP
#define USBPDCORE_GETSNKCAP
#define USBPDCORE_ALERT
#define USBPDCORE_STATUS
#define USBPDCORE_BATTERY
#define USBPDCORE_MANU_INFO
#define USBPDCORE_SECURITY_MSG
#define USBPDCORE_FWUPD
#define USBPDCORE_COUNTRY_MSG
/* #define USBPDCORE_PING_SUPPORT */
#define USBPDCORE_BIST
#define USBPDCORE_ERROR_RECOVERY
/* #define USBPDCORE_PECABLE */
/* #define USBPDCORE_GOTOMIN */
/* #define USBPDCORE_TCPM_SUPPORT */
/* -------------------------------------- */

/* --------------  MINDRP  -------------- */
#elif defined(USBPDCORE_LIB_PD3_CONFIG_MINDRP)
/* #define USBPDCORE_NOPD */
/* #define USBPDCORE_EPR */
/* #define USBPDCORE_PPS */
/* #define USBPDCORE_UNCHUNCKED_MODE */
/* #define USBPDCORE_USBDATA */
#define USBPDCORE_DATA_SWAP
/* #define USBPDCORE_FASTROLESWAP */
#define USBPDCORE_SRC
#define USBPDCORE_SNK
#define USBPDCORE_DRP
#define USBPDCORE_SRC_CAPA_EXT
#define USBPDCORE_SNK_CAPA_EXT
/* #define USBPDCORE_SVDM */
/* #define USBPDCORE_UVDM */
#define USBPDCORE_VCONN_SUPPORT
#define USBPDCORE_ANSWER_DISCOIDENT
#define USBPDCORE_GIVESNKCAP
#define USBPDCORE_GETSRCCAP
#define USBPDCORE_GETSNKCAP
/* #define USBPDCORE_ALERT */
/* #define USBPDCORE_STATUS */
#define USBPDCORE_BATTERY
/* #define USBPDCORE_MANU_INFO */
/* #define USBPDCORE_SECURITY_MSG */
/* #define USBPDCORE_FWUPD */
/* #define USBPDCORE_COUNTRY_MSG */
/* #define USBPDCORE_PING_SUPPORT */
#define USBPDCORE_BIST
#define USBPDCORE_ERROR_RECOVERY
/* #define USBPDCORE_PECABLE */
/* #define USBPDCORE_GOTOMIN */
/* #define USBPDCORE_TCPM_SUPPORT */
/* -------------------------------------- */

/* --------------  MINSRC  -------------- */
#elif defined(USBPDCORE_LIB_PD3_CONFIG_MINSRC)
/* #define USBPDCORE_NOPD */
/* #define USBPDCORE_EPR */
/* #define USBPDCORE_PPS */
/* #define USBPDCORE_UNCHUNCKED_MODE */
/* #define USBPDCORE_USBDATA */
#define USBPDCORE_DATA_SWAP
/* #define USBPDCORE_FASTROLESWAP */
#define USBPDCORE_SRC
/* #define USBPDCORE_SNK */
/* #define USBPDCORE_DRP */
#define USBPDCORE_SRC_CAPA_EXT
/* #define USBPDCORE_SNK_CAPA_EXT */
/* #define USBPDCORE_SVDM */
/* #define USBPDCORE_UVDM */
#define USBPDCORE_VCONN_SUPPORT
#define USBPDCORE_ANSWER_DISCOIDENT
/* #define USBPDCORE_GIVESNKCAP */
/* #define USBPDCORE_GETSRCCAP */
#define USBPDCORE_GETSNKCAP
/* #define USBPDCORE_ALERT */
/* #define USBPDCORE_STATUS */
#define USBPDCORE_BATTERY
/* #define USBPDCORE_MANU_INFO */
/* #define USBPDCORE_SECURITY_MSG */
/* #define USBPDCORE_FWUPD */
/* #define USBPDCORE_COUNTRY_MSG */
/* #define USBPDCORE_PING_SUPPORT */
#define USBPDCORE_BIST
#define USBPDCORE_ERROR_RECOVERY
/* #define USBPDCORE_PECABLE */
/* #define USBPDCORE_GOTOMIN */
/* #define USBPDCORE_TCPM_SUPPORT */
/* -------------------------------------- */

/* --------------  MINSNK  -------------- */
#elif defined(USBPDCORE_LIB_PD3_CONFIG_MINSNK)
/* #define USBPDCORE_NOPD */
/* #define USBPDCORE_EPR */
/* #define USBPDCORE_PPS */
/* #define USBPDCORE_UNCHUNCKED_MODE */
/* #define USBPDCORE_USBDATA */
#define USBPDCORE_DATA_SWAP
/* #define USBPDCORE_FASTROLESWAP */
/* #define USBPDCORE_SRC */
#define USBPDCORE_SNK
/* #define USBPDCORE_DRP */
/* #define USBPDCORE_SRC_CAPA_EXT */
#define USBPDCORE_SNK_CAPA_EXT
/* #define USBPDCORE_SVDM */
/* #define USBPDCORE_UVDM */
/* #define USBPDCORE_VCONN_SUPPORT */
/* #define USBPDCORE_ANSWER_DISCOIDENT */
#define USBPDCORE_GIVESNKCAP
#define USBPDCORE_GETSRCCAP
/* #define USBPDCORE_GETSNKCAP */
/* #define USBPDCORE_ALERT */
/* #define USBPDCORE_STATUS */
#define USBPDCORE_BATTERY
/* #define USBPDCORE_MANU_INFO */
/* #define USBPDCORE_SECURITY_MSG */
/* #define USBPDCORE_FWUPD */
/* #define USBPDCORE_COUNTRY_MSG */
/* #define USBPDCORE_PING_SUPPORT */
#define USBPDCORE_BIST
#define USBPDCORE_ERROR_RECOVERY
/* #define USBPDCORE_PECABLE */
/* #define USBPDCORE_GOTOMIN */
/* #define USBPDCORE_TCPM_SUPPORT */
/* -------------------------------------- */

/* ---------------  TCPM  --------------- */
#elif defined(USBPD_TCPM_LIB_PD3_FULL)
/* #define USBPDCORE_NOPD */
/* #define USBPDCORE_EPR */
#define USBPDCORE_PPS
#define USBPDCORE_UNCHUNCKED_MODE
/* #define USBPDCORE_USBDATA */
#define USBPDCORE_DATA_SWAP
#define USBPDCORE_FASTROLESWAP
#define USBPDCORE_SRC
#define USBPDCORE_SNK
#define USBPDCORE_DRP
#define USBPDCORE_SRC_CAPA_EXT
#define USBPDCORE_SNK_CAPA_EXT
#define USBPDCORE_SVDM
#define USBPDCORE_UVDM
#define USBPDCORE_VCONN_SUPPORT
/* #define USBPDCORE_ANSWER_DISCOIDENT */
#define USBPDCORE_GIVESNKCAP
#define USBPDCORE_GETSRCCAP
#define USBPDCORE_GETSNKCAP
#define USBPDCORE_ALERT
#define USBPDCORE_STATUS
#define USBPDCORE_BATTERY
#define USBPDCORE_MANU_INFO
#define USBPDCORE_SECURITY_MSG
#define USBPDCORE_FWUPD
#define USBPDCORE_COUNTRY_MSG
/* #define USBPDCORE_PING_SUPPORT */
#define USBPDCORE_BIST
#define USBPDCORE_ERROR_RECOVERY
/* #define USBPDCORE_PECABLE */
/* #define USBPDCORE_GOTOMIN */
#define USBPDCORE_TCPM_SUPPORT
/* -------------------------------------- */

/* ---------------  NO_PD  -------------- */
#elif defined(USBPDCORE_LIB_NO_PD)
#define USBPDCORE_NOPD
/* -------------------------------------- */

#else
#error USBPDCORE_LIB... is undefined
#endif /* USBPDCORE_LIB_PD3_FULL */


/* LIB_ID definition */
/*
  LIB_ID is constructed in this format: 0xXYVVVWWW
  * X: 3 (PD3.0) or 2 (PD2.0)
  * Y: 0 (CORE) or 1 (TCPM)
  * VVV: Stack version (ex 200 for Stack 2.0.0)
  * WWW: 0 (FULL VERSION) or config_x
*/
/* Defines for PD revision */
#define LIB_PD_VERSION_POS  (28U)                          /*!< Position of the PD version in LIB_ID */
#define LIB_PD_VERSION_MSK  (0xFU << LIB_PD_VERSION_POS)   /*!< Mask of the PD version in LIB_ID     */
#define LIB_PD2             (2U   << LIB_PD_VERSION_POS)   /*!< PD2.0                                 */
#define LIB_PD3             (3U   << LIB_PD_VERSION_POS)   /*!< PD3.0                                 */
#define LIB_PD31            (4U   << LIB_PD_VERSION_POS)   /*!< PD3.1                                 */
#define LIB_PD32            (5U   << LIB_PD_VERSION_POS)   /*!< PD3.2                                 */

/* Defines for CORE or TCPM */
#define LIB_CORE_TCPM_POS   (24U)                          /*!< Position of the CORE/TCPM in LIB_ID */
#define LIB_CORE_TCPM_MSK   (0xFU << LIB_CORE_TCPM_POS)    /*!< Mask of the CORE/TCPM in LIB_ID     */
#define LIB_CORE            (0U   << LIB_CORE_TCPM_POS)    /*!< CORE                                  */
#define LIB_TCPM            (1U   << LIB_CORE_TCPM_POS)    /*!< TCPM                                  */
/* Defines for STACK version */
#define LIB_STACK_VER_POS   (12U)                          /*!< Position of the Stack version in LIB_ID */
#define LIB_STACK_VER_MSK   (0xFFFU << LIB_STACK_VER_POS)  /*!< Mask of the Stack version in LIB_ID     */
#define LIB_STACK_VER       (0x200U  << LIB_STACK_VER_POS) /*!< Stack version 2.0.0                     */
/* Defines for configuration */
#define LIB_CONFIG_MSK      (0xFFFU)                       /*!< Mask of the configuration in LIB_ID */
#define LIB_FULL            (0x000U)                       /*!< Lib Full                                */
#define LIB_CONFIG_SPR      (0x001U)                       /*!< Lib SPR                                 */
#define LIB_CONFIG_MINSRC   (0x002U)                       /*!< Lib MinSrc                              */
#define LIB_CONFIG_MINSNK   (0x004U)                       /*!< Lib MinSnk                              */
#define LIB_CONFIG_VDM      (0x010U)                       /*!< Lib VDM                                 */
#define LIB_CONFIG_NOPD     (0x100U)                       /*!< Lib No PD                               */
#define LIB_CONFIG_MINDRP   (0x200U)                       /*!< Lib MinDrp                              */

#if defined(USBPDCORE_LIB_PD3_FULL)
#define LIB_ID   (LIB_PD32 | LIB_CORE | LIB_STACK_VER | LIB_FULL)
#endif /* USBPDCORE_LIB_PD3_FULL */

#if defined(USBPDCORE_LIB_PD3_CONFIG_SPR)
#define LIB_ID   (LIB_PD32 | LIB_CORE | LIB_STACK_VER | LIB_CONFIG_SPR)
#endif /* USBPDCORE_LIB_PD3_CONFIG_SPR */

#if defined(USBPDCORE_LIB_PD3_CONFIG_VDM)
#define LIB_ID   (LIB_PD32 | LIB_CORE | LIB_STACK_VER | LIB_CONFIG_VDM)
#endif /* USBPDCORE_LIB_PD3_CONFIG_VDM */

#if defined(USBPDCORE_LIB_PD3_CONFIG_MINDRP)
#define LIB_ID   (LIB_PD32 | LIB_CORE | LIB_STACK_VER | LIB_CONFIG_MINDRP)
#endif /* USBPDCORE_LIB_PD3_CONFIG_MINSRC */

#if defined(USBPDCORE_LIB_PD3_CONFIG_MINSRC)
#define LIB_ID   (LIB_PD32 | LIB_CORE | LIB_STACK_VER | LIB_CONFIG_MINSRC)
#endif /* USBPDCORE_LIB_PD3_CONFIG_MINSRC */

#if defined(USBPDCORE_LIB_PD3_CONFIG_MINSNK)
#define LIB_ID   (LIB_PD32 | LIB_CORE | LIB_STACK_VER | LIB_CONFIG_MINSNK)
#endif /* USBPDCORE_LIB_PD3_CONFIG_MINSNK */

#if defined(USBPDCORE_LIB_NO_PD)
#define LIB_ID   (LIB_STACK_VER | LIB_CONFIG_NOPD)
#endif /* USBPDCORE_LIB_NO_PD */

#if defined(USBPD_TCPM_LIB_PD3_FULL)
#define LIB_ID   (LIB_PD32 | LIB_TCPM | LIB_STACK_VER | LIB_FULL)
#endif /* USBPD_TCPM_LIB_PD3_FULL */

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
#define USBPD_REV30_SUPPORT             /*!< Legacy define for revision 3.0 support. Now mandatory */

#define USBPD_PORT_0            (0U)    /*!< Port 0 identifier */
#define USBPD_PORT_1            (1U)    /*!< Port 1 identifier */
#define USBPD_PORT_2            (2U)    /*!< Port 2 identifier */

#define USBPD_MAX_NB_PDO        (7U)    /*!< Maximum number of supported SPR Power Data Objects: fix by the Specification */
#define USBPD_MAX_NB_EPRPDO     (6U)    /*!< Maximum number of supported EPR Power Data Objects: fix by the Specification */
#define BIST_CARRIER_MODE_MS    (50U)   /*!< Time in ms of the BIST signal*/
#define USBPD_PDO_SIZE_IN_BYTES (4U)    /*!< Nb of size used for PDO coding */

/* USBPD version and revision */
#define USBPD_REV_MAJOR         (3U)    /*!< USBPD Specification revision major */
#define USBPD_REV_MINOR         (2U)    /*!< USBPD Specification revision minor */
#define USBPD_VERSION_MAJOR     (1U)    /*!< USBPD Specification version major  */
#define USBPD_VERSION_MINOR     (1U)    /*!< USBPD Specification version minor  */

/*
  @brief Maximum size of the RX buffer allocated in the stack to receive a PD frame.
  @note TX buffer size is used internally in the stack (size if available in @ref USBPD_PHY_SendMessage)
   */
#if defined(USBPDCORE_UNCHUNCKED_MODE)
#define USBPD_MAX_RX_BUFFER_SIZE (264U) /*!< Maximum size of Rx buffer used when unchuncked is supported by the stack */
#else
#define USBPD_MAX_RX_BUFFER_SIZE (30U) /*!< Maximum size of Rx buffer used when unchuncked
                                            is NOT supported by the stack */
#endif /* USBPDCORE_UNCHUNCKED_MODE */
#define USBPD_CHUNK_BUFFER_SIZE  (26U) /*!< chunk buffer size  */

/*
 * Maximum size of a Power Delivery packet (in bits on the wire) :
 *    16-bit header + 0..7 32-bit data objects  (+ 4b5b encoding)
 *    64-bit preamble + SOP (4x 5b) + header (16-bit) + message in 4b5b + 32-bit CRC  + EOP (1x 5b)
 * =  64bit           + 4*5bit      + 16bit * 5/4 + 7 * 32bit * 5/4 + 32bit * 5/4 + 5
 */
#define PHY_BIT_LEN             ((uint16_t)429U)
#define PHY_MAX_RAW_SIZE        ((uint16_t)((PHY_BIT_LEN * 2U) + 3U))
#define PHY_MAX_RAW_BYTE_SIZE   ((uint8_t)60U) /*!<PHY_BIT_LEN / 8 + SAFE Bytes */

/** @defgroup USBPD_PWR_Macro_Constants USBPD Power Macro Constants
  * @{
  */

/* Macros used to convert values into PDO representation */
#define USBPD_PWR_MV_20MV(MV)        ((uint16_t)((MV) / 20U))     /* From mV to 20mV multiples     */
#define USBPD_PWR_MV_50MV(MV)        ((uint16_t)((MV) / 50U))     /* From mV to 50mV multiples     */
#define USBPD_PWR_MV_100MV(MV)       ((uint16_t)((MV) / 100U))    /* From mV to 100mV multiples    */
#define USBPD_PWR_MA_10MA(MA)        ((uint16_t)((MA) / 10U))     /* From mA to 10mA multiples     */
#define USBPD_PWR_MA_50MA(MA)        ((uint16_t)((MA) / 50U))     /* From mA to 50mA multiples     */
#define USBPD_PWR_MW_250MW(MW)       ((uint16_t)((MW) / 250U))    /* From mWatt to 250mW multiples */

/* Macros used to get values from PDO representation */
#define USBPD_PWR_DECODE_20MV(VAL)   ((uint16_t)(((VAL) * 20U)))  /* From 20mV multiples to mV     */
#define USBPD_PWR_DECODE_25MV(VAL)   ((uint16_t)(((VAL) * 25U)))  /* From 25mV multiples to mV     */
#define USBPD_PWR_DECODE_50MV(VAL)   ((uint16_t)(((VAL) * 50U)))  /* From 50mV multiples to mV     */
#define USBPD_PWR_DECODE_100MV(VAL)  ((uint16_t)(((VAL) * 100U))) /* From 100mV multiples to mV    */
#define USBPD_PWR_DECODE_10MA(VAL)   ((uint16_t)(((VAL) * 10U)))  /* From 10mA multiples to mA     */
#define USBPD_PWR_DECODE_50MA(VAL)   ((uint16_t)(((VAL) * 50U)))  /* From 50mA multiples to mA     */
#define USBPD_PWR_DECODE_250MW(VAL)  ((uint16_t)(((VAL) * 250U))) /* From 250mW multiples to mW    */

/* Macros used to get values from PDO representation using signed integers */
#define USBPD_PWR_DECODE_20MV_SIGNED(VAL) ((int16_t)(((VAL) * 20))) /* From 20mV multiples to mV   */

/* ---------------------------- FIXED --------------------------- */

/* Set voltage in mV in SRC Fixed PDO */
#define USBPD_PDO_SRC_FIXED_SET_VOLTAGE(MV)        ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SRC_FIXED_VOLTAGE_POS)        \
                                                    & (USBPD_PDO_SRC_FIXED_VOLTAGE_MSK))

/* Set max current in mA in SRC Fixed PDO */
#define USBPD_PDO_SRC_FIXED_SET_MAX_CURRENT(MA)    ((USBPD_PWR_MA_10MA(MA)                      \
                                                     << USBPD_PDO_SRC_FIXED_MAX_CURRENT_POS)    \
                                                    & (USBPD_PDO_SRC_FIXED_MAX_CURRENT_MSK))

/* Set voltage in mV in SNK Fixed PDO */
#define USBPD_PDO_SNK_FIXED_SET_VOLTAGE(MV)        ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SNK_FIXED_VOLTAGE_POS)        \
                                                    & (USBPD_PDO_SNK_FIXED_VOLTAGE_MSK))

/* Set operating current in mA in SNK Fixed PDO */
#define USBPD_PDO_SNK_FIXED_SET_OP_CURRENT(MA)     ((USBPD_PWR_MA_10MA(MA)                      \
                                                     << USBPD_PDO_SNK_FIXED_OP_CURRENT_POS)     \
                                                    & (USBPD_PDO_SNK_FIXED_OP_CURRENT_MSK))

/* ---------------------------- VARIABLE ------------------------ */

/* Set max voltage in mV in SRC VARIABLE PDO */
#define USBPD_PDO_SRC_VARIABLE_SET_MAX_VOLTAGE(MV) ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_POS) \
                                                    & (USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_MSK))

/* Set min voltage in mV in SRC VARIABLE PDO */
#define USBPD_PDO_SRC_VARIABLE_SET_MIN_VOLTAGE(MV) ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_POS) \
                                                    & (USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_MSK))

/* Set max current in mA in SRC VARIABLE PDO */
#define USBPD_PDO_SRC_VARIABLE_SET_MAX_CURRENT(MA) ((USBPD_PWR_MA_10MA(MA)                      \
                                                     << USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_POS) \
                                                    & (USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_MSK))

/* Set max voltage in mV in SNK VARIABLE PDO */
#define USBPD_PDO_SNK_VARIABLE_SET_MAX_VOLTAGE(MV) ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SNK_VARIABLE_MAX_VOLTAGE_POS) \
                                                    & (USBPD_PDO_SNK_VARIABLE_MAX_VOLTAGE_MSK))

/* Set min voltage in mV in SNK VARIABLE PDO */
#define USBPD_PDO_SNK_VARIABLE_SET_MIN_VOLTAGE(MV) ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SNK_VARIABLE_MIN_VOLTAGE_POS) \
                                                    & (USBPD_PDO_SNK_VARIABLE_MIN_VOLTAGE_MSK))

/* Set operating current in mA in SNK VARIABLE */
#define USBPD_PDO_SNK_VARIABLE_SET_OP_CURRENT(MA)  ((USBPD_PWR_MA_10MA(MA)                      \
                                                     << USBPD_PDO_SNK_VARIABLE_OP_CURRENT_POS)  \
                                                    & (USBPD_PDO_SNK_VARIABLE_OP_CURRENT_MSK))

/* ---------------------------- BARTTERY ------------------------ */

/* Set max voltage in mV in SRC BATTERY PDO */
#define USBPD_PDO_SRC_BATTERY_SET_MAX_VOLTAGE(MV)  ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_POS)  \
                                                    & (USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_MSK))

/* Set min voltage in mV in SRC BATTERY PDO */
#define USBPD_PDO_SRC_BATTERY_SET_MIN_VOLTAGE(MV)  ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_POS)  \
                                                    & (USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_MSK))

/* Set max power in mW in SRC BATTERY PDO */
#define USBPD_PDO_SRC_BATTERY_SET_MAX_POWER(MW)    ((USBPD_PWR_MW_250MW(MW)                     \
                                                     << USBPD_PDO_SRC_BATTERY_MAX_POWER_POS)    \
                                                    & (USBPD_PDO_SRC_BATTERY_MAX_POWER_MSK))

/* Set max voltage in mV in SNK BATTERY PDO */
#define USBPD_PDO_SNK_BATTERY_SET_MAX_VOLTAGE(MV)  ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SNK_BATTERY_MAX_VOLTAGE_POS)  \
                                                    & (USBPD_PDO_SNK_BATTERY_MAX_VOLTAGE_MSK))

/* Set min voltage in mV in SNK BATTERY PDO */
#define USBPD_PDO_SNK_BATTERY_SET_MIN_VOLTAGE(MV)  ((USBPD_PWR_MV_50MV(MV)                      \
                                                     << USBPD_PDO_SNK_BATTERY_MIN_VOLTAGE_POS)  \
                                                    & (USBPD_PDO_SNK_BATTERY_MIN_VOLTAGE_MSK))

/* Set operating power in mW in SNK BATTERY PDO */
#define USBPD_PDO_SNK_BATTERY_SET_OP_POWER(MW)     ((USBPD_PWR_MW_250MW(MW)                     \
                                                     << USBPD_PDO_SNK_BATTERY_OP_POWER_POS)     \
                                                    & (USBPD_PDO_SNK_BATTERY_OP_POWER_MSK))

/* ---------------------------- APDO ---------------------------- */

/* Set min voltage in mV in SRC APDO */
#define USBPD_PDO_SRC_APDO_SET_MIN_VOLTAGE(MV)     ((USBPD_PWR_MV_100MV(MV)                     \
                                                     << USBPD_PDO_SRC_APDO_MIN_VOLTAGE_POS)     \
                                                    & (USBPD_PDO_SRC_APDO_MIN_VOLTAGE_MSK))

/* Set max voltage in mV in SRC APDO */
#define USBPD_PDO_SRC_APDO_SET_MAX_VOLTAGE(MV)     ((USBPD_PWR_MV_100MV(MV)                     \
                                                     << USBPD_PDO_SRC_APDO_MAX_VOLTAGE_POS)     \
                                                    & (USBPD_PDO_SRC_APDO_MAX_VOLTAGE_MSK))

/* Set max current in mA in SRC APDO */
#define USBPD_PDO_SRC_APDO_SET_MAX_CURRENT(MA)     ((USBPD_PWR_MA_50MA((MA) )                   \
                                                     << USBPD_PDO_SRC_APDO_MAX_CURRENT_POS)     \
                                                    & (USBPD_PDO_SRC_APDO_MAX_CURRENT_MSK))

/* Set min voltage in mV in SNK APDO */
#define USBPD_PDO_SNK_APDO_SET_MIN_VOLTAGE(MV)     ((USBPD_PWR_MV_100MV(MV)                     \
                                                     << USBPD_PDO_SNK_APDO_MIN_VOLTAGE_POS)     \
                                                    & (USBPD_PDO_SNK_APDO_MIN_VOLTAGE_MSK))

/* Set max voltage in mV in SNK APDO */
#define USBPD_PDO_SNK_APDO_SET_MAX_VOLTAGE(MV)     ((USBPD_PWR_MV_100MV(MV)                     \
                                                     << USBPD_PDO_SNK_APDO_MAX_VOLTAGE_POS)     \
                                                    & (USBPD_PDO_SNK_APDO_MAX_VOLTAGE_MSK))

/* Set max current in mA in SNK APDO */
#define USBPD_PDO_SNK_APDO_SET_MAX_CURRENT(MA)     ((USBPD_PWR_MA_50MA(MA)                      \
                                                     << USBPD_PDO_SNK_APDO_MAX_CURRENT_POS)     \
                                                    & (USBPD_PDO_SNK_APDO_MAX_CURRENT_MSK))

/* ---------------------------- SPR AVS APDO ---------------------------- */

/* Set max current in mA in SRC SPR AVS APDO */
#define USBPD_PDO_SPR_SRC_AVS_15V_SET_MAX_CURRENT(MA)  ((USBPD_PWR_MA_10MA(MA)                                  \
                                                         << USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_15V_POS)       \
                                                        & (USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_15V_MSK))

#define USBPD_PDO_SPR_SRC_AVS_20V_SET_MAX_CURRENT(MA)  ((USBPD_PWR_MA_10MA(MA)                                  \
                                                         << USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_20V_POS)       \
                                                        & (USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_20V_MSK))
/* Set max current in mA in SNK SPR AVS APDO */
#define USBPD_PDO_SPR_SNK_AVS_15V_SET_MAX_CURRENT(MA)  ((USBPD_PWR_MA_10MA(MA)                                  \
                                                         << USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_15V_POS)       \
                                                        & (USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_15V_MSK))

#define USBPD_PDO_SPR_SNK_AVS_20V_SET_MAX_CURRENT(MA)  ((USBPD_PWR_MA_10MA(MA)                                  \
                                                         << USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_20V_POS)       \
                                                        & (USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_20V_MSK))

/**
  * @}
  */

/** @defgroup USBPD_PDO_Index_And_Mask_Constants Index and Mask constants used in PDO bits handling
  * @{
  */

#define USBPD_PDO_TYPE_POS       (30U)                                                          /*!< PDO Type
                                                                                                     bits position */
#define USBPD_PDO_TYPE_MSK       (3U << USBPD_PDO_TYPE_POS)                                     /*!< PDO Type
                                                                                                     bits mask :
                                                                                                     0xC0000000    */
#define USBPD_PDO_TYPE_FIXED     (uint32_t)(USBPD_CORE_PDO_TYPE_FIXED << USBPD_PDO_TYPE_POS)    /*!< PDO Type
                                                                                                     = FIXED       */
#define USBPD_PDO_TYPE_BATTERY   (uint32_t)(USBPD_CORE_PDO_TYPE_BATTERY << USBPD_PDO_TYPE_POS)  /*!< PDO Type
                                                                                                     = BATTERY     */
#define USBPD_PDO_TYPE_VARIABLE  (uint32_t)(USBPD_CORE_PDO_TYPE_VARIABLE << USBPD_PDO_TYPE_POS) /*!< PDO Type
                                                                                                     = VARIABLE    */
#define USBPD_PDO_TYPE_APDO      (uint32_t)(USBPD_CORE_PDO_TYPE_APDO << USBPD_PDO_TYPE_POS)     /*!< PDO Type
                                                                                                     = APDO        */
#define USBPD_PDO_TYPE_AVSPDO    USBPD_PDO_TYPE_APDO                                            /*!< PDO Type
                                                                                                     = AVSPDO      */

/* Source Fixed type PDO elements */
#define USBPD_PDO_SRC_FIXED_DRP_SUPPORT_POS          (29U)                                         /*!< DRP Support bit position                        */
#define USBPD_PDO_SRC_FIXED_DRP_SUPPORT_MSK          (0x1U << USBPD_PDO_SRC_FIXED_DRP_SUPPORT_POS) /*!< DRP Support bit mask : 0x20000000               */
#define USBPD_PDO_SRC_FIXED_DRP_NOT_SUPPORTED        (0U)                                          /*!< DRP not supported                               */
#define USBPD_PDO_SRC_FIXED_DRP_SUPPORTED            USBPD_PDO_SRC_FIXED_DRP_SUPPORT_MSK           /*!< DRP supported                                   */

#define USBPD_PDO_SRC_FIXED_USBSUSPEND_POS           (28U)                                         /*!< USB Suspend Support bit position                */
#define USBPD_PDO_SRC_FIXED_USBSUSPEND_MSK           (0x1U << USBPD_PDO_SRC_FIXED_USBSUSPEND_POS)  /*!< USB Suspend Support bit mask : 0x10000000       */
#define USBPD_PDO_SRC_FIXED_USBSUSPEND_NOT_SUPPORTED (0U)                                          /*!< USB Suspend not supported                       */
#define USBPD_PDO_SRC_FIXED_USBSUSPEND_SUPPORTED     USBPD_PDO_SRC_FIXED_USBSUSPEND_MSK            /*!< USB Suspend supported                           */

#define USBPD_PDO_SRC_FIXED_EXT_POWER_POS            (27U)                                         /*!< External Power available bit position           */
#define USBPD_PDO_SRC_FIXED_EXT_POWER_MSK            (0x1U << USBPD_PDO_SRC_FIXED_EXT_POWER_POS)   /*!< External Power available bit mask : 0x08000000  */
#define USBPD_PDO_SRC_FIXED_EXT_POWER_NOT_AVAILABLE  (0U)                                          /*!< External Power not available                    */
#define USBPD_PDO_SRC_FIXED_EXT_POWER_AVAILABLE      USBPD_PDO_SRC_FIXED_EXT_POWER_MSK             /*!< External Power available                        */

#define USBPD_PDO_SRC_FIXED_USBCOMM_POS              (26U)                                         /*!< USB Communication Support bit position          */
#define USBPD_PDO_SRC_FIXED_USBCOMM_MSK              (0x1U << USBPD_PDO_SRC_FIXED_USBCOMM_POS)     /*!< USB Communication Support bit mask : 0x04000000 */
#define USBPD_PDO_SRC_FIXED_USBCOMM_NOT_SUPPORTED    (0U)                                          /*!< USB Communication not supported                 */
#define USBPD_PDO_SRC_FIXED_USBCOMM_SUPPORTED        USBPD_PDO_SRC_FIXED_USBCOMM_MSK               /*!< USB Communication supported                     */

#define USBPD_PDO_SRC_FIXED_DRD_SUPPORT_POS          (25U)                                         /*!< Dual Role Data Support bit position             */
#define USBPD_PDO_SRC_FIXED_DRD_SUPPORT_MSK          (0x1U << USBPD_PDO_SRC_FIXED_DRD_SUPPORT_POS) /*!< Dual Role Data Support bit mask : 0x02000000    */
#define USBPD_PDO_SRC_FIXED_DRD_NOT_SUPPORTED        (0U)                                          /*!< Dual Role Data not supported                    */
#define USBPD_PDO_SRC_FIXED_DRD_SUPPORTED            USBPD_PDO_SRC_FIXED_DRD_SUPPORT_MSK           /*!< Dual Role Data supported                        */

#define USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORT_POS      (24U)                                            /*!< Unchunked Extended Messages Support bit position             */
#define USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORT_MSK      (0x1U << USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORT_POS)/*!< Unchunked Extended Messages Support bit mask : 0x01000000    */
#define USBPD_PDO_SRC_FIXED_UNCHUNK_NOT_SUPPORTED    (0U)                                             /*!< Unchunked Extended Messages not supported                    */
#define USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORTED        USBPD_PDO_SRC_FIXED_UNCHUNK_SUPPORT_MSK          /*!< Unchunked Extended Messages supported                        */

#if defined(USBPDCORE_EPR)
#define USBPD_PDO_SRC_FIXED_EPR_SUPPORT_POS          (23U)                                            /*!< EPR Support bit position             */
#define USBPD_PDO_SRC_FIXED_EPR_SUPPORT_MSK          (0x1U << USBPD_PDO_SRC_FIXED_EPR_SUPPORT_POS)    /*!< EPR Support bit mask :               */
#define USBPD_PDO_SRC_FIXED_EPR_NOT_SUPPORTED        (0U)                                             /*!< ERP not supported                    */
#define USBPD_PDO_SRC_FIXED_EPR_SUPPORTED            USBPD_PDO_SRC_FIXED_EPR_SUPPORT_MSK              /*!< EPR supported                        */
#endif /* USBPDCORE_EPR */

#define USBPD_PDO_SRC_FIXED_PEAKCURRENT_POS          (20U)                                                             /*!< Peak Current info bits position            */
#define USBPD_PDO_SRC_FIXED_PEAKCURRENT_MSK          (0x3U << USBPD_PDO_SRC_FIXED_PEAKCURRENT_POS)                     /*!< Peak Current info bits mask : 0x00300000   */
#define USBPD_PDO_SRC_FIXED_PEAKCURRENT_EQUAL        (USBPD_CORE_PDO_PEAKEQUAL << USBPD_PDO_SRC_FIXED_PEAKCURRENT_POS) /*!< Peak Current info : Equal to Ioc           */
#define USBPD_PDO_SRC_FIXED_PEAKCURRENT_OVER1        (USBPD_CORE_PDO_PEAKOVER1 << USBPD_PDO_SRC_FIXED_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 1         */
#define USBPD_PDO_SRC_FIXED_PEAKCURRENT_OVER2        (USBPD_CORE_PDO_PEAKOVER2 << USBPD_PDO_SRC_FIXED_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 2         */
#define USBPD_PDO_SRC_FIXED_PEAKCURRENT_OVER3        (USBPD_CORE_PDO_PEAKOVER3 << USBPD_PDO_SRC_FIXED_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 3         */

#define USBPD_PDO_SRC_FIXED_VOLTAGE_POS              (10U)                                            /*!< Voltage in 50 mV units bits position               */
#define USBPD_PDO_SRC_FIXED_VOLTAGE_MSK              (0x3FFU << USBPD_PDO_SRC_FIXED_VOLTAGE_POS)      /*!< Voltage in 50 mV units bits mask : 0x000FFC00      */

#define USBPD_PDO_SRC_FIXED_MAX_CURRENT_POS          (0U)                                             /*!< Max current in 10 mA units bits position           */
#define USBPD_PDO_SRC_FIXED_MAX_CURRENT_MSK          (0x3FFU << USBPD_PDO_SRC_FIXED_MAX_CURRENT_POS)  /*!< Max current in 10 mA units bits mask : 0x000003FF  */

/* Source Variable type PDO elements */
#define USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_POS       (20U)                                              /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_MSK       (0x3FFU << USBPD_PDO_SRC_VARIABLE_MAX_VOLTAGE_POS) /*!< Max Voltage in 50 mV units bits mask : 0x3FF00000  */

#define USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_POS       (10U)                                              /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_MSK       (0x3FFU << USBPD_PDO_SRC_VARIABLE_MIN_VOLTAGE_POS) /*!< Max Voltage in 50 mV units bits mask : 0x000FFC00  */

#define USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_POS       (0U)                                               /*!< Max current in 10 mA units bits position           */
#define USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_MSK       (0x3FFU << USBPD_PDO_SRC_VARIABLE_MAX_CURRENT_POS) /*!< Max current in 10 mA units bits mask : 0x000003FF  */

/* Source Battery type PDO elements */
#define USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_POS        (20U)                                              /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_MSK        (0x3FFU << USBPD_PDO_SRC_BATTERY_MAX_VOLTAGE_POS)  /*!< Max Voltage in 50 mV units bits mask : 0x3FF00000  */

#define USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_POS        (10U)                                              /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_MSK        (0x3FFU << USBPD_PDO_SRC_BATTERY_MIN_VOLTAGE_POS)  /*!< Max Voltage in 50 mV units bits mask : 0x000FFC00  */

#define USBPD_PDO_SRC_BATTERY_MAX_POWER_POS          (0U)                                               /*!< Max allowable power in 250mW units bits position          */
#define USBPD_PDO_SRC_BATTERY_MAX_POWER_MSK          (0x3FFU << USBPD_PDO_SRC_BATTERY_MAX_POWER_POS)    /*!< Max allowable power in 250mW units bits mask : 0x000003FF */

/* Sink Fixed type PDO elements */
#define USBPD_PDO_SNK_FIXED_DRP_SUPPORT_POS          (29U)                                              /*!< DRP Support bit position                        */
#define USBPD_PDO_SNK_FIXED_DRP_SUPPORT_MSK          (0x1U << USBPD_PDO_SNK_FIXED_DRP_SUPPORT_POS)      /*!< DRP Support bit mask : 0x20000000               */
#define USBPD_PDO_SNK_FIXED_DRP_NOT_SUPPORTED        (0U)                                               /*!< DRP not supported                               */
#define USBPD_PDO_SNK_FIXED_DRP_SUPPORTED            USBPD_PDO_SNK_FIXED_DRP_SUPPORT_MSK                /*!< DRP supported                                   */

#define USBPD_PDO_SNK_FIXED_HIGHERCAPAB_POS           (28U)                                             /*!< Higher capability support bit position          */
#define USBPD_PDO_SNK_FIXED_HIGHERCAPAB_MSK           (0x1U << USBPD_PDO_SNK_FIXED_HIGHERCAPAB_POS)     /*!< Higher capability support bit mask : 0x10000000 */
#define USBPD_PDO_SNK_FIXED_HIGHERCAPAB_NOT_SUPPORTED (0U)                                              /*!< Higher capability not supported                 */
#define USBPD_PDO_SNK_FIXED_HIGHERCAPAB_SUPPORTED     USBPD_PDO_SNK_FIXED_HIGHERCAPAB_MSK               /*!< Higher capability supported                     */

#define USBPD_PDO_SNK_FIXED_EXT_POWER_POS            (27U)                                                     /*!< External Power available bit position           */
#define USBPD_PDO_SNK_FIXED_EXT_POWER_MSK            (0x1U << USBPD_PDO_SNK_FIXED_EXT_POWER_POS)               /*!< External Power available bit mask : 0x08000000  */
#define USBPD_PDO_SNK_FIXED_EXT_POWER_NOT_AVAILABLE  (0U)                                                      /*!< External Power not available                    */
#define USBPD_PDO_SNK_FIXED_EXT_POWER_AVAILABLE      USBPD_PDO_SNK_FIXED_EXT_POWER_MSK                         /*!< External Power available                        */

#define USBPD_PDO_SNK_FIXED_USBCOMM_POS              (26U)                                                     /*!< USB Communication Support bit position          */
#define USBPD_PDO_SNK_FIXED_USBCOMM_MSK              (0x1U << USBPD_PDO_SNK_FIXED_USBCOMM_POS)                 /*!< USB Communication Support bit mask : 0x04000000 */
#define USBPD_PDO_SNK_FIXED_USBCOMM_NOT_SUPPORTED    (0U)                                                      /*!< USB Communication not supported                 */
#define USBPD_PDO_SNK_FIXED_USBCOMM_SUPPORTED        USBPD_PDO_SNK_FIXED_USBCOMM_MSK                           /*!< USB Communication supported                     */

#define USBPD_PDO_SNK_FIXED_DRD_SUPPORT_POS          (25U)                                                     /*!< Dual Role Data Support bit position             */
#define USBPD_PDO_SNK_FIXED_DRD_SUPPORT_MSK          (0x1U << USBPD_PDO_SNK_FIXED_DRD_SUPPORT_POS)             /*!< Dual Role Data Support bit mask : 0x02000000    */
#define USBPD_PDO_SNK_FIXED_DRD_NOT_SUPPORTED        (0U)                                                      /*!< Dual Role Data not supported                    */
#define USBPD_PDO_SNK_FIXED_DRD_SUPPORTED            USBPD_PDO_SNK_FIXED_DRD_SUPPORT_MSK                       /*!< Dual Role Data supported                        */

#define USBPD_PDO_SNK_FIXED_FRS_SUPPORT_POS          (23U)                                                     /*!< Fast Role Swap required Current bit position             */
#define USBPD_PDO_SNK_FIXED_FRS_SUPPORT_MSK          (0x3U << USBPD_PDO_SNK_FIXED_FRS_SUPPORT_POS)             /*!< Fast Role Swap required Current bit mask : 0x01800000    */
#define USBPD_PDO_SNK_FIXED_FRS_NOT_SUPPORTED        (0U)                                                      /*!< Fast Role Swap not supported                             */
#define USBPD_PDO_SNK_FIXED_FRS_DEFAULT              (USBPD_CORE_PDO_FRS_DEFAULT_USB_POWER << USBPD_PDO_SNK_FIXED_FRS_SUPPORT_POS)  /*!< Fast Role Swap required default USB power  */
#define USBPD_PDO_SNK_FIXED_FRS_1_5A                 (USBPD_CORE_PDO_FRS_1_5A_5V << USBPD_PDO_SNK_FIXED_FRS_SUPPORT_POS)            /*!< Fast Role Swap 1.5A at 5V                  */
#define USBPD_PDO_SNK_FIXED_FRS_3A                   (USBPD_CORE_PDO_FRS_3A_5V << USBPD_PDO_SNK_FIXED_FRS_SUPPORT_POS)              /*!< Fast Role Swap 3A at 5V                    */

#define USBPD_PDO_SNK_FIXED_VOLTAGE_POS              (10U)                                                     /*!< Voltage in 50 mV units bits position               */
#define USBPD_PDO_SNK_FIXED_VOLTAGE_MSK              (0x3FFU << USBPD_PDO_SNK_FIXED_VOLTAGE_POS)               /*!< Voltage in 50 mV units bits mask : 0x000FFC00      */

#define USBPD_PDO_SNK_FIXED_OP_CURRENT_POS           (0U)                                                      /*!< Operational current in 10 mA units bits position           */
#define USBPD_PDO_SNK_FIXED_OP_CURRENT_MSK           (0x3FFU << USBPD_PDO_SNK_FIXED_OP_CURRENT_POS)            /*!< Operational current in 10 mA units bits mask : 0x000003FF  */

/* Sink Variable type PDO elements */
#define USBPD_PDO_SNK_VARIABLE_MAX_VOLTAGE_POS       (20U)                                                     /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SNK_VARIABLE_MAX_VOLTAGE_MSK       (0x3FFU << USBPD_PDO_SNK_VARIABLE_MAX_VOLTAGE_POS)        /*!< Max Voltage in 50 mV units bits mask : 0x3FF00000  */

#define USBPD_PDO_SNK_VARIABLE_MIN_VOLTAGE_POS       (10U)                                                     /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SNK_VARIABLE_MIN_VOLTAGE_MSK       (0x3FFU << USBPD_PDO_SNK_VARIABLE_MIN_VOLTAGE_POS)        /*!< Max Voltage in 50 mV units bits mask : 0x000FFC00  */

#define USBPD_PDO_SNK_VARIABLE_OP_CURRENT_POS        (0U)                                                      /*!< Operational current in 10 mA units bits position           */
#define USBPD_PDO_SNK_VARIABLE_OP_CURRENT_MSK        (0x3FFU << USBPD_PDO_SNK_VARIABLE_OP_CURRENT_POS)         /*!< Operational current in 10 mA units bits mask : 0x000003FF  */

/* Sink Battery type PDO elements */
#define USBPD_PDO_SNK_BATTERY_MAX_VOLTAGE_POS        (20U)                                                     /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SNK_BATTERY_MAX_VOLTAGE_MSK        (0x3FFU << USBPD_PDO_SNK_BATTERY_MAX_VOLTAGE_POS)         /*!< Max Voltage in 50 mV units bits mask : 0x3FF00000  */

#define USBPD_PDO_SNK_BATTERY_MIN_VOLTAGE_POS        (10U)                                                     /*!< Max Voltage in 50 mV units bits position           */
#define USBPD_PDO_SNK_BATTERY_MIN_VOLTAGE_MSK        (0x3FFU << USBPD_PDO_SNK_BATTERY_MIN_VOLTAGE_POS)         /*!< Max Voltage in 50 mV units bits mask : 0x000FFC00  */

#define USBPD_PDO_SNK_BATTERY_OP_POWER_POS           (0U)                                                      /*!< Operational power in 250mW units bits position          */
#define USBPD_PDO_SNK_BATTERY_OP_POWER_MSK           (0x3FFU << USBPD_PDO_SNK_BATTERY_OP_POWER_POS)            /*!< Operational power in 250mW units bits mask : 0x000003FF */

#if defined(USBPDCORE_PPS)
/* Source APDO type PDO elements */
#define USBPD_PDO_SRC_APDO_PPS_POS                   (28U)                                                     /*!< Programmable Power Supply bit position           */
#define USBPD_PDO_SRC_APDO_PPS_MSK                   (0x3U << USBPD_PDO_SRC_APDO_PPS_POS)                      /*!< Programmable Power Supply bit mask : 0x300000000 */
#define USBPD_PDO_SRC_APDO_PPS                       (0U)                                                      /*!< Programmable Power Supply field value 00         */

#define USBPD_PDO_SRC_APDO_PPS_PWR_LIMITED_POS       (27U)                                                     /*!< PPS Power Limited bit position                   */
#define USBPD_PDO_SRC_APDO_PPS_PWR_LIMITED_SET       (0x1U << USBPD_PDO_SRC_APDO_PPS_PWR_LIMITED_POS)          /*!< PPS Power Limited bit is set                     */
#define USBPD_PDO_SRC_APDO_PPS_PWR_LIMITED_CLEAR     (0x0U << USBPD_PDO_SRC_APDO_PPS_PWR_LIMITED_POS)          /*!< PPS Power Limited bit is cleared                 */

#define USBPD_PDO_SRC_APDO_MAX_VOLTAGE_POS           (17U)                                                     /*!< APDO Max Voltage in 100 mV increments bits position  */
#define USBPD_PDO_SRC_APDO_MAX_VOLTAGE_MSK           (0xFFU << USBPD_PDO_SRC_APDO_MAX_VOLTAGE_POS)             /*!< APDO Max Voltage in 100 mV increments bits mask : 0x01FE0000 */

#define USBPD_PDO_SRC_APDO_MIN_VOLTAGE_POS           (8U)                                                      /*!< APDO Min Voltage in 100 mV increments bits position  */
#define USBPD_PDO_SRC_APDO_MIN_VOLTAGE_MSK           (0xFFU << USBPD_PDO_SRC_APDO_MIN_VOLTAGE_POS)             /*!< APDO Min Voltage in 100 mV increments bits mask : 0x0000FF00 */

#define USBPD_PDO_SRC_APDO_MAX_CURRENT_POS           (0U)                                                      /*!< APDO Max Current in 50 mA increments bits position  */
#define USBPD_PDO_SRC_APDO_MAX_CURRENT_MSK           (0x7FU << USBPD_PDO_SRC_APDO_MAX_CURRENT_POS)             /*!< APDO Max Current in 50 mA increments bits mask : 0x0000007F */

/* Sink APDO type PDO elements */
#define USBPD_PDO_SNK_APDO_PPS_POS                   (28U)                                                     /*!< Programmable Power Supply bit position           */
#define USBPD_PDO_SNK_APDO_PPS_MSK                   (0x3U << USBPD_PDO_SNK_APDO_PPS_POS)                      /*!< Programmable Power Supply bit mask : 0x300000000 */
#define USBPD_PDO_SNK_APDO_PPS                       (0U)                                                      /*!< Programmable Power Supply field value 00         */

#define USBPD_PDO_SNK_APDO_MAX_VOLTAGE_POS           (17U)                                                     /*!< APDO Max Voltage in 100 mV increments bits position  */
#define USBPD_PDO_SNK_APDO_MAX_VOLTAGE_MSK           (0xFFU << USBPD_PDO_SNK_APDO_MAX_VOLTAGE_POS)             /*!< APDO Max Voltage in 100 mV increments bits mask : 0x01FE0000 */

#define USBPD_PDO_SNK_APDO_MIN_VOLTAGE_POS           (8U)                                                      /*!< APDO Min Voltage in 100 mV increments bits position  */
#define USBPD_PDO_SNK_APDO_MIN_VOLTAGE_MSK           (0xFFU << USBPD_PDO_SNK_APDO_MIN_VOLTAGE_POS)             /*!< APDO Min Voltage in 100 mV increments bits mask : 0x0000FF00 */

#define USBPD_PDO_SNK_APDO_MAX_CURRENT_POS           (0U)                                                      /*!< APDO Max Current in 50 mA increments bits position  */
#define USBPD_PDO_SNK_APDO_MAX_CURRENT_MSK           (0x7FU << USBPD_PDO_SNK_APDO_MAX_CURRENT_POS)             /*!< APDO Max Current in 50 mA increments bits mask : 0x0000007F */
#endif /* USBPDCORE_PPS */

#if defined(USBPDCORE_SPR_AVS)
/* Source SPR AVSPDO type PDO elements */
#define USBPD_PDO_SRC_AVSPDO_SPR_POS                  (28U)                                                     /*!< Ajustable Voltage Supply bit position            */
#define USBPD_PDO_SRC_AVSPDO_SPR_MSK                  (0x3U << USBPD_PDO_SRC_AVSPDO_SPR_POS)                    /*!< Ajustable Voltage Supply bit mask : 0x300000000  */
#define USBPD_PDO_SRC_AVSPDO_SPR                      (2U   << USBPD_PDO_SRC_AVSPDO_SPR_POS)                    /*!< Ajustable Voltage Supply field value 10b         */

#define USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_POS      (26U)                                                                  /*!< Peak Current info bits position            */
#define USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_MSK      (0x3U << USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_POS)                     /*!< Peak Current info bits mask : 0x00300000   */
#define USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_EQUAL    (USBPD_CORE_PDO_PEAKEQUAL << USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_POS) /*!< Peak Current info : Equal to Ioc           */
#define USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_OVER1    (USBPD_CORE_PDO_PEAKOVER1 << USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 1         */
#define USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_OVER2    (USBPD_CORE_PDO_PEAKOVER2 << USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 2         */
#define USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_OVER3    (USBPD_CORE_PDO_PEAKOVER3 << USBPD_PDO_SRC_AVSPDO_SPR_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 3         */

#define USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_15V_POS  (10U)                                                     /*!< Max current in 10 mA units bits position           */
#define USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_15V_MSK  (0x3FFU << USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_15V_POS)  /*!< Max current in 10 mA units bits mask : 0x000003FF  */

#define USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_20V_POS  (0U)                                                      /*!< Max current in 10 mA units bits position           */
#define USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_20V_MSK  (0x3FFU << USBPD_PDO_SRC_AVSPDO_SPR_MAX_CURRENT_20V_POS)  /*!< Max current in 10 mA units bits mask : 0x000003FF  */

/* Sink SPR AVSPDO type PDO elements */
#define USBPD_PDO_SNK_AVSPDO_SPR_POS                  (28U)                                                     /*!< Ajustable Voltage Supply bit position            */
#define USBPD_PDO_SNK_AVSPDO_SPR_MSK                  (0x3U << USBPD_PDO_SNK_AVSPDO_SPR_POS)                    /*!< Ajustable Voltage Supply bit mask : 0x300000000  */
#define USBPD_PDO_SNK_AVSPDO_SPR                      (2U   << USBPD_PDO_SNK_AVSPDO_SPR_POS)                    /*!< Ajustable Voltage Supply field value 10b         */

#define USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_15V_POS  (10U)                                                     /*!< Max current in 10 mA units bits position           */
#define USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_15V_MSK  (0x3FFU << USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_15V_POS)  /*!< Max current in 10 mA units bits mask : 0x000003FF  */

#define USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_20V_POS  (0U)                                                      /*!< Max current in 10 mA units bits position           */
#define USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_20V_MSK  (0x3FFU << USBPD_PDO_SNK_AVSPDO_SPR_MAX_CURRENT_20V_POS)  /*!< Max current in 10 mA units bits mask : 0x000003FF  */
#endif /* USBPDCORE_SPR_AVS */

/* Source EPR AVSPDO type PDO elements */
#define USBPD_PDO_SRC_AVSPDO_EPR_POS                  (28U)                                                     /*!< Ajustable Voltage Supply bit position            */
#define USBPD_PDO_SRC_AVSPDO_EPR_MSK                  (0x3U << USBPD_PDO_SRC_AVSPDO_EPR_POS)                    /*!< Ajustable Voltage Supply bit mask : 0x300000000  */
#define USBPD_PDO_SRC_AVSPDO_EPR                      (1U   << USBPD_PDO_SRC_AVSPDO_EPR_POS)                    /*!< Ajustable Voltage Supply field value 01b         */

#define USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_POS          (26U)                                                     /*!< AVSPDO Peak Current bits position  */
#define USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_MSK          (0x3U << USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_POS)            /*!< AVSPDO Peak Current bits mask : 0x06000000 */
#define USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_EQUAL        (USBPD_CORE_PDO_PEAKEQUAL << USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_POS) /*!< Peak Current info : Equal to Ioc           */
#define USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_OVER1        (USBPD_CORE_PDO_PEAKOVER1 << USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 1         */
#define USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_OVER2        (USBPD_CORE_PDO_PEAKOVER2 << USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 2         */
#define USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_OVER3        (USBPD_CORE_PDO_PEAKOVER3 << USBPD_PDO_SRC_AVSPDO_PEAKCURRENT_POS) /*!< Peak Current info : Overload Cap 3         */

#define USBPD_PDO_SRC_AVSPDO_MAX_VOLTAGE_POS          (17U)                                                     /*!< AVSPDO Max Voltage in 100 mV increments bits position  */
#define USBPD_PDO_SRC_AVSPDO_MAX_VOLTAGE_MSK          (0x1FFU << USBPD_PDO_SRC_AVSPDO_MAX_VOLTAGE_POS)          /*!< AVSPDO Max Voltage in 100 mV increments bits mask : 0x01FE0000 */

#define USBPD_PDO_SRC_AVSPDO_MIN_VOLTAGE_POS          (8U)                                                      /*!< AVSPDO Min Voltage in 100 mV increments bits position  */
#define USBPD_PDO_SRC_AVSPDO_MIN_VOLTAGE_MSK          (0xFFU << USBPD_PDO_SRC_APDO_MIN_VOLTAGE_POS)             /*!< AVSPDO Min Voltage in 100 mV increments bits mask : 0x0000FF00 */

/* Sink EPR AVSPDO type PDO elements */
#define USBPD_PDO_SNK_AVSPDO_EPR_POS                  (28U)                                                     /*!< Ajustable Voltage Supply bit position            */
#define USBPD_PDO_SNK_AVSPDO_EPR_MSK                  (0x3U << USBPD_PDO_SNK_AVSPDO_EPR_POS)                    /*!< Ajustable Voltage Supply bit mask : 0x300000000  */
#define USBPD_PDO_SNK_AVSPDO_EPR                      (1U   << USBPD_PDO_SNK_AVSPDO_EPR_POS)                    /*!< Ajustable Voltage Supply field value 01b         */

#define USBPD_PDO_SNK_AVSPDO_PDPIN1W_POS              (0U)                                                      /*!< AVSPDO PDP in 1W increments bits position  */
#define USBPD_PDO_SNK_AVSPDO_PDPIN1W_MSK              (0xFFU << USBPD_PDO_SNK_AVSPDO_PDPIN1W_POS)               /*!< AVSPDO PDP in 1W increments bits mask : 0x0000007F */

#define USBPD_EXTENDED_MESSAGE                        (0x80U)                                                   /*!< Flag to indicate that it is a extended message     */

/**
  * @}
  */

/** @defgroup USBPD_ADO_TYPE_ALERT USBPD Type alert definition used for Alert Data Object
  * @{
  */

#define USBPD_ADO_TYPE_ALERT_BATTERY_STATUS (1U << 1U) /*!< Battery Status Change Event
                                                            (Attach/Detach/charging/discharging/idle)                */
#define USBPD_ADO_TYPE_ALERT_OCP            (1U << 2U) /*!< Over-Current Protection event when set
                                                            (Source only, for Sink Reserved and Must be set to zero) */
#define USBPD_ADO_TYPE_ALERT_OTP            (1U << 3U) /*!< Over-Temperature Protection event when set               */
#define USBPD_ADO_TYPE_ALERT_OPERATING_COND (1U << 4U) /*!< Operating Condition Change when set                      */
#define USBPD_ADO_TYPE_ALERT_SRC_INPUT      (1U << 5U) /*!< Source Input Change Event when set                       */
#define USBPD_ADO_TYPE_ALERT_OVP            (1U << 6U) /*!< Over-Voltage Protection event when set (Sink only,
                                                            for Source Reserved and Must be set to zero)             */
#define USBPD_ADO_TYPE_ALERT_EXT            (1U << 7U) /*!< Extended Alert Event                                     */

/**
  * @}
  */

/** @defgroup USBPD_ADO_FIXED_BATT USBPD Fixed Batteries definition used for Alert Data Object
  * @{
  */

#define USBPD_ADO_FIXED_BATT_BATTERY_0 (1U << 0U) /*!< Fixed Batterie 0 had a status change */
#define USBPD_ADO_FIXED_BATT_BATTERY_1 (1U << 1U) /*!< Fixed Batterie 1 had a status change */
#define USBPD_ADO_FIXED_BATT_BATTERY_2 (1U << 2U) /*!< Fixed Batterie 2 had a status change */
#define USBPD_ADO_FIXED_BATT_BATTERY_3 (1U << 3U) /*!< Fixed Batterie 3 had a status change */

/**
  * @}
  */

/** @defgroup USBPD_ADO_HOT_SWAP_BATT USBPD Hot Swappable Batteries definition used for Alert Data Object
  * @{
  */

#define USBPD_ADO_HOT_SWAP_BATT_BATTERY_4 (1U << 0U) /*!< Hot Swappable Batterie 4 had a status change */
#define USBPD_ADO_HOT_SWAP_BATT_BATTERY_5 (1U << 1U) /*!< Hot Swappable Batterie 5 had a status change */
#define USBPD_ADO_HOT_SWAP_BATT_BATTERY_6 (1U << 2U) /*!< Hot Swappable Batterie 6 had a status change */
#define USBPD_ADO_HOT_SWAP_BATT_BATTERY_7 (1U << 3U) /*!< Hot Swappable Batterie 7 had a status change */

/**
  * @}
  */

/** @defgroup USBPD_ADO_EXT_TYPE_ALERT USBPD Extended Alert Event Type definition used for Alert Data Object
  * @{
  */

#define USBPD_ADO_EXT_TYPE_ALERT_PWR_STATE_CHANGE   (1U) /*!< Power state change (DFP only)                               */
#define USBPD_ADO_EXT_TYPE_ALERT_PWR_BUTTON_PRESS   (2U) /*!< Power button press (UFP only)                               */
#define USBPD_ADO_EXT_TYPE_ALERT_PWR_BUTTON_RELEASE (3U) /*!< Power button release (UFP only)                             */
#define USBPD_ADO_EXT_TYPE_ALERT_CTRL_INITIATED     (4U) /*!< Controller initiated wake e.g. Wake on Lan (UFP only)       */
#define USBPD_ADO_EXT_TYPE_ALERT_SRC_REDUCE_SRC_CAP (5U) /*!< Source is about to reduce Source Capabilities (Source only) */

/**
  * @}
  */

/** @defgroup USBPD_SDB_PRESENT_INPUT USBPD Status Data Block - Present Input
  * @{
  */

#define USBPD_SDB_PRESENT_INPUT_EXT_PWR             (1U << 1U)    /*!< External Power when set                     */
#define USBPD_SDB_PRESENT_INPUT_EXT_PWR_ACDC        (1U << 2U)    /*!< External Power AC/DC (Valid when Bit 1 set)
                                                                       0: DC
                                                                       1: AC
                                                                       Reserved when Bit 1 is zero                 */
#define USBPD_SDB_PRESENT_INPUT_INT_PWR_FROM_BAT    (1U << 3U)    /*!< Internal Power from Battery when set        */
#define USBPD_SDB_PRESENT_INPUT_INT_PWR_FROM_N0_BAT (1U << 4U)    /*!< Internal Power from non-Battery power
                                                                       source when set                             */
#define USBPD_SDB_PRESENT_INPUT_INT_MASK            (0x0FU << 1U) /*!< Present Input mask                          */

/**
  * @}
  */

/** @defgroup USBPD_SDB_EVENT_FLAGS USBPD Status Data Block - event_flags
  * @{
  */

#define USBPD_SDB_EVENT_FLAGS_OCP   (1U << 1U)    /*!< OCP event when set                     */
#define USBPD_SDB_EVENT_FLAGS_OTP   (1U << 2U)    /*!< OTP event when set                     */
#define USBPD_SDB_EVENT_FLAGS_OVP   (1U << 3U)    /*!< OVP event when set                     */
#define USBPD_SDB_EVENT_FLAGS_CF_CV (1U << 4U)    /*!< CF mode when set, CV mode when cleared */
#define USBPD_SDB_EVENT_FLAGS_MASK  (0x0FU << 1U) /*!< event_flags mask                       */

/**
  * @}
  */

/** @defgroup USBPD_SDB_TEMP_STATUS USBPD Status Data Block - Temperature Status
  * @{
  */

#define USBPD_SDB_EVENT_TEMP_STATUS_NOT_SUPP  (0U << 1U) /*!< 00 - Not Supported.   */
#define USBPD_SDB_EVENT_TEMP_STATUS_NORMAL    (1U << 1U) /*!< 01 - Normal           */
#define USBPD_SDB_EVENT_TEMP_STATUS_WARNING   (2U << 1U) /*!< 10 - Warning          */
#define USBPD_SDB_EVENT_TEMP_STATUS_OVER_TEMP (3U << 1U) /*!< 11 - Over temperature */
#define USBPD_SDB_EVENT_TEMP_STATUS_MASK      (3U << 1U) /*!< Temp status mask      */

/**
  * @}
  */

/** @defgroup USBPD_SDB_POWER_STATUS USBPD Status Data Block - Power Status
  * @{
  */

#define USBPD_SDB_POWER_STATUS_CABLE                  (1U << 1U)    /*!< Source power limited due to cable
                                                                         supported current                          */
#define USBPD_SDB_POWER_STATUS_INSUFFICIENT_POWER     (1U << 2U)    /*!< Source power limited due to insufficient
                                                                         power available while sourcing other ports */
#define USBPD_SDB_POWER_STATUS_INSUFFICIENT_EXT_POWER (1U << 3U)    /*!< Source power limited due to insufficient
                                                                         external power                             */
#define USBPD_SDB_POWER_STATUS_EVENT_FLAGS            (1U << 4U)    /*!< Source power limited due to Event Flags
                                                                         in place (Event Flags must also be set)    */
#define USBPD_SDB_POWER_STATUS_TEMPERATURE            (1U << 5U)    /*!< Source power limited due to temperature    */
#define USBPD_SDB_POWER_STATUS_MASK                   (0x1FU << 1U) /*!< Power status mask                          */

/**
  * @}
  */

/** @defgroup USBPD_SDB_PWR_STATE USBPD Status Data Block - Power State Change
  * @{
  */

#define USBPD_SDB_PWR_STATE_NEW_NOT_SUPP         (0U << 0U) /*!< New Power State: Status not supported     */
#define USBPD_SDB_PWR_STATE_NEW_S0               (1U << 0U) /*!< New Power State: S0                       */
#define USBPD_SDB_PWR_STATE_NEW_MODERN_STDBY     (2U << 0U) /*!< New Power State: Modern Standby           */
#define USBPD_SDB_PWR_STATE_NEW_S3               (3U << 0U) /*!< New Power State: S3                       */
#define USBPD_SDB_PWR_STATE_NEW_S4               (4U << 0U) /*!< New Power State: S4                       */
#define USBPD_SDB_PWR_STATE_NEW_S5               (5U << 0U) /*!< New Power State: S5 (Off with battery,
                                                                 wake events supported)                    */
#define USBPD_SDB_PWR_STATE_NEW_G3               (6U << 0U) /*!< New Power State: G3 (Off with no battery,
                                                                 wake events not supported)                */
#define USBPD_SDB_PWR_STATE_NEW_INDIC_OFF_LED    (0U << 3U) /*!< New power state indicator: Off LED        */
#define USBPD_SDB_PWR_STATE_NEW_INDIC_ON_LED     (1U << 3U) /*!< New power state indicator: Off LED        */
#define USBPD_SDB_PWR_STATE_NEW_INDIC_BLINK_LED  (2U << 3U) /*!< New power state indicator: Blinking LED   */
#define USBPD_SDB_PWR_STATE_NEW_INDIC_BREATH_LED (3U << 3U) /*!< New power state indicator: Breathing LED  */
#define USBPD_SDB_PWR_STATE_MASK                 (0x3FU)    /*!< Power State Change mask                   */

/**
  * @}
  */

/** @defgroup USBPD_MANUFINFO_TARGET USBPD Manufacturer Info Target
  * @{
  */

#define USBPD_MANUFINFO_TARGET_PORT_CABLE_PLUG (0U) /*!< Manufacturer Info Target Port/Cable Plug */
#define USBPD_MANUFINFO_TARGET_BATTERY         (1U) /*!< Manufacturer Info Target  Battery        */

/**
  * @}
  */

/** @defgroup USBPD_MANUFINFO_REF USBPD Manufacturer Info Ref
  * @{
  */

#define USBPD_MANUFINFO_REF_MAX_VALUES (7U) /*!< Manufacturer Info Ref 0..3:Fixed Batteries and
                                                 4..7: Hot Swappable Batteries */

/**
  * @}
  */

/** @defgroup USBPD_BSDO_BATT_INFO USBPD Battery Status - Info
  * @{
  */

#define USBPD_BSDO_BATT_INFO_INVALID_REF        (1U << 0U) /*!< Battery Status - Info: Invalid Battery reference   */
#define USBPD_BSDO_BATT_INFO_BATT_PRESENT       (1U << 1U) /*!< Battery Status - Info: Battery is present when set */
#define USBPD_BSDO_BATT_INFO_BATT_ISCHARGING    (0U << 2U) /*!< Battery Status - Info: Battery is Charging         */
#define USBPD_BSDO_BATT_INFO_BATT_ISDISCHARGING (1U << 2U) /*!< Battery Status - Info: Battery is Discharging      */
#define USBPD_BSDO_BATT_INFO_BATT_ISIDLE        (2U << 2U) /*!< Battery Status - Info: Battery is Idle             */

/**
  * @}
  */

#ifdef USBPDCORE_PPS
/** @defgroup USBPD_CORE_DEF_REAL_TIME_FLAGS USBPD CORE DEF Real Time Flags
  * @{
  */

#define USBPD_PPS_REALTIMEFLAGS_PTF_NOT_SUPPORTED    (00U << 1U) /*!< PTF: 00 - Not Supported                         */
#define USBPD_PPS_REALTIMEFLAGS_PTF_NORMAL           (01U << 1U) /*!< PTF: 01 - Normal                                */
#define USBPD_PPS_REALTIMEFLAGS_PTF_WARNING          (10U << 1U) /*!< PTF: 10 - Warning                               */
#define USBPD_PPS_REALTIMEFLAGS_PTF_OVER_TEMPERATURE (11U << 1U) /*!< PTF: 11 - Over temperature                      */
#define USBPD_PPS_REALTIMEFLAGS_OMF_ENABLED          (1U  << 3U) /*!< OMF set when operating in Current Foldback mode */
#define USBPD_PPS_REALTIMEFLAGS_OMF_DISABLED         (0U  << 3U) /*!< OMF set when operating in Current Foldback mode */

/**
  * @}
  */
#endif /* USBPDCORE_PPS */

#if defined(USBPDCORE_SNK_CAPA_EXT)
/** @defgroup USBPD_SKEDB_VERSION USBPD Sink Capabilities Extended - SKEDB version
  * @{
  */

#define USBPD_SKEDB_VERSION_1P0 (1U) /*!< Version 1.0 */

/**
  * @}
  */

/** @defgroup USBPD_SKEDB_LOADSTEP USBPD Sink Capabilities Extended - Load Step field
  * @{
  */

#define USBPD_SKEDB_LOADSTEP_150MA (0U << 0U) /*!< 150mA/1s Load Step (default) */
#define USBPD_SKEDB_LOADSTEP_500MA (1U << 0U) /*!< 500mA/1s Load Step           */

/**
  * @}
  */

/** @defgroup USBPD_SKEDB_COMPLIANCE USBPD Sink Capabilities Extended - Compliance field
  * @{
  */

#define USBPD_SKEDB_COMPLIANCE_LPS (1U << 0U) /*!< Requires LPS Source when set */
#define USBPD_SKEDB_COMPLIANCE_PS1 (1U << 1U) /*!< Requires PS1 Source when set */
#define USBPD_SKEDB_COMPLIANCE_PS2 (1U << 2U) /*!< Requires PS2 Source when set */

/**
  * @}
  */

/** @defgroup USBPD_SKEDB_TOUCHTEMP USBPD Sink Capabilities Extended - Touch Temperature
  * @{
  */

#define USBPD_SKEDB_TOUCHTEMP_NA      (0U) /*!< Temperature conforms to Not applicable          */
#define USBPD_SKEDB_TOUCHTEMP_DEFAULT (1U) /*!< Temperature conforms to [IEC 60950-1] (default) */
#define USBPD_SKEDB_TOUCHTEMP_TS1     (2U) /*!< Temperature conforms to [IEC 62368-1] TS1       */
#define USBPD_SKEDB_TOUCHTEMP_TS2     (2U) /*!< Temperature conforms to [IEC 62368-1] TS2       */

/**
  * @}
  */

/** @defgroup USBPD_SKEDB_SINKMODES USBPD Sink Capabilities Extended - Sink modes
  * @{
  */

#define USBPD_SKEDB_SINKMODES_PPS    (1U << 0U) /*!< 1: PPS charging supported        */
#define USBPD_SKEDB_SINKMODES_VBUS   (1U << 1U) /*!< 1: VBUS powered                  */
#define USBPD_SKEDB_SINKMODES_MAINS  (1U << 2U) /*!< 1: Mains powered                 */
#define USBPD_SKEDB_SINKMODES_BATPOW (1U << 3U) /*!< 1: Battery powered               */
#define USBPD_SKEDB_SINKMODES_BATUNL (1U << 4U) /*!< 1: Battery essentially unlimited */
#define USBPD_SKEDB_SINKMODES_AVS    (1U << 5U) /*!< 1: AVS supported                 */

/**
  * @}
  */
#endif /* USBPDCORE_SNK_CAPA_EXT */

#if defined(USBPDCORE_VPD)
/**
  * @brief  USBPD VPD Status.
  */
typedef enum
{
  VPD_NONE                   = 0U, /*!< status none, no VPD detection ongoing */
  VPD_UNKNOWN                = 1U, /*!< status unknown                        */
  VPD_NOPD                   = 2U, /*!< status NOPD support                   */
  VPD_FAILED_ENTER_ALTERNATE = 3U, /*!< status Failed to enter alternate mode */
  VPD_DETECTED               = 4U, /*!< status VPD detected                   */
} usbpd_vpd_status_t;

#endif /* USBPDCORE_VPD */

#if defined(USBPDCORE_FWUPD)

/** @defgroup USBPD_FWUPD_MSGTYPE USBPD Firmware Update Message Request and Responses Defines
  * @{
  */

/**
  * @brief  USBPD Firmware Update Message Type.
  */
typedef enum
{
  USBPD_FWUPD_MSGTYPE_NONE                = 0x00U, /*!< Reserved value                                                */
  USBPD_FWUPD_MSGTYPE_RSP_GET_FW_ID       = 0x01U, /*!< Response is used to respond to a GET_FW_ID Request            */
  USBPD_FWUPD_MSGTYPE_RSP_PDFU_INITIATE   = 0x02U, /*!< Response is used to respond to a PDFU_INITIATE Request        */
  USBPD_FWUPD_MSGTYPE_RSP_PDFU_DATA       = 0x03U, /*!< Response is used to respond to a PDFU_DATA Request            */
  USBPD_FWUPD_MSGTYPE_RSP_PDFU_VALIDATE   = 0x05U, /*!< Response is used to respond to a PDFU_VALIDATE Request        */
  USBPD_FWUPD_MSGTYPE_RSP_PDFU_DATA_PAUSE = 0x07U, /*!< Response is used to respond to a PDFU_DATA_PAUSE Request      */
  USBPD_FWUPD_MSGTYPE_RSP_VENDOR_SPECIFIC = 0x7FU, /*!< Response is used to respond to a VENDOR_SPECIFIC Request      */
  USBPD_FWUPD_MSGTYPE_REQ_GET_FW_ID       = 0x81U, /*!< Request is used to retrieve information about a PDFU Responder
                                                        and determine if a firmware update is necessary               */
  USBPD_FWUPD_MSGTYPE_REQ_PDFU_INITIATE   = 0x82U, /*!< Request is used to initiate firmware update                   */
  USBPD_FWUPD_MSGTYPE_REQ_PDFU_DATA       = 0x83U, /*!< Request is used to transfer a data block from a firmware image
                                                        (response required)                                           */
  USBPD_FWUPD_MSGTYPE_REQ_PDFU_DATA_NR    = 0x84U, /*!< Request is used to transfer a data block from a firmware image
                                                        (response not required)                                       */
  USBPD_FWUPD_MSGTYPE_REQ_PDFU_VALIDATE   = 0x85U, /*!< Request is used to request validation of a firmware image     */
  USBPD_FWUPD_MSGTYPE_REQ_PDFU_ABORT      = 0x86U, /*!< Request is used to end firmware image update prematurely      */
  USBPD_FWUPD_MSGTYPE_REQ_PDFU_DATA_PAUSE = 0x87U, /*!< Request is used to pause a firmware image update              */
  USBPD_FWUPD_MSGTYPE_REQ_VENDOR_SPECIFIC = 0xFFU, /*!< Request is for vendor-specific use                            */
} usbpd_fwupd_msg_type_t;

/**
  * @}
  */

/** @defgroup USBPD_FWUPD_PROT_VER USBPD Firmware Update Message Protocol version Defines
  * @{
  */

#define  USBPD_FWUPD_PROT_VER_V1P0 0x01U /*!< USBPD Firmware Update Protocol Version 1.0 */

/**
  * @}
  */

/** @defgroup USBPD_FWUPD_SIZE_PAYLOAD USBPD Firmware Update Size Payload Defines
  * @{
  */

#define  USBPD_FWUPD_SIZE_PAYLOAD_RSP_GET_FW_ID           (sizeof(usbpd_fw_upd_get_fw_id_rsp_payload_t)) /*!< Payload size of Response is used to respond to a GET_FW_ID Request             */
#define  USBPD_FWUPD_SIZE_PAYLOAD_RSP_PDFU_INITIATE       (sizeof(usbpd_fw_upd_pdfu_init_rsp_payload_t)) /*!< Payload size of Response is used to respond to a PDFU_INITIATE Request         */
#define  USBPD_FWUPD_SIZE_PAYLOAD_RSP_PDFU_DATA           (sizeof(usbpd_fw_upd_pdfu_data_rsp_payload_t)) /*!< Payload size of Response is used to respond to a PDFU_DATA Request             */
#define  USBPD_FWUPD_SIZE_PAYLOAD_RSP_PDFU_VALIDATE       (sizeof(usbpd_fw_upd_pdfu_validate_rsp_payload_t)) /*!< Payload size of Response is used to respond to a PDFU_VALIDATE Request         */
#define  USBPD_FWUPD_SIZE_PAYLOAD_RSP_PDFU_DATA_PAUSE     (sizeof(usbpd_fw_upd_pdfu_data_pause_rsp_payload_t)) /*!< Payload size of Response is used to respond to a PDFU_DATA_PAUSE Request       */
#define  USBPD_FWUPD_SIZE_PAYLOAD_RSP_VENDOR_SPECIFIC     (sizeof(usbpd_fw_upd_pdfu_vendor_specific_rsp_payload_t)) /*!< Payload size of Response is used to respond to a VENDOR_SPECIFIC Request       */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_GET_FW_ID           (0U) /*!< Payload size of Request is used to retrieve information */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_PDFU_INITIATE       (8U) /*!< Payload size of Request is used to initiate firmware update                    */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_PDFU_DATA_MAX       (2U + 256U) /*!< Payload size of Request is used to transfer a data block from a firmware image (response required) */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_PDFU_DATA_NR_MAX    (2U + 256U) /*!< Payload size of Request is used to transfer a data block from a firmware image (response not required)                                        */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_PDFU_VALIDATE       (0U) /*!< Payload size of Request is used to request validation of a firmware image      */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_PDFU_ABORT          (0U) /*!< Payload size of Request is used to end firmware image update prematurely       */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_PDFU_DATA_PAUSE     (0U) /*!< Payload size of Request is used to pause a firmware image update               */
#define  USBPD_FWUPD_SIZE_PAYLOAD_REQ_VENDOR_SPECIFIC_MAX (2U + 256U) /*!< Payload size of Request is for vendor-specific use                  */

/**
  * @}
  */
#endif /* USBPDCORE_FWUPD */

/**
  * @brief  USBPD Supported SOP Types structure definition
  */
typedef enum
{
  USBPD_SUPPORTED_SOP_NONE       = 0U,         /*<! Not supported */
  USBPD_SUPPORTED_SOP_SOP        = (1U << 0U), /*<! SOP           */
  USBPD_SUPPORTED_SOP_SOP1       = (1U << 1U), /*<! SOP '         */
  USBPD_SUPPORTED_SOP_SOP2       = (1U << 2U), /*<! SOP ''        */
  USBPD_SUPPORTED_SOP_SOP1_DEBUG = (1U << 3U), /*<! SOP' Debug    */
  USBPD_SUPPORTED_SOP_SOP2_DEBUG = (1U << 4U)  /*<! SOP '' Debug  */
} usbpd_sop_support_t;

/**
  * @brief  USBPD Hard Reset Status structures definition.
  */
typedef enum
{
  USBPD_HR_STATUS_START_ACK,         /*!< Hard Reset start acknowledge received */
  USBPD_HR_STATUS_START_REQ,         /*!< Hard Reset start request received     */
  USBPD_HR_STATUS_MSG_SENT,          /*!< Hard Reset message sent               */
  USBPD_HR_STATUS_WAIT_VBUS_VSAFE0V, /*!< Hard Reset wait VBUS VSAFE0V          */
  USBPD_HR_STATUS_WAIT_VBUS_VSAFE5V, /*!< Hard Reset wait VBUS VSAFE5V          */
  USBPD_HR_STATUS_COMPLETED,         /*!< Hard Reset completed                  */
  USBPD_HR_STATUS_FAILED,            /*!< Hard Reset failed                     */
} usbpd_hard_reset_status_t;

/**
  * @brief  USBPD Power Role Swap Status structures definition.
  */
typedef enum
{
  USBPD_PRS_STATUS_NA,                    /*!< Power Role Swap not applicable             */
  USBPD_PRS_STATUS_START_ACK,             /*!< Power Role Swap start acknowledge received */
  USBPD_PRS_STATUS_START_REQ,             /*!< Power Role Swap start request received     */
  USBPD_PRS_STATUS_ACCEPTED,              /*!< Power Role Swap accepted                   */
  USBPD_PRS_STATUS_REJECTED,              /*!< Power Role Swap rejected                   */
  USBPD_PRS_STATUS_WAIT,                  /*!< Power Role Swap wait                       */
  USBPD_PRS_STATUS_VBUS_OFF,              /*!< Power Role Swap VBUS off                   */
  USBPD_PRS_STATUS_SRC_RP2RD,             /*!< Power Role Swap Source Rp to Rd            */
  USBPD_PRS_STATUS_SRC_PS_READY_SENT,     /*!< Power Role Swap Source PS Ready sent       */
  USBPD_PRS_STATUS_SNK_PS_READY_RECEIVED, /*!< Power Role Swap Sink PS Ready received     */
  USBPD_PRS_STATUS_SNK_RD2RP,             /*!< Power Role Swap Sink Rd to Rp              */
  USBPD_PRS_STATUS_VBUS_ON,               /*!< Power Role Swap VBUS on                    */
  USBPD_PRS_STATUS_SNK_PS_READY_SENT,     /*!< Power Role Swap Sink PS Ready sent         */
  USBPD_PRS_STATUS_SRC_PS_READY_RECEIVED, /*!< Power Role Swap Source PS Ready received   */
  USBPD_PRS_STATUS_COMPLETED,             /*!< Power Role Swap completed                  */
  USBPD_PRS_STATUS_FAILED,                /*!< Power Role Swap failed                     */
  USBPD_PRS_STATUS_ABORTED,               /*!< Power Role Swap aborted                    */
} usbpd_prs_status_t;

/**
  * @brief Status of VSafe.
  */
typedef enum
{
  USBPD_VSAFE_0V = 0U, /*!< USBPD VSAFE0V   */
  USBPD_VSAFE_5V = 1U, /*!< USBPD VSAFE5V   */
} usbpd_vsafe_status_t;

/**
  * @brief  USB Power Delivery Status structures definition.
  */
typedef enum
{
  USBPD_OK = 0U,
  USBPD_NOTSUPPORTED,
  USBPD_ERROR,
  USBPD_BUSY,
  USBPD_TIMEOUT,

  /* PRL status */
  USBPD_PRL_GOODCRC,
  USBPD_PRL_DISCARDED,
  USBPD_PRL_SOFTRESET,
  USBPD_PRL_CABLERESET,
  USBPD_PRL_SNKTX,

  /* Message reply */
  USBPD_ACCEPT,
  /* USBPD_GOTOMIN,*/
  USBPD_REJECT,
  USBPD_WAIT,
  USBPD_NAK,
  USBPD_ACK,

  USBPD_FAIL,
  USBPD_RXEVENT_SOP,
  USBPD_RXEVENT_SOP1,
  USBPD_RXEVENT_SOP2,
  USBPD_NOEVENT,
  USBPD_DISCARDRX,

  /* Stack initialization errors  */
  USBPD_MALLOCERROR,         /*<! Malloc error during CORE handles creation                                      */
  USBPD_INVALID_PORT_NUMBER, /*<! Exceed the maximum of supported ports by the stack (@ref USBPD_MAXPORT_COUNT)  */

  /* PDFU status  */
#if defined(USBPDCORE_FWUPD)
  USBPD_PDFU_NODATA,
  USBPD_PDFU_PAUSE,
  USBPD_PDFU_RESUME,
#endif /* USBPDCORE_FWUPD */

  USBPD_ERROR_CALLBACKMISSING,
  USBPD_WAIT_CHUNKREQUEST,
} usbpd_status_t;

/**
  * @brief  USBPD CC line definition.
  */
typedef enum
{
  CCNONE  = 0U, /*!< No CC line */
  CC1     = 1U, /*!< CC1 line   */
  CC2     = 2U, /*!< CC2 line   */
} usbpd_cc_line_t;


/**
  * @brief  USBPD Specification Revision structure definition.
  */
typedef enum
{
  USBPD_SPECIFICATION_REV1 = 0U, /*!< Revision 1.0 */
  USBPD_SPECIFICATION_REV2 = 1U, /*!< Revision 2.0 */
  USBPD_SPECIFICATION_REV3 = 2U, /*!< Revision 3.0 */
} usbpd_spec_rev_t;

/**
  * @brief  USBPD Power mode Revision structure definition.
  */
typedef enum
{
  USBPD_SPR_MODE = 0U, /*!< SPR : standard power role */
  USBPD_EPR_MODE = 1U, /*!< EPR : extended power role */
} usbpd_power_mode_t;

/**
  * @brief  USBPD SPR APDO type structure definition
  */
typedef enum
{
  USBPD_SPR_APDO_PPS = 0U,  /*!< SPR PPS APDO */
  USBPD_SPR_APDO_AVS = 1U,   /*!< SPR AVS APDO */
} usbpd_spr_apdo_t;

/**
  * @brief  USBPD VDM Specification Revision structure definition.
  */
typedef enum
{
  USBPD_VDMVERSION_REV1 = 0U, /*!< Revision 1.0                                       */
  USBPD_VDMVERSION_REV2 = 1U, /*!< Revision 2.0 only used if USBPD_SPECIFICATION_REV3 */
} usbpd_vdm_version_t;

/**
  * @brief  USBPD VDM Specification Revision structure definition.
  */
typedef enum
{
  USBPD_VDMVERSION_MINOR_0 = 0x00U, /*!< Version 2.0 (Used prior to USBPD Revision 3.1, Version 1.6) */
  USBPD_VDMVERSION_MINOR_1 = 0x01U, /*!< Version 2.1 (Used after USBPD Revision 3.1, Version 1.6)    */
} usbpd_vdm_version_minor_t;

/**
  * @brief  USBPD Passive Cable VDOs version structure definition.
  */
typedef enum
{
  USBPD_VDM_VDO_PASSIVE_CABLE_VERSION_REV1P0  = 0U, /*!< Version Number of the Passive calbe VDO Revision 1.0 */
} usbpd_vdm_vdo_passive_cable_version_t;

/**
  * @brief  USBPD Active Cable VDOs version structure definition.
  */
typedef enum
{
  USBPD_VDM_VDO_ACTIVE_CABLE_VERSION_REV1P3  = 3U, /*!< Version Number of the Active Passive VDO Revision 1.3 */
} usbpd_vdm_vdo_active_cable_version_t;

/**
  * @brief  USBPD UFP VDO version structure definition.
  */
typedef enum
{
  USBPD_VDM_VDO_UFP_VERSION_REV1P1 = 1U,  /*!< Version Number of the UFP VDO Revision 1.1 */
  USBPD_VDM_VDO_UFP_VERSION_REV1P2 = 2U,  /*!< Version Number of the UFP VDO Revision 1.2 */
  USBPD_VDM_VDO_UFP_VERSION_REV1P3 = 3U,  /*!< Version Number of the UFP VDO Revision 1.3 */
} usbpd_vdm_vdo_ufp_version_t;

/**
  * @brief  USBPD DFP VDO version structure definition.
  */
typedef enum
{
  USBPD_VDM_VDO_DFP_VERSION_REV1P1 = 1U, /*!< Version Number of the DFP VDO Revision 1.1 */
  USBPD_VDM_VDO_DFP_VERSION_REV1P2 = 2U, /*!< Version Number of the DFP VDO Revision 1.2 */
} usbpd_vdm_vdo_dfp_version_t;

#if defined(USBPDCORE_VPD)
/**
  * @brief  USBPD Vconn Powered USB Device VDO version structure definition.
  */
typedef enum
{
  USBPD_VDM_VDO_VPD_VERSION_REV1P0 = 0U, /*!< Version Number of the VPD VDO Revision 1.0 */
} usbpd_vdm_vdo_vpd_version_t;

#endif /* USBPDCORE_VPD */

/**
  * @brief CAD event value.
  */
typedef enum
{
  USBPD_CAD_EVENT_NONE           = 0U,  /*!< USBPD CAD event None                                 */
  USBPD_CAD_EVENT_DETACHED       = 1U,  /*!< USBPD CAD event No cable detected                    */
  USBPD_CAD_EVENT_ATTACHED       = 2U,  /*!< USBPD CAD event Port partner attached                */
  USBPD_CAD_EVENT_EMC            = 3U,  /*!< USBPD CAD event Electronically Marked Cable detected */
  USBPD_CAD_EVENT_ATTEMC         = 4U,  /*!< USBPD CAD event Port Partner detected through EMC    */
  USBPD_CAD_EVENT_ACCESSORY      = 5U,  /*!< USBPD CAD event Accessory detected                   */
  USBPD_CAD_EVENT_DEBUG          = 6U,  /*!< USBPD CAD event Debug detected                       */
  USBPD_CAD_EVENT_LEGACY         = 7U,  /*!< USBPD CAD event legacy cables detected               */
  USPPD_CAD_EVENT_VPD            = 8U,  /*!< USBPD CAD event VPD                                  */
  USPPD_CAD_EVENT_UNKNOW         = 9U,  /*!< USBPD CAD event unknown                              */
  USBPD_CAD_EVENT_CABLE_ATTACHED = 10U, /*!< USBPD CAD event cable attached                       */
  USBPD_CAD_EVENT_CABLE_DETACHED = 11U, /*!< USBPD CAD event cable detached                       */
} usbpd_cad_event_t;

/**
  * @brief  USBPD Port Data Role Types structure definition.
  */
typedef enum
{
  USBPD_PORTDATAROLE_UFP       = 0x00U,                   /*!< UFP                                             */
  USBPD_PORTDATAROLE_SOP1_SOP2 = USBPD_PORTDATAROLE_UFP,  /*!<  For all other SOP* Packets the Port Data Role
                                                               field is Reserved and must be set to zero.     */
  USBPD_PORTDATAROLE_DFP       = 0x01U,                   /*!< DFP                                             */
} usbpd_port_data_role_t;

/**
  * @brief  USBPD Control Message Types structure definition.
  */
typedef enum
{
  USBPD_CONTROLMSG_GOODCRC               = 0x01U,  /*!< GoodCRC Control Message             */
  USBPD_CONTROLMSG_GOTOMIN               = 0x02U,  /*!< GotoMin Control Message             */
  USBPD_CONTROLMSG_ACCEPT                = 0x03U,  /*!< Accept Control Message              */
  USBPD_CONTROLMSG_REJECT                = 0x04U,  /*!< Reject Control Message              */
  USBPD_CONTROLMSG_PING                  = 0x05U,  /*!< Ping Control Message                */
  USBPD_CONTROLMSG_PS_RDY                = 0x06U,  /*!< PS_RDY Control Message              */
  USBPD_CONTROLMSG_GET_SRC_CAP           = 0x07U,  /*!< Get_Source_Cap Control Message      */
  USBPD_CONTROLMSG_GET_SNK_CAP           = 0x08U,  /*!< Get_Sink_Cap Control Message        */
  USBPD_CONTROLMSG_DR_SWAP               = 0x09U,  /*!< DR_Swap Control Message             */
  USBPD_CONTROLMSG_PR_SWAP               = 0x0AU,  /*!< PR_Swap Control Message             */
  USBPD_CONTROLMSG_VCONN_SWAP            = 0x0BU,  /*!< VCONN_Swap Control Message          */
  USBPD_CONTROLMSG_WAIT                  = 0x0CU,  /*!< Wait Control Message                */
  USBPD_CONTROLMSG_SOFT_RESET            = 0x0DU,  /*!< Soft_Reset Control Message          */
#if defined(USBPDCORE_USBDATA)
  USBPD_CONTROLMSG_DATA_RESET            = 0x0EU,  /*!< data_Reset Control Message          */
  USBPD_CONTROLMSG_DATA_RESET_COMPLETE   = 0x0FU,  /*!< data_Reset_complete Control Message */
#endif /* USBPDCORE_USBDATA */
  USBPD_CONTROLMSG_NOT_SUPPORTED         = 0x10U,  /*!< Not supported                       */
  USBPD_CONTROLMSG_GET_SRC_CAPEXT        = 0x11U,  /*!< Get source capability extended      */
  USBPD_CONTROLMSG_GET_STATUS            = 0x12U,  /*!< Get status                          */
  USBPD_CONTROLMSG_FR_SWAP               = 0x13U,  /*!< Fast role swap                      */
  USBPD_CONTROLMSG_GET_PPS_STATUS        = 0x14U,  /*!< Get PPS Status                      */
  USBPD_CONTROLMSG_GET_COUNTRY_CODES     = 0x15U,  /*!< Get Country codes                   */
#if defined(USBPDCORE_SNK_CAPA_EXT)
  USBPD_CONTROLMSG_GET_SNK_CAPEXT        = 0x16U,  /*!< Get Sink Capability extended        */
#endif /* USBPDCORE_SNK_CAPA_EXT */
  USBPD_CONTROLMSG_GET_SOURCE_INFO       = 0x17U,  /*!< Get source info                     */
  USBPD_CONTROLMSG_GET_REVISION          = 0x18U,  /*!< Get revision                        */
} usbpd_control_msg_t;

/**
  * @brief  USBPD Data Message Types structure definition.
  */
typedef enum
{
  USBPD_DATAMSG_SRC_CAPABILITIES         = 0x01U,  /*!< Source Capabilities Data Message  */
  USBPD_DATAMSG_REQUEST                  = 0x02U,  /*!< Request Data Message              */
  USBPD_DATAMSG_BIST                     = 0x03U,  /*!< BIST Data Message                 */
  USBPD_DATAMSG_SNK_CAPABILITIES         = 0x04U,  /*!< Sink_Capabilities Data Message    */
  USBPD_DATAMSG_BATTERY_STATUS           = 0x05U,  /*!< Battery status                    */
  USBPD_DATAMSG_ALERT                    = 0x06U,  /*!< Alert                             */
  USBPD_DATAMSG_GET_COUNTRY_INFO         = 0x07U,  /*!< Get country info                  */
#if defined(USBPDCORE_USBDATA)
  USBPD_DATAMSG_ENTER_USB                = 0x08U,  /*!< Enter usb                         */
#endif /* USBPDCORE_USBDATA */
  USBPD_DATAMSG_EPR_REQUEST              = 0x09U,  /*!< EPR request                       */
  USBPD_DATAMSG_EPR_MODE                 = 0x0AU,  /*!< EPR mode                          */
  USBPD_DATAMSG_SOURCE_INFO              = 0x0BU,  /*!< source info                       */
  USBPD_DATAMSG_REVISION                 = 0x0CU,  /*!< Revision                          */
  USBPD_DATAMSG_VENDOR_DEFINED           = 0x0FU,  /*!< Vendor_Defined Data Message       */
} usbpd_data_msg_t;

/**
  * @brief  USBPD Extended Message Types structure definition.
  */
typedef enum
{
  USBPD_EXT_NONE                  = 0x00U,
  USBPD_EXT_SOURCE_CAPABILITIES   = 0x01U, /*!< sent by Source or Dual-Role Power    - SOP only  */
  USBPD_EXT_STATUS                = 0x02U, /*!< sent by Source                       - SOP only  */
  USBPD_EXT_GET_BATTERY_CAP       = 0x03U, /*!< sent by Source or Sink               - SOP only  */
  USBPD_EXT_GET_BATTERY_STATUS    = 0x04U, /*!< sent by Source or Sink               - SOP only  */
  USBPD_EXT_BATTERY_CAPABILITIES  = 0x05U, /*!< sent by Source or Sink               - SOP only  */
  USBPD_EXT_GET_MANUFACTURER_INFO = 0x06U, /*!< sent by Source or Sink or Cable Plug - SOP*      */
  USBPD_EXT_MANUFACTURER_INFO     = 0x07U, /*!< sent by Source or Sink or Cable Plug - SOP*      */
  USBPD_EXT_SECURITY_REQUEST      = 0x08U, /*!< sent by Source or Sink               - SOP*      */
  USBPD_EXT_SECURITY_RESPONSE     = 0x09U, /*!< sent by Source or Sink or Cable Plug - SOP*      */
  USBPD_EXT_FIRM_UPDATE_REQUEST   = 0x0AU, /*!< sent by Source or Sink               - SOP*      */
  USBPD_EXT_FIRM_UPDATE_RESPONSE  = 0x0BU, /*!< sent by Source or Sink or Cable Plug - SOP*      */
  USBPD_EXT_PPS_STATUS            = 0x0CU, /*!< sent by Source                       - SOP only  */
  USBPD_EXT_COUNTRY_INFO          = 0x0DU, /*!< sent by Source or Sink               - SOP only  */
  USBPD_EXT_COUNTRY_CODES         = 0x0EU, /*!< sent by Source or Sink               - SOP only  */
#if defined(USBPDCORE_SNK_CAPA_EXT)
  USBPD_EXT_SINK_CAPABILITIES     = 0x0FU, /*!< sent by Sink or Dual-Role Power      - SOP only  */
#endif /* USBPDCORE_SNK_CAPA_EXT */
  USBPD_EXT_CONTROL               = 0x10U, /*!< extended control message             - SOP only  */
  USBPD_EXT_EPR_SOURCE_CAPA       = 0x11U, /*!< extended source capabilities         - SOP only  */
  USBPD_EXT_EPR_SINK_CAPA         = 0x12U, /*!< extended sink capabilities           - SOP only  */
  USBPD_EXT_VDM                   = 0x1EU, /*!< extended vendor defined              - SOP*      */
} usbpd_extended_msg_t;

/**
  * @brief  USBPD Extended Control Message Types structure definition.
  */
typedef enum
{
  USBPD_EXTENDED_CONTROL_EPR_GETSRCCAPA   = 1U, /*!< EPR Get Source Capability Extended Message     */
  USBPD_EXTENDED_CONTROL_EPR_GETSNKCAPA   = 2U, /*!< EPR Get Sink Capability Extended Message       */
  USBPD_EXTENDED_CONTROL_EPR_KEEPALIVE    = 3U, /*!< EPR Keep Alive Extended Message                */
  USBPD_EXTENDED_CONTROL_EPR_KEEPALIVEACK = 4U, /*!< EPR Keep Alive Acknowledgment Extended Message */
} usbpd_extended_control_msg_epr_t;

/**
  * @brief Sink CC pins Multiple Source Current Advertisements.
  */
typedef enum
{
  USBPD_CAD_SOURCE_CURRENT_ADV_UNDEF  = 0U, /*!< Port Power Role Source     */
  USBPD_CAD_SOURCE_CURRENT_ADV_VRDUSB = 1U, /*!< Default USB Power          */
  USBPD_CAD_SOURCE_CURRENT_ADV_VRD1A5 = 2U, /*!< USB Type-C Current @ 1.5 A */
  USBPD_CAD_SOURCE_CURRENT_ADV_VRD3A0 = 3U, /*!< USB Type-C Current @ 3 A   */
} usbpd_cad_source_current_adv_t;


/**
  * @brief  USBPD CAD Rp value definition.
  */
typedef enum
{
  USBPD_CAD_RP_DEFAULT = 0U, /*!< Default USB Power          */
  USBPD_CAD_RP_1A5     = 1U, /*!< USB Type-C Current @ 1.5 A */
  USBPD_CAD_RP_3A0     = 2U, /*!< USB Type-C Current @ 3 A   */
} usbpd_cad_rp_value_t;

/**
  * @brief  USBPD SOP Message Types structure definition.
  */
typedef enum
{
  USBPD_SOPTYPE_SOP            = 0U,    /*!< SOP*  MESSAGES       */
  USBPD_SOPTYPE_SOP1           = 1U,    /*!< SOP'  MESSAGES       */
  USBPD_SOPTYPE_SOP2           = 2U,    /*!< SOP'' MESSAGES       */
  USBPD_SOPTYPE_SOP1_DEBUG     = 3U,    /*!< SOP'  DEBUG_MESSAGES */
  USBPD_SOPTYPE_SOP2_DEBUG     = 4U,    /*!< SOP'' DEBUG_MESSAGES */
  USBPD_SOPTYPE_HARD_RESET     = 5U,    /*!< HARD RESET MESSAGE   */
  USBPD_SOPTYPE_CABLE_RESET    = 6U,    /*!< CABLE RESET MESSAGE  */
  USBPD_SOPTYPE_BIST_MODE_2    = 7U,    /*!< BIST_MODE2 MESSAGE   */
  USBPD_SOPTYPE_INVALID        = 0xFFU, /*!< Invalid type         */
} usbpd_sop_type_t;

/**
  * @brief  USBPD funtionnal status definition.
  */
typedef enum
{
  USBPD_DISABLE = 0U,  /*!< Disable */
  USBPD_ENABLE  = 1U,  /*!< Enable  */
} usbpd_functional_status_t;

/**
  * @brief  USBPD PHY Tx status definition.
  */
typedef enum
{
  USBPD_PHY_TX_CPLT    = 0U,  /*!< Tx completed */
  USBPD_PHY_TX_DISCARD = 1U,  /*!< Tx discard   */
  USBPD_PHY_TX_ABORT   = 2U,  /*!< Tx abort     */
} usbpd_phy_tx_status_t;

/**
  * @brief  USBPD Port Power Role Types structure definition.
  */
typedef enum
{
  USBPD_PORT_PWR_ROLE_CABLEPLUG_FROMDFPUFP    = 0U,                      /*!< Message originated from a DFP or UFP */
  USBPD_PORT_PWR_ROLE_SNK = USBPD_PORT_PWR_ROLE_CABLEPLUG_FROMDFPUFP,    /*!< Sink power role                      */
  USBPD_PORT_PWR_ROLE_CABLEPLUG_FROMCABLEPLUG = 1U,                      /*!< Message originated from a Cable Plug */
  USBPD_PORT_PWR_ROLE_SRC = USBPD_PORT_PWR_ROLE_CABLEPLUG_FROMCABLEPLUG, /*!< Source power role                    */
} usbpd_port_pwr_role_t;

/**
  * @brief  USBPD BIST Mode Types structure definition.
  */
typedef enum
{
  USBPD_BIST_CARRIER_MODE2              = 0x05U,  /*!< Request Transmitter to enter BIST Carrier Mode   */
  USBPD_BIST_TEST_DATA                  = 0x08U,  /*!< Sends a Test Data Frame.                         */
#if defined(USBPDCORE_DRP) || defined(USBPDCORE_SRC)
  USBPD_BIST_SHARED_TEST_MODE_ENTRY     = 0x09U,  /*!< Requests UUT to enter Shared Capacity Test Mode  */
  USBPD_BIST_SHARED_TEST_MODE_EXIT      = 0x0AU,  /*!< Requests UUT to exit Shared Capacity Test Mode   */
#endif /* USBPDCORE_DRP || USBPDCORE_SRC */
} usbpd_bist_mode_t;

/**
  * @brief  USBPD Core PDO types definition.
  */
typedef enum
{
  USBPD_CORE_PDO_TYPE_FIXED    = 0U, /*!< Fixed Supply PDO                                       */
  USBPD_CORE_PDO_TYPE_BATTERY  = 1U, /*!< Battery Supply PDO                                     */
  USBPD_CORE_PDO_TYPE_VARIABLE = 2U, /*!< Variable Supply (non-battery) PDO                      */
  USBPD_CORE_PDO_TYPE_APDO     = 3U, /*!< Augmented Power Data Object (APDO)                     */
#if defined(USBPDCORE_EPR)
  USBPD_CORE_PDO_TYPE_AVSPDO   = 3U, /*!< EPR Adjustable Voltage Supply Power Data object (APDO) */
#endif /* USBPDCORE_EPR */
} usbpd_core_pdo_type_t;

/**
  * @brief  USBPD Core APDO types definition.
  */
typedef enum
{
  USBPD_CORE_APDO_TYPE_PPS     = 0U, /*!< APDO sub-type = SPR Programmable Power Supply */
  USBPD_CORE_APDO_TYPE_EPR_AVS = 1U, /*!< APDO sub-type = EPR Adjustable Voltage Supply */
  USBPD_CORE_APDO_TYPE_SPR_AVS = 2U, /*!< APDO sub-type = SPR Adjustable Voltage Supply */
} usbpd_core_apdo_type_t;

/**
  * @brief  USBPD power state.
  */
typedef enum
{
  USBPD_PWR_STATE_NO               = 0U, /*!< No power contract */
  USBPD_PWR_STATE_DEFAULT5V        = 1U, /*!< Default 5V        */
  USBPD_PWR_STATE_IMPLICITCONTRACT = 2U, /*!< Implicit contract */
  USBPD_PWR_STATE_EXPLICITCONTRACT = 3U, /*!< Explicit contract */
  USBPD_PWR_STATE_TRANSITION       = 4U, /*!< Power transition  */
} usbpd_pwr_state_t;

/**
  * @brief  Power VBUS connection status.
  */
typedef enum
{
  USBPD_PWR_VBUS_CONNECTED     = 0U, /*!< VBUS is connected     */
  USBPD_PWR_VBUS_NOT_CONNECTED = 1U, /*!< VBUS is not connected */
} usbpd_pwr_vbus_connection_status_t;


/**
  * @brief  VBUS detect callback type.
  */
typedef void (* usbpd_pwr_vbusdetect_cb_t)(uint8_t, usbpd_pwr_vbus_connection_status_t);

/**
  * @brief  DRP support indication in PDO definition (Source or Sink).
  */
typedef enum
{
  USBPD_CORE_PDO_DRP_NOT_SUPPORTED = 0U, /*!< Dual Role Power not supported */
  USBPD_CORE_PDO_DRP_SUPPORTED     = 1U, /*!< Dual Role Power supported     */
} usbpd_core_pdo_drp_support_t;

/**
  * @brief  USB Suspend support indication in PDO definition (Source).
  */
typedef enum
{
  USBPD_CORE_PDO_USBSUSP_NOT_SUPPORTED = 0U, /*!< USB Suspend not supported */
  USBPD_CORE_PDO_USBSUSP_SUPPORTED     = 1U, /*!< USB Suspend supported     */
} usbpd_core_pdo_usb_suspend_support_t;

/**
  * @brief  Fixed Power Source Externally Powered indication in PDO definition (Source or Sink).
  */
typedef enum
{
  USBPD_CORE_PDO_NOT_EXT_POWERED = 0U, /*!< No external power source is available */
  USBPD_CORE_PDO_EXT_POWERED     = 1U, /*!< External power source is available    */
} usbpd_core_pdo_extpowered_support_t;

/**
  * @brief  USB Communication capability over USB Data lines indication in PDO definition (Source or Sink).
  */
typedef enum
{
  USBPD_CORE_PDO_USBCOMM_NOT_CAPABLE = 0U, /*!< Device not capable of communication over USB Data lines */
  USBPD_CORE_PDO_USBCOMM_CAPABLE     = 1U, /*!< Device capable of communication over USB Data lines     */
} usbpd_core_pdo_usb_com_support_t;

/**
  * @brief  Dual Role Data support indication in PDO definition (Source or Sink).
  */
typedef enum
{
  USBPD_CORE_PDO_DRD_NOT_SUPPORTED = 0U, /*!< Dual Role Data not supported */
  USBPD_CORE_PDO_DRD_SUPPORTED     = 1U, /*!< Dual Role Data supported     */
} usbpd_core_pdo_drd_support_t;

/**
  * @brief  Fixed Power Source Peak Current Capability type definition (Source).
  */
typedef enum
{
  USBPD_CORE_PDO_PEAKEQUAL = 0U, /*!< Peak current equals                                        */
  USBPD_CORE_PDO_PEAKOVER1 = 1U, /*!< Overload Capabilities:
                                      1. Peak current equals 150% IOC for 1ms @ 5% duty cycle
                                         (low current equals 97% IOC for 19ms)
                                      2. Peak current equals 125% IOC for 2ms @ 10% duty cycle
                                         (low current equals 97% IOC for 18ms)
                                      3. Peak current equals 110% IOC for 10ms @ 50% duty cycle
                                         (low current equals 90% IOC for 10ms                    */
  USBPD_CORE_PDO_PEAKOVER2 = 2U, /*!< Overload Capabilities:
                                      1. Peak current equals 200% IOC for 1ms @ 5% duty cycle
                                         (low current equals 95% IOC for 19ms)
                                      2. Peak current equals 150% IOC for 2ms @ 10% duty cycle
                                         (low current equals 94% IOC for 18ms)
                                      3. Peak current equals 125% IOC for 10ms @ 50% duty cycle
                                         (low current equals 75% IOC for 10ms)                   */
  USBPD_CORE_PDO_PEAKOVER3 = 3U, /*!< Overload Capabilities:
                                      1. Peak current equals 200% IOC for 1ms @ 5% duty cycle
                                         (low current equals 95% IOC for 19ms)
                                      2. Peak current equals 175% IOC for 2ms @ 10% duty cycle
                                         (low current equals 92% IOC for 18ms)
                                      3. Peak current equals 150% IOC for 10ms @ 50% duty cycle
                                         (low current equals 50% IOC for 10ms)                   */
} usbpd_core_pdo_peak_current_t;


/**
  * @brief  PDO definition values (Sink) indicating if the Sink requires more than vSafe5V for full functionality.
  */
typedef enum
{
  USBPD_CORE_PDO_HIGHER_CAPA_NO  = 0U, /*!< No need for more than vSafe5V to provide full functionality */
  USBPD_CORE_PDO_HIGHER_CAPA_YES = 1U, /*!< Sink needs more than vSafe5V to provide full functionality  */
} usbpd_core_pdo_higher_capa_t;

/**
  * @brief  Unchunked Extended Messages support values in PDO definition (Source).
  */
typedef enum
{
  USBPD_CORE_PDO_UNCHUNK_NOT_SUPPORTED = 0U, /*!< Unchunked Extended Messages not supported */
  USBPD_CORE_PDO_UNCHUNK_SUPPORTED     = 1U, /*!< Unchunked Extended Messages supported     */
} usbpd_core_pdo_unchunk_support_t;

/**
  * @brief  Fast Role Swap Required Current values in PDO definition (Sink).
  */
typedef enum
{
  USBPD_CORE_PDO_FRS_NOT_SUPPORTED     = 0U, /*!< Fast Role Swap not supported */
  USBPD_CORE_PDO_FRS_DEFAULT_USB_POWER = 1U, /*!< Default USB Power            */
  USBPD_CORE_PDO_FRS_1_5A_5V           = 2U, /*!< 1_5A at 5V supported         */
  USBPD_CORE_PDO_FRS_3A_5V             = 3U, /*!< 3A at 5V supported           */
} usbpd_core_pdo_frs_current_support_t;

/**
  * @brief  Notification event used inside PE callbacks (USBPD_PE_NotifyDPM) to inform DPM.
  */
typedef enum
{
  USBPD_NOTIFY_REQUEST_ACCEPTED            = 1U,   /*!< Request accepted notification                          */
  USBPD_NOTIFY_REQUEST_REJECTED            = 2U,   /*!< Request rejected notification                          */
  USBPD_NOTIFY_REQUEST_WAIT                = 3U,   /*!< Request wait notification                              */
  USBPD_NOTIFY_REQUEST_GOTOMIN             = 4U,   /*!< Request go to minimum notification                     */
  USBPD_NOTIFY_GETSNKCAP_SENT              = 5U,   /*!< Get Sink Capabilities sent notification                */
  USBPD_NOTIFY_GETSNKCAP_RECEIVED          = 6U,   /*!< Get Sink Capabilities received notification            */
  USBPD_NOTIFY_GETSNKCAP_ACCEPTED          = 7U,   /*!< Get Sink Capabilities accepted notification            */
  USBPD_NOTIFY_GETSNKCAP_REJECTED          = 8U,   /*!< Get Sink Capabilities rejected notification            */
  USBPD_NOTIFY_GETSNKCAP_TIMEOUT           = 9U,   /*!< Get Sink Capabilities timeout notification             */
  USBPD_NOTIFY_SNKCAP_SENT                 = 10U,  /*!< Sink Capabilities sent notification                    */
  USBPD_NOTIFY_GETSRCCAP_SENT              = 11U,  /*!< Get Source Capabilities sent notification              */
  USBPD_NOTIFY_GETSRCCAP_RECEIVED          = 12U,  /*!< Get Source Capabilities received notification          */
  USBPD_NOTIFY_GETSRCCAP_ACCEPTED          = 13U,  /*!< Get Source Capabilities accepted notification          */
  USBPD_NOTIFY_GETSRCCAP_REJECTED          = 14U,  /*!< Get Source Capabilities rejected notification          */
  USBPD_NOTIFY_SRCCAP_SENT                 = 15U,  /*!< Source Capabilities sent notification                  */
  USBPD_NOTIFY_POWER_EXPLICIT_CONTRACT     = 16U,  /*!< Power explicit contract notification                   */
  USBPD_NOTIFY_POWER_SRC_READY             = 17U,  /*!< Power source ready notification                        */
  USBPD_NOTIFY_POWER_SNK_READY             = 18U,  /*!< Power sink ready notification                          */
  USBPD_NOTIFY_POWER_SNK_STOP              = 19U,  /*!< Power sink stop notification                           */
  USBPD_NOTIFY_POWER_SWAP_TO_SNK_DONE      = 20U,  /*!< Power swap to sink done notification                   */
  USBPD_NOTIFY_POWER_SWAP_TO_SRC_DONE      = 21U,  /*!< Power swap to source done notification                 */
  USBPD_NOTIFY_POWER_SWAP_REJ              = 22U,  /*!< Power swap rejected notification                       */
  USBPD_NOTIFY_POWER_SWAP_NOT_SUPPORTED    = 23U,  /*!< Power swap not supported notification                  */
  USBPD_NOTIFY_RESISTOR_ASSERT_RP          = 24U,  /*!< Resistor assert Rp notification                        */
  USBPD_NOTIFY_RESISTOR_ASSERT_RD          = 25U,  /*!< Resistor assert Rd notification                        */
  USBPD_NOTIFY_CABLERESET_REQUESTED        = 26U,  /*!< Cable reset requested notification                     */
  USBPD_NOTIFY_PROTOCOL_ERROR              = 27U,  /*!< Protocol error notification                            */
  USBPD_NOTIFY_VCONN_SWAP_NOT_COMPLETED    = 28U,  /*!< VCONN swap not completed notification                  */
  USBPD_NOTIFY_SVDM_TIMEOUT                = 29U,  /*!< SVDM timeout notification                              */
  USBPD_NOTIFY_HARDRESET_RX                = 30U,  /*!< Hard reset received notification                       */
  USBPD_NOTIFY_HARDRESET_TX                = 31U,  /*!< Hard reset transmitted notification                    */
  USBPD_NOTIFY_STATE_SNK_READY             = 32U,  /*!< State sink ready notification                          */
  USBPD_NOTIFY_STATE_SRC_DISABLED          = 33U,  /*!< State source disabled notification                     */
  USBPD_NOTIFY_DATAROLESWAP_SENT           = 34U,  /*!< Data role swap sent notification                       */
  USBPD_NOTIFY_DATAROLESWAP_RECEIVED       = 35U,  /*!< Data role swap received notification                   */
  USBPD_NOTIFY_DATAROLESWAP_UFP            = 36U,  /*!< Data role swap to UFP notification                     */
  USBPD_NOTIFY_DATAROLESWAP_DFP            = 37U,  /*!< Data role swap to DFP notification                     */
  USBPD_NOTIFY_DATAROLESWAP_WAIT           = 38U,  /*!< Data role swap wait notification                       */
  USBPD_NOTIFY_DATAROLESWAP_REJECTED       = 39U,  /*!< Data role swap rejected notification                   */
  USBPD_NOTIFY_DATAROLESWAP_NOT_SUPPORTED  = 40U,  /*!< Data role swap not supported notification              */
  USBPD_NOTIFY_GOTOMIN_SENT                = 41U,  /*!< Go to minimum sent notification                        */
  USBPD_NOTIFY_GOTOMIN_POWERREADY          = 42U,  /*!< Go to minimum power ready notification                 */
  USBPD_NOTIFY_SNK_GOTOMIN                 = 43U,  /*!< Sink go to minimum notification                        */
  USBPD_NOTIFY_SNK_GOTOMIN_READY           = 44U,  /*!< Sink go to minimum ready notification                  */
  USBPD_NOTIFY_REQUEST_ERROR               = 45U,  /*!< Request error notification                             */
  USBPD_NOTIFY_REQUEST_COMPLETE            = 46U,  /*!< Request complete notification                          */
  USBPD_NOTIFY_REQUEST_CANCELED            = 47U,  /*!< Request canceled notification                          */
  USBPD_NOTIFY_SOFTRESET_SENT              = 48U,  /*!< Soft reset sent notification                           */
  USBPD_NOTIFY_SOFTRESET_ACCEPTED          = 49U,  /*!< Soft reset accepted notification                       */
  USBPD_NOTIFY_SOFTRESET_RECEIVED          = 50U,  /*!< Soft reset received notification                       */
  USBPD_NOTIFY_PING_RECEIVED               = 51U,  /*!< Ping received notification                             */
  USBPD_NOTIFY_REQUEST_ENTER_MODE          = 52U,  /*!< Request enter mode notification                        */
  USBPD_NOTIFY_REQUEST_ENTER_MODE_ACK      = 53U,  /*!< Request enter mode acknowledged notification           */
  USBPD_NOTIFY_REQUEST_ENTER_MODE_NAK      = 54U,  /*!< Request enter mode not acknowledged notification       */
  USBPD_NOTIFY_REQUEST_ENTER_MODE_BUSY     = 55U,  /*!< Request enter mode busy notification                   */
  USBPD_NOTIFY_PD_SPECIFICATION_CHANGE     = 56U,  /*!< PD specification change notification                   */
  USBPD_NOTIFY_POWER_SWAP_SENT             = 57U,  /*!< Power swap sent notification                           */
  USBPD_NOTIFY_POWER_SWAP_ACCEPTED         = 58U,  /*!< Power swap accepted notification                       */
  USBPD_NOTIFY_POWER_SWAP_WAIT             = 59U,  /*!< Power swap wait notification                           */
  USBPD_NOTIFY_POWER_SWAP_RECEIVED         = 60U,  /*!< Power swap received notification                       */
  USBPD_NOTIFY_VCONN_SWAP_RECEIVED         = 61U,  /*!< VCONN swap received notification                       */
  USBPD_NOTIFY_VCONN_SWAP_SENT             = 62U,  /*!< VCONN swap sent notification                           */
  USBPD_NOTIFY_VCONN_SWAP_ACCEPTED         = 63U,  /*!< VCONN swap accepted notification                       */
  USBPD_NOTIFY_VCONN_SWAP_WAIT             = 64U,  /*!< VCONN swap wait notification                           */
  USBPD_NOTIFY_VCONN_SWAP_REJECTED         = 65U,  /*!< VCONN swap rejected notification                       */
  USBPD_NOTIFY_VCONN_SWAP_COMPLETE         = 66U,  /*!< VCONN swap complete notification                       */
  USBPD_NOTIFY_VCONN_SWAP_NOT_SUPPORTED    = 67U,  /*!< VCONN swap not supported notification                  */
  USBPD_NOTIFY_CTRL_MSG_SENT               = 68U,  /*!< Control message sent notification                      */
  USBPD_NOTIFY_DATA_MSG_SENT               = 69U,  /*!< Data message sent notification                         */
  USBPD_NOTIFY_GET_SRC_CAP_EXT_RECEIVED    = 70U,  /*!< Get source capabilities extended received notification */
  USBPD_NOTIFY_SRC_CAP_EXT_RECEIVED        = 71U,  /*!< Source capabilities extended received notification     */
  USBPD_NOTIFY_SRC_CAP_EXT_SENT            = 72U,  /*!< Source capabilities extended sent notification         */
  USBPD_NOTIFY_GET_PPS_STATUS_RECEIVED     = 73U,  /*!< Get PPS status received notification                   */
  USBPD_NOTIFY_GET_PPS_STATUS_SENT         = 74U,  /*!< Get PPS status sent notification                       */
  USBPD_NOTIFY_PPS_STATUS_RECEIVED         = 75U,  /*!< PPS status received notification                       */
  USBPD_NOTIFY_PPS_STATUS_SENT             = 76U,  /*!< PPS status sent notification                           */
  USBPD_NOTIFY_GET_STATUS_RECEIVED         = 77U,  /*!< Get status received notification                       */
  USBPD_NOTIFY_STATUS_RECEIVED             = 78U,  /*!< Status received notification                           */
  USBPD_NOTIFY_STATUS_SENT                 = 79U,  /*!< Status sent notification                               */
  USBPD_NOTIFY_ALERT_RECEIVED              = 80U,  /*!< Alert received notification                            */
  USBPD_NOTIFY_VDM_IDENTIFY_RECEIVED       = 81U,  /*!< VDM identify received notification                     */
  USBPD_NOTIFY_VDM_CABLE_IDENT_RECEIVED    = 82U,  /*!< VDM cable identify received notification               */
  USBPD_NOTIFY_VDM_SVID_RECEIVED           = 83U,  /*!< VDM SVID received notification                         */
  USBPD_NOTIFY_VDM_MODE_RECEIVED           = 84U,  /*!< VDM mode received notification                         */
  USBPD_NOTIFY_REQUEST_EXIT_MODE           = 85U,  /*!< Request exit mode notification                         */
  USBPD_NOTIFY_REQUEST_EXIT_MODE_ACK       = 86U,  /*!< Request exit mode acknowledged notification            */
  USBPD_NOTIFY_REQUEST_EXIT_MODE_NAK       = 87U,  /*!< Request exit mode not acknowledged notification        */
  USBPD_NOTIFY_REQUEST_EXIT_MODE_BUSY      = 88U,  /*!< Request exit mode busy notification                    */
  USBPD_NOTIFY_MSG_NOT_SUPPORTED           = 89U,  /*!< Message not supported notification                     */
  USBPD_NOTIFY_POWER_STATE_CHANGE          = 90U,  /*!< Power state change notification                        */
  USBPD_NOTIFY_REQUEST_DISCARDED           = 91U,  /*!< Request discarded notification                         */
  USBPD_NOTIFY_AMS_INTERRUPTED             = 92U,  /*!< AMS interrupted notification                           */
  USBPD_NOTIFY_ALERT_SENT                  = 93U,  /*!< Alert sent notification                                */
  USBPD_NOTIFY_CABLERESET_TX               = 94U,  /*!< Cable reset transmitted notification                   */
  USBPD_NOTIFY_PE_DISABLED                 = 95U,  /*!< PE disabled notification                               */
  USBPD_NOTIFY_GET_SNK_CAP_EXT_RECEIVED    = 96U,  /*!< Get sink capabilities extended received notification   */
  USBPD_NOTIFY_SNK_CAP_EXT_SENT            = 97U,  /*!< Sink capabilities extended sent notification           */
  USBPD_NOTIFY_SNK_CAP_EXT_RECEIVED        = 98U,  /*!< Sink capabilities extended received notification       */
  USBPD_NOTIFY_DETACH                      = 99U,  /*!< Detach notification                                    */
  USBPD_NOTIFY_CABLERESET_RX               = 100U, /*!< Cable reset received notification                      */
  USBPD_NOTIFY_BIST_SHARED_TEST_MODE_ENTRY = 101U, /*!< BIST shared test mode entry notification               */
  USBPD_NOTIFY_BIST_SHARED_TEST_MODE_EXIT  = 102U, /*!< BIST shared test mode exit notification                */
  USBPD_NOTIFY_STATE_SRC_READY             = 103U, /*!< State source ready notification                        */
  USBPD_NOTIFY_USBSTACK_START              = 104U, /*!< USB stack start notification                           */
  USBPD_NOTIFY_USBSTACK_STOP               = 105U, /*!< USB stack stop notification                            */
  USBPD_NOTIFY_ENTERUSB_INVALID            = 106U, /*!< Enter USB invalid notification                         */
  USBPD_NOTIFY_ENTERUSB_SENT               = 107U, /*!< Enter USB sent notification                            */
  USBPD_NOTIFY_ENTERUSB_ACCEPTED           = 108U, /*!< Enter USB accepted notification                        */
  USBPD_NOTIFY_ENTERUSB_REJECTED           = 109U, /*!< Enter USB rejected notification                        */
  USBPD_NOTIFY_DATARESET_EXECUTE           = 110U, /*!< Data reset execute notification                        */
  USBPD_NOTIFY_DATARESET_RESTORE           = 111U, /*!< Data reset restore notification                        */
  USBPD_NOTIFY_EPRMODE_INVALID             = 112U, /*!< EPR mode invalid notification                          */
  USBPD_NOTIFY_EPRMODE_ACK                 = 113U, /*!< EPR mode acknowledged notification                     */
  USBPD_NOTIFY_EPRMODE_SUCCEEDED           = 114U, /*!< EPR mode succeeded notification                        */
  USBPD_NOTIFY_EPRMODE_FAILED              = 115U, /*!< EPR mode failed notification                           */
  USBPD_NOTIFY_EPRMODE_INIT                = 116U, /*!< EPR mode initialization notification                   */
  USBPD_NOTIFY_EPRMODE_EXIT                = 117U, /*!< EPR mode exit notification                             */
  USBPD_NOTIFY_EPR_SNKCAP_RECEIVED         = 118U, /*!< EPR sink capabilities received notification            */
  USBPD_NOTIFY_EPR_SRCCAP_RECEIVED         = 119U, /*!< EPR source capabilities received notification          */
  USBPD_NOTIFY_TAMETIMEOUT                 = 120U, /*!< TAME timeout notification                              */
  USBPD_NOTIFY_TAMESTOP                    = 121U, /*!< TAME stop notification                                 */
  USBPD_NOTIFY_DEBUG_ACCESSORY_ATTACHED    = 122U, /*!< DAM notification                                       */
  USBPD_NOTIFY_ALL                         = (USBPD_NOTIFY_DEBUG_ACCESSORY_ATTACHED + 1U), /*!< All notifications */
} usbpd_notify_event_t;

/** @defgroup USBPD_CORE_VDM_Exported_Defines USBPD CORE VDM Exported Defines
  * @{
  */

#define MAX_MODES_PER_SVID (6U) /*!< Maximum number of modes per SVID */

/**
  * @brief  USBPD VDM Product Type field in ID Header.
  */
typedef enum
{
  /* ##### SOP #####*/
  /* Product Type (UFP/DFP): */
  USBPD_VDM_PRODUCT_TYPE_HUB           = 1U, /*!< PDUSB Hub (UFP or DFP)         */

  /* Product Type (UFP): */
  USBPD_VDM_PRODUCT_TYPE_NOT_UFP       = 0U, /*!< Not a UFP                      */
  USBPD_VDM_PRODUCT_TYPE_PERIPHERAL    = 2U, /*!< PDUSB Peripheral (UFP)         */
  USBPD_VDM_PRODUCT_TYPE_PSD           = 3U, /*!< PSD, e.g. power bank (UFP)     */

  /* Product Type (DFP): */
  USBPD_VDM_PRODUCT_TYPE_NOT_DFP       = 0U, /*!< Not a DFP                      */
  USBPD_VDM_PRODUCT_TYPE_HOST          = 2U, /*!< PDUSB Host  (DFP)              */
  USBPD_VDM_PRODUCT_TYPE_POWER_BRICK   = 3U, /*!< Power Brick (DFP)              */

  /* ##### SOP1 (Cable Plug/VPD) #####*/
  /* Product Type (Cable Plug): */
  USBPD_VDM_PRODUCT_TYPE_PASSIVE_CABLE = 3U, /*!< Passive Cable (Cable Plug)     */
  USBPD_VDM_PRODUCT_TYPE_ACTIVE_CABLE  = 4U, /*!< Active Cable (Cable Plug)      */
#if defined(USBPDCORE_VPD)
  USBPD_VDM_PRODUCT_TYPE_VPD           = 6U, /*!< VCONN-Powered USB Device (VPD) */
#endif /* USBPDCORE_VPD */

  /* Kept for PD2.0 legacy reasons - DEPRECATED in PD3.0 */
  USBPD_VDM_PRODUCT_TYPE_UNDEFINED     = USBPD_VDM_PRODUCT_TYPE_NOT_UFP, /*!< DEPRECATED - Undefined (UFP) */
  /* Product Type (UFP): */
  USBPD_VDM_PRODUCT_TYPE_AMA           = 5U, /*!< DEPRECATED - Alternate Mode Adapter (AMA) (UFP)          */
  USBPD_VDM_PRODUCT_TYPE_AMC           = 4U, /*!< DEPRECATED - Alternate Mode Controller (AMC) (DFP)       */
} usbpd_vdm_product_type_t;

/**
  * @brief  USBPD VDM Connector Type field in ID Header.
  */
typedef enum
{
  USBPD_VDM_CONNECTOR_TYPE_RESERVED     = 0U, /*!< Reserved, for compatibility with legacy systems */
  USBPD_VDM_CONNECTOR_TYPE_C_RECEPTACLE = 2U, /*!< USB Type-C Receptacle                           */
  USBPD_VDM_CONNECTOR_TYPE_C_PLUG       = 3U, /*!< USB Type-C Plug                                 */
} usbpd_vdm_connector_type_t;

/**
  * @brief  USBPD VDM USB Host or Device Capability field in ID Header.
  */
typedef enum
{
  USBPD_VDM_USB_CAPA_NOTCAPABLE = 0U, /*!< Not USB capable                                     */
  USBPD_VDM_USB_CAPA_CAPABLE    = 1U, /*!< Capable of being enumerated as a USB host or device */
} usbpd_vdm_usb_capa_t;

/**
  * @brief  USBPD VDM Modal operation field in ID Header.
  */
typedef enum
{
  USBPD_VDM_MODAL_OPERATION_NOT_SUPPORTED = 0U, /*!< Product not supports Modal Operation */
  USBPD_VDM_MODAL_OPERATION_SUPPORTED     = 1U, /*!< Product supports Modal Operation     */
} usbpd_vdm_modal_op_support_t;

/**
  * @brief  USBPD VDM Cable to USB field in Active/Passive cable.
  */
typedef enum
{
  USBPD_VDM_CABLE_TO_TYPE_A = 0U, /*!< USB Type-A (PD 2.0 only) */
  USBPD_VDM_CABLE_TO_TYPE_B = 1U, /*!< USB Type-B (PD 2.0 only) */
  USBPD_VDM_CABLE_TO_TYPE_C = 2U, /*!< USB Type-C               */
  USBPD_VDM_CABLE_CAPTIVE   = 3U, /*!< Captive cable            */
} usbpd_vdm_cable_to_type_t;

/**
  * @brief  USBPD VDM Cable latency values in nanoseconds (max) in Active/Passive cable.
  */
typedef enum
{
  USBPD_VDM_CABLE_LATENCY_10NS   = 1U, /*!< <10ns (~1m)                                      */
  USBPD_VDM_CABLE_LATENCY_20NS   = 2U, /*!< 10ns to 20ns (~2m)                               */
  USBPD_VDM_CABLE_LATENCY_30NS   = 3U, /*!< 20ns to 30ns (~3m)                               */
  USBPD_VDM_CABLE_LATENCY_40NS   = 4U, /*!< 30ns to 40ns (~4m)                               */
  USBPD_VDM_CABLE_LATENCY_50NS   = 5U, /*!< 40ns to 50ns (~5m)                               */
  USBPD_VDM_CABLE_LATENCY_60NS   = 6U, /*!< 50ns to 60ns (~6m)                               */
  USBPD_VDM_CABLE_LATENCY_70NS   = 7U, /*!< 60ns to 70ns (~7m)                               */
  USBPD_VDM_CABLE_LATENCY_1000NS = 8U, /*!< >70ns (>~7m) for P2.0 or 1000ns (~100m) for P3.0 */
  USBPD_VDM_CABLE_LATENCY_2000NS = 9U, /*!< 2000ns (~200m)                                   */
  USBPD_VDM_CABLE_LATENCY_3000NS = 10U /*!< 3000ns (~300m)                                   */
} usbpd_vdm_cable_latency_t;

/**
  * @brief  USBPD VDM Cable maximum VBUS voltage.
  */
typedef enum
{
  USBPD_VDM_CABLE_VBUS_MAX_20V = 0U, /*!< VBUS cable max 20V */
  USBPD_VDM_CABLE_VBUS_MAX_30V = 1U, /*!< VBUS cable max 30V */
  USBPD_VDM_CABLE_VBUS_MAX_40V = 2U, /*!< VBUS cable max 40V */
  USBPD_VDM_CABLE_VBUS_MAX_50V = 3U, /*!< VBUS cable max 50V */
} usbpd_vdm_cable_voltage_t;

/**
  * @brief  USBPD VDM Cable Termination Type in Active/Passive cable.
  */
typedef enum
{
  USBPD_VDM_CABLE_TERM_BOTH_PASSIVE_NO_VCONN = 0U, /*!< VCONN not required (PD2.0 only)                 */
  USBPD_VDM_CABLE_TERM_BOTH_PASSIVE_VCONN    = 1U, /*!< VCONN required (PD2.0 only)                     */
  USBPD_VDM_CABLE_TERM_ONE_EACH_VCONN        = 2U, /*!< One end Active, one end passive, VCONN required */
  USBPD_VDM_CABLE_TERM_BOTH_ACTIVE_VCONN     = 3U, /*!< Both ends Active, VCONN required                */
} usbpd_vdm_cable_term_t;

/**
  * @brief  USBPD VDM Cable maximum VBUS Voltage in Active/Passive cable.
  */
typedef enum
{
  VBUS_20V = 0U, /*!< Maximum Cable VBUS Voltage 20V */
  VBUS_30V = 1U, /*!< Maximum Cable VBUS Voltage 30V */
  VBUS_40V = 2U, /*!< Maximum Cable VBUS Voltage 40V */
  VBUS_50V = 3U, /*!< Maximum Cable VBUS Voltage 50V */
} usbpd_vdm_vbus_max_voltage_t;

/**
  * @brief  USBPD VDM Active cable - SBU Support.
  */
typedef enum
{
  USBPD_VDM_ACTIVE_CABLE_SBU_NOT_SUPPORTED = 0U, /*!< SBUs connections not supported */
  USBPD_VDM_ACTIVE_CABLE_SBU_SUPPORTED     = 1U, /*!< SBUs connections supported     */
} usbpd_vdm_active_cable_sbu_support_t;

/**
  * @brief  USBPD VDM Active cable - SBU Type. Valid only if SBU Connection is supported:
  *         Refer to @ref usbpd_vdm_active_cable_sbu_support_t.
  */
typedef enum
{
  USBPD_VDM_ACTIVE_CABLE_SBU_TYPE_PASSIVE = 0U, /*!< SBU type is passive */
  USBPD_VDM_ACTIVE_CABLE_SBU_TYPE_ACTIVE  = 1U, /*!< SBU type is active  */
} usbpd_vdm_active_cable_sbu_type_t;

/**
  * @brief  USBPD VDP - UFP VDO - Device capability.
  */
typedef enum
{
  USBPD_VDM_VDO_UFP_DEVICE_CAPABILITY_USB2P0           = 0U, /*!< [USB 2.0] Device Capable                  */
  USBPD_VDM_VDO_UFP_DEVICE_CAPABILITY_USB2P0_BILLBOARD = 1U, /*!< [USB 2.0] Device Capable (Billboard only) */
  USBPD_VDM_VDO_UFP_DEVICE_CAPABILITY_USB3P2           = 2U, /*!< [USB 3.2] Device Capable                  */
  USBPD_VDM_VDO_UFP_DEVICE_CAPABILITY_USB4             = 3U, /*!< [USB4] Device Capable                     */
} usbpd_vdm_vdo_ufp_device_capa_t;

/**
  * @brief  USBPD VDM - UFP VDO - Alternate modes.
  */
typedef enum
{
  USBPD_VDM_VDO_UFP_ALTERNATE_MODES_TBT3                  = 0U, /*!< Supports [TBT3] Alternate Mode                */
  USBPD_VDM_VDO_UFP_ALTERNATE_MODES_RECONFIG_TYPEC_2P0    = 1U, /*!< Supports Alternate modes that reconfigure the
                                                                     signals on the [USB Type-C 2.0] connector -
                                                                     except for [TBT3].                            */
  USBPD_VDM_VDO_UFP_ALTERNATE_MODES_NO_RECONFIG_TYPEC_2P0 = 2U, /*!< Supports Alternate modes that do not
                                                                     reconfigure the signals on the
                                                                     [USB Type-C 2.0] connector                    */
} usbpd_vdm_vdo_ufp_alternate_modes_t;

/**
  * @brief  USBPD VDM - DFP VDO - Host capability.
  */
typedef enum
{
  HOST_CAPABILITY_USB2P0 = 0U, /*!< [USB 2.0] Host Capable  */
  HOST_CAPABILITY_USB3P2 = 1U, /*!< [USB 3.2] Host Capable  */
  HOST_CAPABILITY_USB4   = 2U, /*!< [USB4] Host Capable     */
} usbpd_vdm_vdo_dfp_host_capa_t;

/**
  * @brief  USBPD VDM Configurability of SS Directionality in Active/Passive cable and AMA VDO (PD2.0 only).
  */
typedef enum
{
  SS_DIR_FIXED        = 0U, /*!< SSTX Directionality Support Fixed        */
  SS_DIR_CONFIGURABLE = 1U, /*!< SSTX Directionality Support Configurable */
} usbpd_vdm_superspeed_directionality_t;

/**
  * @brief  USBPD VDM SVDM commands definition.
  */
typedef enum
{
  USBPD_VDM_SVDM_RESERVEDCOMMAND   = 0x00U, /**< Reserved command          */
  USBPD_VDM_SVDM_DISCOVER_IDENTITY = 0x01U, /**< Discover identity command */
  USBPD_VDM_SVDM_DISCOVER_SVIDS    = 0x02U, /**< Discover SVIDs command    */
  USBPD_VDM_SVDM_DISCOVER_MODES    = 0x03U, /**< Discover modes command    */
  USBPD_VDM_SVDM_ENTER_MODE        = 0x04U, /**< Enter mode command        */
  USBPD_VDM_SVDM_EXIT_MODE         = 0x05U, /**< Exit mode command         */
  USBPD_VDM_SVDM_ATTENTION         = 0x06U, /**< Attention command         */
  USBPD_VDM_SVDM_SPECIFIC_1        = 0x10U, /**< Specific command 1        */
  USBPD_VDM_SVDM_SPECIFIC_2        = 0x11U, /**< Specific command 2        */
  USBPD_VDM_SVDM_SPECIFIC_3        = 0x12U, /**< Specific command 3        */
  USBPD_VDM_SVDM_SPECIFIC_4        = 0x13U, /**< Specific command 4        */
  USBPD_VDM_SVDM_SPECIFIC_5        = 0x14U, /**< Specific command 5        */
  USBPD_VDM_SVDM_SPECIFIC_6        = 0x15U, /**< Specific command 6        */
  USBPD_VDM_SVDM_SPECIFIC_7        = 0x16U, /**< Specific command 7        */
  USBPD_VDM_SVDM_SPECIFIC_8        = 0x17U, /**< Specific command 8        */
  USBPD_VDM_SVDM_SPECIFIC_9        = 0x18U, /**< Specific command 9        */
  USBPD_VDM_SVDM_SPECIFIC_10       = 0x19U, /**< Specific command 10       */
  USBPD_VDM_SVDM_SPECIFIC_11       = 0x1AU, /**< Specific command 11       */
  USBPD_VDM_SVDM_SPECIFIC_12       = 0x1BU, /**< Specific command 12       */
  USBPD_VDM_SVDM_SPECIFIC_13       = 0x1CU, /**< Specific command 13       */
  USBPD_VDM_SVDM_SPECIFIC_14       = 0x1DU, /**< Specific command 14       */
  USBPD_VDM_SVDM_SPECIFIC_15       = 0x1EU, /**< Specific command 15       */
  USBPD_VDM_SVDM_SPECIFIC_16       = 0x1FU, /**< Specific command 16       */
} usbpd_vdm_svdm_command_t;

/**
  * @brief  USBPD VDM VBUS Current Handling Capability in Active/Passive cable VDO.
  */
typedef enum
{
  USBPD_VDM_VBUS_CURRENT_HANDLING_CAPA_DEFAULT = 0U, /*!< USB Type-C Default Current           */
  USBPD_VDM_VBUS_CURRENT_HANDLING_CAPA_3A      = 1U, /*!< VBUS  Current Handling Capability 3A */
  USBPD_VDM_VBUS_CURRENT_HANDLING_CAPA_5A      = 2U, /*!< VBUS  Current Handling Capability 5A */
} usbpd_vdm_vbus_current_handling_capa_t;

/**
  * @brief  USBPD VDM USB Superspeed Signaling Support in Active/Passive cable VDO.
  */
typedef enum
{
  USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB2P0_ONLY      = 0U, /*!< [USB 2.0] only, no SuperSpeed support */
  USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB3P2_GEN1      = 1U, /*!< [USB 3.2] Gen1                        */
  USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB3P2_USB4_GEN2 = 2U, /*!< [USB 3.2]/[USB4] Gen2                 */
  USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB4_GEN3        = 3U, /*!< [USB4] Gen3                           */

  /* Kept for legacy purpose */
  USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB3P1_GEN1   = USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB3P2_GEN1,      /*!< DEPRECATED
                                                                                                           USB3.1 Gen1
                                                                                                           and USB2.0 */
  USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB3P1_GEN1N2 = USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB3P2_USB4_GEN2, /*!< DEPRECATED
                                                                                                           USB3.1 Gen1,
                                                                                                            Gen2 and
                                                                                                            USB2.0    */
} usbpd_vdm_usb_superspeed_support_t;

/**
  * @brief  USBPD VDM Power needed by adapter for full functionality in AMA VDO header.
  */
typedef enum
{
  USBPD_VDM_VCONN_PWR_1W  = 0U, /*!< VCONN  power 1W   */
  USBPD_VDM_VCONN_PWR_1W5 = 1U, /*!< VCONN  power 1.5W */
  USBPD_VDM_VCONN_PWR_2W  = 2U, /*!< VCONN  power 2W   */
  USBPD_VDM_VCONN_PWR_3W  = 3U, /*!< VCONN  power 3W   */
  USBPD_VDM_VCONN_PWR_4W  = 4U, /*!< VCONN  power 4W   */
  USBPD_VDM_VCONN_PWR_5W  = 5U, /*!< VCONN  power 5W   */
  USBPD_VDM_VCONN_PWR_6W  = 6U, /*!< VCONN  power 6W   */
} usbpd_vdm_vconn_pwr_t;

/**
  * @brief  USBPD VDM VCONN requirement of an adapter in AMA VDO header.
  */
typedef enum
{
  USBPD_VDM_VCONN_NOT_REQUIRED = 0U, /*!< VCONN not required */
  USBPD_VDM_VCONN_REQUIRED     = 1U, /*!< VCONN required     */
} usbpd_vdm_vconn_requirement_t;

/**
  * @brief  USBPD VDM VBUS requirement of an adapter in AMA VDO header.
  */
typedef enum
{
  USBPD_VDM_VBUS_NOT_REQUIRED = 0U, /*!< VBUS not required */
  USBPD_VDM_VBUS_REQUIRED     = 1U, /*!< VBUS required     */
} usbpd_vdm_vbus_requirement_t;

#if defined(USBPDCORE_VPD)
/**
  * @brief  USBPD VDM Vconn Powered USB Device VDO - Charge Through Current Support.
  */
typedef enum
{
  USBPD_VDM_VPD_CHARGE_THROUGH_NOT_SUPPORTED = 0U, /*!< The VPD does not support Charge Through */
  USBPD_VDM_VPD_CHARGE_THROUGH_SUPPORTED     = 1U, /*!< The VPD supports Charge Through         */
} usbpd_vdm_vpd_charge_support_t;

/**
  * @brief  USBPD VDM Vconn Powered USB Device VDO - Charge Through Support.
  */
typedef enum
{
  USBPD_VDM_VPD_CHARGE_CURRENT_3A = 0U, /*!< 3A capable */
  USBPD_VDM_VPD_CHARGE_CURRENT_5A = 1U, /*!< 5A capable */
} usbpd_vdm_vpd_charge_current_t;
#endif /* USBPDCORE_VPD */

/**
  * @brief  USBPD VDM Command Type.
  */
typedef enum
{
  USBPD_VDM_COMMAND_TYPE_SVDM_INITIATOR      = 0U, /*!< Initiator command type      */
  USBPD_VDM_COMMAND_TYPE_SVDM_RESPONDER_ACK  = 1U, /*!< Responder ACK command type  */
  USBPD_VDM_COMMAND_TYPE_SVDM_RESPONDER_NAK  = 2U, /*!< Responder NAK command type  */
  USBPD_VDM_COMMAND_TYPE_SVDM_RESPONDER_BUSY = 3U, /*!< Responder BUSY command type */
} usbpd_vdm_command_type_t;


/**
  * @brief  USBPD VDM Applicative Command Type.
  */
typedef enum
{
  USBPD_VDM_USER_COMMAND_TYPE_SVDM_INITIATOR      = USBPD_VDM_COMMAND_TYPE_SVDM_INITIATOR,      /*!< Initiator
                                                                                                     command type   */
  USBPD_VDM_USER_COMMAND_TYPE_SVDM_RESPONDER_ACK  = USBPD_VDM_COMMAND_TYPE_SVDM_RESPONDER_ACK,  /*!< Responder ACK
                                                                                                     command type   */
  USBPD_VDM_USER_COMMAND_TYPE_SVDM_RESPONDER_NAK  = USBPD_VDM_COMMAND_TYPE_SVDM_RESPONDER_NAK,  /*!< Responder NAK
                                                                                                     command type   */
  USBPD_VDM_USER_COMMAND_TYPE_SVDM_RESPONDER_BUSY = USBPD_VDM_COMMAND_TYPE_SVDM_RESPONDER_BUSY, /*!< Responder BUSY
                                                                                                     command type   */
  USBPD_VDM_USER_COMMAND_TYPE_SVDM_CABLE_TIMEOUT  = 4U,      /*!< Indication that cable is PD capable but
                                                                  no answer to VDM Discovery identity               */
  USBPD_VDM_USER_COMMAND_TYPE_SVDM_CABLE_NO_PD_CAPABLE = 5U, /*!< Indication that cable is not PD capable
                                                                  (no goodCRC to VDM Discovery identity             */
} usbpd_vdm_user_command_type_t;

/**
  * @brief  USBPD VDM AMA USB Superspeed Signaling Support in AMA VDO header.
  */
typedef enum
{
  USBPD_VDM_AMA_USB_SS_SUPPORT_USB2P0_ONLY      = 0U, /*!< [USB 2.0] only, no SuperSpeed support */
  USBPD_VDM_AMA_USB_SS_SUPPORT_USB3P2_GEN1      = 1U, /*!< [USB 3.2] Gen1 and USB 2.0            */
  USBPD_VDM_AMA_USB_SS_SUPPORT_USB3P2_GEN1N2    = 2U, /*!< [USB 3.2] Gen1, Gen2 and USB 2.0      */
  USBPD_VDM_AMA_USB_SS_SUPPORT_USB2P0_BILLBOARD = 3U, /*!< [USB 2.0] billboard only              */

  /* Kept for legacy reasons */
  USBPD_VDM_AMA_USB_SS_SUPPORT_USB3P1_GEN1      = USBPD_VDM_USB_SUPERSPEED_SUPPORT_USB3P2_GEN1, /*!< DEPRECATED -
                                                                                                      USB3.1 Gen1 and
                                                                                                      USB2.0          */
  USBPD_VDM_AMA_USB_SS_SUPPORT_USB3P1_GEN1N2    = USBPD_VDM_AMA_USB_SS_SUPPORT_USB3P2_GEN1N2,   /*!< DEPRECATED -
                                                                                                      USB3.1 Gen1,
                                                                                                      Gen2 and USB2.0 */
} usbpd_vdm_ama_usb_superspeed_signaling_support_t;

/**
  * @}
  */

#if defined(USBPDCORE_FWUPD)
/**
  * @brief  USBPD Firmware Update Status Information.
  */
typedef enum
{
  USBPD_FWUPD_STATUS_OK                         = 0x00U,     /*!< Request completed successfully or delayed           */
  USBPD_FWUPD_STATUS_ERR_TARGET                 = 0x01U,     /*!< FW not targeted for this device                     */
  USBPD_FWUPD_STATUS_ERR_FILE                   = 0x02U,     /*!< Fails vendor-specific verification test             */
  USBPD_FWUPD_STATUS_ERR_WRITE                  = 0x03U,     /*!< Unable to write memory                              */
  USBPD_FWUPD_STATUS_ERR_ERASE                  = 0x04U,     /*!< Memory erase function failed                        */
  USBPD_FWUPD_STATUS_ERR_CHECK_ERASED           = 0x05U,     /*!< Memory erase check failed                           */
  USBPD_FWUPD_STATUS_ERR_PROG                   = 0x06U,     /*!< Program memory function failed                      */
  USBPD_FWUPD_STATUS_ERR_VERIFY                 = 0x07U,     /*!< Program memory failed verification                  */
  USBPD_FWUPD_STATUS_ERR_ADDRESS                = 0x08U,     /*!< Received address is out of range                    */
  USBPD_FWUPD_STATUS_ERR_NOTDONE                = 0x09U,     /*!< Received PDFU_DATA Request with a zero length Data
                                                                  Block, but the PDFU Responder expects more data     */
  USBPD_FWUPD_STATUS_ERR_FIRMWARE               = 0x0AU,     /*!< Device's firmware is corrupt.
                                                                  It cannot return to normal operations.              */
  USBPD_FWUPD_STATUS_ERR_POR                    = 0x0DU,     /*!< Unexpected power on reset                           */
  USBPD_FWUPD_STATUS_ERR_UNKNOWN                = 0x0EU,     /*!< Something went wrong                                */
  USBPD_FWUPD_STATUS_ERR_UNEXPECTED_HARD_RESET  = 0x80U,     /*!< Used when firmware update starts after a hard reset
                                                                  (enumeration, etc.) that occurred in the middle
                                                                  of firmware update                                  */
  USBPD_FWUPD_STATUS_ERR_UNEXPECTED_SOFT_RESET  = 0x81U,     /*!< Used when firmware update starts after soft reset
                                                                  (new power contract, etc.)
                                                                  that occurred in the middle of firmware update      */
  USBPD_FWUPD_STATUS_ERR_UNEXPECTED_REQUEST     = 0x82U,     /*!< PDFU Responder received a request that is not
                                                                  appropriate for the current Phase                   */
  USBPD_FWUPD_STATUS_ERR_REJECT_PAUSE           = 0x83U,     /*!< PDFU Responder is unable or unwilling to pause
                                                                  a firmware image transfer                           */
} usbpd_fwupd_status_t;
#endif /* USBPDCORE_FWUPD */

/**
  * @brief  USBPD Data Info types used in PE callbacks (USBPD_PE_GetDataInfo() and USBPD_PE_SetDataInfo()).
  */
typedef enum
{
  USBPD_CORE_DATAINFO_DATATYPE_SRC_PDO         = 0U,  /*!< Handling of port Source PDO (SRC or DRP configuration used
                                                           only in USBPD_PE_GetDataInfo())                            */
  USBPD_CORE_DATAINFO_DATATYPE_SNK_PDO         = 1U,  /*!< Handling of port Sink PDO, requested by get sink capa
                                                           (SNK or DRP configuration used only in
                                                           USBPD_PE_GetDataInfo())                                    */
  USBPD_CORE_DATAINFO_DATATYPE_RDO_POSITION    = 2U,  /*!< Reset the PDO position selected by the sink only (used
                                                           only in USBPD_PE_SetDataInfo())                            */
  USBPD_CORE_DATAINFO_DATATYPE_REQ_VOLTAGE     = 3U,  /*!< Get voltage value requested for BIST tests, expect 5V
                                                           (used only in USBPD_PE_GetDataInfo())                      */
  USBPD_CORE_DATAINFO_DATATYPE_RCV_SRC_PDO     = 4U,  /*!< Storage of Received Source PDO values (used only in
                                                           USBPD_PE_SetDataInfo())                                    */
  USBPD_CORE_DATAINFO_DATATYPE_RCV_SNK_PDO     = 5U,  /*!< Storage of Received Sink PDO values (used only in
                                                           USBPD_PE_SetDataInfo())                                    */
  USBPD_CORE_DATAINFO_DATATYPE_RCV_REQ_PDO     = 6U,  /*!< Storage of Received Sink Request PDO value (SRC or DRP
                                                           configuration used in USBPD_PE_SetDataInfo())              */
  USBPD_CORE_DATAINFO_DATATYPE_REQUEST_DO      = 7U,  /*!< Not used - kept for legacy reason                          */
  USBPD_CORE_DATAINFO_EXTENDED_CAPA            = 8U,  /*!< Source Extended capability message content (used in
                                                           USBPD_PE_GetDataInfo() and USBPD_PE_SetDataInfo())         */
  USBPD_CORE_DATAINFO_INFO_STATUS              = 9U,  /*!< Information status message content (used in
                                                           USBPD_PE_GetDataInfo() and USBPD_PE_SetDataInfo())         */
  USBPD_CORE_DATAINFO_PPS_STATUS               = 10U, /*!< PPS Status message content (used in USBPD_PE_GetDataInfo()
                                                          and USBPD_PE_SetDataInfo())                                 */
  USBPD_CORE_DATAINFO_ALERT                    = 11U, /*!< Storing of received Alert message content (used only in
                                                           USBPD_PE_SetDataInfo())                                    */
  USBPD_CORE_DATAINFO_GET_MANUFACTURER_INFO    = 12U, /*!< Storing of received Get Manufacturer info message content
                                                           (used only in USBPD_PE_SetDataInfo())                      */
  USBPD_CORE_DATAINFO_MANUFACTURER_INFO        = 13U, /*!< Retrieve of Manufacturer info message content (used in
                                                           USBPD_PE_GetDataInfo() and USBPD_PE_SetDataInfo())         */
  USBPD_CORE_DATAINFO_GET_BATTERY_STATUS       = 14U, /*!< Storing of received Get Battery status message content
                                                           (used only in USBPD_PE_SetDataInfo())                      */
  USBPD_CORE_DATAINFO_BATTERY_STATUS           = 15U, /*!< Retrieve of Battery status message content (used in
                                                           USBPD_PE_GetDataInfo() and USBPD_PE_SetDataInfo())         */
  USBPD_CORE_DATAINFO_GET_BATTERY_CAPABILITY   = 16U, /*!< Storing of received Get Battery capability message content
                                                           (used only in USBPD_PE_SetDataInfo())                      */
  USBPD_CORE_DATAINFO_BATTERY_CAPABILITY       = 17U, /*!< Retrieve of Battery capability message content (used in
                                                           USBPD_PE_GetDataInfo() and USBPD_PE_SetDataInfo())         */
  USBPD_CORE_DATAINFO_UNSTRUCTURED_VDM         = 18U, /*!< Not used - Kept forlegacy reason                           */
#if defined(USBPDCORE_SNK_CAPA_EXT)
  USBPD_CORE_DATAINFO_SNK_EXTENDED_CAPA        = 19U, /*!< Storing and retrieve of Sink Extended capability message
                                                           content (used in USBPD_PE_GetDataInfo() and
                                                           USBPD_PE_SetDataInfo())                                    */
#endif /* USBPDCORE_SNK_CAPA_EXT */
#if defined(USBPDCORE_PECABLE)
  USBPD_CORE_DATAINFO_CABLE_GET_IDENTITY       = 20U, /*!< Get the cable identity information (used in
                                                           USBPD_PE_GetDataInfo())                                    */
  USBPD_CORE_DATAINFO_CABLE_GET_STATUS         = 21U, /*!< Get the cable status information (used in
                                                           USBPD_PE_GetDataInfo())                                    */
#endif /* USBPDCORE_PECABLE */
#if defined(USBPDCORE_USBDATA)
  USBPD_CORE_DATAINFO_DATATYPE_ENTER_USB       = 22U, /*!< Get info to send an ENTER_USB message                      */
#endif /* USBPDCORE_USBDATA */
  USBPD_CORE_DATAINFO_REVISION                 = 23U, /*!< Get/set revision info                                      */
  USBPD_CORE_DATAINFO_SOURCE_INFO              = 24U, /*!< Get/set source info                                        */
  USBPD_CORE_DATAINFO_DATATYPE_EPR_MODE        = 25U, /*!< Set EPR mode info                                          */
#if defined(USBPDCORE_EPR)
  USBPD_CORE_DATAINFO_DATATYPE_SRC_PDO_EPR     = 26U, /*!< Get the SRC EPR PDO                                        */
  USBPD_CORE_DATAINFO_DATATYPE_SNK_PDO_EPR     = 27U, /*!< Get the SNK EPR PDO                                        */
  USBPD_CORE_DATAINFO_DATATYPE_RCV_SRC_PDO_EPR = 28U, /*!< Get the received SRC EPR PDO                               */
  USBPD_CORE_DATAINFO_DATATYPE_RCV_SNK_PDO_EPR = 29U, /*!< Get the received SNK EPR PDO                               */
  USBPD_CORE_DATAINFO_DATATYPE_RCV_REQ_COPYPDO = 30U, /*!< Get the copy of the PDO received in EPR_REQUEST message    */
  USBPD_CORE_DATAINFO_DATATYPE_REQ_COPYPDO     = 31U, /*!< Get the copy of the PDO to be sent in EPR_REQUEST message  */
  USBPD_CORE_DATAINFO_DATATYPE_SNK_PDP_EPR     = 32U, /*!< Get the EPR Sink Operational PDP                           */
#endif /* USBPDCORE_EPR */
} usbpd_core_datainfo_type_t;

/**
  * @brief  USBPD Action type used in PE callback (USBPD_PE_RequestDPMWhatToDo()).
  */
typedef enum
{
  USBPD_CORE_ACTION_TYPE_REPLY_ENTER_USB  = 1U, /*!< Get DPM reply to a ENTER_USB message                     */
  USBPD_CORE_ACTION_TYPE_REPLY_DATA_RESET = 2U, /*!< Get DPM reply to a DATA_RESET message                    */
  USBPD_CORE_ACTION_TYPE_REPLY_ENTER_MODE = 3U, /*!< Get DPM reply to a ENTER_MODE message 0 if no error else
                                                     an error code                                            */
  USBPD_CORE_ACTION_TYPE_CHECK_PDO        = 4U, /*!< Get DPM reply on the PDO validity USBPD_OK else generate
                                                     a hardreset                                              */
} usbpd_core_action_type_t;

#if defined(USBPDCORE_EPR)
/**
  * @brief  USBPD EPR Mode Action definition.
  */
typedef enum
{
  USBPD_EPR_MODE_ACTION_ENTER           = 1U, /*!< EPR Mode Enter (Sink only)              */
  USBPD_EPR_MODE_ACTION_ENTER_ACK       = 2U, /*!< EPR Mode Enter Acknowledged Source only */
  USBPD_EPR_MODE_ACTION_ENTER_SUCCEEDED = 3U, /*!< EPR Mode Enter Succeeded Source only    */
  USBPD_EPR_MODE_ACTION_ENTER_FAILED    = 4U, /*!< EPR Mode Enter Failed Source only       */
  USBPD_EPR_MODE_ACTION_ENTER_EXIT      = 5U, /*!< EPR Mode Exit Sink or Source            */
} usbpd_epr_mode_action_t;

/**
  * @brief  USBPD EPR Mode Data definition.
  */
typedef enum
{
  USBPD_EPR_MODE_DATA_UNKNOWN_CAUSE  = 0U, /*!< EPR Mode Unknown cause                                */
  USBPD_EPR_MODE_DATA_CABLE_NOT_EPR  = 1U, /*!< EPR Mode Cable not EPR capable                        */
  USBPD_EPR_MODE_DATA_SRC_NOT_VCONN  = 2U, /*!< EPR Mode Source failed to become VCONN  source        */
  USBPD_EPR_MODE_DATA_RDO_NOT_EPR    = 3U, /*!< EPR Mode EPR Mode Capable bit not set in RDO          */
  USBPD_EPR_MODE_DATA_SRC_UNABLE_NOW = 4U, /*!< EPR Mode Source unable to enter EPR Mode at this time */
  USBPD_EPR_MODE_DATA_PDO_NOT_EPR    = 5U, /*!< EPR Mode EPR Mode Capable bit not set in PDO          */
} usbpd_epr_mode_data_t;
#endif /* USBPDCORE_EPR */

/**
  * @brief  USBPD Debug Accessory Modes defines.
  */
typedef enum
{
  USBPD_DAM_NOT_CONNECTED  = 0U, /*!< DAM not connected                      */
  USBPD_DAM_TS_SINK        = 1U, /*!< DAM Target System as sink              */
  USBPD_DAM_DTS_SINK       = 2U, /*!< DAM Debug and Test System as sink      */
  USBPD_DAM_TS_SOURCE      = 3U, /*!< DAM Target System as source            */
  USBPD_DAM_DTS_SOURCE     = 4U, /*!< DAM Debug and Testt System as source   */
  USBPD_DAM_TS_DRP         = 5U, /*!< DAM Target System as dual role         */
  USBPD_DAM_DTS_DRP        = 6U, /*!< DAM Debug and Test System as dual role */
} usbpd_dam_mode_t;

/**
  * @}
  */

/* Exported typedef ----------------------------------------------------------*/

/** @defgroup USBPD_CORE_DEF_Exported_Types USBPD CORE DEF Exported Types
  * @{
  */

#if defined(USBPDCORE_PPS)
/**
  * @brief  USBPD Programmable Power Supply APDO Structure definition (same for SRC and SNK).
  */
typedef struct
{
  uint32_t current_max_50ma_units  : 7U; /*!< Maximum Current in 50mA increments       */
  uint32_t reserved1               : 1U; /*!< Reserved  - Must be set to zero          */
  uint32_t voltage_min_100mv_units : 8U; /*!< Minimum Voltage in 100mV increments      */
  uint32_t reserved2               : 1U; /*!< Reserved  - Must be set to zero          */
  uint32_t voltage_max_100mv_units : 8U; /*!< Maximum Voltage in 100mV increments      */
  uint32_t reserved3               : 2U; /*!< Reserved  - Must be set to zero          */
  uint32_t pps_pwr_limited         : 1U; /*!< PPS Power Limited Bit                    */
  uint32_t programmable_pwr_supply : 2U; /*!< 00b - Programmable Power Supply          */
  uint32_t pps_apdo                : 2U; /*!< 11b - Augmented Power Data Object (APDO) */
} usbpd_prog_pwr_supply_apdo_t;
#endif /* USBPDCORE_PPS */

#if defined(USBPDCORE_SPR_AVS)
/**
  * @brief  USB PD SPR Adjustable Voltage Supply APDO Structure definition for SRC
  *
  */
typedef struct
{
  uint32_t current_max_20v_10mA_units : 10u; /*!< Maximum Current 20v in 10mA increments   */
  uint32_t current_max_15v_10mA_units : 10u; /*!< Maximum Current 15v in 10mA increments   */
  uint32_t Reserved1                  : 6u;  /*!< Reserved  - Must be set to zero         */
  uint32_t peak_current               : 2u;  /*!< Peak current                             */
  uint32_t spr_adjustable_voltage     : 2u;  /*!< 10b - SPR Adjustable Voltage Supply      */
  uint32_t avs_apdo                   : 2u;  /*!< 11b - Augmented Power Data Object (APDO) */
} usbpd_src_adjust_volt_supply_apdo_t;

/**
  * @brief  USB PD SPR Adjustable Voltage Supply APDO Structure definition for SNK
  *
  */
typedef struct
{
  uint32_t current_max_20v_10mA_units : 10u; /*!< Maximum Current 20v in 10mA increments   */
  uint32_t current_max_15v_10mA_units : 10u; /*!< Maximum Current 15v in 10mA increments   */
  uint32_t Reserved1                  : 8u;  /*!< Reserved  - Must be set to zero         */
  uint32_t spr_adjustable_voltage     : 2u;  /*!< 10b - SPR Adjustable Voltage Supply      */
  uint32_t avs_apdo                   : 2u;  /*!< 11b - Augmented Power Data Object (APDO) */
} usbpd_snk_adjust_volt_supply_apdo_t;
#endif /* USBPDCORE_SPR_AVS */

#if defined(USBPDCORE_EPR)
/**
  * @brief  USBPD Programmable Adjust Voltage Supply PDO Structure definition for SRC.
  */
typedef struct
{
  uint32_t pdp_1w_units            : 8U; /*!< PDP in 1W increments                                       */
  uint32_t voltage_min_100mv_units : 8U; /*!< Minimum Voltage in 100mV increments                        */
  uint32_t reserved1               : 1U; /*!< Reserved  - Must be set to zero                            */
  uint32_t voltage_max_100mv_units : 9U; /*!< Maximum Voltage in 100mV increments                        */
  uint32_t peak_current            : 2U; /*!< Peak current                                               */
  uint32_t epr_adjustable_voltage  : 2U; /*!< 01b - EPR Adjustable Voltage Supply                        */
  uint32_t avspdo                  : 2U; /*!< 11b - Adjustable Voltage Supply Power Data Object (AVSPDO) */
} usbpd_src_prog_pwr_supply_avspdo_t;

/**
  * @brief  USBPD Programmable Adjust Voltage Supply PDO Structure definition for SNK.
  */
typedef struct
{
  uint32_t pdp_1w_units            : 8U; /*!< PDP in 1W increments                                       */
  uint32_t voltage_min_100mv_units : 8U; /*!< Minimum Voltage in 100mV increments                        */
  uint32_t reserved1               : 1U; /*!< Reserved  - Must be set to zero                            */
  uint32_t voltage_max_100mv_units : 9U; /*!< Maximum Voltage in 100mV increments                        */
  uint32_t reserved2               : 2U; /*!< Reserved  - Must be set to zero                            */
  uint32_t epr_adjustable_voltage  : 2U; /*!< 01b - EPR Adjustable Voltage Supply                        */
  uint32_t avspdo                  : 2U; /*!< 11b - Adjustable Voltage Supply Power Data Object (AVSPDO) */
} usbpd_snk_prog_pwr_supply_avspdo_t;
#endif /* USBPDCORE_EPR */

/**
  * @brief  USBPD Source Fixed Supply Power Data Object structure definition.
  */
typedef struct
{
  uint32_t                             current_max_10ma_units     : 10U; /*!< Maximum Current in 10mA units         */
  uint32_t                             voltage_50mv_units         : 10U; /*!< Voltage in 50mV units                 */
  uint32_t                             peak_current               : 2U;  /*!< Peak Current Capability               */
  uint32_t                             reserved1                  : 1U;  /*!< Reserved  - Must be set to zero       */
  uint32_t                             epr_capable                : 1U;  /*!< Extended Power Range Capable          */
  uint32_t                             unchunked_extended_message : 1U;  /*!< Unchunked Extended Message Capability */
  uint32_t                             data_role_swap             : 1U;  /*!< Data Role Swap capability             */
  uint32_t                             usb_com_capable            : 1U;  /*!< USB Communication Capable             */
  uint32_t                             externally_powered         : 1U;  /*!< Externally Powered                    */
  uint32_t                             usb_suspend_supported      : 1U;  /*!< USB Suspend Supported                 */
  uint32_t                             dual_role_power            : 1U;  /*!< Dual Role Power                       */
  uint32_t                             fixed_supply               : 2U;  /*!< Fixed Supply PDO                      */
} usbpd_src_fixed_supply_pdo_t;

/**
  * @brief  USBPD Source Variable Supply Power Data Object structure definition.
  */
typedef struct
{
  uint32_t current_max_10ma_units : 10U; /*!< Maximum Current in 10mA units     */
  uint32_t voltage_min_50mv_units : 10U; /*!< Minimum Voltage in 50mV units     */
  uint32_t voltage_max_50mv_units : 10U; /*!< Maximum Voltage in 50mV units     */
  uint32_t variable_supply        : 2U;  /*!< Variable Supply (non-Battery) PDO */
} usbpd_src_variable_supply_pdo_t;

/**
  * @brief  USBPD Source Battery Supply Power Data Object structure definition.
  */
typedef struct
{
  uint32_t allowable_pwr_max_250mw_units : 10U; /*!< Maximum Allowable Power in 250mW units */
  uint32_t voltage_min_50mv_units        : 10U; /*!< Minimum Voltage in 50mV units          */
  uint32_t voltage_max_50mv_units        : 10U; /*!< Maximum Voltage in 50mV units          */
  uint32_t battery                       : 2U;  /*!< Battery Supply PDO                     */
} usbpd_src_battery_supply_pdo_t;

/**
  * @brief  USBPD Sink Fixed Supply Power Data Object structure definition.
  */
typedef struct
{
  uint32_t operational_current_10ma_units  : 10U; /*!< Operational Current in 10mA units */
  uint32_t voltage_50mv_units              : 10U; /*!< Voltage in 50mV units             */
  uint32_t fast_role_swap_required_current : 2U;  /*!< Fast Role Swap Required Current   */
  uint32_t reserved1                       : 3U;  /*!< Reserved  - Must be set to zero   */
  uint32_t data_role_swap                  : 1U;  /*!< Data Role Swap capability         */
  uint32_t usb_com_capable                 : 1U;  /*!< USB Communication Capable         */
  uint32_t externally_powered              : 1U;  /*!< Externally Powered                */
  uint32_t higher_capability               : 1U;  /*!< Higher Capability                 */
  uint32_t dual_role_power                 : 1U;  /*!< Dual Role Power                   */
  uint32_t fixed_supply                    : 2U;  /*!< Fixed Supply PDO                  */
} usbpd_snk_fixed_supply_pdo_t;

/**
  * @brief  USBPD Sink Variable Supply Power Data Object structure definition.
  */
typedef struct
{
  uint32_t operational_current_10ma_units : 10U; /*!< Operational Current in 10mA units */
  uint32_t voltage_min_50mv_units         : 10U; /*!< Minimum Voltage in 50mV units     */
  uint32_t voltage_max_50mv_units         : 10U; /*!< Maximum Voltage in 50mV units     */
  uint32_t variable_supply                : 2U;  /*!< Variable Supply (non-Battery) PDO */
} usbpd_snk_variable_supply_pdo_t;

/**
  * @brief  USBPD Sink Battery Supply Power Data Object structure definition.
  */
typedef struct
{
  uint32_t operational_pwr_250mw_units : 10U; /*!< Operational Power in 250mW units */
  uint32_t voltage_min_50mv_units      : 10U; /*!< Minimum Voltage in 50mV units    */
  uint32_t voltage_max_50mv_units      : 10U; /*!< Maximum Voltage in 50mV units    */
  uint32_t battery                     : 2U;  /*!< Battery Supply PDO               */
} usbpd_snk_battery_supply_pdo_t;

/**
  * @brief  USBPD Sink Generic Power Data Object structure definition.
  */
typedef struct
{
  uint32_t bits_0_10             : 10U; /*!< Specific Power Capabilities are described by the (A)PDOs in
                                             the following sections.                                     */
  uint32_t voltage_50mv_units    : 10U; /*!< Maximum Voltage in 50mV units valid for all PDO (not APDO)  */
  uint32_t bits_20_29            : 10U; /*!< Specific Power Capabilities are described by the (A)PDOs in
                                             the following sections.                                     */
  uint32_t             pdo_type : 2U;  /*!< (A) Power Data Object                                       */
} usbpd_generic_pdo_t;

/**
  * @brief  USBPD Power Data Object structure definition.
  */
typedef union usbpd_pdo_u
{
  uint32_t d32;
  usbpd_generic_pdo_t                pdo_generic;      /*!< Generic Power Data Object Structure            */
  usbpd_src_fixed_supply_pdo_t       pdo_src_fixed;    /*!< Fixed Supply PDO - Source                      */
  usbpd_src_variable_supply_pdo_t    pdo_src_variable; /*!< Variable Supply (non-Battery) PDO - Source     */
  usbpd_src_battery_supply_pdo_t     pdo_src_battery;  /*!< Battery Supply PDO - Source                    */
  usbpd_snk_fixed_supply_pdo_t       pdo_snk_fixed;    /*!< Fixed Supply PDO - Sink                        */
  usbpd_snk_variable_supply_pdo_t    pdo_snk_variable; /*!< Variable Supply (non-Battery) PDO - Sink       */
  usbpd_snk_battery_supply_pdo_t     pdo_snk_battery;  /*!< Battery Supply PDO - Sink                      */
#if defined(USBPDCORE_PPS)
  usbpd_prog_pwr_supply_apdo_t       pdo_src_snk_apdo; /*!< Programmable Power Supply APDO - Source / Sink */
#endif /* USBPDCORE_PPS */
#if defined(USBPDCORE_SPR_AVS)
  usbpd_src_adjust_volt_supply_apdo_t pdo_src_spr_avs; /*!< SPR Adjustable Voltage Supply APDO - Source    */
  usbpd_snk_adjust_volt_supply_apdo_t pdo_snk_spr_avs; /*!< SPR Adjustable Voltage Supply APDO - Sink      */
#endif /* USBPDCORE_SPR_AVS */
#if defined(USBPDCORE_EPR)
  usbpd_src_prog_pwr_supply_avspdo_t pdo_src_avs;      /*!< Adjustable Voltage Supply AVSPDO - Source      */
  usbpd_snk_prog_pwr_supply_avspdo_t pdo_snk_avs;      /*!< Adjustable Voltage Supply AVSPDO - Sink        */
#endif /* USBPDCORE_EPR */
} usbpd_pdo_t;

/**
  * @brief  USBPD Sink Fixed and Variable Request Data Object structure definition.
  */
typedef struct
{
  uint32_t operating_current_max_10ma_units : 10U; /*!< Maximum Operating Current in 10mA units                  */
  uint32_t operating_current_10ma_units     : 10U; /*!< Operating Current in 10mA units                          */
  uint32_t reserved1                        : 2U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t epr_capable                      : 1U;  /*!< EPR Mode Capable                                         */
  uint32_t unchunked_extended_message       : 1U;  /*!< Unchunked messages support                               */
  uint32_t no_usb_suspend                   : 1U;  /*!< No USB Suspend                                           */
  uint32_t usb_com_capable                  : 1U;  /*!< USB Communications Capable                               */
  uint32_t capability_mismatch              : 1U;  /*!< Capability Mismatch                                      */
  uint32_t reserved2                        : 1U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t object_position                  : 4U;  /*!< Object position (0000b is Reserved and Must Not be used) */
} usbpd_snk_rdo_fixed_variable_t;

/**
  * @brief  USBPD Sink Battery Request Data Object structure definition.
  */
typedef struct
{
  uint32_t operating_pwr_max_250mw_units : 10U; /*!< Maximum Operating Power in 250mW units                   */
  uint32_t operating_pwr_250mw_units     : 10U; /*!< Operating Power in 250mW units                           */
  uint32_t reserved1                     : 2U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t epr_capable                   : 1U;  /*!< EPR Mode Capable                                         */
  uint32_t unchunked_extended_message    : 1U;  /*!< Unchunked messages support                               */
  uint32_t no_usb_suspend                : 1U;  /*!< No USB Suspend                                           */
  uint32_t usb_com_capable               : 1U;  /*!< USB Communications Capable                               */
  uint32_t capability_mismatch           : 1U;  /*!< Capability Mismatch                                      */
  uint32_t reserved2                     : 1U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t object_position               : 4U;  /*!< Object position (0000b is Reserved and Must Not be used) */
} usbpd_snk_rdo_battery_t;

/**
  * @brief  USBPD Sink Programmable Request Data Object structure definition.
  */
typedef struct
{
  uint32_t operating_current_50ma_units : 7U;  /*!< Operating Current 50mA units                             */
  uint32_t reserved1                    : 2U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t output_voltage_20mv_units    : 12U; /*!< Output Voltage in 20mV units                             */
  uint32_t reserved2                    : 1U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t epr_capable                  : 1U;  /*!< EPR Mode Capable                                         */
  uint32_t unchunked_extended_message   : 1U;  /*!< Unchunked messages support                               */
  uint32_t no_usb_suspend               : 1U;  /*!< No USB Suspend                                           */
  uint32_t usb_com_capable              : 1U;  /*!< USB Communications Capable                               */
  uint32_t capability_mismatch          : 1U;  /*!< Capability Mismatch                                      */
  uint32_t reserved3                    : 1U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t object_position              : 4U;  /*!< Object position (0000b is Reserved and Must Not be used) */
} usbpd_snk_rdo_programmable_t;

/**
  * @brief  USBPD Sink AVS Request Data Object structure definition.
  */
typedef struct
{
  uint32_t operating_current_50ma_units : 7U;  /*!< Operating Current 50mA units                             */
  uint32_t reserved1                    : 2U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t output_voltage_25mv_units    : 12U; /*!< Output Voltage in 25mV units                             */
  uint32_t reserved2                    : 1U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t epr_capable                  : 1U;  /*!< EPR Mode Capable                                         */
  uint32_t unchunked_extended_message   : 1U;  /*!< Unchunked messages support                               */
  uint32_t no_usb_suspend               : 1U;  /*!< No USB Suspend                                           */
  uint32_t usb_com_capable              : 1U;  /*!< USB Communications Capable                               */
  uint32_t capability_mismatch          : 1U;  /*!< Capability Mismatch                                      */
  uint32_t reserved3                    : 1U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t object_position              : 4U;  /*!< Object position (0000b is Reserved and Must Not be used) */
} usbpd_snk_rdo_avs_t;

/**
  * @brief  USBPD Sink Generic Request Data Object structure definition.
  */
typedef struct
{
  uint32_t reserved1                  : 22U; /*!< Reserved  - Must be set to zero                          */
  uint32_t epr_capable                : 1U;  /*!< EPR Mode Capable                                         */
  uint32_t unchunked_extended_message : 1U;  /*!< Unchunked messages support                               */
  uint32_t no_usb_suspend             : 1U;  /*!< No USB Suspend                                           */
  uint32_t usb_com_capable            : 1U;  /*!< USB Communications Capable                               */
  uint32_t capability_mismatch        : 1U;  /*!< Capability Mismatch                                      */
  uint32_t reserved2                  : 1U;  /*!< Reserved  - Must be set to zero                          */
  uint32_t object_position            : 4U;  /*!< Object position (0000b is Reserved and Must Not be used) */
} usbpd_snk_generic_rdo_t;

/**
  * @brief  USBPD Sink Request Data Object structure definition.
  */
typedef union usbpd_rdo_snk_u
{
  uint32_t d32;
  usbpd_snk_generic_rdo_t        rdo_generic;        /*!<  Generic Request Data Object Structure           */
  usbpd_snk_rdo_fixed_variable_t rdo_fixed_variable; /*!< Fixed and Variable Request Data Object Structure */
  usbpd_snk_rdo_battery_t        rdo_battery;        /*!< Battery Request Data Object Structure            */
  usbpd_snk_rdo_programmable_t   rdo_programmable;   /*!< Programmable Request Data Object Structure       */
  usbpd_snk_rdo_avs_t            rdo_avs;            /*!< AVS Request Data Object Structure                */
} usbpd_rdo_snk_t;

#if defined(USBPDCORE_PPS)
/**
  * @brief  USBPD Port APDO structure definition.
  */
typedef struct
{
  uint32_t *p_apdo_list; /*!< Pointer on Augmented Power Data Objects list, defining port capabilities  */
  uint8_t  apdo_count;   /*!< Number of Augmented Power Data Objects defined in p_apdo_list
                              This parameter must not exceed the value defined by @ref USBPD_MAX_NB_PDO */
} usbpd_port_apdo_t;
#endif /* USBPDCORE_PPS */

/**
  * @brief  USBPD BIST Data Object structure definition.
  */
typedef union usbpd_bist_data_object_u
{
  uint32_t d32;                        /*!< BIST Data Object in 32-bit format */
  struct
  {
    uint32_t bist_error_counter : 16U; /*!< BIST Error Counter                */
    uint32_t reserved1          : 12U; /*!< Reserved bits 16 to 27            */
    uint32_t bist_mode          : 4U;  /*!< BIST Mode                         */
  } b;
} usbpd_bist_data_object_t;

/**
  * @brief  Sink requested power profile structure definition.
  */
typedef struct
{
  uint32_t operating_current_max_1ma_units;  /*!< Sink board Max operating current in mA units */
  uint32_t operating_voltage_1mv_units;      /*!< Sink board operating voltage in mV units     */
  uint32_t operating_voltage_max_1mv_units;  /*!< Sink board Max operating voltage in mV units */
  uint32_t operating_voltage_min_1mv_units;  /*!< Sink board Min operating voltage in mV units */
  uint32_t operating_pwr_1mw_units;          /*!< Sink board operating power in mW units       */
  uint32_t operating_pwr_max_1mw_units;      /*!< Sink board Max operating power in mW units   */
} usbpd_snk_pwr_request_t;

/** @defgroup USBPD_CORE_VDM_Exported_Structures USBPD CORE VDM Exported Structures
  * @{
  */

/**
  * @brief  USBPD VDM Product VDO structure definition.
  */
typedef union usbpd_vdm_product_vdo_u
{
  uint32_t d32;                     /*!< Product VDO in 32-bit format */
  struct
  {
    uint32_t bcd_device     : 16U;  /*!< Device version               */
    uint32_t usb_product_id : 16U;  /*!< USB Product ID               */
  } b;
} usbpd_vdm_product_vdo_t;

/**
  * @brief  USBPD VDM ECDB - Extended Control Data Block - structure definition.
  */
typedef union usbpd_vdm_ecdb_u
{
  uint16_t d16;   /*!< Extended Control Data Block in 16-bit format */
  struct
  {
    uint8_t type; /*!< Extended Control Data Block type             */
    uint8_t data; /*!< Extended Control Data Block data             */
  } b8;
} usbpd_vdm_ecdb_t;

/**
  * @brief  USB VDM SVDM ID header VDO structure definition.
  */
typedef union usbpd_vdm_id_header_vdo_u
{
  uint32_t d32;
  struct
  {
    uint32_t                     vid                            : 16U; /*!< SVDM Header's SVDM Version       */
    uint32_t                     reserved1                      : 10U; /*!< Reserved                         */
    uint32_t                     modal_operation                : 1U;  /*!< Modal Operation Supported        */
    uint32_t                     product_type_ufp_or_cable_plug : 3U;  /*!< Product Type (UFP or Cable Plug) */
    uint32_t                     usb_device_capability          : 1U;  /*!< USB Com capable as a USB Device  */
    uint32_t                     usb_host_capability            : 1U;  /*!< USB Com capable as USB Host      */
  } b20;
  struct
  {
    uint32_t                     vid                            : 16U; /*!< SVDM Header's SVDM Version       */
    uint32_t                     reserved1                      : 5U;  /*!< Reserved                         */
    uint32_t                     connector_type                 : 2U;  /*!< Connector Type                   */
    uint32_t                     product_type_dfp               : 3U;  /*!< Product Type (DFP)               */
    uint32_t                     modal_operation                : 1U;  /*!< Modal Operation Supported        */
    uint32_t                     product_type_ufp_or_cable_plug : 3U;  /*!< Product Type (UFP or Cable Plug) */
    uint32_t                     usb_device_capability          : 1U;  /*!< USB Com capable as a USB Device  */
    uint32_t                     usb_host_capability            : 1U;  /*!< USB Com capable as USB Host      */
  } b30;
} usbpd_vdm_id_header_vdo_t;

/**
  * @brief  USBPD VDM cable VDO structure definition (1/2).
  */
typedef union usbpd_vdm_cable_vdo_field1_u
{
  struct /* PD 2.0*/
  {
    uint32_t                               ss_rx2_dir_support : 1U; /*!< SSRX2 Directionality Support (PD2.0)     */
    uint32_t                               ss_rx1_dir_support : 1U; /*!< SSRX1 Directionality Support (PD2.0)     */
    uint32_t                               ss_tx2_dir_support : 1U; /*!< SSTX2 Directionality Support (PD2.0)     */
    uint32_t                               ss_tx1_dir_support : 1U; /*!< SSTX1 Directionality Support (PD2.0)     */
  } pd_v20;
  struct /* PD 3.0*/
  {
    uint8_t                               reserved1          : 2U;  /*!< Reserved                                 */
    uint32_t                               vbus_voltage_max   : 2U;  /*!< Maximum Cable VBUS Voltage               */
  } pd_v30;
} usbpd_vdm_cable_vdo_field1_t;

/**
  * @brief  USBPD VDM Attention Info object structure definition.
  */
typedef struct
{
  uint32_t                 vdo;        /*!< VDO value     */
  uint16_t                 svid;       /*!< SVID value    */
  usbpd_vdm_svdm_command_t command;    /*!< Command value */
  uint8_t                  mode_index; /*!< Mode index    */
} usbpd_attention_info_t;

/**
  * @brief  USBPD VDM Cable VDO structure definition (2/2).
  */
typedef union usbpd_vdm_cable_vdo_field2_u
{
  struct /* PD 2.0*/
  {
    uint8_t reserved1   : 4U; /*!< Reserved                  */
  } pd_v20;
  struct /* PD 3.0*/
  {
    uint8_t vdo_version : 3U; /*!< Version Number of the VDO */
    uint8_t reserved1   : 1U; /*!< Reserved                  */
  } pd_v30;
} usbpd_vdm_cable_vdo_field2_t;

/**
  * @brief  USBPD VDM Type structure definition.
  */
typedef enum
{
  USBPD_VDM_TYPE_UNSTRUCTURED = 0U, /*!< Unstructured VDM Type */
  USBPD_VDM_TYPE_STRUCTURED   = 1U, /*!< Structured VDM Type   */
} usbpd_vdm_type_t;

/**
  * @brief  USBPD VDM SVDM Message header structure definition.
  */
typedef union usbpd_vdm_svdm_header_u
{
  uint32_t d32;                                        /*!< Header in 32-bit format         */
  struct
  {
    uint32_t                 command            : 5U;  /*!< SVDM Header's Command            */
    uint32_t                 reserved1          : 1U;  /*!< Reserved                         */
    uint32_t                 command_type       : 2U;  /*!< SVDM Header's Command Type       */
    uint32_t                 object_position    : 3U;  /*!< SVDM Header's Object Position    */
    uint32_t                 svdm_version_minor : 2U;  /*!< SVDM Header's SVDM Version Minor */
    uint32_t                 svdm_version_major : 2U;  /*!< SVDM Header's SVDM Version Major */
    uint32_t                 vdm_type           : 1U;  /*!< SVDM Header's VDM Type           */
    uint32_t                 svid               : 16U; /*!< SVDM Header's SVID               */
  } b;
} usbpd_vdm_svdm_header_t;

#ifdef USBPDCORE_UVDM
/**
  * @brief  USBPD VDM UVDM Message header structure definition.
  */
typedef union usbpd_vdm_uvdm_header_u
{
  uint32_t d32;                        /*!< Header in 32-bit format                        */
  struct
  {
    uint32_t         vendor_use : 15U; /*!< Content of this field is defined by the vendor */
    uint32_t         vdm_type   : 1U;  /*!< VDM Type                                       */
    uint32_t         vid        : 16U; /*!< Vendor ID (vid)                                */
  } b;
} usbpd_vdm_uvdm_header_t;
#endif /* USBPDCORE_UVDM */

/**
  * @brief  USBPD VDM Passive Cable VDO structure definition.
  */
typedef union usbpd_vdm_cable_vdo_u
{
  uint32_t d32;
  struct
  {
    uint32_t                               usb_superspeed_support     : 3U; /*!< USB SuperSpeed Signaling Support  */
    uint32_t                               reserved                   : 2U; /*!< Reserved                          */
    uint32_t                               vbus_current_handling_capa : 2U; /*!< VBUS Current Handling Capability  */
    uint32_t                               field1                     : 2U; /*!< Based on
                                                                                 @ref usbpd_vdm_cable_vdo_field1_t */
    uint32_t                               cable_max_voltage          : 2U; /*!< Cable maximum voltage             */
    uint32_t                               cable_term_type            : 2U; /*!< Cable Termination Type            */
    uint32_t                               cable_latency              : 4U; /*!< Cable Latency                     */
    uint32_t                               epr_mode_capable           : 1U; /*!< Reserved                          */
    uint32_t                               cable_to_type              : 2U; /*!< USB Type-C plug to USB
                                                                                 Type-A/B/C/Captive (PD 2.0).
                                                                                 USB Type-C plug to USB
                                                                                 Type-C/Captive (PD 3.0)           */
    uint32_t                               field2                     : 1U; /*!< Based on
                                                                                 @ref usbpd_vdm_cable_vdo_field2_t */
    uint32_t                               vdo_version                : 3U; /*!< Version number  of the VDO        */
    uint32_t                               cable_fw_version           : 4U; /*!< Cable FW version number
                                                                                 (vendor defined)                  */
    uint32_t                               cable_hw_version           : 4U; /*!< Cable HW version number
                                                                                 (vendor defined)                  */
  } b;
} usbpd_vdm_cable_vdo_t;

/**
  * @brief  USBPD VDM Active Cable VDO structure definition.
  */
typedef union usbpd_vdm_active_cable_vdo1_u
{
  uint32_t d32;                                                                /*!< Active Cable VDO in 32-bit format */
  struct
  {
    uint32_t                               usb_highest_speed             : 3U; /*!< USB Highest Speed Support         */
    uint32_t                               sop_second_controller_present : 1U; /*!< SOP'' controller present Support  */
    uint32_t                               vbus_through_cable            : 1U; /*!< VBUS Through Cable Support        */
    uint32_t                               vbus_current_handling_capa    : 2U; /*!< VBUS Current Handling Capability  */
    uint32_t                               sbu_type                      : 1U; /*!< SBU Type                          */
    uint32_t                               sbu_supported                 : 1U; /*!< SBUs connections supported        */
    uint32_t                               cable_max_voltage             : 2U; /*!< Cable maximum voltage             */
    uint32_t                               cable_term_type               : 2U; /*!< Cable Termination Type
                                                                       (@ref USBPD_VDM_CABLE_TERM_ONE_EACH_VCONN
                                                                       or @ref USBPD_VDM_CABLE_TERM_BOTH_ACTIVE_VCONN */
    uint32_t                               cable_latency                 : 4U; /*!< Cable Latency                     */
    uint32_t                               epr_mode_capable              : 1U; /*!< B17 Reserved bit                  */
    uint32_t                               connector_type                : 2U; /*!< Connector Type
                                                                                    (@ref USBPD_VDM_CABLE_TO_TYPE_C
                                                                                    or @ref USBPD_VDM_CABLE_CAPTIVE)  */
    uint32_t                               reserved1                     : 1U; /*!< B20 Reserved bit                  */
    uint32_t                               vdo_version                   : 3U; /*!< Version number  of the Active
                                                                                    Cable VDO                         */
    uint32_t                               cable_fw_version              : 4U; /*!< Cable FW version number
                                                                                    (vendor defined)                  */
    uint32_t                               cable_hw_version              : 4U; /*!< Cable HW version number
                                                                                    (vendor defined)                  */
  } b;
} usbpd_vdm_active_cable_vdo1_t;

/**
  * @brief  USBPD VDM Cert stat VDO structure definition.
  */
typedef union usbpd_vdm_cert_stat_vdo_u
{
  uint32_t d32;         /*!< Cert stat VDO in 32-bit format */
  struct
  {
    uint32_t xid : 32U; /*!< USB-IF assigned XID            */
  } b;
} usbpd_vdm_cert_stat_vdo_t;

/**
  * @brief   USBPD VDM Alternate Mode Adapter VDO structure definition.
  * @warning Kept for PD2.0 legacy reasons but must not be used.
  */
typedef union usbpd_vdm_ama_vdo_u
{
  uint32_t d32;
  struct
  {
    uint32_t ama_usb_ss_support : 3U; /*!< AMA USB SuperSpeed Signaling Support based on
                                           @ref usbpd_vdm_ama_usb_superspeed_signaling_support_t */
    uint32_t vbus_requirement   : 1U; /*!< VBUS  required  based on
                                           @ref usbpd_vdm_vbus_requirement_t                     */
    uint32_t vconn_requirement  : 1U; /*!< VCONN  required  based on
                                           @ref usbpd_vdm_vconn_requirement_t                    */
    uint32_t vconn_pwr          : 3U; /*!< VCONN  power  based on
                                           @ref usbpd_vdm_vconn_pwr_t                            */
    uint32_t reserved1          : 13; /*!< Reserved                                              */
    uint32_t vdo_version        : 3U; /*!< Version Number of the VDO                             */
    uint32_t ama_fw_version     : 4U; /*!< AMA FW version number (vendor defined)                */
    uint32_t ama_hw_version     : 4U; /*!< AMA HW version number (vendor defined)                */
  } b;
} usbpd_vdm_ama_vdo_t;

/**
  * @brief  USBPD VDM UFP VDO structure definition.
  */
typedef union usbpd_vdm_ufp_vdo_u
{
  uint32_t d32;                                                  /*!< UFP VDO in 32-bit format    */
  struct
  {
    uint32_t                            usb_highest_speed : 3U;  /*!< USB Highest Speed Support   */
    uint32_t                            alternate_modes   : 3U;  /*!< Alternate modes based       */
    uint32_t                            reserved1         : 16U; /*!< B21_6 Reserved bit          */
    uint32_t                            connector_type    : 2U;  /*!< Connector Type              */
    uint32_t                            device_capability : 4U;  /*!< Device Capability           */
    uint32_t                            reserved2         : 1U;  /*!< B28 Reserved bit            */
    uint32_t                            ufp_vdo_version   : 3U;  /*!< Version Number of the VDO
                                                                      (must be set to Version1.1) */
  } b;
} usbpd_vdm_ufp_vdo_t;

/**
  * @brief  USBPD VDM DFP VDO structure definition.
  */
typedef union usbpd_vdm_dfp_vdo_u
{
  uint32_t d32;                                           /*!< DFP VDO in 32-bit format                  */
  struct
  {
    uint32_t                      port_num        : 5U;  /*!< Unique port number to identify a specific
                                                              port on a multi-port device               */
    uint32_t                      reserved1       : 17U; /*!< B21-5 Reserved bits                       */
    uint32_t                      connector_type  : 2U;  /*!< Connector Type                            */
    uint32_t                      host_capability : 3U;  /*!< Host Capability                           */
    uint32_t                      reserved2       : 2U;  /*!< B28-27 Reserved bits                      */
    uint32_t                      dfp_vdo_version : 3U;  /*!< Version Number of the VDO
                                                              (must be set to Version1.1)               */
  } b;
} usbpd_vdm_dfp_vdo_t;

#if defined(USBPDCORE_VPD)
/**
  * @brief  USBPD Vconn Powered USB Device VDO structure definition.
  */
typedef union usbpd_vdm_vpd_vdo_u
{
  uint32_t d32;                                                 /*!< VPD VDO in 32-bit format                        */
  struct
  {
    usbpd_vdm_vpd_charge_support_t charge_through_support : 1U; /*!< Charge Through Support                          */
    uint32_t                       ground_impedance       : 6U; /*!< Charge Through Support bit = 1b: Ground
                                                                     impedance through the VPD in 1 mOhm increments.
                                                                     Values less than 10 mOhm are Reserved and must
                                                                     not be used.
                                                                     Charge Through Support bit = 0b: Reserved,
                                                                     must be set to zero                             */
    uint32_t                       vbus_impedance         : 6U; /*!< Charge Through Support bit = 1b: Vbus impedance
                                                                     through the VPD in 2 mOhm increments.
                                                                     Values less than 10 mOhm are Reserved and must
                                                                     not be used.
                                                                     Charge Through Support bit = 0b: Reserved,
                                                                     must be set to zero                             */
    uint32_t                       reserved1              : 2U; /*!< B14-13 Reserved bit                             */
    usbpd_vdm_vpd_charge_current_t charge_through_current : 1U; /*!< Charge Through Current Support                  */
    usbpd_vdm_cable_voltage_t      cable_max_voltage      : 2U; /*!< Cable maximum voltage                           */
    uint32_t                       reserved2              : 4U; /*!< B20-17 Reserved bit                             */
    usbpd_vdm_vdo_vpd_version_t    vdo_version            : 3U; /*!< Version number  of the VPD VDO                  */
    uint32_t                       fw_version             : 4U; /*!< Cable FW version number (vendor defined)        */
    uint32_t                       hw_version             : 4U; /*!< Cable HW version number (vendor defined)        */
  } b;
} usbpd_vdm_vpd_vdo_t;
#endif /* USBPDCORE_VPD */

/**
  * @brief  USBPD VDP Data received from Discover Identity messages.
  */
typedef struct
{
  usbpd_vdm_id_header_vdo_t     id_header;                       /*!< ID Header VDO                        */
  usbpd_vdm_cert_stat_vdo_t     cert_stat_vdo;                   /*!< Cert Stat VDO                        */
  usbpd_vdm_product_vdo_t       product_vdo;                     /*!< Product VDO                          */

#if defined(USBPDCORE_VCONN_SUPPORT)
  usbpd_vdm_cable_vdo_t         cable_vdo;                       /*!< Passive Cable VDO                    */
#endif /* USBPDCORE_VCONN_SUPPORT */

  usbpd_vdm_ama_vdo_t           ama_vdo;                         /*!< Alternate Mode Adapter VDO           */

#if defined(USBPDCORE_VCONN_SUPPORT)
  usbpd_vdm_active_cable_vdo1_t active_cable_vdo1;               /*!< Active Cable VDO 1                   */
#endif /* USBPDCORE_VCONN_SUPPORT */

  usbpd_vdm_ufp_vdo_t           ufp_vdo;                         /*!< UFP VDO                              */
  usbpd_vdm_dfp_vdo_t           dfp_vdo;                         /*!< DFP VDO                              */

#if defined(USBPDCORE_VPD)
  usbpd_vdm_vpd_vdo_t           vpd_vdo;                         /*!< VPD VDO                              */
#endif /* USBPDCORE_VPD */

#if defined(USBPDCORE_VCONN_SUPPORT)
  uint8_t                       cable_vdo_presence         : 1U; /*!< Passive Cable VDO presence           */
#endif /* USBPDCORE_VCONN_SUPPORT */

  uint8_t                       ama_vdo_presence           : 1U; /*!< Alternate Mode Adapter VDO presence  */

#if defined(USBPDCORE_VCONN_SUPPORT)
  uint8_t                       active_cable_vdo1_presence : 1U; /*!< indicate active cable vdo 1 presence */
#endif /* USBPDCORE_VCONN_SUPPORT */

  uint8_t                       ufp_vdo_presence           : 1U; /*!< UFP VDO presence                     */
  uint8_t                       dfp_vdo_presence           : 1U; /*!< DFP VDO presence                     */

#if defined(USBPDCORE_VPD)
  uint8_t                       vpd_vdo_presence           : 1U; /*!< VPD VDO presence                     */
#if defined(USBPDCORE_VCONN_SUPPORT)
  uint8_t                       reserved1                  : 2U; /*!< Reserved bits                        */
#else
  uint8_t                       reserved1                  : 3U; /*!< Reserved bits                        */
#endif /* USBPDCORE_VCONN_SUPPORT */
#else
#if defined(USBPDCORE_VCONN_SUPPORT)
  uint8_t                       reserved1                  : 3U; /*!< Reserved bits                        */
#else
  uint8_t                       reserved1                  : 4U; /*!< Reserved bits                        */
#endif /* USBPDCORE_VCONN_SUPPORT */
#endif /* USBPDCORE_VPD */
} usbpd_vdm_discovery_identity_t;

/**
  * @brief  USBPD VDM Mode Info object structure definition.
  */
typedef struct
{
  uint32_t  mode_count;                /*!< Number of modes supported by the SVID */
  uint32_t  modes[MAX_MODES_PER_SVID]; /*!< List of modes supported by the SVID   */
  uint16_t  svid;                      /*!< SVID value                            */
} usbpd_vdm_mode_info_t;

/**
  * @brief  USBPD VDM SVID info structure definition.
  */
typedef struct
{
  uint16_t svids[12u];             /*!< List of SVIDs supported by the devices                  */
  uint8_t  svid_count;             /*!< Number of SVIDs supported by the devices                */
  uint8_t  flag_all_svid_received; /*!< Flag to indicate that all the SVIDs have been received.
                                        No need to send new SVDM Discovery SVID message         */
} usbpd_vdm_svid_info_t;

/**
  * @brief  USBPD Alert Data Object structure definition.
  */
typedef union usbpd_alert_data_object_u
{
  uint32_t d32;                             /*!< Alert Data Object in 32-bit format                            */
  struct
  {
    uint32_t reserved1               : 16U; /*!< Reserved */
    uint32_t hot_swappable_batteries : 4U;  /*!< Hot Swappable Batteries is a combination
                                                 of @ref USBPD_ADO_HOT_SWAP_BATT                               */
    uint32_t fixed_batteries         : 4U;  /*!< Fixed Batteries is a combination of @ref USBPD_ADO_FIXED_BATT */
    uint32_t type_alert              : 8U;  /*!< Type of Alert is a combination of @ref USBPD_ADO_TYPE_ALERT   */
  } b;
} usbpd_alert_data_object_t;

/**
  * @brief  USBPD revision Data Object structure definition.
  */
typedef union usbpd_revision_data_object_u
{
  uint32_t d32;                    /*!< Revision Data Object in 32-bit format */
  struct
  {
    uint32_t reserved1      : 16U; /*!< Reserved                              */
    uint32_t version_minor  : 4U;  /*!< Version minor                         */
    uint32_t version_major  : 4U;  /*!< Version major                         */
    uint32_t revision_minor : 4U;  /*!< Revision minor                        */
    uint32_t revision_major : 4U;  /*!< Revision major                        */
  } b;
} usbpd_revision_data_object_t;

/**
  * @brief  USBPD source info Data Object 1 structure definition.
  */
typedef union usbpd_source_info_data_object1_u
{
  uint32_t d32;                      /*!< Source Info Data Object in 32-bit format                   */
  struct
  {
    uint32_t port_reported_pdp : 8U; /*!< Power the port is actually advertising                      */
    uint32_t port_present_pdp  : 8U; /*!< Power the port is presently capable of supplying            */
    uint32_t port_max_pdp      : 8U; /*!< Power the port is designed to supply                        */
    uint32_t reserved1         : 7U; /*!< Reserved                                                    */
    uint32_t port_type         : 1U; /*!< 0 = Managed Capability Port, 1 = Guaranteed Capability Port */
  } b;
} usbpd_source_info_data_object1_t;

/**
  * @brief  USBPD source info Data Object 2 structure definition.
  */
typedef union usbpd_source_info_data_object2_u
{
  uint32_t d32;                      /*!< Source Info Data Object in 32-bit format                                           */
  struct
  {
    uint32_t port_guaranteed_pdp : 9U;  /*!< Maximum Power the port is guaranteed to always be able to provide in 0.5W steps */
    uint32_t port_maximum_pdp    : 9U;  /*!< Maximum Power the port will provide in 0.5W steps                               */
    uint32_t reserved1           : 12U; /*!< Reserved                        */
    uint32_t dps_port            : 1U;  /*!< 0 = Non DPS Port, 1 = DPS Port (port_type in B31 must be set to 0)             */
    uint32_t port_type           : 1U;  /*!< 0 = Managed Capability Port, 1 = Guaranteed Capability Port                     */
  } b;
} usbpd_source_info_data_object2_t;

/**
  * @brief  USBPD Battery Status Data Object structure definition.
  */
typedef union usbpd_bsdo_u
{
  uint32_t d32;                          /*!< Battery Status Data Object in 32-bit format     */
  struct
  {
    uint32_t reserved1            : 8U;  /*!< Reserved                                         */
    uint32_t battery_info         : 8U;  /*!< Battery info, based on @ref USBPD_BSDO_BATT_INFO */
    uint32_t battery_percent_capa : 16U; /*!< Battery Percent capacity                         */
  } b;
} usbpd_bsdo_t;

/**
  * @brief  USBPD Source Capabilities Extended Data Block (SCEDB) Message structure definition.
  */
typedef __PACKED_STRUCT
{
  uint16_t vid;                /*!< Vendor ID (assigned by the USB-IF)                   */
  uint16_t pid;                /*!< Product ID (assigned by the manufacturer)            */
  uint32_t xid;                /*!< Value provided by the USB-IF assigned to the product */
  uint8_t  fw_revision;        /*!< Firmware version number                              */
  uint8_t  hw_revision;        /*!< Hardware version number                              */
  uint8_t  voltage_regulation; /*!< Voltage Regulation                                   */
  uint8_t  holdup_time;        /*!< Holdup Time                                          */
  uint8_t  compliance;         /*!< Compliance                                           */
  uint8_t  touch_current;      /*!< Touch Current                                        */
  uint16_t peak_current_1;     /*!< Peak Current1                                        */
  uint16_t peak_current_2;     /*!< Peak Current2                                        */
  uint16_t peak_current_3;     /*!< Peak Current3                                        */
  uint8_t  touch_temp;         /*!< Touch Temp                                           */
  uint8_t  source_inputs;      /*!< Source Inputs                                        */
  uint8_t  batteries_count;    /*!< Number of Batteries/Battery Slots                    */
  uint8_t  source_pdp;         /*!< Source PDP  0..6: Source PDP rating
                                    7: Reserved and Must be set to zero                  */
  uint8_t  source_epr_pdp;     /*!< EPR Source PDP 0..7: EPR Source PDP Rating           */
} usbpd_scedb_t;

#if defined(USBPDCORE_SNK_CAPA_EXT)
/**
  * @brief  USBPD Sink Capabilities Extended Data Block (SKEDB) - Sink Load Characteristics structure definition.
  */
typedef union usbpd_skedb_snk_load_charac_u
{
  uint16_t   d16;                    /*!< Sink Load Characteristics in 16-bit format             */
  struct
  {
    uint16_t percent_overload  : 5U; /*!< Percent overload in 10% increments Values higher than
                                          25 (11001b) are clipped to 250%. 0b is default         */
    uint16_t overload_period   : 6U; /*!< Overload period in 20ms when bits 0-4 non-zero         */
    uint16_t duty_cycle        : 4U; /*!< Duty cycle in 5% increments when bits 0-4 are non-zero */
    uint16_t vbus_voltage_drop : 1U; /*!< Can tolerate VBUS Voltage drop                         */
  } b;
} usbpd_skedb_snk_load_charac_t;

/**
  * @brief  USBPD Sink Capabilities Extended Data Block (SKEDB) Message structure definition.
  */
typedef struct
{
  uint16_t                      vid;                     /*!< Vendor ID (assigned by the USB-IF)                     */
  uint16_t                      pid;                     /*!< Product ID (assigned by the manufacturer)              */
  uint32_t                      xid;                     /*!< Value provided by the USB-IF assigned to the product   */
  uint8_t                       fw_revision;             /*!< Firmware version number                                */
  uint8_t                       hw_revision;             /*!< Hardware version number                                */
  uint8_t                       skedb_version;           /*!< SKEDB Version (not the specification Version) based on
                                                              @ref USBPD_SKEDB_VERSION                               */
  uint8_t                       load_step;               /*!< Load Step based on @ref USBPD_SKEDB_LOADSTEP           */
  usbpd_skedb_snk_load_charac_t snk_load_charc;          /*!< Sink Load Characteristics                              */
  uint8_t                       compliance;              /*!< Compliance based on combination of
                                                              @ref USBPD_SKEDB_COMPLIANCE                            */
  uint8_t                       touch_temp;              /*!< Touch Temp based on @ref USBPD_SKEDB_TOUCHTEMP         */
  uint8_t                       battery_info;            /*!< Battery info                                           */
  uint8_t                       snk_modes;               /*!< Sink modes based on combination of
                                                              @ref USBPD_SKEDB_SINKMODES                             */
  uint8_t                       snk_min_pdp;             /*!< The Minimum PDP required by the Sink to operate
                                                              without consuming any power from its Battery(s)
                                                              if it have one                                         */
  uint8_t                       snk_operational_pdp;     /*!< The PDP the Sink requires to operate normally. For
                                                              sinks with a Battery, it is the PDP Rating of the
                                                              charger supplied with it or recommended for it.        */
  uint8_t                       snk_max_pdp;             /*!< The Maximum PDP the Sink can consume to operate and
                                                              charge its Battery(s) if it have one.                  */
  uint8_t                       snk_epr_min_pdp;         /*!< The Minimum PDP required by the EPR Sink to
                                                              operate without consuming any power from its
                                                              battery(s) if it have one.                             */
  uint8_t                       snk_epr_operational_pdp; /*!< The PDP the EPR Sink requires to operate normally.
                                                              For sinks with a battery, it is the PDP Rating of the
                                                              charger supplied with it or recommended for it.        */
  uint8_t                       snk_epr_max_pdp;         /*!< The Maximum PDP the EPR Sink can consume to operate
                                                              and charge its Battery(s) if it have one.              */
} usbpd_skedb_t;
#endif /* USBPDCORE_SNK_CAPA_EXT */

/**
  * @brief  USBPD Status Data Block structure definition.
  */
typedef __PACKED_STRUCT
{
  uint8_t internal_temp;         /*!< Source or Sink internal temperature in degrees centigrade         */
  uint8_t present_input;         /*!< Present Input based on @ref USBPD_SDB_PRESENT_INPUT               */
  uint8_t present_battery_input; /*!< Present Battery Input                                             */
  uint8_t event_flags;           /*!< Event Flags based on @ref USBPD_SDB_EVENT_FLAGS                   */
  uint8_t temperature_status;    /*!< Temperature based on @ref USBPD_SDB_TEMP_STATUS                   */
  uint8_t pwr_status;            /*!< Power Status based on combination of @ref USBPD_SDB_POWER_STATUS  */
  uint8_t pwr_state_change;      /*!< The Power state change status byte indicates a power state change
                                      based on @ref USBPD_SDB_PWR_STATE                                 */
} usbpd_sdb_t;

/**
  * @brief  USBPD Get Battery Capabilities Data Block Extended Message structure definition.
  */
typedef __PACKED_STRUCT
{
  uint8_t battey_cap_ref; /*!< Number of the Battery indexed from zero */
} usbpd_gbcdb_t;

/**
  * @brief  USBPD Get Battery Status Data Block Extended Message structure definition.
  */
typedef __PACKED_STRUCT
{
  uint8_t battery_status_ref; /*!< Number of the Battery indexed from zero */
} usbpd_gbsdb_t;

/**
  * @brief  USBPD  Battery Capability Data Block Extended Message structure definition.
  */
typedef __PACKED_STRUCT
{
  uint16_t vid;                           /*!< Vendor ID (assigned by the USB-IF)        */
  uint16_t pid;                           /*!< Product ID (assigned by the manufacturer) */
  uint16_t battery_design_capa;           /*!< Battery Design Capacity                   */
  uint16_t battery_last_full_charge_capa; /*!< Battery last full charge capacity         */
  uint8_t  battery_type;                  /*!< Battery Type                              */
} usbpd_bcdb_t;

/**
  * @brief  USBPD Get Manufacturer Info Info Data Block Extended Message structure definition.
  */
typedef __PACKED_STRUCT
{
  uint8_t manufacturer_info_target; /*!< Manufacturer Info Target based on @ref USBPD_MANUFINFO_TARGET */
  uint8_t manufacturer_info_ref;    /*!< Manufacturer Info Ref between Min_Data = 0 and
                                         Max_Data = 7 (@ref USBPD_MANUFINFO_REF)                       */
} usbpd_gmidb_t;

/**
  * @brief  USBPD Manufacturer Info Data Block Extended Message structure definition.
  */
typedef struct
{
  uint16_t vid;              /*!< Vendor ID (assigned by the USB-IF)        */
  uint16_t pid;              /*!< Product ID (assigned by the manufacturer) */
  uint8_t  manu_string[22U]; /*!< Vendor defined byte array                 */
} usbpd_midb_t;

#if defined(USBPDCORE_FWUPD)
/**
  * @brief  USBPD Firmware Update GET_FW_ID Response Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  usbpd_fwupd_status_t status;      /*!< Status Information during Firmware Update                 */
  uint16_t             vid;         /*!< USB-IF assigned Vendor ID                                 */
  uint16_t             pid;         /*!< USB-IF assigned Product ID                                */
  uint8_t              hw_version;  /*!< Hardware Version                                          */
  uint8_t              si_version;  /*!< Silicon Version                                           */
  uint16_t             fw_version1; /*!< Most significant component of the firmware version        */
  uint16_t             fw_version2; /*!< Second-most significant component of the firmware version */
  uint16_t             fw_version3; /*!< Third-most significant component of the firmware version  */
  uint16_t             fw_version4; /*!< Least significant component of the firmware version       */
  uint8_t              image_bank;  /*!< Image bank for which firmware is requested                */
  uint8_t              flags1;      /*!< flags1                                                    */
  uint8_t              flags2;      /*!< flags2                                                    */
  uint8_t              flags3;      /*!< flags3                                                    */
  uint8_t              flags4;      /*!< flags4                                                    */
} usbpd_fw_upd_get_fw_id_rsp_payload_t;

/**
  * @brief  USBPD Firmware Update PDFU_INITIATE Request Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  uint16_t fw_version1; /*!< Most significant component of the firmware version        */
  uint16_t fw_version2; /*!< Second-most significant component of the firmware version */
  uint16_t fw_version3; /*!< Third-most significant component of the firmware version  */
  uint16_t fw_version4; /*!< Least significant component of the firmware version       */
} usbpd_fw_upd_pdfu_init_req_payload_t;

/**
  * @brief  USBPD Firmware Update PDFU_INITIATE Response Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  usbpd_fwupd_status_t status;             /*!< Status Information during Firmware Update */
  uint8_t              wait_time;          /*!< Wait time                                 */
  uint8_t              image_size_max[3U]; /*!< Max image size                            */
} usbpd_fw_upd_pdfu_init_rsp_payload_t;

/**
  * @brief  USBPD Firmware Update PDFU_DATA Response Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  usbpd_fwupd_status_t status;         /*!< Status Information during Firmware Update               */
  uint8_t              wait_time;      /*!< Wait time                                               */
  uint8_t              num_data_nr;    /*!< Number of PDFU_DATA_NR Requests                         */
  uint16_t             data_block_num; /*!< Data Block Number of the next PDFU_DATA or PDFU_DATA_NR */
} usbpd_fw_upd_pdfu_data_rsp_payload_t;

/**
  * @brief  USBPD Firmware Update PDFU_VALIDATE Response Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  usbpd_fwupd_status_t status;    /*!< Status Information during Firmware Update */
  uint8_t              wait_time; /*!< Wait time                                 */
  uint8_t              flags;     /*!< Flags                                     */
} usbpd_fw_upd_pdfu_validate_rsp_payload_t;

/**
  * @brief  USBPD Firmware Update PDFU_DATA_PAUSE Response Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  usbpd_fwupd_status_t status; /*!< Status Information during Firmware Update */
} usbpd_fw_upd_pdfu_data_pause_rsp_payload_t;

/**
  * @brief  USBPD Firmware Update VENDOR_SPECIFIC Request Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  uint16_t  vid;                  /*!< USB-IF assigned Vendor ID */
  uint8_t   vendor_defined[256U]; /*!< Vendor defined            */
} usbpd_fw_upd_pdfu_vendor_specific_req_payload_t;

/**
  * @brief  USBPD Firmware Update VENDOR_SPECIFIC Response Payload structure definition.
  */
typedef __PACKED_STRUCT
{
  usbpd_fwupd_status_t status;               /*!< Status Information during Firmware Update */
  uint16_t             vid;                  /*!< USB-IF assigned Vendor ID                 */
  uint8_t              vendor_defined[255U]; /*!< Vendor defined                            */
} usbpd_fw_upd_pdfu_vendor_specific_rsp_payload_t;

/**
  * @brief  USBPD Firmware Update Request Data Block Extended Message structure definition.
  */
typedef __PACKED_STRUCT
{
  uint8_t protocol_version; /*!< Protocol Version (@ref USBPD_FWUPD_PROT_VER)            */
  uint8_t message_type;     /*!< Firmware Update Message type (@ref USBPD_FWUPD_MSGTYPE) */
  uint8_t payload[258U];    /*!< Payload                                                 */
} usbpd_frqdb_t;

#endif /* USBPDCORE_FWUPD */

#if defined(USBPDCORE_PPS)
/**
  * @brief  USBPD PPS Status Data Block Extended Message structure definition.
  */
typedef union usbpd_ppsdb_u
{
  uint32_t d32;                          /*!< PPS Status Data Object in 32-bit format                             */
  struct
  {
    uint16_t output_voltage_20mv_units;  /*!< Source output voltage in 20mV units.
                                              When set to 0xFFFF, the Source does not support this field.          */
    uint8_t  output_current_50ma_units;  /*!< Source output current in 50mA units.
                                              When set to 0xFF, the Source does not support this field.            */
    uint8_t  real_time_flags;            /*!< Real Time Flags, combination of @ref USBPD_CORE_DEF_REAL_TIME_FLAGS  */
  } b;
} usbpd_ppsdb_t;
#endif /* USBPDCORE_PPS */

/**
  * @brief  USBPD Country Code Data Block Extended Message structure definition.
  */
typedef struct
{
  uint32_t length;          /*!< Number of country codes in the message */
  uint16_t *p_country_code; /*!< Pointer of the country codes (1 to n)  */
} usbpd_ccdb_t;

/**
  * @brief  USBPD Country Info Data Block Extended Message structure definition.
  */
typedef struct
{
  uint16_t country_code;            /*!< 1st and 2nd character of the Alpha-2 Country Code defined by [ISO 3166] */
  uint32_t reserved1;               /*!< Reserved - Must be set to 0                                             */
  uint8_t  p_country_specific_data; /*!< Pointer on Content defined by the country authority (0 t 256 bytes)     */
} usbpd_cidb_t;

/**
  * @}
  */

/** @defgroup USBPD_CORE_SETTINGS_Exported_Structures USBPD CORE Settings Exported Structures
  * @brief  USBPD Settings structure definition.
  * @{
  */

/**
  * @brief  USBPD PD3 support structure definition.
 */
typedef union usbpd_pd3_support_u
{
  uint16_t d16;                                       /*!< PD3 support in 16-bit format                          */
  struct
  {
    uint16_t pe_unchunk_support                 : 1U; /*!< Unchunked support                                      */
    uint16_t pe_fast_role_swap_support          : 1U; /*!< Fast role swap support (not yet implemented)           */
    uint16_t is_get_pps_status_supported        : 1U; /*!< Get PPS status message supported by PE                 */
    uint16_t is_src_capa_ext_supported          : 1U; /*!< Source_Capabilities_Extended message supported by PE   */
    uint16_t is_alert_supported                 : 1U; /*!< Alert message supported by PE                          */
    uint16_t is_get_status_supported            : 1U; /*!< Get_Status message supported by PE (is_alert_supported
                                                           must be enabled)                                       */
    uint16_t is_get_manufacturer_info_supported : 1U; /*!< Manufacturer_Info message supported by PE              */
    uint16_t is_get_country_codes_supported     : 1U; /*!< Get_Country_Codes message supported by PE              */
    uint16_t is_get_country_info_supported      : 1U; /*!< Get_Country_Info message supported by PE               */
    uint16_t is_security_request_supported      : 1U; /*!< Security_Response message supported by PE              */
    uint16_t is_fw_update_request_supported     : 1U; /*!< Firmware update response message supported by PE       */
    uint16_t is_snk_epr_supported               : 1U; /*!< EPR support need to use a library supporting EPR else
                                                           it has no effect                                       */
    uint16_t is_src_epr_supported               : 1U; /*!< EPR support need to use a library supporting EPR else
                                                           it has no effect                                       */
    uint16_t is_get_battery_status_supported    : 1U; /*!< Get Battery Capabitity and Status messages supported
                                                           by PE                                                  */
    uint16_t reserved1                          : 2U; /*!< Reserved bits                                          */
  } b;
} usbpd_pd3_support_t;

/**
  * @brief  USBPD Settings structure definition.
  */
typedef struct
{
#if defined(USBPDCORE_LIB_NO_PD)
  usbpd_port_pwr_role_t  pe_default_role                   : 1U; /*!< Default port role                               */
  usbpd_cad_rp_value_t   cad_default_resistor              : 2U; /*!< Default RP resistor                             */
  uint8_t                reserved1                         : 5U; /*!< Reserved bits */
#else
  usbpd_sop_support_t    pe_supported_sop;                       /*!< SOPs type messages managed by the stack.
                                                                      If Vconn support is enabled, this
                                                                      field must be set to:
                                                                        @ref USBPD_SUPPORTED_SOP_SOP  |
                                                                        @ref USBPD_SUPPORTED_SOP_SOP1 |
                                                                        @ref USBPD_SUPPORTED_SOP_SOP2
                                                                      Otherwise, set it to:
                                                                        @ref USBPD_SUPPORTED_SOP_SOP
                                                                                                                      */
  uint32_t               pe_spec_revision                  : 2U; /*!< Spec revision value                             */
  uint32_t               pe_default_role                   : 1U; /*!< Default port role                               */
  uint32_t               pe_pwr_role_swap_support          : 1U; /*!< DRP support                                     */
#if defined(USBPDCORE_VPD)
  uint32_t               vpd_support                       : 1U; /*!< Support of the CTVPD device                     */
#else
  uint32_t               reserved1                         : 1U; /*!< Reserved bit                                    */
#endif /* USBPDCORE_VPD */
  uint32_t               pe_vdm_support                    : 1U; /*!< VDMsupport: If not enabled any VDM message
                                                                      received is replied with "not supported"        */
  uint32_t               pe_ping_support                   : 1U; /*!< Ping support (only for PD3.0): If enabled
                                                                      allows DPM to send ping message                 */
  uint32_t               pe_caps_counter_support           : 1U; /*!< If enabled after an amount of message source
                                                                      capabilities not replied, the stack stop
                                                                      the message send.                               */
  uint32_t               pe_respond_to_discov_sop_support  : 1U; /*!< Can respond successfully to a Discover Identity */
  uint32_t               pe_attempts_to_discov_sop_support : 1U; /*!< Can send a Discover Identity */
  uint32_t               cad_try_feature_support           : 2U; /*!< CAD Try feature - Not implemented               */
  uint32_t               cad_accessory_support             : 1U; /*!< CAD Accessory - Not implemented                 */
  uint32_t               cad_debug_accesory_support        : 3u; /*!< TS Sink supported                               */
  uint32_t               cad_role_toggle_support           : 1U; /*!< If enabled allows the detection state machine
                                                                      switch Rp/Rd means toggle the presented role
                                                                      between source and sink                         */
  uint32_t               cad_default_resistor              : 2U; /*!< Default RP resistor                             */
  uint32_t               cad_snk_toggle_time_ms            : 8U; /*!< Sink toggle time in ms                          */
  uint32_t               cad_src_toggle_time_ms            : 8U; /*!< Source toggle time in ms                        */
  usbpd_pd3_support_t    pe_pd3_support;                         /*!< PD3 structure support flags                     */
#endif /*USBPDCORE_LIB_NO_PD*/
} usbpd_settings_t;

/**
  * @}
  */

/**
  * @brief  USBPD Params structure definition.
  */
typedef struct
{
  uint32_t                       pe_spec_revision              : 2U; /*!< PE Specification revision                */
  uint32_t                       pe_power_role                 : 1U; /*!< PE Power role                            */
  uint32_t                       pe_data_role                  : 1U; /*!< PE Data role                             */
  uint32_t                       pe_flag_swap_ongoing          : 1U; /*!< Power role swap ongoing flag             */
  uint32_t                       vdm_version                   : 1U; /*!< VDM version                              */
  uint32_t                       active_cc                     : 2U; /*!< Active CC line                           */
  uint32_t                       pe_pwr_status                 : 3U; /*!< Power status                             */
  uint32_t                       dpm_flag_initialized          : 1U; /*!< DPM initialized flag                     */
  uint32_t                       pe_flag_is_connected          : 1U; /*!< USBPD PE stack is connected to CC line   */
  uint32_t                       vconn_cc                      : 2U; /*!< VConn CC line                            */
  uint32_t                       vconn_status                  : 1U; /*!< VConn On/Off status                      */
  uint32_t                       rp_resistor                   : 2U; /*!< Rp resistor presented                    */
  uint32_t                       rp_resistor_exposed_at_attach : 2U; /*!< Exposed resistance from source at attach */

#if defined(USBPDCORE_VPD)
  uint32_t                       vpd_flag                      : 1U; /*!< VPD detection flag                       */
  uint32_t                       cad_vpd_status                : 2U; /*!< CAD VPD status used between CAD and PE   */
  uint32_t                       pe_vpd_status                 : 2U; /*!< CAD VPD status used between CAD and PE   */
#else
  uint32_t                       reserved1                     : 5U; /*!< Reserved bits                            */
#endif /* USBPDCORE_VPD */
  uint32_t                       pe_unchunk_support            : 1U; /*!< Unchunked support                        */
#if defined(USBPDCORE_VCONN_SUPPORT)
  uint32_t                       cbl_spec_revision             : 2U; /*!< Cable Specification revision             */
#else
  uint32_t                       reserved3                     : 2U; /*!< Reserved bits                            */
#endif /* USBPDCORE_VCONN_SUPPORT */

#if defined(USBPDCORE_PECABLE)
  uint32_t                       flag_cable_connected          : 1U; /*!< Is Cable connected                       */
#else
  uint32_t                       reserved4                     : 1U; /*!< Reserved bits                            */
#endif /* USBPDCORE_PECABLE */

#if defined(USBPDCORE_EPR)
  uint32_t                       pwr_range                     : 1U; /*!< Current power mode                       */
  uint32_t                       reserved5                     : 2U; /*!< Is Cable connected                       */
#else
  uint32_t                       reserved5                     : 3U; /*!< Reserved bits                            */
#endif /* USBPDCORE_EPR */
#if defined(USBPDCORE_SPR_AVS)
  uint32_t                       spr_apdo                      : 1u; /*!< SPR APDO Type                            */
#else
  uint32_t                       reserved6                     : 1u; /*!< Reserved bits                            */
#endif /* USBPDCORE_SPR_AVS */
  uint32_t                       debug_accessory               : 2u; /*!< Debug Accessory Mode                     */
  uint32_t                       skip_snk_Wait_cap_Timer       : 1u; /*!< Skip tTypeCSinkWaitCap timer             */
} usbpd_params_t;

#if defined(USBPDCORE_USBDATA)
/**
  * @brief  USBPD Enter USB Data object.
  */
typedef union usbpd_enter_usb_data_u
{
  uint32_t d32;                  /*!< Enter USB Data Object in 32-bit format                       */
  struct
  {
    uint32_t reserved1     : 13U; /*!< Reserved  - Must be set to zero                              */
    uint32_t host_presence : 1U;  /*!< Connected to a Host.                                         */
    uint32_t tbt_support   : 1U;  /*!< [TBT3] is supported by the host's USB4 Connection Manager    */
    uint32_t dp_support    : 1U;  /*!< [USB4] DP tunneling supported by the host                    */
    uint32_t pci_support   : 1U;  /*!< [USB4] PCIe tunneling supported by the hosts                 */
    uint32_t cable_current : 2U;  /*!< 00b = VBUS is not supported 01b = Reserved 10b = 3A 11b = 5A */
    uint32_t cable_type    : 2U;  /*!< 00b: Passive,
                                       01b: Active Re-timer
                                       10b: Active Re-driver
                                       11b: Optically Isolated                                      */
    uint32_t cable_speed   : 3U;  /*!< 000b: [USB 2.0] only, no SuperSpeed support,
                                       001b: [USB 3.2] Gen1,
                                       010b: [USB 3.2] Gen2 and [USB4] Gen2
                                       011b: [USB4] Gen3                                            */
    uint32_t reserved2     : 1U;  /*!< Reserved  - Must be set to zero                              */
    uint32_t usb3_drd      : 1U;  /*!< 0b: Not capable of operating as a [USB 3.2] Device
                                       1b: Capable of operating as a [USB 3.2] Device               */
    uint32_t usb4_drd      : 1U;  /*!< 0b: Not capable of operating as a [USB 4] Device
                                       1b: Capable of operating as a [USB 4] Device                 */
    uint32_t reserved3     : 1U;  /*!< Reserved  - Must be set to zero                              */
    uint32_t usb_mode      : 3U;  /*!< 000b: [USB 2.0], 001b: [USB 3.2] 010b: [USB4]                */
    uint32_t reserved4     : 1U;  /*!< Reserved  - Must be set to zero                              */
  } b;
} usbpd_enter_usb_data_t;
#endif /* USBPDCORE_USBDATA */

#if defined(USBPDCORE_EPR)
/**
  * @brief  USBPD EPRMDO Data object (used for message ERP_MODE).
  */
typedef union usbpd_eprmdo_u
{
  uint32_t d32;
  struct
  {
    uint32_t                reserved1 : 16U;
    uint32_t                data      : 8U;  /*!< Enter Must be set to the EPR Sink Operational PDP
                                                  Enter Acknowledged Must be set to zero
                                                  Enter Succeeded Must be set to zero
                                                  Enter Failed Must be one of the following values:
                                                           0x00 - Unknown cause
                                                           0x01 - Cable not EPR capable
                                                           0x02 - Source failed to become VCONN  source
                                                           0x03 - EPR Mode Capable bit not set in RDO
                                                           0x04 - Source unable to enter EPR Mode at this time
                                                           0x05 - EPR Mode Capable bit not set in PDO
                                                           All other values are Reserved and Must Not be used
                                                  Exit Must be set to zero                                     */
    uint32_t                action    : 8U;  /*!< 0x00 Reserved and Must Not be used
                                                  0x01 Enter Sink only
                                                  0x02 Enter Acknowledged Source only
                                                  0x03 Enter Succeeded Source only
                                                  0x04 Enter Failed Source only
                                                  0x05 Exit Sink or Source
                                                  0x06-0xFF Reserved and Must Not be used                      */
  } b;
} usbpd_eprmdo_t;
#endif /* USBPDCORE_EPR */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

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

#endif /* USBPD_DEF_H */

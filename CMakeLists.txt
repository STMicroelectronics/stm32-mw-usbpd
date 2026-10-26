# origin-pack: STMicroelectronics::usbpd@2.0.0
# file-format: 1.0.0
project(STMicroelectronics_usbpd_2_0_0)
cmake_minimum_required(VERSION 3.30)
add_library(STMicroelectronics_usbpd_2_0_0 INTERFACE)

# List of all CMSIS properties that influence conditions for this package
if(NOT DEFINED CMSIS_Dcore)
  set(CMSIS_Dcore "CMSIS_Dcore not set")
  message(DEBUG "CMSIS_Dcore is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dname)
  set(CMSIS_Dname "CMSIS_Dname not set")
  message(DEBUG "CMSIS_Dname is not set to any value")
endif()

if(NOT DEFINED CMSIS_Dvendor)
  set(CMSIS_Dvendor "CMSIS_Dvendor not set")
  message(DEBUG "CMSIS_Dvendor is not set to any value")
endif()

if(NOT DEFINED CMSIS_Tcompiler)
  set(CMSIS_Tcompiler "CMSIS_Tcompiler not set")
  message(DEBUG "CMSIS_Tcompiler is not set to any value")
endif()


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_STMicroelectronics_usbpd_2_0_0)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Core#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Core timerserver#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3FULL#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3MINDRP#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3MINSRC#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3MINSNK#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3SPR#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3TYPEC#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Utils tracer#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Utils monitor#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD Interface#Csub:phy#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD Interface#Csub:trace#Cversion:2.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD Interface#Csub:timerserver#Cversion:2.0.0")
endif()

# All conditions used by this package

# condition: STMicroelectronics.usbpd.2.0.0
# description: 
if((CMSIS_Dname MATCHES "STM32.*" AND CMSIS_Dvendor STREQUAL "STMicroelectronics:13"))
  set(STMicroelectronics.usbpd.2.0.0 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0 enabled")
else()
  set(STMicroelectronics.usbpd.2.0.0 0)
endif()


# condition: STMicroelectronics.usbpd.2.0.0:ARMCC GCC IAR
# description: 
if((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR"))
  set(STMicroelectronics.usbpd.2.0.0_ARMCC_GCC_IAR "1")
  message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_ARMCC_GCC_IAR enabled")
else()
  set(STMicroelectronics.usbpd.2.0.0_ARMCC_GCC_IAR 0)
endif()


# condition: STMicroelectronics.usbpd.2.0.0:M0PLUS
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M0+")))
  set(STMicroelectronics.usbpd.2.0.0_M0PLUS "1")
  message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_M0PLUS enabled")
else()
  set(STMicroelectronics.usbpd.2.0.0_M0PLUS 0)
endif()


# condition: STMicroelectronics.usbpd.2.0.0:M33
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M33")))
  set(STMicroelectronics.usbpd.2.0.0_M33 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_M33 enabled")
else()
  set(STMicroelectronics.usbpd.2.0.0_M33 0)
endif()


# condition: STMicroelectronics.usbpd.2.0.0:M55
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M55")))
  set(STMicroelectronics.usbpd.2.0.0_M55 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_M55 enabled")
else()
  set(STMicroelectronics.usbpd.2.0.0_M55 0)
endif()


# condition: STMicroelectronics.usbpd.2.0.0:M7
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M7")))
  set(STMicroelectronics.usbpd.2.0.0_M7 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_M7 enabled")
else()
  set(STMicroelectronics.usbpd.2.0.0_M7 0)
endif()


# condition: STMicroelectronics.usbpd.2.0.0:M85
# description: 
if(((CMSIS_Tcompiler STREQUAL "ARMCC" OR CMSIS_Tcompiler STREQUAL "GCC" OR CMSIS_Tcompiler STREQUAL "IAR") AND (CMSIS_Dvendor STREQUAL "STMicroelectronics:13" AND CMSIS_Dcore STREQUAL "Cortex-M85")))
  set(STMicroelectronics.usbpd.2.0.0_M85 "1")
  message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_M85 enabled")
else()
  set(STMicroelectronics.usbpd.2.0.0_M85 0)
endif()


# condition: STMicroelectronics.usbpd.2.0.0:Use TCPP01 pwr if
# description: Use TCPP01 pwr if dependency
set(STMicroelectronics.usbpd.2.0.0_Use_TCPP01_pwr_if "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_Use_TCPP01_pwr_if enabled")


# condition: STMicroelectronics.usbpd.2.0.0:Use TCPP0203 pwr if
# description: Use TCPP03 pwr if dependency
set(STMicroelectronics.usbpd.2.0.0_Use_TCPP0203_pwr_if "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_Use_TCPP0203_pwr_if enabled")


# condition: STMicroelectronics.usbpd.2.0.0:Use custom pwr if
# description: Use custom pwr if dependency
set(STMicroelectronics.usbpd.2.0.0_Use_custom_pwr_if "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_Use_custom_pwr_if enabled")


# condition: STMicroelectronics.usbpd.2.0.0:Use usb if
# description: Use usb if dependency
set(STMicroelectronics.usbpd.2.0.0_Use_usb_if "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_Use_usb_if enabled")


# condition: STMicroelectronics.usbpd.2.0.0:Use vdm user app
# description: Use vdm user app dependency
set(STMicroelectronics.usbpd.2.0.0_Use_vdm_user_app "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_Use_vdm_user_app enabled")


# condition: STMicroelectronics.usbpd.2.0.0:usbpd core condition
# description: USBPD Core dependency
set(STMicroelectronics.usbpd.2.0.0_usbpd_core_condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD#.*Csub:Core(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:USB#.*Cgroup:USBPD Interface#.*Csub:phy(#.*|$)>,>>>")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_usbpd_core_condition enabled")


# condition: STMicroelectronics.usbpd.2.0.0:usbpd lib condition
# description: USBPD Lib dependency
set(STMicroelectronics.usbpd.2.0.0_usbpd_lib_condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_usbpd_lib_condition enabled")


# condition: STMicroelectronics.usbpd.2.0.0:usbpd tim peripheral condition
# description: USBPD TIM interface dependency
set(STMicroelectronics.usbpd.2.0.0_usbpd_tim_peripheral_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:TIM(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_usbpd_tim_peripheral_condition enabled")


# condition: STMicroelectronics.usbpd.2.0.0:usbpd uart peripheral condition
# description: USBPD UART interface dependency
set(STMicroelectronics.usbpd.2.0.0_usbpd_uart_peripheral_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:UART(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_usbpd_uart_peripheral_condition enabled")


# condition: STMicroelectronics.usbpd.2.0.0:usbpd ucpd peripheral condition
# description: USBPD UCPD interface dependency
set(STMicroelectronics.usbpd.2.0.0_usbpd_ucpd_peripheral_condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:UCPD(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_usbpd_ucpd_peripheral_condition enabled")


# condition: STMicroelectronics.usbpd.2.0.0:usbpd utils monitor condition
# description: USBPD Utils monitor dependency
set(STMicroelectronics.usbpd.2.0.0_usbpd_utils_monitor_condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_usbpd_utils_monitor_condition enabled")


# condition: STMicroelectronics.usbpd.2.0.0:usbpd utils tracer condition
# description: USBPD Utils tracer dependency
set(STMicroelectronics.usbpd.2.0.0_usbpd_utils_tracer_condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.usbpd.2.0.0_usbpd_utils_tracer_condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Core#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USBPD Common Core
  message(DEBUG "Using component USB_USBPD_Core_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_lib_condition}:-DCMSIS_USE_USB_USBPD_Core_2_0_0=1>")
  target_include_directories(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_lib_condition}:${CMAKE_CURRENT_LIST_DIR}/common/core/>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_lib_condition}:${CMAKE_CURRENT_LIST_DIR}/common/core/usbpd_phy.c>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_lib_condition}:${CMAKE_CURRENT_LIST_DIR}/common/core/usbpd_pwr.c>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_lib_condition}:${CMAKE_CURRENT_LIST_DIR}/common/core/usbpd_trace.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Core timerserver#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USBPD Core timerserver
  message(DEBUG "Using component USB_USBPD_Core_timerserver_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "-DCMSIS_USE_USB_USBPD_Core_timerserver_2_0_0=1")
  target_include_directories(STMicroelectronics_usbpd_2_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/common/core/")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "${CMAKE_CURRENT_LIST_DIR}/common/core/usbpd_timersserver.c")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3FULL#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Lib Full
  message(DEBUG "Using component USB_USBPD_Lib_USBPD_PD3FULL_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "-DCMSIS_USE_USB_USBPD_Lib_USBPD_PD3FULL_2_0_0=1")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M0PLUS}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_FULL_CM0PLUS_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M33}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_FULL_CM33_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M55}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_FULL_CM55_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M7}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_FULL_CM7_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M85}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_FULL_CM85_wc32.a>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3MINDRP#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Lib MinDRP
  message(DEBUG "Using component USB_USBPD_Lib_USBPD_PD3MINDRP_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "-DCMSIS_USE_USB_USBPD_Lib_USBPD_PD3MINDRP_2_0_0=1")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M0PLUS}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINDRP_CM0PLUS_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M33}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINDRP_CM33_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M55}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINDRP_CM55_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M7}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINDRP_CM7_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M85}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINDRP_CM85_wc32.a>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3MINSRC#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Lib MinSRC
  message(DEBUG "Using component USB_USBPD_Lib_USBPD_PD3MINSRC_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "-DCMSIS_USE_USB_USBPD_Lib_USBPD_PD3MINSRC_2_0_0=1")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M0PLUS}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSRC_CM0PLUS_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M33}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSRC_CM33_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M55}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSRC_CM55_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M7}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSRC_CM7_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M85}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSRC_CM85_wc32.a>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3MINSNK#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Lib MinSNK
  message(DEBUG "Using component USB_USBPD_Lib_USBPD_PD3MINSNK_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "-DCMSIS_USE_USB_USBPD_Lib_USBPD_PD3MINSNK_2_0_0=1")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M0PLUS}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSNK_CM0PLUS_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M33}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSNK_CM33_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M55}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSNK_CM55_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M7}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSNK_CM7_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M85}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_MINSNK_CM85_wc32.a>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3SPR#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Lib SPR
  message(DEBUG "Using component USB_USBPD_Lib_USBPD_PD3SPR_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "-DCMSIS_USE_USB_USBPD_Lib_USBPD_PD3SPR_2_0_0=1")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M0PLUS}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_SPR_CM0PLUS_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M33}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_SPR_CM33_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M55}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_SPR_CM55_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M7}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_SPR_CM7_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M85}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_CONFIG_SPR_CM85_wc32.a>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Lib#Cvariant:USBPD_PD3TYPEC#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Lib TypeC
  message(DEBUG "Using component USB_USBPD_Lib_USBPD_PD3TYPEC_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "-DCMSIS_USE_USB_USBPD_Lib_USBPD_PD3TYPEC_2_0_0=1")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M0PLUS}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_NO_PD_CM0PLUS_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M33}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_NO_PD_CM33_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M55}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_NO_PD_CM55_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M7}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_NO_PD_CM7_wc32.a>")
  target_link_libraries(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_M85}:${CMAKE_CURRENT_LIST_DIR}/common/lib/USBPD_LIB_NO_PD_CM85_wc32.a>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Utils tracer#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Utils
  message(DEBUG "Using component USB_USBPD_Utils_tracer_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_utils_tracer_condition}:-DCMSIS_USE_USB_USBPD_Utils_tracer_2_0_0=1>")
  target_include_directories(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_utils_tracer_condition}:${CMAKE_CURRENT_LIST_DIR}/common/utils/tracer/>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_utils_tracer_condition}:${CMAKE_CURRENT_LIST_DIR}/common/utils/tracer/usbpd_utils_tracer.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD#Csub:Utils monitor#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USB Common Utils
  message(DEBUG "Using component USB_USBPD_Utils_monitor_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_utils_monitor_condition}:-DCMSIS_USE_USB_USBPD_Utils_monitor_2_0_0=1>")
  target_include_directories(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_utils_monitor_condition}:${CMAKE_CURRENT_LIST_DIR}/common/utils/monitor/>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_utils_monitor_condition}:${CMAKE_CURRENT_LIST_DIR}/common/utils/monitor/usbpd_utils_monitor.c>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_utils_monitor_condition}:${CMAKE_CURRENT_LIST_DIR}/common/utils/monitor/usbpd_utils_monitor_tlv.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD Interface#Csub:phy#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USBPD interface phy
  message(DEBUG "Using component USB_USBPD_Interface_phy_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_ucpd_peripheral_condition}:-DCMSIS_USE_USB_USBPD_Interface_phy_2_0_0=1>")
  target_include_directories(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_ucpd_peripheral_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/phy/>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_ucpd_peripheral_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/phy/usbpd_cad_hw_if.c>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_ucpd_peripheral_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/phy/usbpd_phy_hw_if.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD Interface#Csub:trace#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USBPD interface trace
  message(DEBUG "Using component USB_USBPD_Interface_trace_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_uart_peripheral_condition}:-DCMSIS_USE_USB_USBPD_Interface_trace_2_0_0=1>")
  target_include_directories(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_uart_peripheral_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/trace/>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_uart_peripheral_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/trace/usbpd_utils_tracer_hw_if.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:USB#Cgroup:USBPD Interface#Csub:timerserver#Cversion:2.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # USBPD interface timerserver
  message(DEBUG "Using component USB_USBPD_Interface_timerserver_2_0_0")
  target_compile_definitions(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_tim_peripheral_condition}:-DCMSIS_USE_USB_USBPD_Interface_timerserver_2_0_0=1>")
  target_include_directories(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_tim_peripheral_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/timerserver/>")
  target_sources(STMicroelectronics_usbpd_2_0_0 INTERFACE "$<${STMicroelectronics.usbpd.2.0.0_usbpd_tim_peripheral_condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/timerserver/usbpd_timerserver_hw_if.c>")
endif()


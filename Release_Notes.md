

# Release Notes for
# <mark>USBPD SW Pack</mark>

# Purpose

STM32Cube enables developers to achieve design success. With a comprehensive suite of professional development tools and embedded software components, STM32Cube allows developers to differentiate products, streamline design cycles, and reduce costs. STM32Cube ecosystem supports all design steps, including selection, configuration, development, debugging, programming, and monitoring.
The STM32Cube embedded software offer provides ready-to-use software components that can be added to a project. It includes STM32 peripheral driver APIs with two levels of abstraction, middleware, board drivers, and examples. There are several distribution channels, including the STM32CubeMX2 tool, the ST website, and GitHub. All embedded software comes with enhanced online documentation, with flowcharts and user sequences.

**USBPD** is a USB Power Delivery middleware for STM32 microcontrollers. It provides a complete USB Type-C and Power Delivery stack, supporting source, sink, and dual-role power configurations. Full PD mode (source, sink, DRP) requires an RTOS. The Type-C only stack (NO_PD) can be used either in RTOS or bare-metal  applications.

More documentation is available at [**USBPD SW Pack online documentation**](https://dev.st.com/stm32cube-docs/mw-usbpd/latest/en/index.html)

# Update history

<label for="collapse-section-2.0.0" aria-hidden="true">__2.0.0 / 21-September-2026__</label>
<div>

## Main changes
First Official release of **USBPD SW Pack**.

## Contents
- USBPD full PD stack support (source, sink, and dual-role power) RTOS, with STM32 UCPD peripheral integration and USB Type-C/Power Delivery protocol management.
- USBPD Type-C only stack (NO_PD) for both bare-metal and RTOS, with STM32 UCPD peripheral integration and USB Type-C connection management.

## Known limitations

- None

## Development toolchains and compilers

- IAR Embedded Workbench for ARM (EWARM) toolchain V9.60.3 + ST-LINK
- MDK-ARM Keil uVision V5.42
- STM32CubeIDE for Visual Studio Code (GCC13 compiler)

## Supported devices and boards

- STM32V8 series.
- STM32G5 series.
- STM32H7P series.

## Backward compatibility

- None

## Dependencies

- STM32V8xx HAL Drivers V2.0.0
- STM32G5xx HAL Drivers V2.0.0
- STM32H7Pxx HAL Drivers V2.0.0
- FreeRTOS SW Pack V2.0.0 (required when selecting RTOS mode)

</div>


For complete documentation on STM32 microcontrollers,
visit: [www.st.com/stm32](http://www.st.com/stm32)

This release note uses up to date web standards and, for this reason, should not be opened with Internet Explorer but preferably with popular browsers such as Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
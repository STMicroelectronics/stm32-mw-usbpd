

# USBPD Software Pack

![tag](https://img.shields.io/badge/tag-2.0.0-brightgreen.svg)
[![release note](https://img.shields.io/badge/release_note-view_html-gold.svg)](https://htmlpreview.github.io/?https://github.com/STMicroelectronics/stm32-mw-usbpd/blob/hal2/Release_Notes.html)

## Overview

This package contains the STM32 USB Power Delivery (USBPD) middleware for STM32 microcontrollers.
It provides a complete USB Type-C and Power Delivery implementation including:

- The USB-PD protocol core and helpers (connection, policy and protocol management).
- A Type-C only stack (NO_PD) for managing USB Type-C attachment/detachment without full PD negotiation.
- Board/PHY/port-controller integration layers under `interfaces/`.
- Precompiled static libraries for multiple CPU cores and build profiles in `common/lib/`.

## Main features

- Full PD stack (Source / Sink / DRP) for RTOS builds.
- NO_PD (Type-C only) mode that can run bare-metal or with an RTOS.
- Port controller and PHY abstraction layers (see `interfaces/`).
- Trace and monitoring helpers, timer server, and power-management utilities.
- Prebuilt library variants for different CPU cores and configurations (see `common/lib/`).

## Prebuilt libraries

The `common/lib/` folder contains precompiled static libraries for multiple CPU cores and profiles such as
`FULL`, `NO_PD`, `MINSRC`, `MINSNK`, and `SPR` across Cortex-M variants.
These are provided to simplify integration into target projects and reduce build time for common configurations.


# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/Makefiles/stm32.mk

BINPATH = C:\ST\STM32CubeIDE_1.10.1\STM32CubeIDE\plugins\com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.10.3-2021.10.win32_1.0.0.202111181127\tools\bin

PREFIX = arm-none-eabi-

AR   = $(BINPATH)/$(PREFIX)ar
AS   = $(BINPATH)/$(PREFIX)gcc -x assembler-with-cpp
GPP  = $(BINPATH)/$(PREFIX)g++
SIZE = $(BINPATH)/$(PREFIX)size

CFLAGS = -mcpu=cortex-m4 -std=gnu++14 -g3 -D_KMS_EMBEDDED_ -DSTM32 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb

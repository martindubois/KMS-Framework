
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022-2023 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/Common.mk

KMS_ARGS           = ../../Args
KMS_LINKER_SCRIPTS = ../../LinkerScripts
KMS_MAKEFILES      = ../../Makefiles
KMS_STARTUP        = ../../Startup

include $(KMS_MAKEFILES)/stm32.mk

INCLUDES = -I ../../Includes/STM -I ../../../Includes

KMS_A_A       = ../Libraries/$(CONFIG)_$(PROCESSOR)/KMS-A.a
KMS_STM32_A   = ../Libraries/$(CONFIG)_$(PROCESSOR)/KMS-STM32.a
KMS_STM32_S_A = ../Libraries/$(CONFIG)_$(PROCESSOR)/KMS-STM32-S.a

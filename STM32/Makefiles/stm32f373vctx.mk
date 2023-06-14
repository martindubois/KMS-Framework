
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022-2023 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/Makefiles/stm32f373vctx.mk

include $(KMS_MAKEFILES)/stm32.mk

CFLAGS += -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -DSTM32F373xC


# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2023 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/Makefiles/stm32h743xx_m7.mk

include $(KMS_MAKEFILES)/stm32.mk

CFLAGS += -mcpu=cortex-m7 -mfpu=fpv5-d16 -DSTM32H743xx

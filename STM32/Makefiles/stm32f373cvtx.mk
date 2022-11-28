
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/Makefiles/stm32f373cvtx.mk

include $(KMS_MAKEFILES)/stm32.mk

CFLAGS += -DSTM32F3 -DSTM32F373xC -DSTM32F373VCTx

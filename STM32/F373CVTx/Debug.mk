
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/F373CVTx/Debug.mk

include ../Common.mk

CFLAGS += -DDEBUG -O0

KMS_A_A = ../Libraries/Debug/KMS-A.a

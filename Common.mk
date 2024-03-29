
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2023 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      Common.mk

UNAME = $(shell uname)

PROCESSOR = $(shell uname -p)

CFLAGS = -DOS_NAME=\"$(UNAME)\" -DOS_PROC=\"$(PROCESSOR)\" @../Args/$(CONFIG).args @../Args/$(PROCESSOR).args

KMS_A_A = ../Libraries/$(CONFIG)_$(PROCESSOR)/KMS-A.a
KMS_B_A = ../Libraries/$(CONFIG)_$(PROCESSOR)/KMS-B.a
KMS_C_A = ../Libraries/$(CONFIG)_$(PROCESSOR)/KMS-C.a

INCLUDES = -I ../Includes

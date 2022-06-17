
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      Release.mk

UNAME = $(shell uname)

UNAME_P = $(shell uname -p)

CFLAGS = -DNDEBUG -DOS_NAME=$(UNAME) -DOS_PROC=$(UNAME_PROC) -fpic -ggdb -O2

INCLUDES = -I ../Includes

KMS_A_A = ../Libraries/Release/KMS-A.a

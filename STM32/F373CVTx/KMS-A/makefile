
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/F373CVTx/KMS-A/makefile

include ../$(CONFIG).mk

OUTPUT = ../Libraries/$(CONFIG)/KMS-A.a

OBJECTS = WOP_FrameBuffer.o \
		  WOP_Object.o      \
		  WOP_Receiver.o    \
		  WOP_Sender.o      \
		  WOP_System.o

# ===== Rules ===============================================================

%.o: ../../../KMS-A/%.cpp
	$(GPP) -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Targets =============================================================

$(OUTPUT) : $(OBJECTS)
	$(AR) r $@ $^

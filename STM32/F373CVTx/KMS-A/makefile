
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/F373CVTx/KMS-A/makefile

include ../$(CONFIG).mk

OUTPUT = ../Libraries/$(CONFIG)/KMS-A.a

# QUESTION Can I move OBJECTS and SOURCES in the makefile in the parent
#          folder ?

OBJECTS = Embedded_USART.o    \
		  Embedded_WorkItem.o \
		  Embedded_WorkLoop.o \
		  Msg_Destination.o   \
		  Msg_IReceiver.o     \
          WOP_FrameBuffer.o   \
		  WOP_Object.o        \
		  WOP_Receiver.o      \
		  WOP_Sender.o        \
		  WOP_System.o

SOURCES = ../../../KMS-A/Embedded_USART.cpp    \
          ../../../KMS-A/Embedded_WorkItem.cpp \
          ../../../KMS-A/Embedded_WorkLoop.cpp \
          ../../../KMS-A/Msg_Destination.cpp   \
		  ../../../KMS-A/Msg_IReceiver.cpp     \
          ../../../KMS-A/WOP_FrameBuffer.cpp   \
          ../../../KMS-A/WOP_Object.cpp        \
		  ../../../KMS-A/WOP_Receiver.cpp      \
	      ../../../KMS-A/WOP_Sender.cpp        \
		  ../../../KMS-A/WOP_System.cpp

# ===== Rules ===============================================================

%.o: ../../../KMS-A/%.cpp
	$(GPP) -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Targets =============================================================

$(OUTPUT) : $(OBJECTS)
	$(AR) r $@ $^

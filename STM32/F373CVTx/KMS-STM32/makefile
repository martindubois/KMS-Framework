
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/F373CVTx/KMS-STM32/makefile

include ../$(CONFIG).mk

OUTPUT = ../Libraries/$(CONFIG)/KMS-STM32.a

OBJECTS = Needed.o \
          STM32F.o

# ===== Rules ===============================================================

%.o: ../../KMS-STM32/%.cpp
	$(GPP) -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Targets =============================================================

$(OUTPUT) : $(OBJECTS)
	$(AR) r $@ $^


# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/F373CVTx/Test/makefile

include ../$(CONFIG).mk

LIBRARIES = $(KMS_STM32_A)

OUTPUT = ../Binaries/$(CONFIG)/Test.elf

SOURCES = Test.cpp

# ===== Macros ==============================================================

OBJECTS = $(SOURCES:.cpp=.o)

# ===== Rules ===============================================================

.cpp.o:
	$(GPP) -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Targets =============================================================

$(OUTPUT): Startup.o $(OBJECTS) $(LIBRARIES)
	$(GPP) $(CFLAGS) -T"../../LinkerScripts/STM32F373VCTX_FLASH.ld" -Wl,-Map="Test.map" -o $@ $^
	$(SIZE) $@

# ===== Specific rules ======================================================

Startup.o: ../../Startup/startup_stm32F373vctx.s
	$(AS) -c $(CFLAGS) -o $@ $<

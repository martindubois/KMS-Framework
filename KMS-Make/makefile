
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      KMS-Make/makefile

ifeq ($(CONFIG), Debug)
	include ../Debug.mk
	OUTPUT = ../Binaries/Debug/KMS-Make
endif

ifeq ($(CONFIG), Release)
	include ../Release.mk
	OUTPUT = ../Binaries/Release/KMS-Make
endif

LIBRARIES = $(KMS_A_A)

SOURCES = KMS-Make.cpp

# ===== Rules ===============================================================

.cpp.o:
	g++ -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Macros ==============================================================

OBJECTS = $(SOURCES:.cpp=.o)

# ===== Targets =============================================================

$(OUTPUT) : $(OBJECTS) $(LIBRARIES)
	g++ -o $@ $^

depend:
	makedepend -T $(INCLUDES) $(SOURCES)

# DO NOT DELETE

KMS-Make.o: ../Includes/KMS/Banner.h ../Includes/KMS/Console/Color.h
KMS-Make.o: ../Includes/KMS/Build/Make.h
KMS-Make.o: ../Includes/KMS/Config/Configurable.h ../Includes/KMS/Types.h
KMS-Make.o: ../Common/Version.h

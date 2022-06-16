
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      KMS-Build/makefile

include ../$(CONFIG).mk

OUTPUT = ../Binaries/$(CONFIG)/KMS-Build

LIBRARIES = $(KMS_A_A)

SOURCES = KMS-Build.cpp

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

KMS-Build.o: ../Includes/KMS/Banner.h ../Includes/KMS/Console/Color.h
KMS-Build.o: ../Includes/KMS/Build/Build.h
KMS-Build.o: ../Includes/KMS/Config/Configurable.h
KMS-Build.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
KMS-Build.o: ../Common/Version.h

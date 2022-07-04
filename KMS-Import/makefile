
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      KMS-Import/makefile

include ../$(CONFIG).mk

OUTPUT = ../Binaries/$(CONFIG)/KMS-Import

LIBRARIES = $(KMS_A_A)

SOURCES = KMS-Import.cpp

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

KMS-Import.o: ../Includes/KMS/Banner.h ../Includes/KMS/Console/Color.h
KMS-Import.o: ../Includes/KMS/Build/Import.h
KMS-Import.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
KMS-Import.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
KMS-Import.o: ../Common/Version.h

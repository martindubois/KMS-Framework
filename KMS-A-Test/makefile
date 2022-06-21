
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      KMS-A-Test/makefile

include ../$(CONFIG).mk

OUTPUT = ../Binaries/$(CONFIG)/KMS-A-Test

LIBRARIES = $(KMS_A_A)

SOURCES = KMS-A-Test.cpp       \
		  Test_TestManager.cpp

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

KMS-A-Test.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
KMS-A-Test.o: /usr/include/features.h /usr/include/features-time64.h
KMS-A-Test.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
KMS-A-Test.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
KMS-A-Test.o: /usr/include/strings.h ../Includes/KMS/Config/Configurator.h
KMS-A-Test.o: ../Includes/KMS/Config/Configurable.h /usr/include/stdio.h
KMS-A-Test.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Exception.h
KMS-A-Test.o: ../Includes/KMS/Console/Color.h ../Includes/KMS/Test/Test.h
KMS-A-Test.o: ../Includes/KMS/Banner.h ../Includes/KMS/Test/TestManager.h
KMS-A-Test.o: ../Common/Version.h
Test_TestManager.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Test_TestManager.o: /usr/include/features.h /usr/include/features-time64.h
Test_TestManager.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Test_TestManager.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Test_TestManager.o: /usr/include/strings.h
Test_TestManager.o: ../Includes/KMS/Config/Configurator.h
Test_TestManager.o: ../Includes/KMS/Config/Configurable.h
Test_TestManager.o: /usr/include/stdio.h ../Includes/KMS/File/Folder.h
Test_TestManager.o: ../Includes/KMS/Exception.h
Test_TestManager.o: ../Includes/KMS/Console/Color.h
Test_TestManager.o: ../Includes/KMS/Test/Test.h
Test_TestManager.o: ../Includes/KMS/Test/TestManager.h


# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      KMS-A/makefile

include ../$(CONFIG).mk

OUTPUT = ../Libraries/$(CONFIG)/KMS-A.a

SOURCES = Build_Build_L.cpp       \
          Build_Build.cpp         \
		  Build_Import.cpp        \
          Build_Make.cpp          \
          Cfg_Configurable.cpp    \
		  Cfg_Configurator.cpp    \
          Console_Color_L.cpp     \
		  Convert.cpp             \
		  Environment_L.cpp       \
		  Environment.cpp         \
          Exception.cpp           \
		  File_Folder_L.cpp       \
          File_Folder.cpp         \
		  Proc_Process_L.cpp      \
		  Proc_Process.cpp        \
		  Test_Test.cpp           \
		  Test_TestManager.cpp    \
          Text_TextFile.cpp       \
          Version.cpp

# ===== Rules ===============================================================

.cpp.o:
	g++ -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Macros ==============================================================

OBJECTS = $(SOURCES:.cpp=.o)

# ===== Targets =============================================================

$(OUTPUT) : $(OBJECTS)
	ar r $@ $^
	ranlib $@

depend:
	makedepend -T $(INCLUDES) $(SOURCES)
	
# DO NOT DELETE

Build_Build_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Build_Build_L.o: /usr/include/features.h /usr/include/features-time64.h
Build_Build_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Build_Build_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Build_Build_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Build_Build_L.o: ../Includes/KMS/Console/Color.h ../Includes/KMS/Build/Make.h
Build_Build_L.o: ../Includes/KMS/Config/Configurable.h /usr/include/stdio.h
Build_Build_L.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
Build_Build_L.o: ../Includes/KMS/Config/Configurator.h
Build_Build_L.o: ../Includes/KMS/Proc/Process.h
Build_Build_L.o: ../Includes/KMS/Build/Build.h
Build_Build.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Build_Build.o: /usr/include/features.h /usr/include/features-time64.h
Build_Build.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Build_Build.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Build_Build.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Build_Build.o: ../Includes/KMS/Console/Color.h
Build_Build.o: ../Includes/KMS/Cfg/Configurator.h
Build_Build.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
Build_Build.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Convert.h
Build_Build.o: ../Includes/KMS/Types.h ../Includes/KMS/Text/TextFile.h
Build_Build.o: ../Includes/KMS/Version.h /usr/include/stdint.h
Build_Build.o: ../Includes/KMS/Build/Build.h
Build_Make.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Build_Make.o: /usr/include/features.h /usr/include/features-time64.h
Build_Make.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Build_Make.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Build_Make.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Build_Make.o: ../Includes/KMS/Console/Color.h /usr/include/unistd.h
Build_Make.o: ../Includes/KMS/Cfg/Configurator.h
Build_Make.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
Build_Make.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Proc/Process.h
Build_Make.o: ../Includes/KMS/Types.h ../Includes/KMS/Build/Make.h
Cfg_Configurable.o: Component.h ../Includes/KMS/Base.h
Cfg_Configurable.o: /usr/include/assert.h /usr/include/features.h
Cfg_Configurable.o: /usr/include/features-time64.h
Cfg_Configurable.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Cfg_Configurable.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Cfg_Configurable.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Cfg_Configurable.o: ../Includes/KMS/Console/Color.h
Cfg_Configurable.o: ../Includes/KMS/Cfg/Configurator.h
Cfg_Configurable.o: ../Includes/KMS/Cfg/Configurable.h
Cfg_Configurable.o: /usr/include/stdio.h ../Includes/KMS/File/Folder.h
Cfg_Configurator.o: Component.h ../Includes/KMS/Base.h
Cfg_Configurator.o: /usr/include/assert.h /usr/include/features.h
Cfg_Configurator.o: /usr/include/features-time64.h
Cfg_Configurator.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Cfg_Configurator.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Cfg_Configurator.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Cfg_Configurator.o: ../Includes/KMS/Console/Color.h
Cfg_Configurator.o: ../Includes/KMS/Cfg/Configurable.h
Cfg_Configurator.o: /usr/include/stdio.h ../Includes/KMS/Environment.h
Cfg_Configurator.o: ../Includes/KMS/Text/TextFile.h
Cfg_Configurator.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
Cfg_Configurator.o: ../Includes/KMS/Cfg/Configurator.h
Console_Color_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Console_Color_L.o: /usr/include/features.h /usr/include/features-time64.h
Console_Color_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Console_Color_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Console_Color_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Console_Color_L.o: ../Includes/KMS/Console/Color.h
Convert.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Convert.o: /usr/include/features.h /usr/include/features-time64.h
Convert.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Convert.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Convert.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Convert.o: ../Includes/KMS/Console/Color.h ../Includes/KMS/Convert.h
Convert.o: ../Includes/KMS/Types.h
Environment_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Environment_L.o: /usr/include/features.h /usr/include/features-time64.h
Environment_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Environment_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Environment_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Environment_L.o: ../Includes/KMS/Console/Color.h
Environment_L.o: ../Includes/KMS/Environment.h
Environment.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Environment.o: /usr/include/features.h /usr/include/features-time64.h
Environment.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Environment.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Environment.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Environment.o: ../Includes/KMS/Console/Color.h ../Includes/KMS/Environment.h
Exception.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Exception.o: /usr/include/features.h /usr/include/features-time64.h
Exception.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Exception.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Exception.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Exception.o: ../Includes/KMS/Console/Color.h /usr/include/stdint.h
File_Folder_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
File_Folder_L.o: /usr/include/features.h /usr/include/features-time64.h
File_Folder_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
File_Folder_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
File_Folder_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
File_Folder_L.o: ../Includes/KMS/Console/Color.h /usr/include/unistd.h
File_Folder_L.o: ../Includes/KMS/Proc/Process.h
File_Folder_L.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
File_Folder.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
File_Folder.o: /usr/include/features.h /usr/include/features-time64.h
File_Folder.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
File_Folder.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
File_Folder.o: /usr/include/strings.h ../Includes/KMS/Exception.h
File_Folder.o: ../Includes/KMS/Console/Color.h ../Includes/KMS/Environment.h
File_Folder.o: ../Includes/KMS/File/Folder.h
Proc_Process_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Proc_Process_L.o: /usr/include/features.h /usr/include/features-time64.h
Proc_Process_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Proc_Process_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Proc_Process_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Proc_Process_L.o: ../Includes/KMS/Console/Color.h /usr/include/unistd.h
Proc_Process_L.o: ../Includes/KMS/Proc/Process.h
Proc_Process_L.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
Proc_Process.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Proc_Process.o: /usr/include/features.h /usr/include/features-time64.h
Proc_Process.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Proc_Process.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Proc_Process.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Proc_Process.o: ../Includes/KMS/Console/Color.h
Proc_Process.o: ../Includes/KMS/Proc/Process.h
Proc_Process.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
Test_Test.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Test_Test.o: /usr/include/features.h /usr/include/features-time64.h
Test_Test.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Test_Test.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Test_Test.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Test_Test.o: ../Includes/KMS/Console/Color.h ../Includes/KMS/Test/Test.h
Test_TestManager.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Test_TestManager.o: /usr/include/features.h /usr/include/features-time64.h
Test_TestManager.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Test_TestManager.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Test_TestManager.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Test_TestManager.o: ../Includes/KMS/Console/Color.h
Test_TestManager.o: ../Includes/KMS/Cfg/Configurator.h
Test_TestManager.o: ../Includes/KMS/Cfg/Configurable.h
Test_TestManager.o: /usr/include/stdio.h ../Includes/KMS/File/Folder.h
Test_TestManager.o: ../Includes/KMS/Test/TestManager.h
Test_TestManager.o: ../Includes/KMS/Test/Test.h
Text_TextFile.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Text_TextFile.o: /usr/include/features.h /usr/include/features-time64.h
Text_TextFile.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Text_TextFile.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Text_TextFile.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Text_TextFile.o: ../Includes/KMS/Console/Color.h
Text_TextFile.o: ../Includes/KMS/Text/TextFile.h
Text_TextFile.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Types.h
Version.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Version.o: /usr/include/features.h /usr/include/features-time64.h
Version.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Version.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Version.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Version.o: ../Includes/KMS/Console/Color.h ../Includes/KMS/Version.h
Version.o: /usr/include/stdint.h ../Includes/KMS/File/Folder.h
Version.o: ../Includes/KMS/Text/TextFile.h ../Includes/KMS/Types.h

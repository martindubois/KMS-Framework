
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
		  Dbg_Log.cpp             \
		  Dbg_LogFile.cpp         \
		  Environment_L.cpp       \
		  Environment.cpp         \
          Exception.cpp           \
		  File_Folder_L.cpp       \
          File_Folder.cpp         \
		  OS_L.cpp                \
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
Build_Build_L.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Build_Build_L.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Build_Build_L.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Build_Build_L.o: /usr/include/stdint.h ../Includes/KMS/Build/Make.h
Build_Build_L.o: ../Includes/KMS/Types.h ../Includes/KMS/Cfg/Configurator.h
Build_Build_L.o: ../Includes/KMS/Proc/Process.h ../Includes/KMS/Build/Build.h
Build_Build.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Build_Build.o: /usr/include/features.h /usr/include/features-time64.h
Build_Build.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Build_Build.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Build_Build.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Build_Build.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Build_Build.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Build_Build.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Build_Build.o: /usr/include/stdint.h ../Includes/KMS/Cfg/Configurator.h
Build_Build.o: ../Includes/KMS/Convert.h ../Includes/KMS/Types.h
Build_Build.o: ../Includes/KMS/Text/TextFile.h ../Includes/KMS/Build/Build.h
Build_Import.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Build_Import.o: /usr/include/features.h /usr/include/features-time64.h
Build_Import.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Build_Import.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Build_Import.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Build_Import.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Build_Import.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Build_Import.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Build_Import.o: /usr/include/stdint.h ../Includes/KMS/Cfg/Configurator.h
Build_Import.o: ../Includes/KMS/Build/Import.h ../Includes/KMS/Types.h
Build_Make.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Build_Make.o: /usr/include/features.h /usr/include/features-time64.h
Build_Make.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Build_Make.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Build_Make.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Build_Make.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Build_Make.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Build_Make.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Build_Make.o: /usr/include/stdint.h /usr/include/unistd.h
Build_Make.o: ../Includes/KMS/Cfg/Configurator.h
Build_Make.o: ../Includes/KMS/Proc/Process.h ../Includes/KMS/Types.h
Build_Make.o: ../Includes/KMS/Build/Make.h
Cfg_Configurable.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Cfg_Configurable.o: /usr/include/features.h /usr/include/features-time64.h
Cfg_Configurable.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Cfg_Configurable.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Cfg_Configurable.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Cfg_Configurable.o: ../Includes/KMS/Dbg/Log.h
Cfg_Configurable.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
Cfg_Configurable.o: ../Includes/KMS/Dbg/LogFile.h
Cfg_Configurable.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Cfg_Configurable.o: /usr/include/stdint.h ../Includes/KMS/Cfg/Configurator.h
Cfg_Configurator.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Cfg_Configurator.o: /usr/include/features.h /usr/include/features-time64.h
Cfg_Configurator.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Cfg_Configurator.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Cfg_Configurator.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Cfg_Configurator.o: ../Includes/KMS/Dbg/Log.h
Cfg_Configurator.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
Cfg_Configurator.o: ../Includes/KMS/Dbg/LogFile.h
Cfg_Configurator.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Cfg_Configurator.o: /usr/include/stdint.h ../Includes/KMS/Environment.h
Cfg_Configurator.o: ../Includes/KMS/Text/TextFile.h ../Includes/KMS/Types.h
Cfg_Configurator.o: ../Includes/KMS/Cfg/Configurator.h
Console_Color_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Console_Color_L.o: /usr/include/features.h /usr/include/features-time64.h
Console_Color_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Console_Color_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Console_Color_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Console_Color_L.o: ../Includes/KMS/Dbg/Log.h
Console_Color_L.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
Console_Color_L.o: ../Includes/KMS/Dbg/LogFile.h
Console_Color_L.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Console_Color_L.o: /usr/include/stdint.h ../Includes/KMS/Console/Color.h
Convert.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Convert.o: /usr/include/features.h /usr/include/features-time64.h
Convert.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Convert.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Convert.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Convert.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Convert.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Convert.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Convert.o: /usr/include/stdint.h ../Includes/KMS/Convert.h
Convert.o: ../Includes/KMS/Types.h
Environment_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Environment_L.o: /usr/include/features.h /usr/include/features-time64.h
Environment_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Environment_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Environment_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Environment_L.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Environment_L.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Environment_L.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Environment_L.o: /usr/include/stdint.h ../Includes/KMS/Environment.h
Environment.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Environment.o: /usr/include/features.h /usr/include/features-time64.h
Environment.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Environment.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Environment.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Environment.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Environment.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Environment.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Environment.o: /usr/include/stdint.h ../Includes/KMS/Environment.h
Exception.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Exception.o: /usr/include/features.h /usr/include/features-time64.h
Exception.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Exception.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Exception.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Exception.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Exception.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Exception.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Exception.o: /usr/include/stdint.h
File_Folder_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
File_Folder_L.o: /usr/include/features.h /usr/include/features-time64.h
File_Folder_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
File_Folder_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
File_Folder_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
File_Folder_L.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
File_Folder_L.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
File_Folder_L.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
File_Folder_L.o: /usr/include/stdint.h /usr/include/unistd.h
File_Folder_L.o: ../Includes/KMS/Proc/Process.h ../Includes/KMS/Types.h
File_Folder.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
File_Folder.o: /usr/include/features.h /usr/include/features-time64.h
File_Folder.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
File_Folder.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
File_Folder.o: /usr/include/strings.h ../Includes/KMS/Exception.h
File_Folder.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
File_Folder.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
File_Folder.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
File_Folder.o: /usr/include/stdint.h ../Includes/KMS/Console/Color.h
File_Folder.o: ../Includes/KMS/Environment.h
Proc_Process_L.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Proc_Process_L.o: /usr/include/features.h /usr/include/features-time64.h
Proc_Process_L.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Proc_Process_L.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Proc_Process_L.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Proc_Process_L.o: ../Includes/KMS/Dbg/Log.h
Proc_Process_L.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
Proc_Process_L.o: ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/File/Folder.h
Proc_Process_L.o: ../Includes/KMS/Version.h /usr/include/stdint.h
Proc_Process_L.o: /usr/include/unistd.h ../Includes/KMS/Proc/Process.h
Proc_Process_L.o: ../Includes/KMS/Types.h
Proc_Process.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Proc_Process.o: /usr/include/features.h /usr/include/features-time64.h
Proc_Process.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Proc_Process.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Proc_Process.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Proc_Process.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Proc_Process.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Proc_Process.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Proc_Process.o: /usr/include/stdint.h ../Includes/KMS/Proc/Process.h
Proc_Process.o: ../Includes/KMS/Types.h
Test_Test.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Test_Test.o: /usr/include/features.h /usr/include/features-time64.h
Test_Test.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Test_Test.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Test_Test.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Test_Test.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Test_Test.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Test_Test.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Test_Test.o: /usr/include/stdint.h ../Includes/KMS/Console/Color.h
Test_Test.o: ../Includes/KMS/Test/Test.h
Test_TestManager.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Test_TestManager.o: /usr/include/features.h /usr/include/features-time64.h
Test_TestManager.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Test_TestManager.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Test_TestManager.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Test_TestManager.o: ../Includes/KMS/Dbg/Log.h
Test_TestManager.o: ../Includes/KMS/Cfg/Configurable.h /usr/include/stdio.h
Test_TestManager.o: ../Includes/KMS/Dbg/LogFile.h
Test_TestManager.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Test_TestManager.o: /usr/include/stdint.h ../Includes/KMS/Cfg/Configurator.h
Test_TestManager.o: ../Includes/KMS/Console/Color.h
Test_TestManager.o: ../Includes/KMS/Test/TestManager.h
Test_TestManager.o: ../Includes/KMS/Test/Test.h
Text_TextFile.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Text_TextFile.o: /usr/include/features.h /usr/include/features-time64.h
Text_TextFile.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Text_TextFile.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Text_TextFile.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Text_TextFile.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Text_TextFile.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Text_TextFile.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Text_TextFile.o: /usr/include/stdint.h ../Includes/KMS/Text/TextFile.h
Text_TextFile.o: ../Includes/KMS/Types.h
Version.o: Component.h ../Includes/KMS/Base.h /usr/include/assert.h
Version.o: /usr/include/features.h /usr/include/features-time64.h
Version.o: /usr/include/stdc-predef.h ../Includes/KMS/SafeAPI.h
Version.o: ../Includes/KMS/WindowsOnLinux.h /usr/include/string.h
Version.o: /usr/include/strings.h ../Includes/KMS/Exception.h
Version.o: ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Cfg/Configurable.h
Version.o: /usr/include/stdio.h ../Includes/KMS/Dbg/LogFile.h
Version.o: ../Includes/KMS/File/Folder.h ../Includes/KMS/Version.h
Version.o: /usr/include/stdint.h ../Includes/KMS/Text/TextFile.h
Version.o: ../Includes/KMS/Types.h

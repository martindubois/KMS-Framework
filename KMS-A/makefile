
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022-2024 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      KMS-A/makefile

include ../Common.mk

OUTPUT = $(KMS_A_A)

SOURCES = Build_Build_L.cpp       \
          Build_Build.cpp         \
		  Build_Depend.cpp        \
          Build_Make.cpp          \
		  Build_Package.cpp       \
		  Callback.cpp            \
		  Cfg_Configurator.cpp    \
		  Cfg_MetaData.cpp        \
		  CLI_Module.cpp          \
		  CLI_Tool.cpp            \
          Console_Color_L.cpp     \
		  Console_HumanScript.cpp \
		  Console_Redirection_L.cpp \
		  Console_Redirection.cpp \
		  Convert.cpp             \
		  DAQ_AnalogConverter.cpp \
		  DAQ_AnalogInput.cpp     \
		  DAQ_AnalogInputConverter.cpp \
		  DAQ_AnalogOutput.cpp    \
		  DAQ_AnalogOutputConverter.cpp \
		  DAQ_DigitalInput.cpp    \
		  DAQ_DigitalOutput.cpp   \
		  DAQ_Dummy.cpp           \
		  Dbg_Log_L.cpp           \
		  Dbg_Log.cpp             \
		  Dbg_Log_Cfg.cpp         \
		  Dbg_LogFile.cpp         \
		  Dbg_Stats_L.cpp         \
		  Dbg_Stats.cpp           \
		  Dbg_Stats_Entry.cpp     \
		  Dbg_Stats_Timer.cpp     \
		  DI_Array.cpp            \
		  DI_Array_Sparse.cpp     \
		  DI_Boolean.cpp          \
		  DI_Container.cpp        \
		  DI_Dictionary.cpp       \
		  DI_File.cpp             \
		  DI_Folder.cpp           \
		  DI_Functions.cpp        \
		  DI_MetaData.cpp         \
		  DI_Object.cpp           \
		  DI_String.cpp           \
		  DI_String_Expand.cpp    \
		  Embedded_Assert.cpp     \
		  Embedded_SPI.cpp        \
		  Embedded_WorkItem.cpp   \
		  Embedded_WorkLoop.cpp   \
		  Environment_L.cpp       \
		  Environment.cpp         \
		  Exception_L.cpp         \
          Exception.cpp           \
		  File_Binary_L.cpp       \
		  File_Binary.cpp         \
		  File_Folder_L.cpp       \
          File_Folder.cpp         \
		  File_Sync.cpp           \
		  FileFormat_BMP.cpp      \
		  FileFormat_ELF.cpp      \
		  Graph_Bitmap.cpp        \
		  Graph_Color.cpp         \
		  Graph_Histogram.cpp     \
		  Graph_Point.cpp         \
		  HTTP.cpp                \
		  Install_Installer.cpp   \
		  Install_Installer_L.cpp \
		  JSON.cpp                \
		  Modbus_CRC.cpp          \
		  Modbus_Master_Cfg.cpp   \
		  Modbus_Master_IDevice.cpp \
		  Modbus_Master.cpp       \
		  Modbus_Monitor.cpp      \
		  Modbus_Slave_Cfg.cpp    \
		  Modbus_Slave_Serial.cpp \
		  Modbus_Slave_USART.cpp  \
		  Modbus_Slave.cpp        \
		  Modbus.cpp              \
		  Msg_Destination.cpp     \
		  Msg_IReceiver.cpp       \
		  OS_L.cpp                \
		  Proc_Process_L.cpp      \
		  Proc_Process.cpp        \
		  RefCount.cpp            \
		  Result.cpp              \
		  Saturated.cpp           \
		  ScaleAndOffset.cpp      \
		  Scope_Channel_DAQ.cpp   \
		  Scope_Channel_Random.cpp \
		  Scope_Channel.cpp       \
		  Scope_Scope.cpp         \
		  Stats_Histogram.cpp     \
		  Stats_Stats.cpp         \
		  Test_Test.cpp           \
		  Test_TestManager.cpp    \
          Text_File_ASCII.cpp     \
		  Text_File_UTF16.cpp     \
		  Text_ReadPtr.cpp        \
		  Text_WritePtr.cpp       \
		  Thread_Gate_L.cpp       \
		  Thread_Lock.cpp         \
		  Thread_Thread_L.cpp     \
		  Thread_Thread.cpp       \
          Version.cpp             \
		  WOP_FrameBuffer.cpp     \
		  WOP_Object_Dynamic.cpp  \
		  WOP_Object.cpp          \
		  WOP_System.cpp

# ===== Rules ===============================================================

.cpp.o:
	g++ -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Macros ==============================================================

OBJECTS = $(SOURCES:.cpp=.o)

# ===== Targets =============================================================

$(OUTPUT) : $(OBJECTS)
	ar r $@ $^
	ranlib $@

# DO NOT DELETE - Generated by KMS::Build::Make !

Build_Build.o: ../Includes/KMS/Base.h ../Includes/KMS/Build/Build.h ../Includes/KMS/Build/Make.h ../Includes/KMS/Build/Package.h ../Includes/KMS/CLI/Module.h ../Includes/KMS/CLI/Tool.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/Configurator.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/Console/Color.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Enum.h ../Includes/KMS/DI/File.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Log_Cfg.h ../Includes/KMS/Dbg/Stats.h ../Includes/KMS/Dbg/Stats_Entry.h ../Includes/KMS/Dbg/Stats_Timer.h ../Includes/KMS/Dbg/Stats_Value.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Install/Installer.h ../Includes/KMS/Main.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_ASCII.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Build_Build_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Build/Build.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Proc/Process.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Build_Depend.o: ../Includes/KMS/Base.h ../Includes/KMS/Build/Depend.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_ASCII.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Build_Make.o: ../Includes/KMS/Base.h ../Includes/KMS/Build/Depend.h ../Includes/KMS/Build/Make.h ../Includes/KMS/CLI/Module.h ../Includes/KMS/CLI/Tool.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/Configurator.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Enum.h ../Includes/KMS/DI/File.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Log_Cfg.h ../Includes/KMS/Dbg/Stats.h ../Includes/KMS/Dbg/Stats_Entry.h ../Includes/KMS/Dbg/Stats_Timer.h ../Includes/KMS/Dbg/Stats_Value.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Install/Installer.h ../Includes/KMS/Main.h ../Includes/KMS/Proc/Process.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_ASCII.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Build_Package.o: ../Includes/KMS/Base.h ../Includes/KMS/Build/Package.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
CLI_Module.o: ../Includes/KMS/Base.h ../Includes/KMS/CLI/Module.h ../Includes/KMS/SafeAPI.h Component.h
CLI_Tool.o: ../Includes/KMS/Base.h ../Includes/KMS/CLI/Module.h ../Includes/KMS/CLI/Tool.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/Console/Color.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Functions.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_ASCII.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Callback.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/SafeAPI.h Component.h
Cfg_Configurator.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/Configurator.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Array_Sparse.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/File.h ../Includes/KMS/DI/Functions.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_ASCII.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Cfg_MetaData.o: ../Includes/KMS/Base.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/SafeAPI.h Component.h
Console_Color_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Console/Color.h ../Includes/KMS/SafeAPI.h Component.h
Console_HumanScript.o: ../Includes/KMS/Base.h ../Includes/KMS/Console/Color.h ../Includes/KMS/Console/HumanScript.h ../Includes/KMS/SafeAPI.h Component.h
Console_Redirection.o: ../Includes/KMS/Base.h ../Includes/KMS/Console/Redirection.h ../Includes/KMS/File/Folder.h ../Includes/KMS/OS.h ../Includes/KMS/SafeAPI.h Component.h
Console_Redirection_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Console/Redirection.h ../Includes/KMS/File/Folder.h ../Includes/KMS/OS.h ../Includes/KMS/SafeAPI.h Component.h
Convert.o: ../Includes/KMS/Base.h ../Includes/KMS/Convert.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h Component.h
DAQ_AnalogConverter.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/AnalogConverter.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/ScaleAndOffset.h Component.h
DAQ_AnalogInput.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/AnalogInput.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/IAnalogInputs.h ../Includes/KMS/SafeAPI.h Component.h
DAQ_AnalogInputConverter.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/AnalogConverter.h ../Includes/KMS/DAQ/AnalogInputConverter.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/IAnalogInputs.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/ScaleAndOffset.h Component.h
DAQ_AnalogOutput.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/AnalogOutput.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/IAnalogOutputs.h ../Includes/KMS/SafeAPI.h Component.h
DAQ_AnalogOutputConverter.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/AnalogConverter.h ../Includes/KMS/DAQ/AnalogOutputConverter.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/IAnalogOutputs.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/ScaleAndOffset.h Component.h
DAQ_DigitalInput.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/DigitalInput.h ../Includes/KMS/DAQ/IDigitalInputs.h ../Includes/KMS/SafeAPI.h Component.h
DAQ_DigitalOutput.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/DigitalOutput.h ../Includes/KMS/DAQ/IDigitalOutputs.h ../Includes/KMS/SafeAPI.h Component.h
DAQ_Dummy.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/Dummy.h ../Includes/KMS/DAQ/IAnalogInputs.h ../Includes/KMS/DAQ/IAnalogOutputs.h ../Includes/KMS/DAQ/IDigitalInputs.h ../Includes/KMS/DAQ/IDigitalOutputs.h ../Includes/KMS/SafeAPI.h Component.h
DI_Array.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Object.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
DI_Array_Sparse.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Array_Sparse.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Object.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
DI_Boolean.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Convert.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/Value.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h Component.h
DI_Container.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Object.h ../Includes/KMS/SafeAPI.h Component.h
DI_Dictionary.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Object.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
DI_File.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/File.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/SafeAPI.h Component.h
DI_Folder.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h Component.h
DI_Functions.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Functions.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
DI_MetaData.o: ../Includes/KMS/Base.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/SafeAPI.h Component.h
DI_Object.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Object.h ../Includes/KMS/SafeAPI.h Component.h
DI_String.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/Value.h ../Includes/KMS/SafeAPI.h Component.h
DI_String_Expand.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Environment.h ../Includes/KMS/SafeAPI.h Component.h
Dbg_Log.o: ../Includes/KMS/Base.h ../Includes/KMS/Console/Color.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/OS.h ../Includes/KMS/Proc/Process.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Dbg_LogFile.o: ../Common/Version.h ../Includes/KMS/Base.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/File/Folder.h ../Includes/KMS/OS.h ../Includes/KMS/SafeAPI.h Component.h
Dbg_Log_Cfg.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Enum.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Log_Cfg.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
Dbg_Log_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
Dbg_Stats.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Stats.h ../Includes/KMS/Dbg/Stats_Entry.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/OS.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
Dbg_Stats_Entry.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Stats.h ../Includes/KMS/Dbg/Stats_Entry.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
Dbg_Stats_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Stats.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
Dbg_Stats_Timer.o: ../Includes/KMS/Base.h ../Includes/KMS/Dbg/Stats_Entry.h ../Includes/KMS/Dbg/Stats_Timer.h ../Includes/KMS/Dbg/Stats_Value.h ../Includes/KMS/OS.h ../Includes/KMS/SafeAPI.h Component.h
Embedded_Assert.o: ../Includes/KMS/Base.h ../Includes/KMS/Embedded/Assert.h ../Includes/KMS/SafeAPI.h Component.h
Embedded_SPI.o: ../Includes/KMS/Base.h ../Includes/KMS/Embedded/SPI.h ../Includes/KMS/SafeAPI.h Component.h
Embedded_WorkItem.o: ../Includes/KMS/Base.h ../Includes/KMS/Embedded/WorkItem.h ../Includes/KMS/SafeAPI.h Component.h
Embedded_WorkLoop.o: ../Includes/KMS/Base.h ../Includes/KMS/Embedded/WorkItem.h ../Includes/KMS/Embedded/WorkLoop.h ../Includes/KMS/SafeAPI.h Component.h
Environment.o: ../Includes/KMS/Base.h ../Includes/KMS/Environment.h ../Includes/KMS/SafeAPI.h Component.h
Environment_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Environment.h ../Includes/KMS/SafeAPI.h Component.h
Exception.o: ../Includes/KMS/Base.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h Component.h
Exception_L.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h Component.h
FileFormat_BMP.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Binary.h ../Includes/KMS/File/Folder.h ../Includes/KMS/FileFormat/BMP.h ../Includes/KMS/SafeAPI.h Component.h
FileFormat_ELF.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Binary.h ../Includes/KMS/File/Folder.h ../Includes/KMS/FileFormat/ELF.h ../Includes/KMS/SafeAPI.h Component.h
File_Binary.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Binary.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h Component.h
File_Binary_L.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Binary.h ../Includes/KMS/SafeAPI.h Component.h
File_Folder.o: ../Includes/KMS/Base.h ../Includes/KMS/Console/Color.h ../Includes/KMS/Environment.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h Component.h
File_Folder_L.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Proc/Process.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h Component.h
File_Sync.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/Configurator.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Enum.h ../Includes/KMS/DI/File.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Log_Cfg.h ../Includes/KMS/Dbg/Stats.h ../Includes/KMS/Dbg/Stats_Entry.h ../Includes/KMS/Dbg/Stats_Timer.h ../Includes/KMS/Dbg/Stats_Value.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/FileInfoList.h ../Includes/KMS/File/Folder.h ../Includes/KMS/File/Sync.h ../Includes/KMS/Install/Installer.h ../Includes/KMS/Main.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Graph_Bitmap.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Binary.h ../Includes/KMS/File/Folder.h ../Includes/KMS/FileFormat/BMP.h ../Includes/KMS/Graph/Bitmap.h ../Includes/KMS/Graph/Color.h ../Includes/KMS/Graph/Point.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Saturated.h Component.h
Graph_Color.o: ../Includes/KMS/Base.h ../Includes/KMS/Graph/Color.h ../Includes/KMS/SafeAPI.h Component.h
Graph_Histogram.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Binary.h ../Includes/KMS/File/Folder.h ../Includes/KMS/FileFormat/BMP.h ../Includes/KMS/Graph/Bitmap.h ../Includes/KMS/Graph/Color.h ../Includes/KMS/Graph/Histogram.h ../Includes/KMS/Graph/Point.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Stats/Histogram.h ../Includes/KMS/Stats/Stats.h Component.h
Graph_Point.o: ../Includes/KMS/Base.h ../Includes/KMS/Graph/Point.h ../Includes/KMS/SafeAPI.h Component.h
HTTP.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Convert.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/UInt.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/HTTP/HTTP.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/ReadPtr.h ../Includes/KMS/Text/WritePtr.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Install_Installer.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Install/Installer.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
Install_Installer_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Install/Installer.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h Component.h
JSON.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Convert.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/UInt.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/JSON/JSON.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/ReadPtr.h ../Includes/KMS/Text/WritePtr.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Modbus.o: ../Includes/KMS/Base.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/SafeAPI.h Component.h
Modbus_CRC.o: ../Includes/KMS/Base.h ../Includes/KMS/Modbus/CRC.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/SafeAPI.h Component.h
Modbus_Master.o: ../Includes/KMS/Base.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/Modbus/Master.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/SafeAPI.h Component.h
Modbus_Master_Cfg.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/Convert.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/UInt.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Modbus/Master.h ../Includes/KMS/Modbus/Master_Cfg.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Modbus_Master_IDevice.o: ../Includes/KMS/Base.h ../Includes/KMS/Dev/IDevice.h ../Includes/KMS/Modbus/CRC.h ../Includes/KMS/Modbus/Master.h ../Includes/KMS/Modbus/Master_IDevice.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/SafeAPI.h Component.h
Modbus_Monitor.o: ../Includes/KMS/Base.h ../Includes/KMS/Modbus/Master.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/Modbus/Monitor.h ../Includes/KMS/SafeAPI.h Component.h
Modbus_Slave.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/Modbus/Slave.h ../Includes/KMS/SafeAPI.h Component.h
Modbus_Slave_Cfg.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/Convert.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/UInt.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/Modbus/Slave.h ../Includes/KMS/Modbus/Slave_Cfg.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
Modbus_Slave_Serial.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Modbus/CRC.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/Modbus/Slave.h ../Includes/KMS/Modbus/Slave_Serial.h ../Includes/KMS/SafeAPI.h Component.h
Modbus_Slave_USART.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Embedded/USART.h ../Includes/KMS/Embedded/WorkItem.h ../Includes/KMS/Modbus/CRC.h ../Includes/KMS/Modbus/Modbus.h ../Includes/KMS/Modbus/Slave.h ../Includes/KMS/Modbus/Slave_Serial.h ../Includes/KMS/Modbus/Slave_USART.h ../Includes/KMS/SafeAPI.h Component.h
Msg_Destination.o: ../Includes/KMS/Base.h ../Includes/KMS/Msg/Destination.h ../Includes/KMS/Msg/IReceiver.h ../Includes/KMS/SafeAPI.h Component.h
Msg_IReceiver.o: ../Includes/KMS/Base.h ../Includes/KMS/Msg/IReceiver.h ../Includes/KMS/SafeAPI.h Component.h
OS_L.o: ../Includes/KMS/Base.h ../Includes/KMS/OS.h ../Includes/KMS/SafeAPI.h Component.h
Proc_Process.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Proc/Process.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h Component.h
Proc_Process_L.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Proc/Process.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Types.h Component.h
RefCount.o: ../Includes/KMS/Base.h ../Includes/KMS/RefCount.h ../Includes/KMS/SafeAPI.h Component.h
Result.o: ../Includes/KMS/Base.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h Component.h
Saturated.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Saturated.h Component.h
ScaleAndOffset.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/ScaleAndOffset.h Component.h
Scope_Channel.o: ../Includes/KMS/Base.h ../Includes/KMS/Graph/Color.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Scope/Channel.h ../Includes/KMS/Stats/Stats.h Component.h
Scope_Channel_DAQ.o: ../Includes/KMS/Base.h ../Includes/KMS/DAQ/AnalogInput.h ../Includes/KMS/DAQ/AnalogOutput.h ../Includes/KMS/DAQ/DAQ.h ../Includes/KMS/DAQ/DigitalInput.h ../Includes/KMS/DAQ/DigitalOutput.h ../Includes/KMS/Graph/Color.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Scope/Channel.h ../Includes/KMS/Scope/Channel_DAQ.h ../Includes/KMS/Stats/Stats.h Component.h
Scope_Channel_Random.o: ../Includes/KMS/Base.h ../Includes/KMS/Graph/Color.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Scope/Channel.h ../Includes/KMS/Scope/Channel_Random.h ../Includes/KMS/Stats/Stats.h Component.h
Scope_Scope.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/File/Binary.h ../Includes/KMS/File/Folder.h ../Includes/KMS/FileFormat/BMP.h ../Includes/KMS/Graph/Bitmap.h ../Includes/KMS/Graph/Color.h ../Includes/KMS/Graph/Point.h ../Includes/KMS/OS.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Scope/Channel.h ../Includes/KMS/Scope/Scope.h ../Includes/KMS/Stats/Stats.h Component.h
Stats_Histogram.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Stats/Histogram.h ../Includes/KMS/Stats/Stats.h Component.h
Stats_Stats.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Stats/Stats.h Component.h
Test_Test.o: ../Includes/KMS/Base.h ../Includes/KMS/Console/Color.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Test/Test.h Component.h
Test_TestManager.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/Cfg/Configurator.h ../Includes/KMS/Cfg/MetaData.h ../Includes/KMS/Console/Color.h ../Includes/KMS/DI/Array.h ../Includes/KMS/DI/Boolean.h ../Includes/KMS/DI/Container.h ../Includes/KMS/DI/Dictionary.h ../Includes/KMS/DI/Enum.h ../Includes/KMS/DI/File.h ../Includes/KMS/DI/Folder.h ../Includes/KMS/DI/MetaData.h ../Includes/KMS/DI/Object.h ../Includes/KMS/DI/String.h ../Includes/KMS/DI/String_Expand.h ../Includes/KMS/DI/Value.h ../Includes/KMS/Dbg/Log.h ../Includes/KMS/Dbg/LogFile.h ../Includes/KMS/Dbg/Log_Cfg.h ../Includes/KMS/Dbg/Stats.h ../Includes/KMS/Dbg/Stats_Entry.h ../Includes/KMS/Dbg/Stats_Timer.h ../Includes/KMS/Dbg/Stats_Value.h ../Includes/KMS/Enum.h ../Includes/KMS/Exception.h ../Includes/KMS/File/Folder.h ../Includes/KMS/Install/Installer.h ../Includes/KMS/Main.h ../Includes/KMS/Ptr.h ../Includes/KMS/Result.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Test/Test.h ../Includes/KMS/Test/TestManager.h ../Includes/KMS/Version.h Component.h
Text_File_ASCII.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_ASCII.h ../Includes/KMS/Types.h Component.h
Text_File_UTF16.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_UTF16.h ../Includes/KMS/Types.h Component.h
Text_ReadPtr.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/ReadPtr.h Component.h
Text_WritePtr.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/WritePtr.h Component.h
Thread_Gate_L.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Thread/Gate.h Component.h
Thread_Lock.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Thread/Gate.h ../Includes/KMS/Thread/Lock.h Component.h
Thread_Thread.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Thread/Gate.h ../Includes/KMS/Thread/Lock.h ../Includes/KMS/Thread/Thread.h Component.h
Thread_Thread_L.o: ../Includes/KMS/Base.h ../Includes/KMS/Callback.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Thread/Gate.h ../Includes/KMS/Thread/Thread.h Component.h
Version.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Text/File_ASCII.h ../Includes/KMS/Types.h ../Includes/KMS/Version.h Component.h
WOP_FrameBuffer.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/WOP/FrameBuffer.h ../Includes/KMS/WOP/WOP.h Component.h
WOP_Object.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/WOP/FrameBuffer.h ../Includes/KMS/WOP/Object.h ../Includes/KMS/WOP/WOP.h Component.h
WOP_Object_Dynamic.o: ../Includes/KMS/Base.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/WOP/FrameBuffer.h ../Includes/KMS/WOP/Object.h ../Includes/KMS/WOP/Object_Dynamic.h ../Includes/KMS/WOP/WOP.h Component.h
WOP_System.o: ../Includes/KMS/Base.h ../Includes/KMS/File/Folder.h ../Includes/KMS/SafeAPI.h ../Includes/KMS/Version.h ../Includes/KMS/WOP/FrameBuffer.h ../Includes/KMS/WOP/Object.h ../Includes/KMS/WOP/System.h ../Includes/KMS/WOP/WOP.h Component.h


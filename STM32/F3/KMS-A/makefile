
# Author    KMS - Martin Dubois, P. Eng.
# Copyright (C) 2022-2023 KMS
# License   http://www.apache.org/licenses/LICENSE-2.0
# Product   KMS-Framework
# File      STM32/F3/KMS-A/makefile

include ../../Common.mk

OUTPUT = $(KMS_A_A)

OBJECTS = DAQ_DigitalInput.o      \
          DAQ_DigitalOutput.o     \
          Embedded_Assert.o       \
          Embedded_SPI.o          \
          Embedded_Test.o         \
          Embedded_USART.o        \
          Embedded_WorkItem.o     \
          Embedded_WorkLoop.o     \
          Modbus.o                \
          Modbus_CRC.o            \
          Modbus_Master.o         \
          Modbus_Master_IDevice.o \
          Modbus_Monitor.o        \
          Modbus_Slave.o          \
          Modbus_Slave_Serial.o   \
          Modbus_Slave_USART.o    \
          Msg_Destination.o       \
          Msg_IReceiver.o         \
          Version.o               \
          WOP_FrameBuffer.o       \
          WOP_Link_USART.o        \
          WOP_Object.o            \
          WOP_System.o

SOURCES = ../../../KMS-A/DAQ_DigitalInput.cpp      \
          ../../../KMS-A/DAQ_DigitalOutput.cpp     \
          ../../../KMS-A/Embedded_Assert.cpp       \
          ../../../KMS-A/Embedded_SPI.cpp          \
          ../../../KMS-A/Embedded_Test.cpp         \
          ../../../KMS-A/Embedded_USART.cpp        \
          ../../../KMS-A/Embedded_WorkItem.cpp     \
          ../../../KMS-A/Embedded_WorkLoop.cpp     \
          ../../../KMS-A/Modbus.cpp                \
          ../../../KMS-A/Modbus_CRC.cpp            \
          ../../../KMS-A/Modbus_Master.cpp         \
          ../../../KMS-A/Modbus_Master_IDevice.cpp \
          ../../../KMS-A/Modbus_Monitor.cpp        \
          ../../../KMS-A/Modbus_Slave.cpp          \
          ../../../KMS-A/Modbus_Slave_Serial.cpp   \
          ../../../KMS-A/Modbus_Slave_USART.cpp    \
          ../../../KMS-A/Msg_Destination.cpp       \
          ../../../KMS-A/Msg_IReceiver.cpp         \
          ../../../KMS-A/Version.cpp               \
          ../../../KMS-A/WOP_FrameBuffer.cpp       \
          ../../../KMS-A/WOP_Link_USART.cpp        \
          ../../../KMS-A/WOP_Object.cpp            \
          ../../../KMS-A/WOP_System.cpp

# ===== Rules ===============================================================

%.o: ../../../KMS-A/%.cpp
	$(GPP) -c $(CFLAGS) -o $@ $(INCLUDES) $<

# ===== Targets =============================================================

$(OUTPUT) : $(OBJECTS)
	$(AR) r $@ $^

# DO NOT DELETE - Generated by KMS::Build::Make !

../../../KMS-A/DAQ_DigitalInput.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/DAQ/DAQ.h ../../../Includes/KMS/DAQ/DigitalInput.h ../../../Includes/KMS/DAQ/IDigitalInputs.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/DAQ_DigitalOutput.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/DAQ/DAQ.h ../../../Includes/KMS/DAQ/DigitalOutput.h ../../../Includes/KMS/DAQ/IDigitalOutputs.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Embedded_Assert.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Embedded/Assert.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Embedded_SPI.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Embedded/SPI.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Embedded_Test.o: ../../../Common/Version.h ../../../Includes/KMS/Base.h ../../../Includes/KMS/DAQ/DAQ.h ../../../Includes/KMS/DAQ/DigitalOutput.h ../../../Includes/KMS/Embedded/IInterruptHandler.h ../../../Includes/KMS/Embedded/Test.h ../../../Includes/KMS/Embedded/USART.h ../../../Includes/KMS/Embedded/WorkItem.h ../../../Includes/KMS/Embedded/WorkLoop.h ../../../Includes/KMS/File/Folder.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/Msg/IReceiver.h ../../../Includes/KMS/SafeAPI.h ../../../Includes/KMS/Version.h ../../../Includes/KMS/WOP/FrameBuffer.h ../../../Includes/KMS/WOP/Link_USART.h ../../../Includes/KMS/WOP/Object.h ../../../Includes/KMS/WOP/System.h ../../../Includes/KMS/WOP/WOP.h ../../../KMS-A/Component.h
../../../KMS-A/Embedded_USART.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Embedded/USART.h ../../../Includes/KMS/Embedded/WorkItem.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Embedded_WorkItem.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Embedded/WorkItem.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Embedded_WorkLoop.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Embedded/WorkItem.h ../../../Includes/KMS/Embedded/WorkLoop.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus_CRC.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Modbus/CRC.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus_Master.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Cfg/MetaData.h ../../../Includes/KMS/DI/MetaData.h ../../../Includes/KMS/Modbus/Master.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus_Master_IDevice.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Dev/IDevice.h ../../../Includes/KMS/Modbus/CRC.h ../../../Includes/KMS/Modbus/Master.h ../../../Includes/KMS/Modbus/Master_IDevice.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus_Monitor.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Modbus/Master.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/Modbus/Monitor.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus_Slave.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Cfg/MetaData.h ../../../Includes/KMS/DI/MetaData.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/Modbus/Slave.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/Msg/IReceiver.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus_Slave_Serial.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Modbus/CRC.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/Modbus/Slave.h ../../../Includes/KMS/Modbus/Slave_Serial.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/Msg/IReceiver.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Modbus_Slave_USART.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Embedded/USART.h ../../../Includes/KMS/Embedded/WorkItem.h ../../../Includes/KMS/Modbus/CRC.h ../../../Includes/KMS/Modbus/Modbus.h ../../../Includes/KMS/Modbus/Slave.h ../../../Includes/KMS/Modbus/Slave_Serial.h ../../../Includes/KMS/Modbus/Slave_USART.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/Msg/IReceiver.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Msg_Destination.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/Msg/IReceiver.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Msg_IReceiver.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Msg/IReceiver.h ../../../Includes/KMS/SafeAPI.h ../../../KMS-A/Component.h
../../../KMS-A/Version.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/File/Folder.h ../../../Includes/KMS/SafeAPI.h ../../../Includes/KMS/Text/File_ASCII.h ../../../Includes/KMS/Types.h ../../../Includes/KMS/Version.h ../../../KMS-A/Component.h
../../../KMS-A/WOP_FrameBuffer.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/SafeAPI.h ../../../Includes/KMS/WOP/FrameBuffer.h ../../../Includes/KMS/WOP/WOP.h ../../../KMS-A/Component.h
../../../KMS-A/WOP_Link_USART.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/Embedded/USART.h ../../../Includes/KMS/Embedded/WorkItem.h ../../../Includes/KMS/File/Folder.h ../../../Includes/KMS/Msg/Destination.h ../../../Includes/KMS/Msg/IReceiver.h ../../../Includes/KMS/SafeAPI.h ../../../Includes/KMS/Version.h ../../../Includes/KMS/WOP/FrameBuffer.h ../../../Includes/KMS/WOP/Link_USART.h ../../../Includes/KMS/WOP/Object.h ../../../Includes/KMS/WOP/System.h ../../../Includes/KMS/WOP/WOP.h ../../../KMS-A/Component.h
../../../KMS-A/WOP_Object.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/SafeAPI.h ../../../Includes/KMS/WOP/FrameBuffer.h ../../../Includes/KMS/WOP/Object.h ../../../Includes/KMS/WOP/WOP.h ../../../KMS-A/Component.h
../../../KMS-A/WOP_System.o: ../../../Includes/KMS/Base.h ../../../Includes/KMS/File/Folder.h ../../../Includes/KMS/SafeAPI.h ../../../Includes/KMS/Version.h ../../../Includes/KMS/WOP/FrameBuffer.h ../../../Includes/KMS/WOP/Object.h ../../../Includes/KMS/WOP/System.h ../../../Includes/KMS/WOP/WOP.h ../../../KMS-A/Component.h


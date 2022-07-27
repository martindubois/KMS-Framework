
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Master.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

#include <KMS/Modbus/Master.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_DEVICE_ADDRESS (BROADCAST)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Master::SetDeviceAddress(uint8_t aDA) { mDeviceAddress = aDA; }

        // ===== Modbus functions ===========================================

        bool Master::ReadCoil(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte = Request_A(Function::READ_COILS, lBuffer, 4, lBuffer, 1);
            if (2 != lRet_byte)
            {
                KMS_EXCEPTION_WITH_INFO(MODBUS_MASTER, "The device returned less data than expected", lRet_byte);
            }

            return ReadBit(lBuffer, 0, 0);
        }

        bool Master::ReadDiscreteInput(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte = Request_A(Function::READ_DISCRETE_INPUTS, lBuffer, 4, lBuffer, 1);
            if (2 != lRet_byte)
            {
                KMS_EXCEPTION_WITH_INFO(MODBUS_MASTER, "The device returned less data than expected", lRet_byte);
            }

            return ReadBit(lBuffer, 0, 0);
        }

        uint16_t Master::ReadHoldingRegister(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte = Request_A(Function::READ_HOLDING_REGISTERS, lBuffer, 4, lBuffer, 2);
            if (2 != lRet_byte)
            {
                KMS_EXCEPTION_WITH_INFO(MODBUS_MASTER, "The device returned less data than expected", lRet_byte);
            }

            return ReadUInt16(lBuffer, 0);
        }

        uint16_t Master::ReadInputRegister(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte = Request_A(Function::READ_INPUT_REGISTERS, lBuffer, 4, lBuffer, 2);
            if (2 != lRet_byte)
            {
                KMS_EXCEPTION_WITH_INFO(MODBUS_MASTER, "The device returned less data than expected", lRet_byte);
            }

            return ReadUInt16(lBuffer, 0);
        }

        void Master::WriteSingleCoil(uint16_t aAddr, bool aValue)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, aValue ? ON : OFF);

            unsigned int lRet_byte = Request_B(Function::WRITE_SINGLE_COIL, lBuffer, 4, lBuffer, 4);
            if (4 != lRet_byte)
            {
                KMS_EXCEPTION_WITH_INFO(MODBUS_MASTER, "The device returned less data than expected", lRet_byte);
            }

            assert(aAddr == ReadUInt16(lBuffer, 0));
        }

        void Master::WriteSingleRegister(uint16_t aAddr, uint16_t aValue)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, aValue);

            unsigned int lRet_byte = Request_B(Function::WRITE_SINGLE_REGISTER, lBuffer, 4, lBuffer, 4);
            if (4 != lRet_byte)
            {
                KMS_EXCEPTION_WITH_INFO(MODBUS_MASTER, "The device returned less data than expected", lRet_byte);
            }

            assert(aAddr == ReadUInt16(lBuffer, 0));
        }

        // ===== Cfg::Configurable ==========================================

        void Master::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Modbus::Link =====\n"
                "DeviceAddress\n"
                "    Set the device address to the default value\n"
                "    Default: 0 (Broadcast)\n"
                "DeviceAddress = {Addr}\n"
                "    Device address\n");

            Cfg::Configurable::DisplayHelp(aOut);
        }

        bool Master::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("DeviceAddress") { SetDeviceAddress(DEFAULT_DEVICE_ADDRESS); return true; }
            }
            else
            {
                CFG_CONVERT("DeviceAddress", SetDeviceAddress, Convert::ToUInt8);
            }

            return Cfg::Configurable::SetAttribute(aA, aV);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Master::Master() : mDeviceAddress(DEFAULT_DEVICE_ADDRESS), mLastException(Exception::NO_EXCEPTION) {}

        uint8_t Master::GetDeviceAddress() const { return mDeviceAddress; }

        void Master::VerifyFunction(Function aFunction, const uint8_t* aData)
        {
            assert(NULL != aData);

            uint8_t lFunction = static_cast<uint8_t>(aFunction);

            if ((0x80 | lFunction) == aData[0])
            {
                mLastException = static_cast<Exception>(aData[1]);
                
                KMS_EXCEPTION_WITH_INFO(MODBUS_EXCEPTION, "Modbus exception", aData[1]);
            }

            if (lFunction != aData[0])
            {
                KMS_EXCEPTION_WITH_INFO(MODBUS_MASTER, "Invalid function code", aData[0]);
            }
        }

    }
}

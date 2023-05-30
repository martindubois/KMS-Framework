
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Modbus_Master.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Dbg/Log.h>

#include <KMS/Modbus/Master.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_DEVICE_ADDRESS (BROADCAST)
#define DEFAULT_RETRY_COUNT    (3)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_DEVICE_ADDRESS("DeviceAddress = {Address}");
static const KMS::Cfg::MetaData MD_RETRY_COUNT   ("RetryCount = {Count}");

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Master::SetDeviceAddress(uint8_t aDA) { mDeviceAddress = aDA; }

        // ===== Modbus functions ===========================================

        #define RETRY_BEGIN          \
            unsigned int lRetry = 0; \
            for (;;)                 \
            {                        \
                try

        #define RETRY_END                                                   \
                catch (...)                                                 \
                {                                                           \
                    if (mRetryCount.Get() == lRetry)                        \
                    {                                                       \
                        KMS_DBG_LOG_ERROR_F(Dbg::Log::FLAG_USER_REDUNDANT); \
                        throw;                                              \
                    }                                                       \
                    KMS_DBG_LOG_INFO();                                     \
                    lRetry++;                                               \
                }                                                           \
            }

        bool Master::ReadCoil(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte;

            RETRY_BEGIN
            {
                lRet_byte = Request_A(Function::READ_COILS, lBuffer, 4, lBuffer, 1);
                break;
            }
            RETRY_END;

            KMS_EXCEPTION_ASSERT(1 == lRet_byte, MODBUS_ERROR, "The device returned less data than expected", lRet_byte);

            return ReadBit(lBuffer, 0, 0);
        }

        bool Master::ReadDiscreteInput(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte;
            
            RETRY_BEGIN
            {
                lRet_byte = Request_A(Function::READ_DISCRETE_INPUTS, lBuffer, 4, lBuffer, 1);
                break;
            }
            RETRY_END;

            KMS_EXCEPTION_ASSERT(1 == lRet_byte, MODBUS_ERROR, "The device returned less data than expected", lRet_byte);

            return ReadBit(lBuffer, 0, 0);
        }

        uint16_t Master::ReadHoldingRegister(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte;

            RETRY_BEGIN
            {
                lRet_byte = Request_A(Function::READ_HOLDING_REGISTERS, lBuffer, 4, lBuffer, 2);
                break;
            }
            RETRY_END;

            KMS_EXCEPTION_ASSERT(2 == lRet_byte, MODBUS_ERROR, "The device returned less data than expected", lRet_byte);

            return ReadUInt16(lBuffer, 0);
        }

        uint16_t Master::ReadInputRegister(uint16_t aAddr)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, 1);

            unsigned int lRet_byte;

            RETRY_BEGIN
            {
                lRet_byte = Request_A(Function::READ_INPUT_REGISTERS, lBuffer, 4, lBuffer, 2);
                break;
            }
            RETRY_END;

            KMS_EXCEPTION_ASSERT(2 == lRet_byte, MODBUS_ERROR, "The device returned less data than expected", lRet_byte);

            return ReadUInt16(lBuffer, 0);
        }

        void Master::WriteSingleCoil(uint16_t aAddr, bool aValue)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, aValue ? ON : OFF);

            unsigned int lRet_byte;

            RETRY_BEGIN
            {
                lRet_byte = Request_B(Function::WRITE_SINGLE_COIL, lBuffer, 4, lBuffer, 4);
                break;
            }
            RETRY_END;

            KMS_EXCEPTION_ASSERT(4 == lRet_byte, MODBUS_ERROR, "The device returned less data than expected", lRet_byte);

            assert(aAddr == ReadUInt16(lBuffer, 0));
        }

        void Master::WriteSingleRegister(uint16_t aAddr, uint16_t aValue)
        {
            uint8_t lBuffer[4];

            WriteUInt16(lBuffer, 0, aAddr);
            WriteUInt16(lBuffer, 2, aValue);

            unsigned int lRet_byte;

            RETRY_BEGIN
            {
                lRet_byte = Request_B(Function::WRITE_SINGLE_REGISTER, lBuffer, 4, lBuffer, 4);
                break;
            }
            RETRY_END;

            KMS_EXCEPTION_ASSERT(4 == lRet_byte, MODBUS_ERROR, "The device returned less data than expected", lRet_byte);

            assert(aAddr == ReadUInt16(lBuffer, 0));
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Master::Master()
            : mDeviceAddress(DEFAULT_DEVICE_ADDRESS)
            , mRetryCount   (DEFAULT_RETRY_COUNT   )
            , mLastException(Exception::NO_EXCEPTION)
        {
            AddEntry("DeviceAddress", &mDeviceAddress, false, &MD_DEVICE_ADDRESS);
            AddEntry("RetryCount"   , &mRetryCount   , false, &MD_RETRY_COUNT);
        }

        void Master::VerifyDeviceAddress(const uint8_t* aData)
        {
            assert(NULL != aData);

            KMS_EXCEPTION_ASSERT(mDeviceAddress == aData[0], MODBUS_CONFIG_INVALID, "Invalid slave address", aData[0]);
        }

        void Master::VerifyFunction(Function aFunction, const uint8_t* aData)
        {
            assert(NULL != aData);

            auto lFunction = static_cast<uint8_t>(aFunction);

            if ((0x80 | lFunction) == aData[0])
            {
                mLastException = static_cast<Exception>(aData[1]);
                
                auto lMsg = "Modbus exception";

                switch (static_cast<Modbus::Exception>(aData[1]))
                {
                case Modbus::Exception::ACKNOWLEDGE                            : lMsg = "Modbus exception  ACKNOWLEDGE"                            ; break;
                case Modbus::Exception::GATEWAY_PATH_UNAVAILABLE               : lMsg = "Modbus exception  GATEWAY PATH UNAVAILABLE"               ; break;
                case Modbus::Exception::GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND: lMsg = "Modbus exception  GATEWAY TARGET DEVICE FAILED TO RESPOND"; break;
                case Modbus::Exception::ILLEGAL_DATA_ADDRESS                   : lMsg = "Modbus exception  ILLEGAL DATA ADDRESS"                   ; break;
                case Modbus::Exception::ILLEGAL_DATA_VALUE                     : lMsg = "Modbus exception  ILLEGAL DATA VALUE"                     ; break;
                case Modbus::Exception::ILLEGAL_FUNCTION                       : lMsg = "Modbus exception  ILLEGAL FUNCTION"                       ; break;
                case Modbus::Exception::MEMORY_PARITY_ERROR                    : lMsg = "Modbus exception  MEMORY PARITY ERROR"                    ; break;
                case Modbus::Exception::SLAVE_DEVICE_BUSY                      : lMsg = "Modbus exception  SLAVE DEVICE BUSY"                      ; break;
                case Modbus::Exception::SLAVE_DEVICE_FAILURE                   : lMsg = "Modbus exception  SLAVE DEVICE FAILURE"                   ; break;
                }

                KMS_EXCEPTION(MODBUS_EXCEPTION, lMsg, aData[1]);
            }

            KMS_EXCEPTION_ASSERT(lFunction == aData[0], MODBUS_ERROR, "Invalid function code", aData[0]);
        }

    }
}

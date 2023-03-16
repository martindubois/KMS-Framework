
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_Master_Com.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/CRC.h>

#include <KMS/Modbus/Master_Com.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MIN_SIZE_byte (3)
#define CRC_SIZE_byte (2)

#define BUFFER_SIZE_byte (MIN_SIZE_byte + 255 + CRC_SIZE_byte)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Master_Com::Master_Com() { AddEntry("Port", &mPort, false); }

        // ===== Master =====================================================

        void Master_Com::Connect()
        {
            mPort.Connect(Dev::Device::FLAG_READ_ACCESS | Dev::Device::FLAG_WRITE_ACCESS);
        }

        void Master_Com::Disconnect()
        {
            mPort.Disconnect();
        }

        unsigned int Master_Com::Request_A(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);

            Request_Send(aFunction, aIn, aInSize_byte);

            uint8_t lBuffer[BUFFER_SIZE_byte];

            mPort.Read(lBuffer, MIN_SIZE_byte, Dev::Device::FLAG_READ_ALL);

            VerifyDeviceAddress(lBuffer);
            VerifyFunction(aFunction, lBuffer + 1);

            mPort.Read(lBuffer + MIN_SIZE_byte, lBuffer[2] + CRC_SIZE_byte, Dev::Device::FLAG_READ_ALL);

            CRC::Verify(lBuffer, MIN_SIZE_byte + lBuffer[2] + CRC_SIZE_byte);

            KMS_EXCEPTION_ASSERT(lBuffer[2] <= aOutSize_byte, MODBUS_OUTPUT_TOO_SHORT, "Output buffer too short", lBuffer[1]);

            memcpy(aOut, lBuffer + MIN_SIZE_byte, lBuffer[1]);

            return lBuffer[2];
        }

        unsigned int Master_Com::Request_B(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);

            Request_Send(aFunction, aIn, aInSize_byte);

            uint8_t lBuffer[BUFFER_SIZE_byte];

            mPort.Read(lBuffer, MIN_SIZE_byte, Dev::Device::FLAG_READ_ALL);

            VerifyDeviceAddress(lBuffer);
            VerifyFunction(aFunction, lBuffer + 1);

            mPort.Read(lBuffer + MIN_SIZE_byte, aOutSize_byte - 1 + CRC_SIZE_byte);

            CRC::Verify(lBuffer, MIN_SIZE_byte - 1 + aOutSize_byte + CRC_SIZE_byte);

            memcpy(aOut, lBuffer + MIN_SIZE_byte - 1, aOutSize_byte);

            return aOutSize_byte;
        }

        unsigned int Master_Com::Request_C(Function aFunction, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            assert(false);
            return 0;
        }

        unsigned int Master_Com::Request_D(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            assert(false);
            return 0;
        }

        unsigned int Master_Com::Request_E(Function aFunction, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            assert(false);
            return 0;
        }

        void Master_Com::Request_F(Function aFunction, uint16_t aAddr, uint16_t aCount, const void* aIn, unsigned int aInSize_byte)
        {
            assert(false);
            // TODO
        }

        unsigned int Master_Com::Request_G(Function aFunction, const void* aIn, unsigned int aSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            assert(false);
            return 0;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Master_Com::Request_Send(Function aFunction, const void* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);
            assert(aInSize_byte > 0);
            assert(aInSize_byte <= BUFFER_SIZE_byte - 4);

            uint8_t lBuffer[BUFFER_SIZE_byte];

            lBuffer[0] = GetDeviceAddress();
            lBuffer[1] = static_cast<uint8_t>(aFunction);

            memcpy(lBuffer + 2, aIn, aInSize_byte);

            unsigned int lSize_byte = 2 + aInSize_byte + CRC_SIZE_byte;

            CRC::Add(lBuffer, lSize_byte);

            mPort.ClearReadBuffer();

            mPort.Write(lBuffer, lSize_byte);
        }

    }
}

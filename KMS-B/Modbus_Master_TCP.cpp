
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Modbus_Master_TCP.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Master_TCP.h>

KMS_RESULT_STATIC(RESULT_MODBUS_ERROR);
KMS_RESULT_STATIC(RESULT_RECV_ERROR);

// Data type
// //////////////////////////////////////////////////////////////////////////

#define PROTOCOL_ID (0)

#define HEADER_SIZE_byte (7)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Master_TCP::Master_TCP(Net::Socket_Client* aSocket) : Master(aSocket), mTransactionId(0)
        {
            assert(nullptr != aSocket);

            aSocket->mKeepALive = true;
            aSocket->mNoDelay   = true;
            aSocket->mReuseAddr = true;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== Master =====================================================

        unsigned int Master_TCP::Request_A(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);

            assert(nullptr != mStream);

            unsigned int lResult_byte = 0;

            try
            {
                Request_Send(aFunction, aIn, aInSize_byte);

                Buffer lBuffer;

                unsigned int lSize_byte = HEADER_SIZE_byte + 2; // Function code + Byte count (or exception)

                Answer_Receive(&lBuffer, lSize_byte);

                VerifyFunction(aFunction, &lBuffer.mFunctionCode);

                lResult_byte = lBuffer.mLength_byte - 3; // Device address + Function code + Byte count
                KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte, RESULT_OUTPUT_TOO_SHORT, "Output too short", lResult_byte);

                auto lRet_byte = mStream->Read(aOut, lResult_byte);
                KMS_EXCEPTION_ASSERT(lResult_byte == lRet_byte, RESULT_RECV_ERROR, "Incomplete answer", lRet_byte);
            }
            catch (...)
            {
                Disconnect();
                throw;
            }

            return lResult_byte;
        }

        unsigned int Master_TCP::Request_B(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != mStream);

            unsigned int lResult_byte = 0;

            try
            {
                Request_Send(aFunction, aIn, aInSize_byte);

                if (0 < aOutSize_byte)
                {
                    assert(nullptr != aOut);

                    Buffer lBuffer;

                    unsigned int lSize_byte = HEADER_SIZE_byte + 2; // Function code + First data byte (or exception)

                    Answer_Receive(&lBuffer, lSize_byte);

                    VerifyFunction(aFunction, &lBuffer.mFunctionCode);

                    lResult_byte = lBuffer.mLength_byte - 2; // Device address + Function code

                    auto lOut = reinterpret_cast<uint8_t*>(aOut);

                    lOut[0] = lBuffer.mData[0];

                    lSize_byte = lResult_byte - 1; // First data byte alread read

                    auto lRet_byte = mStream->Read(lOut + 1, lSize_byte);
                    KMS_EXCEPTION_ASSERT(lSize_byte == lRet_byte, RESULT_RECV_ERROR, "Incomplete answer", lRet_byte);
                }
            }
            catch (...)
            {
                Disconnect();
                throw;
            }

            return lResult_byte;
        }

        unsigned int Master_TCP::Request_C(Function aFunction, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            return 0;
        }

        unsigned int Master_TCP::Request_D(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            return 0;
        }

        unsigned int Master_TCP::Request_E(Function aFunction, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            return 0;
        }

        void Master_TCP::Request_F(Function aFunction, uint16_t aAddr, uint16_t aCount, const void* aIn, unsigned int aInSize_byte)
        {
            // TODO
        }

        unsigned int Master_TCP::Request_G(Function aFunction, const void* aIn, unsigned int aSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            // TODO
            return 0;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Master_TCP::Answer_Receive(Buffer* aBuffer, unsigned int aSize_byte)
        {
            assert(nullptr != aBuffer);
            assert(HEADER_SIZE_byte < aSize_byte);

            assert(nullptr != mStream);

            auto lRet_byte = mStream->Read(aBuffer, aSize_byte);
            KMS_EXCEPTION_ASSERT(aSize_byte == lRet_byte, RESULT_RECV_ERROR, "Incomplete header", lRet_byte);

            aBuffer->mLength_byte   = ntohs(aBuffer->mLength_byte);
            aBuffer->mProtocolId    = ntohs(aBuffer->mProtocolId);
            aBuffer->mTransactionId = ntohs(aBuffer->mTransactionId);

            KMS_EXCEPTION_ASSERT(PROTOCOL_ID    == aBuffer->mProtocolId   , RESULT_RECV_ERROR, "Invalid protocol ID"   , aBuffer->mProtocolId);
            KMS_EXCEPTION_ASSERT(mTransactionId == aBuffer->mTransactionId, RESULT_RECV_ERROR, "Invalid transaction ID", aBuffer->mTransactionId);

            bool lRetB = VerifyDeviceAddress(&aBuffer->mDeviceAddress);
            KMS_EXCEPTION_ASSERT(lRetB, RESULT_MODBUS_ERROR, "VerifyDeviceAddress failed", "");
        }

        void Master_TCP::Request_Send(Function aFunction, const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != mStream);

            mTransactionId++;

            union
            {
                Buffer mBuffer;
                uint8_t mBytes[HEADER_SIZE_byte + 260];
            }
            lBuffer;

            lBuffer.mBuffer.mDeviceAddress = mDeviceAddress;
            lBuffer.mBuffer.mFunctionCode  = static_cast<uint8_t>(aFunction);
            lBuffer.mBuffer.mLength_byte   = htons(aInSize_byte + 2); // Device address + Function code
            lBuffer.mBuffer.mProtocolId    = htons(PROTOCOL_ID);
            lBuffer.mBuffer.mTransactionId = htons(mTransactionId);

            memcpy(lBuffer.mBytes + HEADER_SIZE_byte + 1, aIn, aInSize_byte); // Function code

            mStream->Write(lBuffer.mBytes, HEADER_SIZE_byte + 1 + aInSize_byte);
        }

    }
}

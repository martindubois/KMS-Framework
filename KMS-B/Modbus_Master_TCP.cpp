
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Modbus_Master_TCP.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Master_TCP.h>

// Data type
// //////////////////////////////////////////////////////////////////////////

#define PROTOCOL_ID (0)

#define HEADER_SIZE_byte (7)

typedef struct
{
    uint16_t mTransactionId;
    uint16_t mProtocolId;
    uint16_t mLength_byte;
    uint8_t  mDeviceAddress;
    uint8_t  mFunctionCode;

    uint8_t  mData[4];
}
Buffer;

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Master_TCP::Master_TCP() : mSocket(Net::Socket::Type::STREAM), mTransactionId(0)
        {
            AddEntry("Socket", &mSocket, false);

            mSocket.mKeepALive = true;
            mSocket.mNoDelay   = true;
            mSocket.mReuseAddr = true;
        }

        // ===== Master =====================================================

        void Master_TCP::Connect   () { mSocket.Connect   (); }
        void Master_TCP::Disconnect() { mSocket.Disconnect(); }

        unsigned int Master_TCP::Request_A(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);

            Request_Send(aFunction, aIn, aInSize_byte);

            Buffer lBuffer;

            unsigned int lSize_byte = HEADER_SIZE_byte + 2; // Function code + Byte count (or exception)

            unsigned int lRet_byte = mSocket.Receive(&lBuffer, lSize_byte);
            KMS_EXCEPTION_ASSERT(lSize_byte     == lRet_byte             , MODBUS_RECV_ERROR, "Incomplete header"     , lRet_byte);
            KMS_EXCEPTION_ASSERT(PROTOCOL_ID    == lBuffer.mProtocolId   , MODBUS_RECV_ERROR, "Invalid protocol ID"   , lBuffer.mProtocolId);
            KMS_EXCEPTION_ASSERT(mTransactionId == lBuffer.mTransactionId, MODBUS_RECV_ERROR, "Invalid transaction ID", lBuffer.mTransactionId);

            VerifyDeviceAddress(&lBuffer.mDeviceAddress);
            VerifyFunction(aFunction, &lBuffer.mFunctionCode);

            unsigned int lResult_byte = lBuffer.mLength_byte - 2; // Function code + Byte count
            KMS_EXCEPTION_ASSERT(aOutSize_byte >= lResult_byte, MODBUS_OUTPUT_TOO_SHORT, "Output too short", lResult_byte);

            lRet_byte = mSocket.Receive(aOut, lResult_byte);
            KMS_EXCEPTION_ASSERT(lResult_byte == lRet_byte, MODBUS_RECV_ERROR, "Incomplete answer", lRet_byte);

            return lResult_byte;
        }

        unsigned int Master_TCP::Request_B(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);

            Request_Send(aFunction, aIn, aInSize_byte);

            Buffer lBuffer;

            unsigned int lSize_byte = HEADER_SIZE_byte + 2; // Function code + First data byte (or exception)

            unsigned int lRet_byte = mSocket.Receive(&lBuffer, lSize_byte);
            KMS_EXCEPTION_ASSERT(lSize_byte     == lRet_byte             , MODBUS_RECV_ERROR, "Incomplete header"     , lRet_byte);
            KMS_EXCEPTION_ASSERT(PROTOCOL_ID    == lBuffer.mProtocolId   , MODBUS_RECV_ERROR, "Invalid protocol ID"   , lBuffer.mProtocolId);
            KMS_EXCEPTION_ASSERT(mTransactionId == lBuffer.mTransactionId, MODBUS_RECV_ERROR, "Invalid transaction ID", lBuffer.mTransactionId);

            VerifyDeviceAddress(&lBuffer.mDeviceAddress);
            VerifyFunction(aFunction, &lBuffer.mFunctionCode);

            unsigned int lResult_byte = lBuffer.mLength_byte - 1; // Function code

            uint8_t* lOut = reinterpret_cast<uint8_t*>(aOut);

            lOut[0] = lBuffer.mData[0];

            lSize_byte = lResult_byte - 1; // First data byte alread read

            lRet_byte = mSocket.Receive(lOut + 1, lSize_byte);

            KMS_EXCEPTION_ASSERT(lSize_byte == lRet_byte, MODBUS_RECV_ERROR, "Incomplete answer", lRet_byte);

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

        void Master_TCP::Request_Send(Function aFunction, const void* aIn, unsigned int aInSize_byte)
        {
            mTransactionId++;

            Buffer lBuffer;

            lBuffer.mDeviceAddress = GetDeviceAddress();
            lBuffer.mFunctionCode  = static_cast<uint8_t>(aFunction);
            lBuffer.mLength_byte   = aInSize_byte + 1;
            lBuffer.mProtocolId    = PROTOCOL_ID;
            lBuffer.mTransactionId = mTransactionId;

            mSocket.Send(&lBuffer, HEADER_SIZE_byte + 1);
            mSocket.Send(aIn, aInSize_byte);
        }

    }
}

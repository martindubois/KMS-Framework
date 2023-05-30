
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Master_TCP.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Modbus/Master.h>
#include <KMS/Net/Socket_Client.h>

namespace KMS
{
    namespace Modbus
    {

        class Master_TCP : public Master
        {

        public:

            Master_TCP();

            Net::Socket_Client* GetSocket();

            // ===== Master =================================================
            virtual void         Connect();
            virtual void         Disconnect();

        protected:

            // ===== Master =================================================
            virtual unsigned int Request_A(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_B(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_C(Function aFunction, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_D(Function aFunction, const void* aIn, unsigned int aInSize_byte, void* aOut, unsigned int aOutSize_byte);
            virtual unsigned int Request_E(Function aFunction, void* aOut, unsigned int aOutSize_byte);
            virtual void         Request_F(Function aFunction, uint16_t aAddr, uint16_t aCount, const void* aIn, unsigned int aInSize_byte);
            virtual unsigned int Request_G(Function aFunction, const void* aIn, unsigned int aSize_byte, void* aOut, unsigned int aOutSize_byte);

        private:

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

            void Answer_Receive(Buffer* aBuffer, unsigned int aSize_byte);

            void Request_Send(Function aFunction, const void* aIn, unsigned int aInSize_byte);

            Net::Socket_Client mSocket;

            uint16_t mTransactionId;

        };

    }
}

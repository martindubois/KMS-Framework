
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave_Serial.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Modbus/CRC.h>
#include <KMS/Modbus/Slave.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave_Serial : public Slave
        {

        protected:

            Slave_Serial();

            void OnByte(uint8_t aByte);

            void Reset();

            virtual void SendData(const uint8_t* aIn, unsigned int aInSize_byte) = 0;

            // ===== Slave ==================================================
            virtual void SendException (Function aFunction, Exception aException);
            virtual void SendResponse_A(Function aFunction, uint8_t aSize_byte, const void* aData);
            virtual void SendResponse_B(Function aFunction, Address aAddress, const void* aData);

            unsigned int mSilentCounter;

        private:

            NO_COPY(Slave_Serial);

            void OnRequest();

            uint8_t mBuffer[260];

            CRC mCRC;

            uint16_t mAddress;

            Function mFunction;

            uint16_t mQty;

            uint16_t mReceivedCRC;

            unsigned int mState;

            uint8_t mTarget;

        };

    }
}

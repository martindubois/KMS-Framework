
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave_Com.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Com/Port.h>
#include <KMS/Modbus/CRC.h>
#include <KMS/Modbus/Slave.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave_Com : public Slave
        {

        public:

            Slave_Com();

            // ===== Slave ==================================================
            virtual void Connect();

        protected:

            // ===== Slave ==================================================
            virtual unsigned int OnIterate();
            virtual void SendException(Function aFunction, Exception aException);
            virtual void SendResponse_A(Function aFunction, uint8_t aSize_byte, const void* aData);
            virtual void SendResponse_B(Function aFunction, Address aAddress, const void* aData);

        private:

            Slave_Com(const Slave_Com&);

            const Slave_Com operator = (Slave_Com&);

            void OnByte(uint8_t aByte);
            void OnRequest();

            void Reset();

            uint8_t mBuffer[260];

            CRC mCRC;

            uint16_t mAddress;

            Function mFunction;

            uint16_t mQty;

            uint16_t mReceivedCRC;

            unsigned int mSilentCounter;

            unsigned int mState;

            uint8_t mTarget;

            Com::Port mPort;

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Modbus_Slave_USART.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Modbus/Slave_USART.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_RX_BYTE (2)

namespace KMS
{
    namespace Modbus
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Slave_USART::Slave_USART(Embedded::USART* aUSART) : ON_RX_BYTE(this, MSG_RX_BYTE), mUSART(aUSART)
        {
            assert(NULL != aUSART);

            mUSART->mOnRxByte = ON_RX_BYTE;
        }

        // ===== Msg::IReceiver =========================================
        unsigned int Slave_USART::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult;

            switch (aCode)
            {
            case MSG_RX_BYTE: lResult = OnRxByte(aData);

            default: lResult = Slave::Receive(aSender, aCode, aData);
            }

            return lResult;
        }

        // Protected
        // //////////////////////////////////////////////////////////////

        // ===== Slave_Serial ===========================================
        void Slave_USART::SendData(const uint8_t* aIn, unsigned int aInSize_byte)
        {
            mUSART->Tx(aIn, aInSize_byte);
        }

        // ===== Embedded::WorkItem =====================================
        void Slave_USART::Work()
        {
            if (30 <= mSilentCounter)
            {
                Reset();
            }
            else
            {
                mSilentCounter++;
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int Slave_USART::OnRxByte(void* aData)
        {
            assert(NULL != aData);

            auto lData = reinterpret_cast<uint8_t*>(aData);

            OnByte(*lData);

            return 0;
        }

    }
}

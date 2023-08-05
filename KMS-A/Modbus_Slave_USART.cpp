
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

        Slave_USART::Slave_USART(Embedded::USART* aUSART)
            : mUSART(aUSART)
            // ===== Callbacks ==============================================
            , ON_RX_BYTE(this, &Slave_USART::OnRxByte)
        {
            assert(NULL != aUSART);

            mUSART->mOnRxByte = &ON_RX_BYTE;
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

        // ===== Callbacks ==================================================

        unsigned int Slave_USART::OnRxByte(void*, void* aData)
        {
            assert(NULL != aData);

            auto lData = reinterpret_cast<uint8_t*>(aData);

            OnByte(*lData);

            return 0;
        }

    }
}

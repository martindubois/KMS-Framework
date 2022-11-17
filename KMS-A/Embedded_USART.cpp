
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded_USART.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/USART.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

// --> READY <------------+
//      |                 |
//      +--> PENDING --> COMPLETED
#define STATE_COMPLETED (0)
#define STATE_PENDING   (1)
#define STATE_READY     (2)

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool USART::Tx_IsReady()
        {
            bool lResult = false;

            switch (mTx_State)
            {
            case STATE_COMPLETED:
                mTx_State = STATE_READY;
                // No break
            case STATE_READY:
                lResult = true;
                break;
            }

            return lResult;
        }

        // ===== WorkItem ===================================================
        void USART::Work()
        {
            Rx_DisableInterrupt();
            {
                while (mRx_BufferOut != mRx_BufferIn)
                {
                    mOnRxByte.Send(this, mRx_Buffer + mRx_BufferOut);
                    Rx_IncIndex(&mRx_BufferOut);
                }
            }
            Rx_EnableInterrupt();

            switch (mTx_State)
            {
            case STATE_COMPLETED:
                mOnTxCompleted.Send(this);
                mTx_State = STATE_READY;
                break;
            }
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        USART::USART() : mRx_BufferIn(0), mRx_BufferOut(0), mTx_State(STATE_READY) {}

        void USART::Rx_Signal(uint8_t aByte)
        {
            mRx_Buffer[mRx_BufferIn] = aByte;
            Rx_IncIndex(&mRx_BufferIn);

            if (mRx_BufferOut == mRx_BufferIn)
            {
                // TODO Signal overflow
                Rx_IncIndex(&mRx_BufferOut);
            }
        }

        void USART::Tx       () { mTx_State = STATE_PENDING; }
        void USART::Tx_Signal() { mTx_State = STATE_COMPLETED; }

        // Private
        // //////////////////////////////////////////////////////////////////

        void USART::Rx_IncIndex(uint8_t* aIndex)
        {
            *aIndex = (*aIndex + 1) % sizeof(mRx_Buffer);
        }

    }
}

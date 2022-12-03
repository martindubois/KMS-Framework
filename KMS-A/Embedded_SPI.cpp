
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded_SPI.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/SPI.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

// --> READY <--+
//      |       |
//      +--> PENDING
#define STATE_PENDING   (0)
#define STATE_READY     (1)

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool SPI::Tx_IsReady() { return STATE_READY == mTx_State; }

        void SPI::Slave_Connect(Msg::IReceiver* aReceiver, unsigned int aRx, unsigned int aTx)
        {
            mOnRxByte .Set(aReceiver, aRx);
            mOnTxReady.Set(aReceiver, aTx);
        }

        void SPI::Slave_Disconnect()
        {
            mOnRxByte .Clear();
            mOnTxReady.Clear();
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        SPI::SPI() : mTx_State(STATE_READY) {}

        void SPI::Rx_Signal(uint16_t aWord) { mOnRxByte.Send(this, &aWord); }

        void SPI::Tx() { mTx_State = STATE_PENDING; }

        void SPI::Tx_Signal()
        {
            mTx_State = STATE_READY;
            mOnTxReady.Send(this, NULL);
        }

    }
}

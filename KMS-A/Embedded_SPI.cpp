
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

namespace KMS
{
    namespace Embedded
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void SPI::Slave_Connect(ISlave* aSlave)
        {
            // assert(NULL != aSlave);

            // assert(NULL == mSlave);

            mSlave = aSlave;

            mSlave->OnConnect(this);
        }

        void SPI::Slave_Disconnect()
        {
            // assert(NULL != mSlave);

            mSlave->OnDisconnect(this);

            mSlave = NULL;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        SPI::SPI() : mSlave(NULL) {}

        void SPI::Rx_Signal(uint16_t aWord)
        {
            // assert(NULL != mSlave);

            mSlave->OnRxWord(this, aWord);
        }

        void SPI::Tx_Signal()
        {
            // assert(NULL != mSlave);

            mSlave->OnTxReady(this);
        }

    }
}

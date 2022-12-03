
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

        void SPI::Slave_Connect(IDevice* aDevice)
        {
            // assert(NULL != aDevice);

            mDevice = aDevice;

            mDevice->OnConnect(this);
        }

        void SPI::Slave_Disconnect()
        {
            // assert(NULL != mDevice);

            mDevice->OnDisconnect(this);

            mDevice = NULL;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        SPI::SPI() : mDevice(NULL) {}

        void SPI::Rx_Signal(uint16_t aWord)
        {
            // assert(NULL != mDevice);

            mDevice->OnRxWord(this, aWord);
        }

        void SPI::Tx_Signal()
        {
            // assert(NULL != mDevice);

            mDevice->OnTxReady(this);
        }

    }
}

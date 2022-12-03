
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/SPI.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Msg/Destination.h>

namespace KMS
{
    namespace Embedded
    {

        class SPI
        {

        public:

            bool Tx_IsReady();

            virtual void Tx(uint8_t aByte) = 0;

            // The messages are sent at ISR level.
            virtual void Slave_Connect(Msg::IReceiver* aReceiver, unsigned int aRx, unsigned int aTx);

            virtual void Slave_Disconnect();

        protected:

            SPI();

            void Rx_Signal(uint16_t aWord);

            void Tx();
            void Tx_Signal();

        private:

            Msg::Destination mOnRxByte;
            Msg::Destination mOnTxReady;

            uint8_t mTx_State;

        };

    }
}

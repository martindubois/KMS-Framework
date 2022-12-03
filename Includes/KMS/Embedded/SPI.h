
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/SPI.h

#pragma once

namespace KMS
{
    namespace Embedded
    {

        class SPI
        {

        public:

            class IDevice
            {

            public:

                virtual void OnConnect   (SPI* aSPI) = 0;
                virtual void OnDisconnect(SPI* aSPI) = 0;
                virtual void OnRxWord    (SPI* aSPI, uint16_t aWord) = 0;
                virtual void OnTxReady   (SPI* aSPI) = 0;

            };

            bool Tx_IsReady();

            virtual void Tx(uint16_t aWord) = 0;

            // The messages are sent at ISR level.
            virtual void Slave_Connect(IDevice* aDevice);

            virtual void Slave_Disconnect();

        protected:

            SPI();

            void Rx_Signal(uint16_t aWord);

            void Tx_Signal();

        private:

            IDevice* mDevice;

        };

    }
}

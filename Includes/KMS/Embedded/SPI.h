
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

            class ISlave
            {

            public:

                enum
                {
                    FLAG_TX_LAST_WORD = 0x01,
                    FLAG_TX_WORD      = 0x02,
                };

                virtual uint8_t OnConnect   (uint16_t* aWord) = 0;
                virtual void    OnDisconnect() = 0;
                virtual uint8_t OnRxWord    (uint16_t* aWord) = 0;
                virtual uint8_t OnTxReady   (uint16_t* aWord) = 0;

            };

            virtual void Tx(uint16_t aWord, uint8_t aFlags) = 0;

            // Level: ISR
            virtual void Slave_Connect(ISlave* aSlave) = 0;

            // Level: ISR
            virtual void Slave_Disconnect() = 0;

        protected:

            SPI();

            ISlave* mSlave;

        };

    }
}

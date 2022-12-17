
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

                // FLAG_CLOCK_LOW     The initial clock level is LOW rather
                //                    than de default HIGH
                // FLAG_TX_LAST_WORD  This is the last data word, do not call
                //                    OnTxReady after transmitting this one
                // FLAG_TX_WORD       Transmit the returned word
                // FLAG_WORD_09       Use 9 bits word rather than the default
                //                    16 bits word
                enum
                {
                    FLAG_CLOCK_LOW    = 0x01,
                    FLAG_TX_LAST_WORD = 0x02,
                    FLAG_TX_WORD      = 0x04,
                    FLAG_WORD_09      = 0x08,
                };

                // aWord  The word to transmit
                // Return  See FLAG_CLOCK_LOW, FLAG_TX_LAST_WORD,
                //         FLAG_WORD_09
                // Level  ISR
                virtual uint8_t OnConnect(uint16_t* aWord) = 0;

                // aWord  The received word and the word to transmit if
                //        needed
                // Return  See FLAG_TX_LAST_WORD, FLAG_TX_WORD, FLAG_WORD_09
                // Level ISR
                virtual uint8_t OnRxWord(uint16_t* aWord) = 0;

                // aWord  The word to transmit
                // Return  See FLAG_TX_WORD, FLAG_TX_LAST_WORD, FLAG_WORD_09
                // Level  ISR
                virtual uint8_t OnTxReady(uint16_t* aWord) = 0;

            };

            // aSlave  The slace to connect
            // Level  ISR
            virtual void Slave_Connect(ISlave* aSlave) = 0;

            // Level  ISR
            virtual void Slave_Disconnect() = 0;

        protected:

            SPI();

            ISlave* mSlave;

        };

    }
}

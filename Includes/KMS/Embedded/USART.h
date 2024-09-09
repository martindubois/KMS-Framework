
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/UART.h
// Status

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Embedded/WorkItem.h>

namespace KMS
{
    namespace Embedded
    {

        class USART : public WorkItem
        {

        public:

            bool Tx_IsReady();

            // Busy wait until Tx_IsReady return true. Do not call this
            // function when the interrupts are disabled.
            void Tx_Wait();

            virtual void Rx_Disable() = 0;
            virtual void Rx_Enable () = 0;

            virtual void Tx(const void* aData, uint16_t aSize_byte) = 0;

            // mOnRxByte
            //  Level: WORK INTERRUPT DISABLED
            Callback_Ptr mOnRxByte;
            Callback_Ptr mOnTxCompleted;

            // ===== WorkItem ===============================================
            virtual void Work();

        protected:

            USART();

            // Level: ISR
            void Rx_Signal(uint8_t aByte);

            void Tx();

            // Level: ISR
            void Tx_Signal();

            virtual void Rx_DisableInterrupt() = 0;
            virtual void Rx_EnableInterrupt () = 0;

        private:

            NO_COPY(USART);

            void Rx_IncIndex(uint8_t* aIndex);

            uint8_t mRx_Buffer[4];
            uint8_t mRx_BufferIn;
            uint8_t mRx_BufferOut;

            uint8_t mTx_State;

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/Slave_USART.h
// Status

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Embedded/USART.h>
#include <KMS/Embedded/WorkItem.h>
#include <KMS/Modbus/Slave_Serial.h>

namespace KMS
{
    namespace Modbus
    {

        class Slave_USART final : public Slave_Serial, public Embedded::WorkItem
        {

        public:

            Slave_USART(KMS::Embedded::USART* aUSART);

        protected:

            // ===== Slave_Serial ===========================================
            virtual void SendData(const uint8_t* aIn, unsigned int aInSize_byte);

            // ===== Embedded::WorkItem =====================================
            virtual void Work();

        private:

            NO_COPY(Slave_USART);

            // ===== Callbacks ==============================================
            const Callback<Slave_USART> ON_RX_BYTE;
            unsigned int OnRxByte(void* aSender, void* aData);

            Embedded::USART* mUSART;

        };

    }
}

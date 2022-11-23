
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Link_USART.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Embedded/USART.h>
#include <KMS/Embedded/WorkItem.h>
#include <KMS/Msg/IReceiver.h>

namespace KMS
{
    namespace WOP
    {

        class System;

        class Link_USART : public Embedded::WorkItem, public Msg::IReceiver
        {

        public:

            Link_USART(System* aSystem, Embedded::USART* aUSART);

            Embedded::USART* GetUSART();

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

            // ===== Embedded::WorkItem =====================================
            virtual void Work();

        private:

            unsigned int OnRxByte(void* aData);

            System         * mSystem;
            Embedded::USART* mUSART;

        };

    }
}

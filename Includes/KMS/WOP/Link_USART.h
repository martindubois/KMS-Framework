
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Link_USART.h
// Status

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Embedded/USART.h>
#include <KMS/Embedded/WorkItem.h>

namespace KMS
{
    namespace WOP
    {

        class System;

        class Link_USART : public Embedded::WorkItem
        {

        public:

            Link_USART(System* aSystem, Embedded::USART* aUSART);

            Embedded::USART* GetUSART();

            // ===== Embedded::WorkItem =====================================
            virtual void Work();

        private:

            // ===== Callbacks ==============================================
            const Callback<Link_USART> ON_RX_BYTE;
            unsigned int OnRxByte(void* aData);

            System         * mSystem;
            Embedded::USART* mUSART;

        };

    }
}

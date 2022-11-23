
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_Link_USART.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/System.h>

#include <KMS/WOP/Link_USART.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_RX_BYTE (1)

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Link_USART::Link_USART(System* aSystem, Embedded::USART* aUSART)
            : mSystem(aSystem)
            , mUSART(aUSART)
        {
            // assert(NULL != aSystem);
            // assert(NULL != aUSART);

            mUSART->mOnRxByte.Set(this, MSG_RX_BYTE);
        }

        Embedded::USART* Link_USART::GetUSART() { return mUSART; }

        // ===== Msg::IReceiver =============================================

        unsigned int Link_USART::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

            switch (aCode)
            {
            case MSG_RX_BYTE: lResult = OnRxByte(aData); break;
            }

            return lResult;
        }

        // ===== Embedded::WorkItem =========================================

        void Link_USART::Work()
        {
            // assert(NULL != mSystem);
            // assert(NULL != mUSART);

            if (mUSART->Tx_IsReady())
            {
                const FrameBuffer* lFB = mSystem->PrepareFrame();
                if (NULL != lFB)
                {
                    mUSART->Tx(lFB->GetRawFrame(), lFB->GetFrameSize_byte());
                }
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int Link_USART::OnRxByte(void* aData)
        {
            // assert(NULL != aData);

            // assert(NULL != mSystem);

            uint8_t* lData = reinterpret_cast<uint8_t*>(aData);

            mSystem->AddReceivedByte(*lData);

            return 0;
        }

    }
}

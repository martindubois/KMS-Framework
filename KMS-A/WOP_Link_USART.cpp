
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_Link_USART.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/Receiver.h>
#include <KMS/WOP/Sender.h>

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

        Link_USART::Link_USART(Receiver* aReceiver, Sender* aSender, Embedded::USART* aUSART)
            : mReceiver(aReceiver)
            , mSender(aSender)
            , mUSART(aUSART)
        {
            // assert(NULL != aReceiver);
            // assert(NULL != aSender);
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
            // assert(NULL != mSender);
            // assert(NULL != mUSART);

            if (mUSART->Tx_IsReady())
            {
                const FrameBuffer* lFB = mSender->PrepareFrame();
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

            // assert(NULL != mReceiver);

            uint8_t* lData = reinterpret_cast<uint8_t*>(aData);

            mReceiver->AddReceivedByte(*lData);

            return 0;
        }

    }
}

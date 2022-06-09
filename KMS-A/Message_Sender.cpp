
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Message_Sender.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Message/IReceiver.h>

#include <KMS/Message/Sender.h>

namespace KMS
{
    namespace Message
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Sender::Sender(void* aSender) : mCode(0), mReceiver(NULL), mSender(aSender)
        {
            assert(NULL != aSender);
        }

        bool Sender::IsSet() const { return NULL != mReceiver; }

        void Sender::Set(IReceiver* aR, unsigned int aCode)
        {
            assert(NULL != aR);
            assert(0 < aCode);

            mCode     = aCode;
            mReceiver = aR;
        }

        bool Sender::Send(void* aData)
        {
            bool lResult = true;

            assert(NULL != mSender);

            if (NULL != mReceiver)
            {
                assert(0 < mCode);

                lResult = false;

                try
                {
                    lResult = mReceiver->Receive(mSender, mCode, aData);
                }
                KMS_CATCH
            }

            return lResult;
        }

    }
}

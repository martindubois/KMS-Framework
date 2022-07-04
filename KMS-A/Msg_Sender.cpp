
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Msg_Sender.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>

#include <KMS/Msg/Sender.h>

namespace KMS
{
    namespace Msg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Sender::Sender(void* aSender) : mCode(0), mReceiver(NULL), mSender(aSender)
        {
            assert(NULL != aSender);
        }

        Sender::Sender(void* aSender, IReceiver* aReceiver, unsigned int aCode) : mCode(aCode), mReceiver(aReceiver), mSender(aSender)
        {
            assert(NULL != aSender);
            assert(NULL != aReceiver);
            assert(0 < aCode);
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

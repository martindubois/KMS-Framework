
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Msg_Sender.cpp

// TEST COVERAGE 2022-08-15 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Msg/Sender.h>

namespace KMS
{
    namespace Msg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Sender::Sender(void* aSender) : mSender(aSender) {}

        Sender::Sender(void* aSender, IReceiver* aR, unsigned int aCode) : Destination(aR, aCode), mSender(aSender) {}

        void Sender::operator = (const Destination& aD) { *static_cast<Destination*>(this) = aD; }

        unsigned int Sender::Send(void* aData) { return Destination::Send(mSender, aData); }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Msg/Sender.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Msg/Destination.h>

namespace KMS
{
    namespace Msg
    {

        class Sender : public Destination
        {

        public:

            Sender(void* aSender);

            Sender(void* aSender, IReceiver* aR, unsigned int aCode);

            void operator = (const Destination& aD);

            unsigned int Send(void* aData = NULL);

        private:

            void* mSender;

        };

    }
}

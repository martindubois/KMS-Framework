
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Message/Receiver.h

#pragma once

namespace KMS
{
    namespace Message
    {

        class IReceiver
        {

        public:

            virtual bool Receive(void* aSender, unsigned int aCode, void* aData) = 0;

        };

    }
}

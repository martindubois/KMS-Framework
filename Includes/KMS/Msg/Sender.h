
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Msg/Receiver.h

#pragma once

namespace KMS
{
    namespace Msg
    {

        class IReceiver;

        class Sender
        {

        public:

            Sender(void* aSender);

            Sender(void* aSender, IReceiver* aReceiver, unsigned int aCode);

            void Clear();

            bool IsSet() const;

            void Set(IReceiver* aR, unsigned int aCode);

            unsigned int Send(void* aData = NULL);

        private:

            unsigned int mCode;

            IReceiver* mReceiver;

            void* mSender;

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Msg/Destination.h

#pragma once

namespace KMS
{
    namespace Msg
    {

        class IReceiver;

        class Destination
        {

        public:

            Destination();

            Destination(IReceiver* aR, unsigned int aCode);

            void Clear();

            bool IsSet() const;

            void Set(IReceiver* aR, unsigned int aCode);

            unsigned int Send(void* aSender, void* aData = NULL);

        private:

            unsigned int mCode;

            IReceiver* mReceiver;

        };

    }
}

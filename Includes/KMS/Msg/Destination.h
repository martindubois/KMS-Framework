
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Msg/Destination.h
// Status    OBSOLETE
// Library   KMS-A

#pragma once

namespace KMS
{
    namespace Msg
    {

        class IReceiver;

        class Destination final
        {

        public:

            Destination();

            Destination(IReceiver* aR, unsigned int aCode);

            void Clear();

            bool IsSet() const;

            void Set(IReceiver* aR, unsigned int aCode);

            unsigned int Send(void* aSender, void* aData = nullptr);

        private:

            unsigned int mCode;

            IReceiver* mReceiver;

        };

    }
}

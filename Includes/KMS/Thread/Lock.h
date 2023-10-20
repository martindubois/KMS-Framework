
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Thread/Lock.h

#pragma once

namespace KMS
{
    namespace Thread
    {

        class Gate;

        class Lock final
        {

        public:

            Lock(Gate* aGate);

            ~Lock();

            void Relock();
            void Unlock();

        private:

            bool mLocked;

            Gate* mGate;

        };

    }
}

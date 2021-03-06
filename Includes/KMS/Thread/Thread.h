
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Thread/Thread.h

#pragma once

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Msg/Sender.h>
#include <KMS/Thread/Gate.h>

namespace KMS
{
    namespace Thread
    {

        class Thread
        {

        public:

            Thread();

            ~Thread();

            void Kill();
            void Start();
            void Stop();
            void StopAndWait(unsigned int aTimeout_ms);
            void Wait(unsigned int aTimeout_ms);

            Gate mGate;

            Msg::Sender mOnIterate;
            Msg::Sender mOnRun;
            Msg::Sender mOnStarting;
            Msg::Sender mOnStop;
            Msg::Sender mOnStopping;

        // internal

            void Run();

        private:

            // --> STOPPED <==+------------+
            //      |         |            |
            //      +--> STARTING          |
            //            |                |
            //            +--> RUNNING     |
            //                  |          |
            //                  +--> STOPPING
            enum class State
            {
                RUNNING,
                STOPPED,
                STOPPING,
                STARTING,
            };

            void CloseIfNeeded();

            HANDLE mHandle;

            State mState;

        };

    }
}

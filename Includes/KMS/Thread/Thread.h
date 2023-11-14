
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Thread/Thread.h
// Status
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Thread/Gate.h>

namespace KMS
{
    namespace Thread
    {

        class Thread final
        {

        public:

            Thread();

            ~Thread();

            void Kill();
            void Start();
            void Stop();
            void StopAndWait(unsigned int aTimeout_ms);

            // LIMITATION  On Linux, the method igores aTimeout_ms and wait
            //             for ever.
            void Wait(unsigned int aTimeout_ms);

            Gate mGate;

            Callback_Ptr mOnIterate;
            Callback_Ptr mOnRun;
            Callback_Ptr mOnStarting;
            Callback_Ptr mOnStop;
            Callback_Ptr mOnStopping;

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

            void Construct_OSDep();

            void CloseIfNeeded();

            void Wait_OSDep(unsigned int aTimeout_ms);

            State mState;

            #ifdef _KMS_LINUX_
                pthread_t mThread;
            #endif

            #ifdef _KMS_WINDOWS_
                HANDLE mHandle;
            #endif

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Thread/Gate.h

#pragma once

#ifdef _KMS_LINUX_
    // ===== C ==============================================================
    #include <pthread.h>
#endif

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

namespace KMS
{
    namespace Thread
    {

        class Gate final
        {

        public:

            Gate();

            ~Gate();

            void Lock();
            void Unlock();

        private:

            #ifdef _KMS_LINUX_
                pthread_mutex_t mInternal;
            #endif

            #ifdef _KMS_WINDOWS_
                CRITICAL_SECTION mInternal;
            #endif

        };

    }
}

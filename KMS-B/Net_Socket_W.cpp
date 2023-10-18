
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_W.cpp

// TEST COVERAGE  2023-10-17  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Socket.h>

KMS_RESULT_STATIC(RESULT_SOCKET_STARTUP_FAILED);

namespace KMS
{
    namespace Net
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        void Thread_Startup()
        {
            WSADATA lData;

            auto lRet = WSAStartup(MAKEWORD(2, 2), &lData);
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SOCKET_STARTUP_FAILED, "WSAStartup failed", lRet);
        }

        void Thread_Cleanup()
        {
            auto lRet = WSACleanup();
            assert(0 == lRet);
        }

    }
}

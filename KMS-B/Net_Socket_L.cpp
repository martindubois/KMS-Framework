
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Socket.h>

KMS_RESULT_STATIC(RESULT_SOCKET_OPTION_FAILED);

namespace KMS
{
    namespace Net
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        void Thread_Startup() {}
        void Thread_Cleanup() {}

        // Private
        // //////////////////////////////////////////////////////////////////

        void Socket::SetOption(int aOptName, uint32_t aValue)
        {
            assert(INVALID_SOCKET != mSocket);

            int lRet;
            struct timeval lT;

            switch (aOptName)
            {
            case SO_RCVTIMEO:
            case SO_SNDTIMEO:
                lT.tv_sec  =  aValue / 1000;
                lT.tv_usec = (aValue % 1000) * 1000;

                lRet = setsockopt(mSocket, SOL_SOCKET, aOptName, &lT, sizeof(lT));
                break;

            default:
                lRet = setsockopt(mSocket, SOL_SOCKET, aOptName, reinterpret_cast<char*>(&aValue), sizeof(aValue));
            }

            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SOCKET_OPTION_FAILED, "setsockopt failed (NOT TESTED)", lRet);
        }

    }
}

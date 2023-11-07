
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_Client_TLS_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Socket_Client_TLS.h>

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Socket_Client_TLS::Connect()
        {
            KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "Not implemented", "");
        }

        void Socket_Client_TLS::Disconnect()
        {
            KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "Not implemented", "");
        }

        unsigned int Socket_Client_TLS::Receive(void* aOut, unsigned int aOutSize_byte)
        {
            KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "Not implemented", "");
            return 0;
        }

        void Socket_Client_TLS::Send(const void* aIn, unsigned int aInSize_byte)
        {
            KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "Not implemented", "");
        }

    }
}

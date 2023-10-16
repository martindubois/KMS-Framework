
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Client.h
// Status
// Library   KMS-B

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>
#include <KMS/Net/Socket_Client.h>

namespace KMS
{
    namespace HTTP
    {

        class Client
        {

        public:

            Net::Socket_Client mSocket;

            Client();

            void Get(const char* aURL, File::Binary* aOutFile);

        private:

            void ConnectSocket(const char* aHost);

            void ReceiveRestOfFile(unsigned int aToRecv_byte, File::Binary* aOutFile);

        };

    }
}

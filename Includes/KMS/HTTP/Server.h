
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h
// Status
// Library   KMS-B

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/Net/Server.h>

namespace KMS
{
    namespace HTTP
    {

        class Server : public Net::Server
        {

        public:

            Server();

            DI::Dictionary mResponseHeader;

            Callback_Ptr mOnRequest;

        protected:

            virtual void OnConnect(Net::Socket* aSocket);

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Dictionary.h>
#include <KMS/Msg/Sender.h>
#include <KMS/Net/Server.h>

namespace KMS
{
    namespace HTTP
    {

        class Server : public Net::Server
        {

        public:

            Server();

            Dictionary mResponseHeader;

            Msg::Sender mOnRequest;

        protected:

            virtual void OnConnect(Net::Socket* aSocket);

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Network/Server.h>
#include <KMS/Message/Sender.h>

namespace KMS
{
    namespace HTTP
    {

        class Server : public Network::Server
        {

        public:

            Server();

            Message::Sender mOnRequest;

        protected:

            virtual void OnConnect(Network::Socket* aSocket);

        };

    }
}

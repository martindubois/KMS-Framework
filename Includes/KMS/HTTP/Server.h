
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Server.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Network/Socket.h>
#include <KMS/Message/IReceiver.h>
#include <KMS/Message/Sender.h>
#include <KMS/Thread/Thread.h>

namespace KMS
{
    namespace HTTP
    {

        class Request;

        class Server : public Message::IReceiver
        {

        public:

            Server();

            Message::Sender mOnRequest;

            Network::Socket mSocket;
            Thread::Thread  mThread;

            // ===== Message::IReceiver =====================================
            virtual bool Receive(void* aSender, unsigned int aCode, void* aData);

        private:

            bool OnIterate ();
            bool OnStarting();
            bool OnStop    ();
            bool OnStopping();

        };

    }
}

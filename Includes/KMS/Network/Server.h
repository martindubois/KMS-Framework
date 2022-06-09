
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Network/Server.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Network/Socket.h>
#include <KMS/Message/IReceiver.h>
#include <KMS/Message/Sender.h>
#include <KMS/Thread/Thread.h>

namespace KMS
{
    namespace Network
    {

        class Server : public Message::IReceiver
        {

        public:

            Network::Socket mSocket;
            Thread::Thread  mThread;

            // ===== Message::IReceiver =====================================
            virtual bool Receive(void* aSender, unsigned int aCode, void* aData);

        protected:

            Server();

            virtual void OnConnect(Socket* aSocket) = 0;

        private:

            bool OnIterate ();
            bool OnStarting();
            bool OnStop    ();
            bool OnStopping();

        };

    }
}

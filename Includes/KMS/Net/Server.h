
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Server.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Msg/IReceiver.h>
#include <KMS/Net/Socket.h>
#include <KMS/Thread/Thread.h>

namespace KMS
{
    namespace Net
    {

        class Server : public Msg::IReceiver
        {

        public:

            Net::Socket     mSocket;
            Thread::Thread  mThread;

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        protected:

            Server();

            virtual void OnConnect(Socket* aSocket) = 0;

        private:

            unsigned int OnIterate ();
            unsigned int OnStarting();
            unsigned int OnStop    ();
            unsigned int OnStopping();

        };

    }
}

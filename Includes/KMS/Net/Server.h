
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Server.h
// Status
// Library   KMS-B

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Net/Socket_Server.h>
#include <KMS/Thread/Thread.h>

namespace KMS
{
    namespace Net
    {

        class Server
        {

        public:

            Net::Socket_Server mSocket;
            Thread::Thread     mThread;

        protected:

            Server();

            virtual void OnConnect(Socket* aSocket) = 0;

        private:

            // ===== Callbacks ==============================================
            Callback<Server> ON_ITERATE;
            Callback<Server> ON_STARTING;
            Callback<Server> ON_STOP;
            Callback<Server> ON_STOPPING;
            unsigned int OnIterate (void* aSender, void* aData);
            unsigned int OnStarting(void* aSender, void* aData);
            unsigned int OnStop    (void* aSender, void* aData);
            unsigned int OnStopping(void* aSender, void* aData);

        };

    }
}

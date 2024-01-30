
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
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

            // Start the server thread and wait until the socket is binded
            //
            // Exception
            //  RESULT_TIMEOUT
            //  See Thread::Thread::Start
            void Start();

            // See Thread::Thread::StopAndWait
            void StopAndWait(unsigned int aTimeout_ms);

            Net::Socket_Server mSocket;

        protected:

            Server();

            virtual void OnConnect(Socket* aSocket) = 0;

        private:

            Callback<Server> ON_ITERATE;
            Callback<Server> ON_STARTING;
            Callback<Server> ON_STOP;
            Callback<Server> ON_STOPPING;

            unsigned int OnIterate (void* aSender, void* aData);
            unsigned int OnStarting(void* aSender, void* aData);
            unsigned int OnStop    (void* aSender, void* aData);
            unsigned int OnStopping(void* aSender, void* aData);

            Thread::Thread mThread;

        };

    }
}

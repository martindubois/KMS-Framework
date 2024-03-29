
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <thread>

// ===== Includes ===========================================================
#include <KMS/Net/Server.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define ACCEPT_TIMEOUT_ms (1000)

namespace KMS
{
    namespace Net
    {

        // Protected
        // //////////////////////////////////////////////////////////////////

        Server::Server()
            : mSocket(Net::Socket::Type::STREAM)
            // ===== Callbacks ==============================================
            , ON_ITERATE (this, &Server::OnIterate)
            , ON_STARTING(this, &Server::OnStarting)
            , ON_STOP    (this, &Server::OnStop)
            , ON_STOPPING(this, &Server::OnStopping)
        {
            mThread.mOnIterate  = &ON_ITERATE;
            mThread.mOnStarting = &ON_STARTING;
            mThread.mOnStop     = &ON_STOP;
            mThread.mOnStopping = &ON_STOPPING;
        }

        void Server::Start()
        {
            mThread.Start();

            unsigned int lIteration = 0;

            while (0 == mSocket.mLocalAddress.Get().GetPortNumber())
            {
                KMS_EXCEPTION_ASSERT(10 > lIteration, RESULT_TIMEOUT, "Socket not binded in the allowed time", "");

                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                lIteration++;
            }
        }

        void Server::StopAndWait(unsigned int aTimeout_ms) { mThread.StopAndWait(aTimeout_ms); }

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Callbacks ==================================================

        unsigned int Server::OnIterate(void*, void*)
        {
            Net::Address lFrom;

            auto lSocket = mSocket.Accept(ACCEPT_TIMEOUT_ms, &lFrom);
            if (nullptr != lSocket)
            {
                OnConnect(lSocket);
            }

            return 0;
        }

        unsigned int Server::OnStarting(void*, void*)
        {
            Net::Thread_Startup();

            return 0;
        }

        unsigned int Server::OnStop(void*, void*)
        {
            mSocket.Close();

            return 0;
        }

        unsigned int Server::OnStopping(void*, void*)
        {
            Net::Thread_Cleanup();

            return 0;
        }

    }
}

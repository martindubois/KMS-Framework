
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

// TODO Use a base class Network::Server.

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/HTTP/Request.h>

#include <KMS/HTTP/Server.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_LOCAL_PORT (80)

// Constants
// //////////////////////////////////////////////////////////////////////////

#define ACCEPT_TIMEOUT_ms (1000)

#define CODE_ON_ITERATE  (1)
#define CODE_ON_START    (2)
#define CODE_ON_STOP     (3)
#define CODE_ON_STOPPING (4)

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Server::Server() : mOnRequest(this), mSocket(Network::Socket::Type::STREAM)
        {
            mSocket.SetLocalPort(DEFAULT_LOCAL_PORT);

            mThread.mOnIterate .Set(this, CODE_ON_ITERATE);
            mThread.mOnStarting.Set(this, CODE_ON_START);
            mThread.mOnStop    .Set(this, CODE_ON_STOP);
            mThread.mOnStopping.Set(this, CODE_ON_STOPPING);
        }

        // ===== Message::IReceiver =========================================

        bool Server::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            assert(&mThread == aSender);

            bool lResult = false;

            switch (aCode)
            {
            case CODE_ON_ITERATE : lResult = OnIterate (); break;
            case CODE_ON_START   : lResult = OnStarting(); break;
            case CODE_ON_STOP    : lResult = OnStop    (); break;
            case CODE_ON_STOPPING: lResult = OnStopping(); break;

            default: assert(false);
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Server::OnIterate()
        {
            bool lResult = true;

            Network::Address lFrom;

            Network::Socket* lSocket = mSocket.Accept(ACCEPT_TIMEOUT_ms, &lFrom);
            if (NULL != lSocket)
            {
                Request lRequest(lSocket);

                lResult = mOnRequest.Send(&lRequest);
                if (lResult)
                {
                    lRequest.Reply();
                }
            }

            return true;
        }

        bool Server::OnStarting()
        {
            Network::Thread_Startup();

            return true;
        }

        bool Server::OnStop()
        {
            mSocket.Close();

            return true;
        }

        bool Server::OnStopping()
        {
            Network::Thread_Cleanup();

            return true;
        }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/HTTP/Request.h>

#include <KMS/HTTP/Server.h>

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_LOCAL_PORT (80)

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Server::Server() : mOnRequest(this)
        {
            mResponseHeader.Set("Server", "KMS-Framework");

            mSocket.SetLocalPort(DEFAULT_LOCAL_PORT);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Server::OnConnect(Net::Socket* aSocket)
        {
            try
            {
                Request lRequest(aSocket);

                lRequest.mResponseHeader = mResponseHeader;

                if (lRequest.Receive())
                {
                    if (mOnRequest.Send(&lRequest))
                    {
                        lRequest.Reply();
                    }
                }
            }
            catch (Exception eE)
            {
                switch (eE.GetCode())
                {
                case Exception::Code::SOCKET_RECEIVE: break;

                default: throw eE;
                }
            }
        }

    }
}

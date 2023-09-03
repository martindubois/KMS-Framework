
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <WinSock2.h>

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/HTTP/Request.h>

#include <KMS/HTTP/Server.h>

KMS_RESULT_STATIC(RESULT_SOCKET_RECEIVE_FAILED);

// Config
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_LOCAL_PORT (80)

// Constants
// //////////////////////////////////////////////////////////////////////////

#define NAME_SERVER "Server"

static const KMS::DI::String SERVER("KMS-Framework");

namespace KMS
{
    namespace HTTP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Server::Server()
        {
            mResponseHeader.AddConstEntry(NAME_SERVER, &SERVER);

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
                    auto lRet = mOnRequest.Send(this, &lRequest);
                    if (CALLBACK_SUCCESS(lRet))
                    {
                        lRequest.Reply();
                    }
                }
            }
            catch (Exception eE)
            {
                if (RESULT_SOCKET_RECEIVE_FAILED != eE.GetCode())
                {
                    throw eE;
                }
            }
        }

    }
}

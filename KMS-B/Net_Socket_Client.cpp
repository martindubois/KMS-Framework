
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_Client.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Net/Socket_Client.h>

KMS_RESULT_STATIC(RESULT_SOCKET_CONNECT_FAILED);

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_REMOTE_ADDRESS("RemoteAddress = {Address}");

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const Net::Address Socket_Client::REMOTE_ADDRESS_DEFAULT;

        Socket_Client::Socket_Client(Type aType) : Socket(aType)
        {
            AddEntry("RemoteAddress", &mRemoteAddress, false, &MD_REMOTE_ADDRESS);
        }

        void Socket_Client::Connect   () { VerifyState(State::CONNECTED); }
        void Socket_Client::Disconnect() { VerifyState(State::CLOSED   ); }

        // ===== Socket =====================================================

        Socket_Client::~Socket_Client() {}

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== Socket =====================================================

        void Socket_Client::VerifyState_CLOSED(State aS)
        {
            switch (aS)
            {
            case State::CONNECTED: Open(); Connect_Internal(); break;

            default: Socket::VerifyState_CLOSED(aS);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Socket_Client::Connect_Internal()
        {
            auto lRet = connect(mSocket, mRemoteAddress.Get(), mRemoteAddress.Get().GetInternalSize());
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SOCKET_CONNECT_FAILED, "Connect failed", lRet);

            mState = State::CONNECTED;
        }

        void Socket_Client::VerifyState_OPEN(State aS)
        {
            switch (aS)
            {
            case State::CONNECTED: Connect_Internal(); break;

            default: Socket::VerifyState_OPEN(aS);
            }
        }

    }
}

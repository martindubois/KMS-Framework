
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_Server.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/NetAddressRange.h>

#include <KMS/Net/Socket_Server.h>

KMS_RESULT_STATIC(RESULT_SOCKET_LISTEN_FAILED);

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_ALLOWED_RANGES("AllowedRanges += {Address}");

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Socket_Server::Socket_Server(Type aType) : Socket(aType)
        {
            mAllowedRanges.SetCreator(DI::NetAddressRange::Create);

            AddEntry("AllowedRanges", &mAllowedRanges, false, &MD_ALLOWED_RANGES);
        }

        Socket* Socket_Server::Accept(unsigned int aTimeout_ms, Address* aFrom)
        {
            assert(nullptr != aFrom);

            VerifyState(State::LISTEN);

            assert(INVALID_SOCKET != mSocket);

            Socket* lResult = nullptr;

            if (Select(aTimeout_ms))
            {
                int lSize_byte = Address::INTERNAL_SIZE_byte;
                assert(0 < lSize_byte);

                auto lSocket = accept(mSocket, *aFrom, &lSize_byte);
                if (INVALID_SOCKET != lSocket)
                {
                    aFrom->SetInternalSize(lSize_byte);

                    if (IsInAllowedRanges(*aFrom))
                    {
                        lResult = new Socket(Socket::Type::STREAM);

                        lResult->CopyAttributes(*this);

                        lResult->Set(lSocket);
                    }
                    else
                    {
                        auto lRet = closesocket(lSocket);
                        assert(0 == lRet);
                    }
                }
            }

            return lResult;
        }

        // ===== Socket =====================================================

        Socket_Server::~Socket_Server() { VerifyState(State::CLOSED); }

        // Protected
        // //////////////////////////////////////////////////////////////////

        // ===== Socket =====================================================

        void Socket_Server::VerifyState(State aS)
        {
            switch (mState)
            {
            case State::CLOSED: VerifyState_CLOSED(aS); break;
            case State::LISTEN: VerifyState_LISTEN(aS); break;
            case State::OPEN  : VerifyState_OPEN  (aS); break;

            default: assert(false);
            }
        }

        void Socket_Server::VerifyState_CLOSED(State aS)
        {
            switch (aS)
            {
            case State::LISTEN: Open(); Listen(); break;

            default: Socket::VerifyState_CLOSED(aS);
            }
        }

        void Socket_Server::VerifyState_LISTEN(State aS)
        {
            switch (aS)
            {
            case State::CLOSED: CloseSocket(); break;
            case State::LISTEN: break;

            case State::OPEN:
                KMS_EXCEPTION(RESULT_INVALID_STATE, "The oepration in impossible in the current state", static_cast<unsigned int>(aS));

            default: assert(false);
            }
        }

        void Socket_Server::VerifyState_OPEN(State aS)
        {
            switch (aS)
            {
            case State::LISTEN: Listen(); break;

            default: Socket::VerifyState_OPEN(aS);
            }
        }
        
        // Private
        // //////////////////////////////////////////////////////////////////

        bool Socket_Server::IsInAllowedRanges(const Address& aA) const
        {
            for (const DI::Object* lObj : mAllowedRanges.mInternal)
            {
                assert(nullptr != lObj);

                auto lAR = dynamic_cast<const DI::NetAddressRange*>(lObj);
                assert(nullptr != lAR);

                if (lAR->Get() == aA)
                {
                    return true;
                }
            }

            return false;
        }

        void Socket_Server::Listen()
        {
            assert(INVALID_SOCKET != mSocket);
            assert(State::OPEN == mState);

            auto lRet = listen(mSocket, 1);
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SOCKET_LISTEN_FAILED, "listen failed", lRet);

            mState = State::LISTEN;
        }

    }
}

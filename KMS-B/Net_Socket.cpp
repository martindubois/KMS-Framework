
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/NetAddressRange.h>

#include <KMS/Net/Socket.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_LOCAL_ADDRESS      ("127.0.0.1")
#define DEFAULT_LOCAL_PORT         (0)
#define DEFAULT_RECEIVE_TIMEOUT_ms (1000)
#define DEFAULT_SEND_TIMEOUT_ms    (1000)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_ALLOWED_RANGES  ("AllowedRanges += {Address}");
static const KMS::Cfg::MetaData MD_LOCAL_ADDRESS   ("LocalAddress = {Address}");
static const KMS::Cfg::MetaData MD_RECEIVER_TIMEOUT("ReceiverTimeout = {ms}");
static const KMS::Cfg::MetaData MD_SEND_TIMEOUT    ("SendTimeout = {ms}");

namespace KMS
{
    namespace Net
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        void Thread_Startup()
        {
            WSADATA lData;

            int lRet = WSAStartup(MAKEWORD(2, 2), &lData);
            KMS_EXCEPTION_ASSERT(0 == lRet, SOCKET_STARTUP, "WSAStartup failed", lRet);
        }

        void Thread_Cleanup()
        {
            int lRet = WSACleanup();
            assert(0 == lRet);
        }

        // Public
        // //////////////////////////////////////////////////////////////////

        Socket::Socket(Type aType)
            : mLocalAddress     (DEFAULT_LOCAL_ADDRESS)
            , mReceiveTimeout_ms(DEFAULT_RECEIVE_TIMEOUT_ms)
            , mSendTimeout_ms   (DEFAULT_SEND_TIMEOUT_ms)
            , mBroadcastReceive(false)
            , mSocket(INVALID_SOCKET)
            , mState(State::CLOSED)
            , mType(aType)
        {
            mAllowedRanges.SetCreator(DI::NetAddressRange::Create);

            AddEntry("AllowedRanges" , &mAllowedRanges    , false, &MD_ALLOWED_RANGES);
            AddEntry("LocalAddress"  , &mLocalAddress     , false, &MD_LOCAL_ADDRESS);
            AddEntry("ReceiveTimeout", &mReceiveTimeout_ms, false, &MD_RECEIVER_TIMEOUT);
            AddEntry("SendTimeout"   , &mSendTimeout_ms   , false, &MD_SEND_TIMEOUT);
        }

        Socket::~Socket() { VerifyState(State::CLOSED); }

        uint16_t Socket::GetLocalPort() const { return mLocalAddress.Get().GetPortNumber(); }

        void Socket::SetLocalAddress(const Address& aA) { mLocalAddress = aA; }

        void Socket::SetLocalPort(uint16_t aP) { mLocalAddress.Get().SetPortNumber(aP); }

        void Socket::SetReceiveTimeout(unsigned int aRT_ms) { mReceiveTimeout_ms = aRT_ms; }
        void Socket::SetSendTimeout(unsigned int aST_ms) { mSendTimeout_ms = aST_ms; }

        Socket* Socket::Accept(unsigned int aTimeout_ms, Address* aFrom)
        {
            assert(NULL != aFrom);

            VerifyState(State::LISTEN);

            assert(INVALID_SOCKET != mSocket);

            Socket* lResult = NULL;

            if (Select(aTimeout_ms))
            {
                int lSize_byte = Address::INTERNAL_SIZE_byte;
                assert(0 < lSize_byte);

                SOCKET lSocket = accept(mSocket, *aFrom, &lSize_byte);
                if (INVALID_SOCKET != lSocket)
                {
                    aFrom->SetInternalSize(lSize_byte);

                    if (IsInAllowedRanges(*aFrom))
                    {
                        lResult = new Socket(Socket::Type::STREAM);

                        lResult->Set(lSocket);
                    }
                    else
                    {
                        int lRet = closesocket(lSocket);
                        assert(0 == lRet);
                    }
                }
            }

            return lResult;
        }

        void Socket::Close() { VerifyState(State::CLOSED); }

        void Socket::Open()
        {
            VerifyState(State::CLOSED);

            assert(INVALID_SOCKET == mSocket);

            int lType;
            int lProt;

            switch (mType)
            {
            case Type::DGRAM : lProt = IPPROTO_UDP; lType = SOCK_DGRAM ; break;
            case Type::STREAM: lProt = IPPROTO_TCP; lType = SOCK_STREAM; break;

            default: assert(false);
            }

            mSocket = socket(mLocalAddress.Get().GetInternalFamily(), lType, lProt);
            KMS_EXCEPTION_ASSERT(INVALID_SOCKET != mSocket, SOCKET, "socket failed", "");

            SetOption(SO_RCVTIMEO, mReceiveTimeout_ms);
            SetOption(SO_SNDTIMEO, mSendTimeout_ms);

            Bind();

            mState = State::OPEN;
        }

        unsigned int Socket::Receive(void* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aOut);

            assert(INVALID_SOCKET != mSocket);

            int lResult = recv(mSocket, reinterpret_cast<char*>(aOut), aOutSize_byte, 0);
            KMS_EXCEPTION_ASSERT((0 <= lResult) && (aOutSize_byte >= static_cast<unsigned int>(lResult)), SOCKET_RECEIVE, "recv failed", lResult);

            return lResult;
        }

        void Socket::Send(const void* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            assert(INVALID_SOCKET != mSocket);

            int lRet = send(mSocket, reinterpret_cast<const char*>(aIn), aInSize_byte, 0);
            KMS_EXCEPTION_ASSERT(aInSize_byte == lRet, SOCKET_SEND, "send failed", lRet);
        }

        void Socket::Send(File::Binary* aFile)
        {
            uint8_t lBuffer[4096];

            unsigned int lSize_byte;

            do
            {
                lSize_byte = aFile->Read(lBuffer, sizeof(lBuffer));
                if (0 < lSize_byte)
                {
                    Send(lBuffer, lSize_byte);
                }
            }
            while (sizeof(lBuffer) == lSize_byte);
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Socket::Set(SOCKET aSocket)
        {
            assert(INVALID_SOCKET != aSocket);

            assert(INVALID_SOCKET == mSocket);
            assert(State::CLOSED == mState);

            mSocket = aSocket;
            mState = State::CONNECTED;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Socket::Bind()
        {
            assert(INVALID_SOCKET != mSocket);

            if (0 != bind(mSocket, mLocalAddress.Get(), mLocalAddress.Get().GetInternalSize()))
            {
                closesocket(mSocket);
                mSocket = INVALID_SOCKET;

                KMS_EXCEPTION(SOCKET_BIND, "Cannot bind the socket", "");
            }
        }

        void Socket::CloseSocket()
        {
            assert(INVALID_SOCKET != mSocket);

            int lRet = closesocket(mSocket);
            // TODO assert(0 == lRet);

            mSocket = INVALID_SOCKET;
            mState = State::CLOSED;
        }

        void Socket::Listen()
        {
            assert(INVALID_SOCKET != mSocket);
            assert(State::OPEN == mState);

            int lRet = listen(mSocket, 1);
            KMS_EXCEPTION_ASSERT(0 == lRet, SOCKET_LISTEN, "listen failed", lRet);

            mState = State::LISTEN;
        }

        bool Socket::Select(unsigned int aTimeout_ms)
        {
            assert(INVALID_SOCKET != mSocket);

            fd_set  lSet;
            timeval lTimeout;

            FD_ZERO(&lSet);
            FD_SET(mSocket, &lSet);

            lTimeout.tv_sec = aTimeout_ms / 1000;
            lTimeout.tv_usec = (aTimeout_ms % 1000) * 1000;

            return 1 == select(0, &lSet, NULL, NULL, &lTimeout);
        }

        void Socket::SetOption(int aOptName, DWORD aValue)
        {
            assert(INVALID_SOCKET != mSocket);

            int lRet = setsockopt(mSocket, SOL_SOCKET, aOptName, reinterpret_cast<char*>(&aValue), sizeof(aValue));
            KMS_EXCEPTION_ASSERT(0 == lRet, SOCKET_OPTION, "setsockopt failed", lRet);
        }

        bool Socket::IsInAllowedRanges(const Address& aA) const
        {
            for (const DI::Object* lObj : mAllowedRanges.mInternal)
            {
                assert(NULL != lObj);

                const DI::NetAddressRange* lAR = dynamic_cast<const DI::NetAddressRange*>(lObj);
                assert(NULL != lAR);

                if (lAR->Get() == aA)
                {
                    return true;
                }
            }

            return false;
        }

        void Socket::VerifyState(State aS)
        {
            switch (mState)
            {
            case State::CLOSED   : VerifyState_CLOSED   (aS); break;
            case State::CONNECTED: VerifyState_CONNECTED(aS); break;
            case State::LISTEN   : VerifyState_LISTEN   (aS); break;
            case State::OPEN     : VerifyState_OPEN     (aS); break;

            default: assert(false);
            }
        }

        void Socket::VerifyState_CLOSED(State aS)
        {
            switch (aS)
            {
            case State::CLOSED   : break;
            case State::CONNECTED: KMS_EXCEPTION(STATE, "The oepration in impossible in the current state", static_cast<unsigned int>(aS));
            case State::LISTEN   : Open(); Listen(); break;
            case State::OPEN     : Open(); break;

            default: assert(false);
            }
        }

        void Socket::VerifyState_CONNECTED(State aS)
        {
            switch (aS)
            {
            case State::CLOSED   : CloseSocket(); break;
            case State::CONNECTED: break;

            case State::LISTEN:
            case State::OPEN  :
                KMS_EXCEPTION(STATE, "The oepration in impossible in the current state", static_cast<unsigned int>(aS));

            default: assert(false);
            }
        }

        void Socket::VerifyState_LISTEN(State aS)
        {
            switch (aS)
            {
            case State::CLOSED: CloseSocket(); break;
            case State::LISTEN: break;

            case State::CONNECTED:
            case State::OPEN     :
                KMS_EXCEPTION(STATE, "The oepration in impossible in the current state", static_cast<unsigned int>(aS));

            default: assert(false);
            }
        }

        void Socket::VerifyState_OPEN(State aS)
        {
            switch (aS)
            {
            case State::CLOSED   : CloseSocket(); break;
            case State::CONNECTED: KMS_EXCEPTION(STATE, "The operation in impossible in the current state", static_cast<unsigned int>(aS));
            case State::LISTEN   : Listen(); break;
            case State::OPEN     : break;

            default: assert(false);
            }
        }
    }
}

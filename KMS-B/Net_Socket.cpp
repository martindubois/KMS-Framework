
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket.cpp

// TEST COVERAGE  2023-10-17  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Net/Socket.h>

KMS_RESULT_STATIC(RESULT_SOCKET_BIND_FAILED);
KMS_RESULT_STATIC(RESULT_SOCKET_FAILED);
KMS_RESULT_STATIC(RESULT_SOCKET_OPTION_FAILED);

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_KEEP_A_LIVE     ("KeepALive = false | true");
static const KMS::Cfg::MetaData MD_LOCAL_ADDRESS   ("LocalAddress = {Address}");
static const KMS::Cfg::MetaData MD_NO_DELAY        ("NoDelay = false | true");
static const KMS::Cfg::MetaData MD_RECEIVER_TIMEOUT("ReceiverTimeout = {ms}");
static const KMS::Cfg::MetaData MD_REUSE_ADDR      ("ReuseAddr = false | true");
static const KMS::Cfg::MetaData MD_SEND_TIMEOUT    ("SendTimeout = {ms}");

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const bool     Socket::KEEP_A_LIVE_DEFAULT        = true;
        const char   * Socket::LOCAL_ADDRESS_DEFAULT      = "127.0.0.1";
        const bool     Socket::NO_DELAY_DEFAULT           = false;
        const uint32_t Socket::RECEIVE_TIMEOUT_DEFAULT_ms = 1000;
        const bool     Socket::REUSE_ADDR_DEFAULT         = false;
        const uint32_t Socket::SEND_TIMEOUT_DEFAULT_ms    = 1000;

        Socket::Socket(Type aType)
            : mKeepALive        (KEEP_A_LIVE_DEFAULT)
            , mLocalAddress     (LOCAL_ADDRESS_DEFAULT)
            , mNoDelay          (NO_DELAY_DEFAULT)
            , mReceiveTimeout_ms(RECEIVE_TIMEOUT_DEFAULT_ms)
            , mReuseAddr        (REUSE_ADDR_DEFAULT)
            , mSendTimeout_ms   (SEND_TIMEOUT_DEFAULT_ms)
            , mBroadcastReceive(false)
            , mSocket(INVALID_SOCKET)
            , mState(State::CLOSED)
            , mType(aType)
        {
            AddEntry("KeepALive"     , &mKeepALive        , false, &MD_KEEP_A_LIVE);
            AddEntry("LocalAddress"  , &mLocalAddress     , false, &MD_LOCAL_ADDRESS);
            AddEntry("NoDelay"       , &mNoDelay          , false, &MD_NO_DELAY);
            AddEntry("ReceiveTimeout", &mReceiveTimeout_ms, false, &MD_RECEIVER_TIMEOUT);
            AddEntry("ReuseAddr"     , &mReuseAddr        , false, &MD_REUSE_ADDR);
            AddEntry("SendTimeout"   , &mSendTimeout_ms   , false, &MD_SEND_TIMEOUT);
        }

        Socket::~Socket() { VerifyState(State::CLOSED); }

        uint16_t Socket::GetLocalPort() const { return mLocalAddress.Get().GetPortNumber(); }

        void Socket::SetLocalPort(uint16_t aP) { mLocalAddress.Get().SetPortNumber(aP); }

        void Socket::Close() { VerifyState(State::CLOSED); }

        // NOT TESTED  Type::DGRAM
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
            KMS_EXCEPTION_ASSERT(INVALID_SOCKET != mSocket, RESULT_SOCKET_FAILED, "socket failed (NOT TESTED)", "");

            Configure();

            Bind();

            mState = State::OPEN;
        }

        unsigned int Socket::Receive(void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);

            assert(INVALID_SOCKET != mSocket);

            auto lResult = recv(mSocket, reinterpret_cast<char*>(aOut), aOutSize_byte, 0);
            KMS_EXCEPTION_ASSERT((0 <= lResult) && (aOutSize_byte >= static_cast<unsigned int>(lResult)), RESULT_RECEIVE_FAILED, "recv failed", lResult);

            return lResult;
        }

        void Socket::Send(const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);

            VerifyState(State::CONNECTED);

            assert(INVALID_SOCKET != mSocket);

            auto lRet = send(mSocket, reinterpret_cast<const char*>(aIn), aInSize_byte, 0);
            KMS_EXCEPTION_ASSERT(aInSize_byte == lRet, RESULT_SEND_FAILED, "send failed", lRet);
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

        void Socket::Shutdown()
        {
            assert(INVALID_SOCKET != mSocket);
            
            auto lRet = shutdown(mSocket, SB_BOTH);
            // assert(0 == lRet);
        }

        // ===== DI::Container ==============================================
        void Socket::Validate() const
        {
            DI::Dictionary::Validate();

            // TODO
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Socket::CopyAttributes(const Socket& aIn)
        {
            mKeepALive         = aIn.mKeepALive;
            mLocalAddress      = aIn.mLocalAddress;
            mNoDelay           = aIn.mNoDelay;
            mReceiveTimeout_ms = aIn.mReceiveTimeout_ms;
            mReuseAddr         = aIn.mReuseAddr;
            mSendTimeout_ms    = aIn.mSendTimeout_ms;
        }

        void Socket::Set(SOCKET aSocket)
        {
            assert(INVALID_SOCKET != aSocket);

            assert(INVALID_SOCKET == mSocket);
            assert(State::CLOSED == mState);

            mSocket = aSocket;
            mState = State::CONNECTED;

            Configure();
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Socket::VerifyState(State aS)
        {
            switch (mState)
            {
            case State::CLOSED   : VerifyState_CLOSED   (aS); break;
            case State::CONNECTED: VerifyState_CONNECTED(aS); break;
            case State::OPEN     : VerifyState_OPEN     (aS); break; // NOT TESTED

            default: assert(false);
            }
        }

        void Socket::VerifyState_CLOSED(State aS)
        {
            switch (aS)
            {
            case State::CLOSED   : break;
            case State::CONNECTED: KMS_EXCEPTION(RESULT_INVALID_STATE, "The oepration in impossible in the current state (NOT TESTED)", static_cast<unsigned int>(aS));
            case State::OPEN     : Open(); break; // NOT TESTED

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
                KMS_EXCEPTION(RESULT_INVALID_STATE, "The operation in impossible in the current state (NOT TESTED)", static_cast<unsigned int>(aS));

            default: assert(false);
            }
        }

        // NOT TESTED
        void Socket::VerifyState_OPEN(State aS)
        {
            switch (aS)
            {
            case State::CLOSED   : CloseSocket(); break;
            case State::CONNECTED: KMS_EXCEPTION(RESULT_INVALID_STATE, "The operation in impossible in the current state (NOT TESTED)", static_cast<unsigned int>(aS));
            case State::OPEN     : break;

            default: assert(false);
            }
        }

        void Socket::CloseSocket()
        {
            assert(INVALID_SOCKET != mSocket);

            auto lRet = closesocket(mSocket);
            // TODO assert(0 == lRet);

            mSocket = INVALID_SOCKET;
            mState = State::CLOSED;
        }

        bool Socket::Select(unsigned int aTimeout_ms)
        {
            assert(INVALID_SOCKET != mSocket);

            fd_set  lSet;
            timeval lTimeout;

            FD_ZERO(&lSet);
            FD_SET(mSocket, &lSet);

            lTimeout.tv_sec  =  aTimeout_ms / 1000;
            lTimeout.tv_usec = (aTimeout_ms % 1000) * 1000;

            return 1 == select(0, &lSet, NULL, NULL, &lTimeout);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Socket::Bind()
        {
            assert(INVALID_SOCKET != mSocket);

            auto lLA = mLocalAddress.Get();

            auto lRet = bind(mSocket, lLA, lLA.GetInternalSize());
            if (0 != lRet)
            {
                closesocket(mSocket);
                mSocket = INVALID_SOCKET;

                KMS_EXCEPTION(RESULT_SOCKET_BIND_FAILED, "Cannot bind the socket (NOT TESTED)", lRet);
            }
        }

        void Socket::Configure()
        {
            SetOption(SO_RCVTIMEO, mReceiveTimeout_ms);
            SetOption(SO_SNDTIMEO, mSendTimeout_ms);

            SetOption(SO_KEEPALIVE, mKeepALive);
            SetOption(SO_REUSEADDR, mReuseAddr);

            if (Type::STREAM == mType)
            {
                SetOption_TCP(TCP_NODELAY, mNoDelay);
            }
        }

        void Socket::SetOption(int aOptName, DWORD aValue)
        {
            assert(INVALID_SOCKET != mSocket);

            auto lRet = setsockopt(mSocket, SOL_SOCKET, aOptName, reinterpret_cast<char*>(&aValue), sizeof(aValue));
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SOCKET_OPTION_FAILED, "setsockopt failed (NOT TESTED)", lRet);
        }

        void Socket::SetOption_TCP(int aOptName, DWORD aValue)
        {
            assert(INVALID_SOCKET != mSocket);

            auto lRet = setsockopt(mSocket, IPPROTO_TCP, aOptName, reinterpret_cast<char*>(&aValue), sizeof(aValue));
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_SOCKET_OPTION_FAILED, "setsockopt failed (NOT TESTED)", lRet);
        }

    }
}

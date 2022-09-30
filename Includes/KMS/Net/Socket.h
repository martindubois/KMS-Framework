
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Socket.h
// Library   KMS-B

#pragma once

// ===== C ===============================================================
#include <stdint.h>

// ===== Windows =========================================================
#include <winsock2.h>

// ===== Includes ========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/NetAddress.h>
#include <KMS/DI/UInt32.h>
#include <KMS/File/Binary.h>
#include <KMS/Net/AddressRangeList.h>

namespace KMS
{
    namespace Net
    {

        extern void Thread_Startup();
        extern void Thread_Cleanup();

        class Socket : public DI::Dictionary
        {

        public:

            enum class Type
            {
                DGRAM,
                STREAM,
            };

            Socket(Type aType);

            ~Socket();

            void AddMulticastReceive(Address& aMR);

            void ClearLocalAddress();

            Address  GetLocalAddress () const;
            uint16_t GetLocalPort    () const;
            Address  GetRemoteAddress() const;

            void SetBroadcastReceive(bool aAllow = true);

            void SetLocalAddress(const Address& aA);
            void SetLocalPort   (uint16_t aP = 0);

            void SetReceiveTimeout(unsigned int aRT_ms);
            void SetSendTimeout   (unsigned int aST_ms);

            Socket * Accept(unsigned int aTimeout_ms, Address* aFrom);

            void Close();

            void Connect(const Address& aTo);

            void Disconnect();

            void Open();

            unsigned int Receive(void* aOut, unsigned int aOutSize_byte);

            unsigned int ReceiveFrom(void* aOut, unsigned int aOutSize_byte, Address* aFrom);

            void Send(const void* aIn, unsigned int aInSize_byte);

            void Send(File::Binary* aFile);

            void SendTo(const Address& aAddress, const void* aIn, unsigned int aInSize_byte);

            // ===== Configurable attributes ================================
            DI::Array      mAllowedRanges;
            DI::NetAddress mLocalAddress;
            DI::UInt32     mReceiveTimeout_ms;
            DI::UInt32     mSendTimeout_ms;

        // Internal

            void Set(SOCKET aS);

        private:

            // --> CLOSED <==+=======+------+
            //      |        |       |      |
            //      +----> OPEN --> LISTEN  |
            //              |               |
            //              +--> CONNETED --+
            enum class State
            {
                CLOSED,
                CONNECTED,
                LISTEN,
                OPEN,
            };

            Socket(const Socket&);

            const Socket& operator = (const Socket&);

            void Bind();

            void CloseSocket();

            void Listen();

            bool Select(unsigned int aTimeout_ms);

            void SetOption(int aOptName, DWORD aValue);

            bool IsInAllowedRanges(const Address& aA) const;

            void VerifyState(State aS);

            void VerifyState_CLOSED   (State aS);
            void VerifyState_CONNECTED(State aS);
            void VerifyState_LISTEN   (State aS);
            void VerifyState_OPEN     (State aS);

            bool mBroadcastReceive;

            SOCKET mSocket;

            State mState;

            Type mType;

        };

    }
}

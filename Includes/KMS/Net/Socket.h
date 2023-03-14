
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/NetAddress.h>
#include <KMS/DI/UInt.h>
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

            void Close();

            void Open();

            unsigned int Receive(void* aOut, unsigned int aOutSize_byte);

            unsigned int ReceiveFrom(void* aOut, unsigned int aOutSize_byte, Address* aFrom);

            void Send(const void* aIn, unsigned int aInSize_byte);

            void Send(File::Binary* aFile);

            void SendTo(const Address& aAddress, const void* aIn, unsigned int aInSize_byte);

            // ===== Configurable attributes ================================
            DI::Boolean        mKeepALive;
            DI::NetAddress     mLocalAddress;
            DI::Boolean        mNoDelay;
            DI::UInt<uint32_t> mReceiveTimeout_ms;
            DI::Boolean        mReuseAddr;
            DI::UInt<uint32_t> mSendTimeout_ms;

        // Internal

            void CopyAttributes(const Socket& aIn);

            void Set(SOCKET aS);

        protected:

            // --> CLOSED <==+=======+------+
            //        |      |       |      |
            //        +--> OPEN --> LISTEN  |
            //              |               |
            //              +--> CONNETED --+
            enum class State
            {
                CLOSED,
                CONNECTED,
                LISTEN,
                OPEN,
            };

            void CloseSocket();

            bool Select(unsigned int aTimeout_ms);

            virtual void VerifyState(State aS);
            
            virtual void VerifyState_CLOSED   (State aS);
            virtual void VerifyState_CONNECTED(State aS);
            virtual void VerifyState_OPEN     (State aS);

            SOCKET mSocket;

            State mState;

        private:

            Socket(const Socket&);

            const Socket& operator = (const Socket&);

            void Bind();

            void Configure();

            void SetOption(int aOptName, DWORD aValue);

            void SetOption_TCP(int aOptName, DWORD aValue);

            bool mBroadcastReceive;

            Type mType;

        };

    }
}

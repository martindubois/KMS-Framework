
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Socket.h
// Library   KMS-B

#pragma once

// ===== C ===============================================================
#include <stdint.h>

#ifdef _KMS_WINDOWS_
    // ===== Windows =====================================================
    #include <winsock2.h>
#endif

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

        /// \brief TODO Remove use of DI namespace
        class Socket : public DI::Dictionary
        {

        public:

            static const bool     KEEP_A_LIVE_DEFAULT;
            static const char   * LOCAL_ADDRESS_DEFAULT;
            static const bool     NO_DELAY_DEFAULT;
            static const uint32_t RECEIVE_TIMEOUT_DEFAULT_ms;
            static const bool     REUSE_ADDR_DEFAULT;
            static const uint32_t SEND_TIMEOUT_DEFAULT_ms;

            DI::Boolean        mKeepALive;
            DI::NetAddress     mLocalAddress;
            DI::Boolean        mNoDelay;
            DI::UInt<uint32_t> mReceiveTimeout_ms;
            DI::Boolean        mReuseAddr;
            DI::UInt<uint32_t> mSendTimeout_ms;

            enum class Type
            {
                DGRAM,
                STREAM,
            };

            Socket(Type aType);

            ~Socket();

            void AddMulticastReceive(Address& aMR);

            void ClearLocalAddress();

            uint16_t GetLocalPort    () const;

            void SetBroadcastReceive(bool aAllow = true);

            void SetLocalAddress(const Address& aA);
            void SetLocalPort   (uint16_t aP = 0);

            void Close();

            virtual void Open();

            virtual unsigned int Receive(void* aOut, unsigned int aOutSize_byte);

            unsigned int ReceiveFrom(void* aOut, unsigned int aOutSize_byte, Address* aFrom);

            virtual void Send(const void* aIn, unsigned int aInSize_byte);

            void Send(File::Binary* aFile);

            void SendTo(const Address& aAddress, const void* aIn, unsigned int aInSize_byte);

            void Shutdown();

            // ===== DI::Container ==========================================
            virtual void Validate() const;

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

            virtual void CloseSocket();

            bool Select(unsigned int aTimeout_ms);

            virtual void VerifyState(State aS);
            
            virtual void VerifyState_CLOSED   (State aS);
            virtual void VerifyState_CONNECTED(State aS);
            virtual void VerifyState_OPEN     (State aS);

            SOCKET mSocket;

            State mState;

        private:

            NO_COPY(Socket);

            void Bind();

            void Configure();

            void SetOption(int aOptName, uint32_t aValue);

            void SetOption_TCP(int aOptName, uint32_t aValue);

            bool mBroadcastReceive;

            Type mType;

        };

    }
}

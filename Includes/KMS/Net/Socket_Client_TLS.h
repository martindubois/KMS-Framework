
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Socket_Client_TLS.h
// Library   KMS-B

#pragma once

// ===== Includes ===========================================================
#include <KMS/Net/Socket_Client.h>

namespace KMS
{
    namespace Net
    {

        class Socket_Client_TLS : public Socket_Client
        {

        public:

            static const char* REMOTE_NAME_DEFAULT;

            DI::String mRemoteName;

            Socket_Client_TLS();

            ~Socket_Client_TLS();

            // ===== Socket_Client ==========================================
            virtual void         Connect();
            virtual void         Disconnect();
            virtual unsigned int Receive(void* aOut, unsigned int aOutSize_byte);
            virtual void         Send(const void* aIn, unsigned int aInSize_byte);

        private:

            #ifdef _KMS_WINDOWS_
                void ClearInternal();
                void DiscardConsumedData();

                CtxtHandle                mContext;
                uint8_t                 * mDecrypted;
                unsigned int              mDecrypted_byte;
                CredHandle                mHandle;
                uint8_t                   mIncoming[16384 + 512];
                unsigned int              mReceived_byte;
                SecPkgContext_StreamSizes mStreamSizes;
                unsigned int              mUsed_byte;
            #endif

        };

    }
}

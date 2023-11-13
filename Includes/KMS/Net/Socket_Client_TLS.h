
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Socket_Client_TLS.h
// Library   KMS-B

#pragma once

#ifdef _KMS_LINUX_
    // ===== OpenSSL ========================================================
    #include <openssl/ssl.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Net/Socket_Client.h>

namespace KMS
{
    namespace Net
    {

        class Socket_Client_TLS final : public Socket_Client
        {

        public:

            static const char* REMOTE_NAME_DEFAULT;

            DI::String mRemoteName;

            Socket_Client_TLS();

            ~Socket_Client_TLS();

            // ===== Socket =================================================
            virtual unsigned int Receive(void* aOut, unsigned int aOutSize_byte);
            virtual void         Send(const void* aIn, unsigned int aInSize_byte);

            #ifdef _KMS_LINUX_
                virtual void Open();
            #endif

        protected:

            void Construct_OSDep();

            // ===== Socket_Client ==========================================
            virtual void Connect_Internal();

            // ===== Socket =================================================
            virtual void CloseSocket();

        private:

            #ifdef _KMS_LINUX_
                BIO    * mConnect;
                SSL_CTX* mContext;
            #endif

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


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_Client_TLS_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Socket_Client_TLS.h>

KMS_RESULT_STATIC(RESULT_SOCKET_CONNECT_FAILED);
KMS_RESULT_STATIC(RESULT_SOCKET_SECURITY_ERROR);

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // ===== Socket =====================================================

        unsigned int Socket_Client_TLS::Receive(void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);

            assert(nullptr != mConnect);

            auto lResult_byte = BIO_read(mConnect, aOut, aOutSize_byte);
            KMS_EXCEPTION_ASSERT(0 <= lResult_byte, RESULT_SOCKET_SECURITY_ERROR, "BIO_read failed", lResult_byte);

            return lResult_byte;
        }

        void Socket_Client_TLS::Send(const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);
            assert(0 < aInSize_byte);

            assert(nullptr != mConnect);

            auto lRet = BIO_write(mConnect, aIn, aInSize_byte);
            KMS_EXCEPTION_ASSERT(aInSize_byte == lRet, RESULT_SOCKET_SECURITY_ERROR, "BIO_write failed", lRet);
        }

        void Socket_Client_TLS::Open()
        {
            VerifyState(State::CLOSED);

            assert(nullptr == mContext);
            assert(State::CLOSED == mState);

            SSL_library_init();

            SSL_load_error_strings();

            auto lMethod = SSLv23_method();
            KMS_EXCEPTION_ASSERT(nullptr != lMethod, RESULT_SOCKET_SECURITY_ERROR, "SSLv23_method failed", "");

            mContext = SSL_CTX_new(lMethod);
            KMS_EXCEPTION_ASSERT(nullptr != mContext, RESULT_SOCKET_SECURITY_ERROR, "SSL_CTX_new failed", "");

            mState = State::OPEN;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Socket_Client_TLS::Construct_OSDep()
        {
            mConnect = nullptr;
            mContext = nullptr;
        }

        // ===== Socket_Client ==============================================

        void Socket_Client_TLS::Connect_Internal()
        {
            assert(nullptr == mConnect);
            assert(nullptr != mContext);

            SSL_CTX_set_options(mContext, SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION);

            mConnect = BIO_new_ssl_connect(mContext);
            KMS_EXCEPTION_ASSERT(nullptr != mConnect, RESULT_SOCKET_SECURITY_ERROR, "BIO_new_ssl_connect failed", "");

            char lHostAndPort[PATH_LENGTH];

            sprintf(lHostAndPort, "%s:%u", mRemoteName.Get(), mRemoteAddress.mInternal.GetPortNumber());

            auto lRet = BIO_set_conn_hostname(mConnect, lHostAndPort);
            KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_SOCKET_SECURITY_ERROR, "BIO_set_conn_hostname failed", lRet);

            SSL* lSSL;

            BIO_get_ssl(mConnect, &lSSL);

            lRet = SSL_set_cipher_list(lSSL, "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4");
            KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_SOCKET_SECURITY_ERROR, "SSL_set_cipher_list failed", lRet);

            lRet = SSL_set_tlsext_host_name(lSSL, mRemoteName.Get());
            KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_SOCKET_SECURITY_ERROR, "SSL_set_tlsext_host_name failed", lRet);

            lRet = BIO_do_connect(mConnect);
            KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_SOCKET_CONNECT_FAILED, "BIO_do_connect failed", lRet);

            lRet = BIO_do_handshake(mConnect);
            KMS_EXCEPTION_ASSERT(1 == lRet, RESULT_SOCKET_SECURITY_ERROR, "BIO_do_handshake failed", lRet);
        }

        // ===== Socket =====================================================

        void Socket_Client_TLS::CloseSocket()
        {
            if (nullptr != mContext)
            {
                if (nullptr != mConnect)
                {
                    BIO_free_all(mConnect);
                    mConnect = nullptr;
                }

                SSL_CTX_free(mContext);
                mContext = nullptr;
            }

            mState = State::CLOSED;
        }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/Net_Socket_Client_TLS_W.cpp

// This source code is based on
// https://gist.github.com/mmozeiko/c0dfcc8fec527a90a02145d2cc0bfb6d

// TEST COVERAGE 2023-11-18 Martin Dubois

// CONFIG _CFG_TRACE_
// #define _CFG_TRACE_

#include "Component.h"

// ===== Windows ============================================================
#include <schannel.h>

// ===== Includes ===========================================================
#include <KMS/Net/Socket_Client_TLS.h>

#pragma comment (lib, "secur32.lib")

KMS_RESULT_STATIC(RESULT_SOCKET_SECURITY_ERROR);

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Buffer_Clear(SecBuffer* aBuffer);

static void Buffers_Clear(SecBufferDesc* aDesc);

static unsigned int Buffers_GetTotalSize(const SecBufferDesc& aDesc);

static void Buffers_SetPointers(SecBufferDesc* aDesc, void* aPtr);

#ifdef _CFG_TRACE_
    static void Buffers_Dump(const SecBufferDesc& aDesc);
    static void SEC_Dump(const char* aFunction, SECURITY_STATUS aSec);
#else
    #define Buffers_Dump(D)
    #define SEC_Dump(F,S)
#endif

namespace KMS
{
    namespace Net
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // ===== Socket_Client ==============================================

        unsigned int Socket_Client_TLS::Receive(void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);
            assert(0 < aOutSize_byte);

            unsigned int lResult_byte = 0;

            uint8_t* lOut = reinterpret_cast<uint8_t*>(aOut);
            auto lOutSize_byte = aOutSize_byte;

            while (0 < lOutSize_byte)
            {
                if (nullptr == mDecrypted)
                {
                    if (0 < mReceived_byte)
                    {
                        SecBuffer lBuffers[4];
                        SecBufferDesc lDesc = { SECBUFFER_VERSION, ARRAYSIZE(lBuffers), lBuffers };

                        Buffers_Clear(&lDesc);

                        assert(ARRAYSIZE(lBuffers) == mStreamSizes.cBuffers);

                        lBuffers[0].BufferType = SECBUFFER_DATA;
                        lBuffers[0].pvBuffer = mIncoming;
                        lBuffers[0].cbBuffer = mReceived_byte;

                        for (unsigned int i = 1; i < 4; i++)
                        {
                            lBuffers[i].BufferType = SECBUFFER_EMPTY;
                        }

                        auto lRet = DecryptMessage(&mContext, &lDesc, 0, nullptr);
                        SEC_Dump("DecryptMessage", lRet);
                        Buffers_Dump(lDesc);

                        if (SEC_E_OK == lRet)
                        {
                            assert(SECBUFFER_STREAM_HEADER  == lBuffers[0].BufferType);
                            assert(SECBUFFER_DATA           == lBuffers[1].BufferType);
                            assert(SECBUFFER_STREAM_TRAILER == lBuffers[2].BufferType);

                            mDecrypted      = reinterpret_cast<uint8_t*>(lBuffers[1].pvBuffer);
                            mDecrypted_byte = lBuffers[1].cbBuffer;
                            mUsed_byte      = mReceived_byte - (SECBUFFER_EXTRA == lBuffers[3].BufferType ? lBuffers[3].cbBuffer : 0);

                            continue;
                        }

                        if (SEC_I_CONTEXT_EXPIRED == lRet)
                        {
                            mReceived_byte = 0;
                            break;
                        }

                        KMS_EXCEPTION_ASSERT(SEC_I_RENEGOTIATE        != lRet, RESULT_SOCKET_SECURITY_ERROR, "TLS renegotiate is not suppoerted (NOT TESTED)", "");
                        KMS_EXCEPTION_ASSERT(SEC_E_INCOMPLETE_MESSAGE == lRet, RESULT_SOCKET_SECURITY_ERROR, "TLS protocol error (NOT TESTED)", lRet);
                    }

                    if (0 != lResult_byte)
                    {
                        break;
                    }

                    KMS_EXCEPTION_ASSERT(sizeof(mIncoming) > mReceived_byte, RESULT_SOCKET_SECURITY_ERROR, "Too many data (NOT_TESTED)", mReceived_byte);

                    auto lReceived_byte = Socket_Client::Receive(mIncoming + mReceived_byte, sizeof(mIncoming) - mReceived_byte);
                    if (0 == lReceived_byte)
                    {
                        break;
                    }

                    mReceived_byte += lReceived_byte;
                }
                else
                {
                    assert(0 < mDecrypted_byte);

                    unsigned int lUse_byte = min(lOutSize_byte, mDecrypted_byte);

                    memcpy(lOut, mDecrypted, lUse_byte);

                    // Erase the decrypted data we just return to the caller
                    memset(mDecrypted, 0, lUse_byte);

                    lOut          += lUse_byte;
                    lOutSize_byte -= lUse_byte;
                    lResult_byte  += lUse_byte;

                    mDecrypted      += lUse_byte;
                    mDecrypted_byte -= lUse_byte;

                    if (0 == mDecrypted_byte)
                    {
                        DiscardConsumedData();
                    }
                }
            }

            return lResult_byte;
        }

        void Socket_Client_TLS::Send(const void* aIn, unsigned int aInSize_byte)
        {
            const uint8_t* lIn = reinterpret_cast<const uint8_t*>(aIn);
            auto lInSize_byte = aInSize_byte;

            uint8_t lBuffer[sizeof(mIncoming)];
            assert(sizeof(lBuffer) >= mStreamSizes.cbHeader + mStreamSizes.cbMaximumMessage);

            // Clear buffer before using it
            memset(&lBuffer, 0, sizeof(lBuffer));

            while (0 < lInSize_byte)
            {
                assert(nullptr != lIn);

                auto lUse_byte = min(lInSize_byte, mStreamSizes.cbMaximumMessage);

                SecBuffer lBuffers[3];
                SecBufferDesc lDesc = { SECBUFFER_VERSION, ARRAYSIZE(lBuffers), lBuffers };

                lBuffers[0].BufferType = SECBUFFER_STREAM_HEADER;
                lBuffers[0].cbBuffer = mStreamSizes.cbHeader;
                lBuffers[1].BufferType = SECBUFFER_DATA;
                lBuffers[1].cbBuffer = lUse_byte;
                lBuffers[2].BufferType = SECBUFFER_STREAM_TRAILER;
                lBuffers[2].cbBuffer = mStreamSizes.cbTrailer;

                Buffers_SetPointers(&lDesc, lBuffer);

                assert(nullptr != lBuffers[1].pvBuffer);

                memcpy(lBuffers[1].pvBuffer, lIn, lUse_byte);

                auto lRet = EncryptMessage(&mContext, 0, &lDesc, 0);
                SEC_Dump("EncryptMessage", lRet);
                KMS_EXCEPTION_ASSERT(SEC_E_OK == lRet, RESULT_SOCKET_SECURITY_ERROR, "EncryptMessage failed (NOT TESTED)", lRet);

                auto lSize_byte = Buffers_GetTotalSize(lDesc);
                assert(0 < lSize_byte);

                Socket_Client::Send(lBuffer, lSize_byte);

                // Erase the crypted data we just send
                memset(&lBuffer, 0, sizeof(lBuffer));

                lIn          += lUse_byte;
                lInSize_byte -= lUse_byte;
            }
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Socket_Client_TLS::Construct_OSDep()
        {
            ClearInternal();
        }

        // ===== Socket_Client ==============================================

        void Socket_Client_TLS::Connect_Internal()
        {
            ClearInternal();

            Socket_Client::Connect_Internal();

            SCHANNEL_CRED lCred;

            memset(&lCred, 0, sizeof(lCred));

            lCred.dwFlags               = SCH_USE_STRONG_CRYPTO | SCH_CRED_AUTO_CRED_VALIDATION | SCH_CRED_NO_DEFAULT_CREDS;
            lCred.dwVersion             = SCHANNEL_CRED_VERSION;
            lCred.grbitEnabledProtocols = SP_PROT_TLS1_2;

            auto lRet = AcquireCredentialsHandle(nullptr, const_cast<SEC_CHAR*>(UNISP_NAME), SECPKG_CRED_OUTBOUND, nullptr, &lCred, nullptr, nullptr, &mHandle, nullptr);
            KMS_EXCEPTION_ASSERT(SEC_E_OK == lRet, RESULT_SOCKET_SECURITY_ERROR, "AcquireCredentialsHandle failed (NOT TESTED)", lRet);

            CtxtHandle* lContext = nullptr;
            DWORD       lFlags = ISC_REQ_USE_SUPPLIED_CREDS | ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;

            for (;;)
            {
                SecBuffer lInBuffers [2] = { 0 };
                SecBuffer lOutBuffers[1] = { 0 };

                SecBufferDesc lInDesc  = { SECBUFFER_VERSION, ARRAYSIZE(lInBuffers ), lInBuffers  };
                SecBufferDesc lOutDesc = { SECBUFFER_VERSION, ARRAYSIZE(lOutBuffers), lOutBuffers };

                Buffers_Clear(&lInDesc );
                Buffers_Clear(&lOutDesc);

                lInBuffers[0].BufferType = SECBUFFER_TOKEN;
                lInBuffers[0].pvBuffer   = mIncoming;
                lInBuffers[0].cbBuffer   = mReceived_byte;
                lInBuffers[1].BufferType = SECBUFFER_EMPTY;

                if (nullptr == lContext)
                {
                    lRet = InitializeSecurityContext(&mHandle, nullptr, const_cast<SEC_CHAR*>(mRemoteName.Get()),
                        lFlags, 0, 0, nullptr,
                        0, &mContext,
                        &lOutDesc, &lFlags, nullptr);
                }
                else
                {
                    lRet = InitializeSecurityContext(&mHandle, lContext, nullptr,
                        lFlags, 0, 0, &lInDesc,
                        0, nullptr,
                        &lOutDesc, &lFlags, nullptr);
                }

                SEC_Dump("InitializeSecurityContext", lRet);
                Buffers_Dump(lInDesc);

                lContext = &mContext;

                switch (lInBuffers[1].BufferType)
                {
                case SECBUFFER_EXTRA:
                    memmove(&mIncoming, mIncoming + mReceived_byte - lInBuffers[1].cbBuffer, lInBuffers[1].cbBuffer);
                    // TODO  Erase moved data
                    mReceived_byte = lInBuffers[1].cbBuffer;
                    break;

                case SECBUFFER_MISSING: break;

                default: mReceived_byte = 0;
                }

                if (SEC_E_OK == lRet)
                {
                    break;
                }

                KMS_EXCEPTION_ASSERT(SEC_I_INCOMPLETE_CREDENTIALS != lRet, RESULT_SOCKET_SECURITY_ERROR, "InitializeSecurityContext failed (NOT TESTED)", "");

                if (SEC_I_CONTINUE_NEEDED == lRet)
                {
                    if (0 < lOutBuffers[0].cbBuffer)
                    {
                        Socket_Client::Send(lOutBuffers[0].pvBuffer, lOutBuffers[0].cbBuffer);

                        // Erase sent data before releasing the buffer
                        Buffer_Clear(lOutBuffers + 0);

                        FreeContextBuffer(lOutBuffers[0].pvBuffer);
                    }
                }
                else
                {
                    KMS_EXCEPTION_ASSERT(SEC_E_INCOMPLETE_MESSAGE != lRet, RESULT_SOCKET_SECURITY_ERROR, "InitializeSecurityContext failed (NOT TESTED)", "");
                }

                KMS_EXCEPTION_ASSERT(mReceived_byte < sizeof(mIncoming), RESULT_SOCKET_SECURITY_ERROR, "Received too many data (NOT TESTED)", mReceived_byte);

                auto lReceived_byte = Socket_Client::Receive(mIncoming + mReceived_byte, sizeof(mIncoming) - mReceived_byte);
                KMS_EXCEPTION_ASSERT(0 != lReceived_byte, RESULT_SOCKET_SECURITY_ERROR, "Server disconnect (NOT TESTED)", "");

                mReceived_byte += lReceived_byte;
            }

            QueryContextAttributes(&mContext, SECPKG_ATTR_STREAM_SIZES, &mStreamSizes);
        }

        // ===== Socket =====================================================

        void Socket_Client_TLS::CloseSocket()
        {
            DWORD lFlags = ISC_REQ_ALLOCATE_MEMORY | ISC_REQ_CONFIDENTIALITY | ISC_REQ_REPLAY_DETECT | ISC_REQ_SEQUENCE_DETECT | ISC_REQ_STREAM;

            SecBuffer lOutBuffers[1];
            SecBufferDesc lOutDesc = { SECBUFFER_VERSION, ARRAYSIZE(lOutBuffers), lOutBuffers };

            Buffers_Clear(&lOutDesc);

            lOutBuffers[0].BufferType = SECBUFFER_TOKEN;

            auto lRet = InitializeSecurityContext(&mHandle, &mContext, nullptr, lFlags, 0, 0, &lOutDesc, 0, nullptr, &lOutDesc, &lFlags, nullptr);
            SEC_Dump("InitializeSecurityContext", lRet);

            if (SEC_E_OK == lRet)
            {
                Socket_Client::Send(lOutBuffers[0].pvBuffer, lOutBuffers[0].cbBuffer);

                // Erase sent data before releasing the buffer
                Buffer_Clear(lOutBuffers + 0);

                FreeContextBuffer(lOutBuffers[0].pvBuffer);
            }

            Shutdown();

            DeleteSecurityContext(&mContext);
            FreeCredentialsHandle(&mHandle);

            ClearInternal();

            Socket_Client::CloseSocket();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Socket_Client_TLS::ClearInternal()
        {
            memset(&mContext    , 0, sizeof(mContext));
            memset(&mHandle     , 0, sizeof(mHandle));
            memset(&mIncoming   , 0, sizeof(mIncoming));
            memset(&mStreamSizes, 0, sizeof(mStreamSizes));

            mDecrypted      = nullptr;
            mDecrypted_byte = 0;
            mReceived_byte  = 0;
            mUsed_byte      = 0;
        }

        void Socket_Client_TLS::DiscardConsumedData()
        {
            assert(nullptr != mDecrypted);
            assert(0 < mReceived_byte);
            assert(0 < mUsed_byte);
            assert(mReceived_byte >= mUsed_byte);

            memmove(mIncoming, mIncoming + mUsed_byte, mReceived_byte - mUsed_byte);

            mReceived_byte -= mUsed_byte;

            // Erase the data we just moved
            memset(mIncoming + mReceived_byte, 0, mUsed_byte);

            mUsed_byte = 0;
            mDecrypted = nullptr;
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Buffer_Clear(SecBuffer* aBuffer)
{
    assert(nullptr != aBuffer);
    assert(0 < aBuffer->cbBuffer);
    assert(nullptr != aBuffer->pvBuffer);

    memset(aBuffer->pvBuffer, 0, aBuffer->cbBuffer);
}

void Buffers_Clear(SecBufferDesc* aDesc)
{
    assert(nullptr != aDesc);
    assert(0 < aDesc->cBuffers);
    assert(nullptr != aDesc->pBuffers);

    memset(aDesc->pBuffers, 0, aDesc->cBuffers * sizeof(SecBuffer));
}

unsigned int Buffers_GetTotalSize(const SecBufferDesc& aDesc)
{
    assert(0 < aDesc.cBuffers);
    assert(nullptr != aDesc.pBuffers);

    unsigned int lResult_byte = 0;

    for (unsigned int i = 0; i < aDesc.cBuffers; i++)
    {
        lResult_byte += aDesc.pBuffers[i].cbBuffer;
    }

    return lResult_byte;
}

void Buffers_SetPointers(SecBufferDesc* aDesc, void* aPtr)
{
    assert(nullptr != aDesc);
    assert(0 < aDesc->cBuffers);
    assert(nullptr != aDesc->pBuffers);
    assert(nullptr != aPtr);

    uint8_t* lPtr = reinterpret_cast<uint8_t*>(aPtr);

    for (unsigned int i = 0; i < aDesc->cBuffers; i++)
    {
        auto lBuffer = aDesc->pBuffers + i;

        assert(0 < lBuffer->cbBuffer);

        lBuffer->pvBuffer = lPtr;

        lPtr += lBuffer->cbBuffer;
    }
}

#ifdef _CFG_TRACE_

    static const char* BUFFER_TYPE_NAMES[] =
    {
        "EMPTY",
        "DATA",
        "TOKEN",
        "PKG_PARAMS",
        "MISSING",
        "EXTRA",
        "STREAM_TRAILER",
        "STREAM_HEADER",
        "NEGOTIATION_INFO",
        "PADDING",
        "STREAM",
        "MECHLIST",
        "MECHLIST_SIGNATURE",
        "TARGET",
        "CHANNEL_BINDINGS",
        "CHANGE_PASS_RESPONSE",
        "TARGET_HOST",
        "ALERT",
        "APPLICATION_PROTOCOLS",
        "SRTP_PROTECTION_PROFILES",
        "SRTP_MASTER_KEY_IDENTIFIER",
        "TOKEN_BINDING",
        "PRESHARED_KEY",
        "PRESHARED_KEY_IDENTITY",
        "DTLS_MTU",
        "SEND_GENERIC_TLS_EXTENSION",
        "SUBSCRIBE_GENERIC_TLS_EXTENSION",
        "FLAGS",
        "TRAFFIC_SECRETS",
    };

    void Buffers_Dump(const SecBufferDesc& aDesc)
    {
        for (unsigned int i = 0; i < aDesc.cBuffers; i++)
        {
            auto lBuffer = aDesc.pBuffers[i];

            printf("Buffer %u : %s - %u bytes\n", i, BUFFER_TYPE_NAMES[lBuffer.BufferType], lBuffer.cbBuffer);
        }
    }

    void SEC_Dump(const char* aFunction, SECURITY_STATUS aSec)
    {
        assert(nullptr != aFunction);

        const char* lName;

        switch (aSec)
        {
        case SEC_E_OK: lName = "E_OK"; break;

        case SEC_E_DECRYPT_FAILURE   : lName = "E_DECRYPT_FAILURE"   ; break;
        case SEC_E_INCOMPLETE_MESSAGE: lName = "E_INCOMPLETE_MESSAGE"; break;

        case SEC_I_CONTINUE_NEEDED: lName = "I_CONTINUE_NEEDED"; break;

        default: printf("%s returned SEC_ 0x%08x\n", aFunction, aSec); return;
        }

        printf("%s returned SEC_%s\n", aFunction, lName);
    }

#endif

// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/Dev_Device2.cpp

// TEST COVERAGE 2026-05-17 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dev/Device2.h>

namespace KMS
{
    namespace Dev
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool Device2_Config::IsLinkValid() const { return 0 < strlen(mLink); }

        void Device2_Config::SetLink(const char* aLink) { assert(nullptr != aLink); strcpy_s(mLink, aLink); }

        void Device2_Config::SetDefault() { memset(this, 0, sizeof(*this)); }

        void Device2_Config::Validate() const
        {
            char lMsg[LINE_LENGTH];

            memset(&lMsg, 0, sizeof(lMsg));

            auto lRet = IsValid(lMsg, sizeof(lMsg));

            KMS_EXCEPTION_ASSERT(lRet, RESULT_INVALID_CONFIG, "Invalid configuration", lMsg);
        }

        Device2::Device2()
            : mConnectFlags(IDevice::FLAG_ACCESS_READ | IDevice::FLAG_ACCESS_WRITE)
            , mHandle(INVALID_HANDLE_VALUE)
        {}

        Device2::~Device2()
        {
            if (IsConnected())
            {
                Disconnect();
            }
        }

        // ===== IDevice ====================================================

        void Device2::SetConnectFlags(unsigned int aFlags) { mConnectFlags = aFlags; }

        void Device2::ClearReadBuffer() { assert(false); }

        // ===== Stream::IStream ============================================

        bool Device2::IsConnected() const { return INVALID_HANDLE_VALUE != mHandle; }

    }
}

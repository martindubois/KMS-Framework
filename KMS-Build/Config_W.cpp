
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Config_W.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Config.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

void Config::Init_OSDep(const char* aCertificatSHA1, uint32_t aVisualStudioVersion)
{
    assert(nullptr != aCertificatSHA1);

    mCertificatSHA1      = aCertificatSHA1;
    mVisualStudioVersion = aVisualStudioVersion;
}

const char* Config::GetCertificatSHA1() const { return mCertificatSHA1.c_str(); }

uint32_t Config::GetVisualStudioVersion() const { return mVisualStudioVersion; }

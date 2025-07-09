
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Config_L.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Config.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

void Config::Init_OSDep(const KMS::DI::Array* aPackages)
{
    assert(nullptr != aPackages);

    mPackages = aPackages;
}

const DI::Array* Config::GetPackages() const { return mPackages; }

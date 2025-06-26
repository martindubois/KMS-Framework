
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/KMS-Build.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Banner.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

#include "Build.h"

using namespace KMS;

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-Build");

    return Build::Main(aCount, aVector);
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/KMS-B-Test.cpp

#include "Component.h"

// ====== Includes ==========================================================
#include <KMS/Banner.h>
#include <KMS/Test/TestManager.h>

// ===== KMS-A-Test =========================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-B-Test");

    return KMS::Test::TestManager::Main(aCount, aVector);
}

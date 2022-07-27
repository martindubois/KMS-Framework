
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C-Test/KMS-C-Test.cpp

#include "Component.h"

// ====== Includes ==========================================================
#include <KMS/Banner.h>
#include <KMS/Test/TestManager.h>

// ===== KMS-C-Test =========================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-C-Test");

    return KMS::Test::TestManager::Main(aCount, aVector);
}

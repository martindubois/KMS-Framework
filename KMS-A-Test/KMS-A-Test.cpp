
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/KMS-A-Test.cpp

#include "Component.h"

// ====== Includes ==========================================================
#include <KMS/Banner.h>
#include <KMS/Test/TestManager.h>

using namespace KMS;

// ===== KMS-A-Test =========================================================
#include "../Common/Version.h"

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "KMS-A-Test");

    return Test::TestManager::Main(aCount, aVector);
}

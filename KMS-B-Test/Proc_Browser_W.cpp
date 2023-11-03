
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/Proc_Browser_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Browser.h>

using namespace KMS;

// Tests
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(Proc_Browser_Window, "Auto", sTest_Windows)
{
    Proc::Browser lB;

    lB.SetPrefered(Proc::Browser::Type::EDGE);
    lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");
}

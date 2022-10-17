
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/Proc_Browser.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Browser.h>

// Static variable
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(Proc_Browser_Base, "Proc_Browser_Base", "Auto", sTest_Base)
{
    KMS::Proc::Browser lB;

    KMS_TEST_ASSERT(!lB.IsOpen());

    lB.SetAppMode  (false);
    lB.SetKioskMode(false);
    lB.SetPrefered(KMS::Proc::Browser::Type::NONE);

    lB.Open(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");
    lB.Close();

    lB.SetPrefered(KMS::Proc::Browser::Type::DEFAULT);

    lB.Detach();

    lB.Open(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");

    KMS_TEST_ASSERT(lB.IsOpen());
}

KMS_TEST(Proc_Browser_Chrome, "Proc_Browser_Chrome", "Chrome", sTest_Chrome)
{
    KMS::Proc::Browser lB;

    lB.SetPrefered(KMS::Proc::Browser::Type::CHROME);

    lB.Open(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");

    KMS_TEST_ASSERT(lB.IsOpen());

    lB.Close();

    lB.SetAppMode(true);
    lB.Open(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");
    lB.Close();

    lB.Open(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", NULL);
    lB.Close();
}

KMS_TEST(Proc_Browser_Config, "Proc_Browser_Config", "Auto", sTest_Config)
{
    KMS::Proc::Browser lB;

    lB.mPrefered.Set("CHROME");
    lB.mPrefered.Set("DEFAULT");
    lB.mPrefered.Set("EDGE");
    lB.mPrefered.Set("NONE");
}

KMS_TEST(Proc_Browser_Fail, "Proc_Browser_Fail", "Auto", sTest_Fail)
{
    KMS::Proc::Browser lB;

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lB.mPrefered.Set("INVALID");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEST);
}

KMS_TEST(Proc_Browser_Interactive, "Proc_Browser_Interactive", "Interactive", sTest_Interactive)
{
    KMS::Proc::Browser lB;

    lB.SetPrefered(KMS::Proc::Browser::Type::DEFAULT);

    lB.Open(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test1.html", "KMS-B-Test/Tests/Test1.html");
    lB.Detach();

    lB.Start(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test1.html");
}

#ifdef _KMS_WINDOWS_

    KMS_TEST(Proc_Browser_Window, "Proc_Browser_Windows", "Auto", sTest_Windows)
    {
        KMS::Proc::Browser lB;

        lB.SetPrefered(KMS::Proc::Browser::Type::EDGE);
        lB.Open(KMS::File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");
    }

#endif

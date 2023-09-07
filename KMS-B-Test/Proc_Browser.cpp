
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/Proc_Browser.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Proc/Browser.h>

using namespace KMS;

// Static variable
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(Proc_Browser_Base, "Auto", sTest_Base)
{
    Proc::Browser lB;

    KMS_TEST_ASSERT(!lB.IsOpen());

    lB.SetAppMode  (false);
    lB.SetKioskMode(false);
    lB.SetPrefered(Proc::Browser::Type::NONE);

    lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");
    lB.Close();

    lB.SetPrefered(Proc::Browser::Type::DEFAULT);

    lB.Detach();

    lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");

    KMS_TEST_ASSERT(lB.IsOpen());
}

KMS_TEST(Proc_Browser_Chrome, "Chrome", sTest_Chrome)
{
    Proc::Browser lB;

    lB.SetPrefered(Proc::Browser::Type::CHROME);

    lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");

    KMS_TEST_ASSERT(lB.IsOpen());

    lB.Close();

    lB.SetAppMode(true);
    lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");
    lB.Close();

    lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", nullptr);
    lB.Close();
}

KMS_TEST(Proc_Browser_Config, "Auto", sTest_Config)
{
    Proc::Browser lB;

    lB.mPrefered.Set("CHROME");
    lB.mPrefered.Set("DEFAULT");
    lB.mPrefered.Set("EDGE");
    lB.mPrefered.Set("NONE");
}

KMS_TEST(Proc_Browser_Fail, "Auto", sTest_Fail)
{
    Proc::Browser lB;

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lB.mPrefered.Set("INVALID");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);
}

KMS_TEST(Proc_Browser_Interactive, "Interactive", sTest_Interactive)
{
    Proc::Browser lB;

    lB.SetPrefered(Proc::Browser::Type::DEFAULT);

    lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test1.html", "KMS-B-Test/Tests/Test1.html");
    lB.Detach();

    lB.Start(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test1.html");
}

#ifdef _KMS_WINDOWS_

    KMS_TEST(Proc_Browser_Window, "Auto", sTest_Windows)
    {
        Proc::Browser lB;

        lB.SetPrefered(Proc::Browser::Type::EDGE);
        lB.Open(File::Folder::CURRENT, "KMS-B-Test" SLASH "Tests" SLASH "Test0.html", "KMS-B-Test/Tests/Test0.html");
    }

#endif

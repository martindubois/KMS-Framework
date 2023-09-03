
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Version.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Version.h>

using namespace KMS;

KMS_TEST(Version_Base, "Version_Base", "Auto", sTest_Base)
{
    Version lV0("0.0");
    Version lV1("0.0-Test");
    Version lV2(File::Folder::CURRENT, "Common" SLASH "Version.h");

    char lPackage[FILE_LENGTH];

    lV0.GetPackageName("Product", lPackage, sizeof(lPackage));

    lV1.GetPackageName("Product", lPackage, sizeof(lPackage));

    char lString[NAME_LENGTH];

    lV0.GetString(lString, sizeof(lString), 1);

    KMS_TEST_COMPARE(strcmp("0", lString), 0);

    lV0.GetString(lString, sizeof(lString), 2);

    KMS_TEST_COMPARE(strcmp("0.0", lString), 0);

    lV0.GetString(lString, sizeof(lString), 3);

    KMS_TEST_COMPARE(strcmp("0.0.0", lString), 0);

    lV0.GetString(lString, sizeof(lString), 4);

    KMS_TEST_COMPARE(strcmp("0.0.0.0", lString), 0);
}

KMS_TEST(Version_Fail, "Version_Fail", "Auto", sTest_Fail)
{
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        Version lV0("0");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        Version lV0("0.0.0.256");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_INVALID_NUMBER");
}

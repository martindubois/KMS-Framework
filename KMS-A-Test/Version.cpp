
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Version.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Version.h>

KMS_TEST(Version_Base, "Version_Base", "Auto", sTest_Base)
{
    KMS::Version lV0("0.0");
    KMS::Version lV1("0.0-Test");
    KMS::Version lV2(KMS::File::Folder::CURRENT, "Common" SLASH "Version.h");

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
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Version lV0("0");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(VERSION_FORMAT_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Version lV0("0.0.0.256");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(VERSION_NUMBER_INVALID);
}

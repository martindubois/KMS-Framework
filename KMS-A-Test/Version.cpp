
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Version.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Version.h>

using namespace KMS;

KMS_TEST(Version_Base, "Auto", sTest_Base)
{
    // Constructor
    Version lV0("0.0");
    Version lV1("0.0-Test");
    Version lV2(File::Folder::CURRENT, "Common" SLASH "Version.h");

    // GetDate
    KMS_TEST_ASSERT(0 == strcmp("", lV0.GetDate()));

    // GetType
    KMS_TEST_ASSERT(0 == strcmp(""    , lV0.GetType()));
    KMS_TEST_ASSERT(0 == strcmp("Test", lV1.GetType()));

    // GetDebug
    KMS_TEST_ASSERT(!lV0.GetDebug());

    // GetMajor
    KMS_TEST_COMPARE(lV0.GetMajor(), 0U);

    // GetMinor
    KMS_TEST_COMPARE(lV0.GetMinor(), 0U);

    // GetBuild
    KMS_TEST_COMPARE(lV0.GetBuild(), 0U);

    // GetCompat
    KMS_TEST_COMPARE(lV0.GetCompat(), 0U);

    // GetString

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

KMS_TEST(Version_Fail, "Auto", sTest_Fail)
{
    // Constructor

    try
    {
        Version lV0("0");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        Version lV0("0.0.0.256");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_INVALID_NUMBER");
}

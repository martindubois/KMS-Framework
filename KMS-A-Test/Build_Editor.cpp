
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Build_Editor.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Build/Editor.h>
#include <KMS/Build/Editor_DI2.h>

using namespace KMS;

KMS_TEST(Build_Editor_Base, "Auto", sTest_Base)
{
    static const char* VECTOR_A[] =
    {
        "VersionFile=Common/Version.h",
        "Operations+=\"DoesNotExist.txt;^Version=.*$;Version={M.m.BT}\""
    };

    static const char* VECTOR_B[] =
    {
        "VersionFile=Common/Version.h",
        "Operations+=\"_DocUser/Documentation.html;^\\s*<h1>KMS-Framework - .*</h1>$;        <h1>KMS-Framework - {M.m.BT}</h1>\""
    };

    char lMsg[LINE_LENGTH];

    Build::Editor        lE0;
    Build::Editor_Config lEC0;

    // ===== IsValid ========================================================

    // No version information
    KMS_TEST_ASSERT(!lEC0.IsValid(lMsg, sizeof(lMsg)));

    // The version file does not exists
    strcpy_s(lEC0.mVersionFile, "DoesNotExist.h");

    KMS_TEST_ASSERT(!lEC0.IsValid(lMsg, sizeof(lMsg)));

    // 2 conflicting version information
    lEC0.mVersion = Version("1.0.0.0-test");

    KMS_TEST_ASSERT(!lEC0.IsValid(lMsg, sizeof(lMsg)));

    // Direct version information
    memset(&lEC0.mVersionFile, 0, sizeof(&lEC0.mVersionFile));

    KMS_TEST_ASSERT(lEC0.IsValid());

    // Invalid operation format
    lEC0.mOperations.push_back("Invalid operation");

    KMS_TEST_ASSERT(!lEC0.IsValid(lMsg, sizeof(lMsg)));

    // ===== Parsing arguments ==============================================

    // Invalid operation file name
    lEC0.mVersion = Version();

    DI2::Decode_ASCII_Arguments(&lEC0, &Build::Editor_Config_TYPE, 2, VECTOR_A);

    KMS_TEST_ASSERT(!lEC0.IsValid(lMsg, sizeof(lMsg)));

    // Valid operations
    lEC0.mOperations.clear();

    DI2::Decode_ASCII_Arguments(&lEC0, &Build::Editor_Config_TYPE, 2, VECTOR_B);

    KMS_TEST_ASSERT(lEC0.IsValid());

    // No operation
    lEC0.mOperations.clear();

    DI2::Decode_ASCII_Arguments(&lEC0, &Build::Editor_Config_TYPE, 1, VECTOR_A);

    KMS_TEST_ASSERT(lEC0.IsValid());

    // ===== Validate =======================================================

    // No operations
    lEC0.Validate();

    // ===== Config_Set =====================================================
    lE0.Config_Set(lEC0);

    // ===== Execute ========================================================

    // Version file and no operations
    lE0.Execute();

    // Direct version information and no operation
    lEC0.SetDefault();

    lEC0.mVersion = Version("1.0.0.0-test");

    lE0.Config_Set(lEC0);

    lE0.Execute();
}

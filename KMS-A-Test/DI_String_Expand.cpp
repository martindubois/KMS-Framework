
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_String_Expand.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>

using namespace KMS;

KMS_TEST(DI_String_Expand_Base, "DI_String_Expand_Base", "Auto", sTest_Base)
{
    // Constructor
    DI::String        lS0;
    DI::String_Expand lSE0;

    // ===== DI::String =====================================================

    // operator =
    lS0 = "";

    // operator ==
    KMS_TEST_ASSERT(lS0 == "");

    // GetLength
    KMS_TEST_COMPARE(lS0.GetLength(), 0U);

    // ===== DI::Value ======================================================

    // Set
    lSE0.Set("{");
    lSE0.Set("{}");
    lSE0.Set("{$Path");
    lSE0.Set("{$Path}");
}

KMS_TEST(DI_String_Expand_Exception, "DI_String_Expand_Exception", "Auto", sText_Exception)
{
    char lBuffer[8];

    DI::String        lS0("Test0");
    DI::String_Expand lSE0;

    // ===== DI::Value ======================================================

    // Get
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lS0.Get(lBuffer, 5);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(DI_OUTPUT_TOO_SHORT);

    // Set
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lSE0.Set("{$DoesNotExist}");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(ENV_EXPAND_FAILED);
}

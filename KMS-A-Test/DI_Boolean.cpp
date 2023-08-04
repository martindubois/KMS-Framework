
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Boolean.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Boolean.h>

using namespace KMS;

KMS_TEST(DI_Boolean_Base, "DI_Boolean_Base", "Auto", sTest_Base)
{
    // Create

    // Constructor
    DI::Boolean lB0;

    // opearator =
    lB0 = true;

    // operator bool

    // Get

    // Set

    // ===== DI::Value ======================================================

    // Clear

    // Get

    // Set
    lB0.Set("true");
    lB0.Set("false");
}

KMS_TEST(DI_Boolean_Exception, "DI_Boolean_Exception", "Auto", sTest_Exception)
{
    DI::Boolean lB0;

    // ===== DI::Value ======================================================

    // Set
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lB0.Set("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONVERT_FORMAT_INVALID);
}

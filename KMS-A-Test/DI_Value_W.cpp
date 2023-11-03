
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Value_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/DI/GUID.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const GUID TEST_GUID = { 0x8d057212, 0xac5c, 0x41b0, { 0xb6, 0xaa, 0xc9, 0xf6, 0xe7, 0x65, 0xcb, 0xfb } };

KMS_TEST(DI_Value_Windows, "Auto", sTest_Windows)
{
    // Create
    DI::Object* lObj;
    lObj = DI::GUID::Create(); delete lObj;

    // Constructor
    GUID         lG0I = TEST_GUID;
    DI::GUID_Ptr lG0(&lG0I);
    DI::GUID     lG1;

    // Get
    char lStr[128];
    lG0.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "{8d057212-ac5c-41b0-b6aa-c9f6e765cbfb}"));
    lG1.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "{00000000-0000-0000-0000-000000000000}"));

    // Set
    lG0.Set("{01234567-89ab-cdef-0000-000000000002}");
    lG1.Set("{01234567-89AB-CDEF-0000-000000000003}");

    // operator =
    lG0 = TEST_GUID;
    lG1 = TEST_GUID;

    // operator ==
    KMS_TEST_ASSERT(lG0 == TEST_GUID);
    KMS_TEST_ASSERT(lG1 == TEST_GUID);

    // ===== Object =========================================================

    // Clear
    KMS_TEST_ASSERT(lG0.Clear());
    KMS_TEST_ASSERT(lG1.Clear());

    KMS_TEST_ASSERT(!lG0.Clear());
    KMS_TEST_ASSERT(!lG1.Clear());
}

KMS_TEST(DI_Value_Windows_Exception, "Auto", sTest_Windows_Exception)
{
    DI::GUID lG0;

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lG0.Set("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/String.h>

KMS_TEST(DI_Array_Base, "DI_Array_Base", "Auto", sTest_Base)
{
    char lBuffer[1024];

    KMS::DI::Array lA0;
    KMS::DI::Array lA1;
    KMS::DI::Array lA2;

    KMS::DI::String lS0;

    // Container:: +=
    lA0.AddEntry(&lS0, false);

    // String::Get
    KMS_TEST_COMPARE(lS0.Get(lBuffer, sizeof(lBuffer)), 0UL);

    // String::Set
    lS0.Set("Test");
}

KMS_TEST(DI_Array_Exception, "DI_Array_Exception", "Auto", sTest_Exception)
{
    char lBuffer[1024];

    KMS::DI::String lS0("");

    // String::Get
    for (unsigned int lSize_byte = 0; lSize_byte < 2; lSize_byte++)
    {
        try
        {
            KMS_TEST_EXPECTED_EXCEPTION();
            lS0.Get(lBuffer, lSize_byte);
            KMS_TEST_ASSERT(false);
        }
        KMS_TEST_CATCH(DI_OUTPUT_TOO_SHORT);
    }
}

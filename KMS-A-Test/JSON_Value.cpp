
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/JSON_Value.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/Value.h>

KMS_TEST(JSON_Value_Base, "JSON_Value_Base", "Auto", sTest_Base)
{
    KMS::JSON::Value lV0(20.0);
    KMS::JSON::Value lV1(20 , KMS::Radix::DECIMAL);
    KMS::JSON::Value lV2(20 , KMS::Radix::HEXADECIMAL);
    KMS::JSON::Value lV3(20U, KMS::Radix::HEXADECIMAL);

    KMS::JSON::Value lV4;

    KMS_TEST_ASSERT(lV4.IsEmpty());
    lV4.Set(0.0);
    KMS_TEST_ASSERT(!lV4.IsEmpty());
    lV4.Clear();
    KMS_TEST_ASSERT(lV4.IsEmpty());

    KMS::JSON::Value lV5;

    KMS_TEST_COMPARE(lV5.HTTP_Set(" Test", 5), 5U);

    KMS::JSON::Value lV6;

    KMS_TEST_COMPARE(lV6.JSON_Set("Test", 4), 4U);
}

KMS_TEST(JSON_Value_Exception, "JSON_Value_Exception", "Auto", sTest_Exception)
{
    KMS::JSON::Value lV0("Test");

    char lStr[32];

    for (unsigned int i = 0; i < 6; i++)
    {
        try
        {
            KMS_TEST_EXPECTED_EXCEPTION();
            lV0.HTTP_Get(lStr, i);
            KMS_TEST_ASSERT(false);
        }
        KMS_TEST_CATCH(OUTPUT_TOO_SHORT);
    }

    KMS_TEST_COMPARE(lV0.HTTP_Get(lStr, 7), 4U);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lV0.HTTP_Set("\"", 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(HTTP_FORMAT);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lV0.HTTP_Set("", 0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(HTTP_FORMAT);
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/JSON_Dictionary.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/Dictionary.h>
#include <KMS/JSON/Value.h>

KMS_TEST(JSON_Dictionary_Base, "JSON_Dictionary_Base", "Auto", sTest_Base)
{
    KMS::JSON::Dictionary lD0;
    KMS::JSON::Dictionary lD1(lD0);

    KMS_TEST_ASSERT(NULL == lD0.GetEntry("DoesNotExist"));

    lD0.SetEntry("Test1", new KMS::JSON::Value("Test0"));
    lD0.SetEntry("Test1", new KMS::JSON::Value("Test1"));
    lD0.SetEntry("Test2", new KMS::JSON::Value("Test2"));

    char lStr[32];

    KMS_TEST_COMPARE(lD0.JSON_Get(lStr, sizeof(lStr)), 29L);

    KMS_TEST_ASSERT(0 == strcmp("{\"Test1\":Test1,\"Test2\":Test2}", lStr));

    KMS_TEST_COMPARE(lD1.HTTP_Set("", 0), 0L);

    std::cout << lD0;
}

KMS_TEST(JSON_Dictionary_Exception, "JSON_Dictionary_Exception", "Auto", sTest_Exception)
{
    KMS::JSON::Dictionary lD0;

    lD0.SetEntry("Test0", new KMS::JSON::Value("Test0"));

    char lStr[32];

    for (unsigned int i = 0; i < 15; i++)
    {
        try
        {
            KMS_TEST_EXPECTED_EXCEPTION();
            lD0.HTTP_Get(lStr, i);
            KMS_TEST_ASSERT(false);
        }
        KMS_TEST_CATCH(OUTPUT_TOO_SHORT);
    }

    KMS_TEST_COMPARE(lD0.HTTP_Get(lStr, 16), 14U);

    for (unsigned int i = 0; i < 17; i++)
    {
        try
        {
            KMS_TEST_EXPECTED_EXCEPTION();
            lD0.JSON_Get(lStr, i);
            KMS_TEST_ASSERT(false);
        }
        KMS_TEST_CATCH(OUTPUT_TOO_SHORT);
    }

    KMS_TEST_COMPARE(lD0.JSON_Get(lStr, 18), 15U);
}

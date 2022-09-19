
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/JSON.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/JSON.h>

KMS_TEST(JSON_Base, "JSON_Base", "Auto", sTest_Base)
{
    char             lBuffer[1024];
    const char     * lIn;
    KMS::DI::Object* lO0;

    // Array - Empty
    lIn = "[]";
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, lIn, 2), 2UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 2UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    delete lO0;

    // Array
    lIn = "[\"A\"]";
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, lIn, 5), 5UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 5UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    lIn = "[\"B\"]";
    KMS_TEST_COMPARE(KMS::JSON::Decode(lO0, lIn, 5), 5UL);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 5UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    delete lO0;

    lIn = "[\"C\",\"D\"]";
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, lIn, 9), 9UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 9UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    delete lO0;

    // Dictionary - Empty
    lIn = "{}";
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, lIn, 2), 2UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 2UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    delete lO0;

    // Dictionary
    lIn = "{\"E\":\"F\"}";
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, lIn, 9), 9UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 9UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    lIn = "{\"E\":\"G\"}";
    KMS_TEST_COMPARE(KMS::JSON::Decode(lO0, lIn, 9), 9UL);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 9UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    delete lO0;

    lIn = "{\"H\":\"I\",\"J\":1}";
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, lIn, 15), 15UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 15UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    lIn = "{\"H\":\"K\",\"J\":2}";
    KMS_TEST_COMPARE(KMS::JSON::Decode(lO0, lIn, 15), 15UL);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 15UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    delete lO0;

    // String - Empty
    lIn = "\"\"";
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, lIn, 2), 2UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    KMS_TEST_COMPARE(KMS::JSON::Encode(lO0, lBuffer, sizeof(lBuffer)), 2UL);
    KMS_TEST_ASSERT(0 == strcmp(lIn, lBuffer));
    delete lO0;

    // String - Empty with blank before
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, " \"\"", 3), 3UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    delete lO0;

    // String
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, "\"L\"", 3), 3UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    delete lO0;

    // UIUnt32
    KMS_TEST_COMPARE(KMS::JSON::Decode(&lO0, "1", 1), 1UL);
    KMS_TEST_ASSERT(NULL != &lO0);
    delete lO0;
}

KMS_TEST(JSON_Exception, "JSON_Exception", "Auto", sTest_Exception)
{
    KMS::DI::Object* lO0;

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::JSON::Decode(&lO0, "", 0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(JSON_FORMAT);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::JSON::Decode(&lO0, "\"", 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_TOO_SHORT);
}

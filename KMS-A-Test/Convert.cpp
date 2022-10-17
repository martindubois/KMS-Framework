
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Convert.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

KMS_TEST(Convert_Base, "Convert_Base", "Auto", sTest_Base)
{
    KMS_TEST_ASSERT(!KMS::Convert::ToBool("FALSE"));
    KMS_TEST_ASSERT(!KMS::Convert::ToBool("False"));
    KMS_TEST_ASSERT(!KMS::Convert::ToBool("false"));
    KMS_TEST_ASSERT(!KMS::Convert::ToBool("0"));

    KMS_TEST_ASSERT(KMS::Convert::ToBool("TRUE"));
    KMS_TEST_ASSERT(KMS::Convert::ToBool("True"));
    KMS_TEST_ASSERT(KMS::Convert::ToBool("true"));
    KMS_TEST_ASSERT(KMS::Convert::ToBool("1"));

    KMS_TEST_ASSERT(0.0 == KMS::Convert::ToDouble("0.0"));

    KMS_TEST_COMPARE(0     , KMS::Convert::ToUInt16("0"));
    KMS_TEST_COMPARE(0xffff, KMS::Convert::ToUInt16("0xffff"));

    KMS_TEST_ASSERT(0          == KMS::Convert::ToUInt32("0x0"));
    KMS_TEST_ASSERT(0xffffffff == KMS::Convert::ToUInt32("0xffffffff"));

    KMS_TEST_COMPARE(0   , KMS::Convert::ToUInt8("0"));
    KMS_TEST_COMPARE(0xff, KMS::Convert::ToUInt8("0xff"));
}

KMS_TEST(Convert_Fail, "Convert_Fail", "Auto", sTest_Fail)
{
    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Convert::ToBool("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONVERT_FORMAT_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Convert::ToDouble("0.0 ");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONVERT_FORMAT_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Convert::ToFile("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONVERT_OPEN_FAILED);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Convert::ToUInt16("0x10000");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONVERT_DATA_TYPE_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Convert::ToUInt32("0 ");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONVERT_DATA_TYPE_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        KMS::Convert::ToUInt8("0x100");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CONVERT_DATA_TYPE_INVALID);
}

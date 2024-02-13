
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Convert.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

using namespace KMS;

KMS_TEST(Convert_Base, "Auto", sTest_Base)
{
    uint8_t lArray[16];
    char    lASCII[LINE_LENGTH];
    wchar_t lUTF16[LINE_LENGTH];

    // ToBool
    KMS_TEST_ASSERT(!Convert::ToBool("FALSE"));
    KMS_TEST_ASSERT(!Convert::ToBool("False"));
    KMS_TEST_ASSERT(!Convert::ToBool("false"));
    KMS_TEST_ASSERT(!Convert::ToBool("0"));

    KMS_TEST_ASSERT(Convert::ToBool("TRUE"));
    KMS_TEST_ASSERT(Convert::ToBool("True"));
    KMS_TEST_ASSERT(Convert::ToBool("true"));
    KMS_TEST_ASSERT(Convert::ToBool("1"));

    // ToDouble
    KMS_TEST_ASSERT(0.0 == Convert::ToDouble("0.0"));

    // ToFile
    auto lFile = Convert::ToFile("KMS-A-Test/Tests/Test1.cfg");
    KMS_TEST_ASSERT(nullptr != lFile);
    int lRet = fclose(lFile);
    KMS_TEST_COMPARE(lRet, 0);

    // ToInt16
    KMS_TEST_COMPARE(-1, Convert::ToInt16("-1"));

    // ToInt32
    KMS_TEST_COMPARE(-1, Convert::ToInt32("-1"));
    KMS_TEST_COMPARE( 1, Convert::ToInt32("0x1"));

    // ToInt8
    KMS_TEST_COMPARE(-1, Convert::ToInt8("-1"));

    // ToUInt16
    KMS_TEST_COMPARE(0     , Convert::ToUInt16("0"));
    KMS_TEST_COMPARE(0xffff, Convert::ToUInt16("0xffff"));

    // ToUInt32
    KMS_TEST_ASSERT(0          == Convert::ToUInt32("0x0"));
    KMS_TEST_ASSERT(0xffffffff == Convert::ToUInt32("0xffffffff"));

    // ToUInt8
    KMS_TEST_COMPARE(0   , Convert::ToUInt8("0"));
    KMS_TEST_COMPARE(0xff, Convert::ToUInt8("0xff"));

    // ToASCII
    KMS_TEST_COMPARE(sizeof(char), Convert::ToASCII(L"a", lASCII, sizeof(lASCII)));

    // ToUTF16
    KMS_TEST_COMPARE(sizeof(wchar_t), Convert::ToUTF16("a", lUTF16, sizeof(lUTF16)));

    // ToDisplay
    KMS_TEST_COMPARE(sizeof(char), Convert::ToDisplay("a", sizeof(char), lASCII, sizeof(lASCII)));

    // ToUInt8Array
    KMS_TEST_COMPARE(1U, Convert::ToUInt8Array("01"    , "" , "" , lArray, sizeof(lArray)));
    KMS_TEST_COMPARE(lArray[0], 0x01);
    KMS_TEST_COMPARE(1U, Convert::ToUInt8Array("a"     , ":", "" , lArray, sizeof(lArray)));
    KMS_TEST_COMPARE(lArray[0], 0x0a);
    KMS_TEST_COMPARE(2U, Convert::ToUInt8Array("02 Aa" , " ", "" , lArray, sizeof(lArray)));
    KMS_TEST_COMPARE(lArray[0], 0x02);
    KMS_TEST_COMPARE(lArray[1], 0xaa);
    KMS_TEST_COMPARE(3U, Convert::ToUInt8Array(":03:Aa", ":", "" , lArray, sizeof(lArray)));
    KMS_TEST_COMPARE(lArray[0], 0x00);
    KMS_TEST_COMPARE(lArray[1], 0x03);
    KMS_TEST_COMPARE(lArray[2], 0xaa);
    KMS_TEST_COMPARE(1U, Convert::ToUInt8Array("4 "    , ":", " ", lArray, sizeof(lArray)));
    KMS_TEST_COMPARE(lArray[0], 0x04);
    KMS_TEST_COMPARE(1U, Convert::ToUInt8Array("5:"    , ":", " ", lArray, sizeof(lArray)));
    KMS_TEST_COMPARE(lArray[0], 0x05);
}

KMS_TEST(Convert_Fail, "Auto", sTest_Fail)
{
    uint8_t lArray[4];
    char    lASCII[4];
    wchar_t lUTF16[4];

    // ToBool
    try
    {
        Convert::ToBool("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    // ToDouble
    try
    {
        Convert::ToDouble("0.0 ");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    // ToFile
    try
    {
        Convert::ToFile("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_OPEN_FAILED);

    // ToInt16
    try
    {
        Convert::ToInt16("32769");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    // ToInt32
    try
    {
        Convert::ToInt32("-1 ");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_DATA_TYPE);

    // ToUInt16
    try
    {
        Convert::ToUInt16("0x10000");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    // ToUInt32
    try
    {
        Convert::ToUInt32("0 ");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_DATA_TYPE);

    // ToUInt8
    try
    {
        Convert::ToUInt8("0x100");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_DATA_TYPE);

    // ToASCII
    try
    {
        Convert::ToASCII(L"AB", lASCII, 2);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_OUTPUT_TOO_SHORT);

    // ToUTF16
    try
    {
        Convert::ToUTF16("AB", lUTF16, 2);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_OUTPUT_TOO_SHORT);

    // ToDisplay
    try
    {
        Convert::ToDisplay("AB", 2, lASCII, 2);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_OUTPUT_TOO_SHORT);

    // ToUInt8Array
    try
    {
        Convert::ToUInt8Array("G", "", "", lArray, 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    try
    {
        Convert::ToUInt8Array("FG", "", "", lArray, 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    try
    {
        Convert::ToUInt8Array("EFG", "", "", lArray, 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);
}

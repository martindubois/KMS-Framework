
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Value.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Enum.h>
#include <KMS/DI/File.h>
#include <KMS/DI/Float.h>
#include <KMS/DI/Folder.h>
#include <KMS/DI/Int.h>
#include <KMS/DI/String_Expand.h>
#include <KMS/DI/UInt.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define TEST_FILE_0 "KMS-A-Test/Tests/Test_DI_Value_Base_0.txt"
#define TEST_FILE_1 "KMS-A-Test/Tests/Test_DI_Value_Base_1.txt"

// Data type
// //////////////////////////////////////////////////////////////////////////

enum class TestEnum
{
    TEST_ENUM_0 = 0,
    TEST_ENUM_1,

    QTY
};

static const char* TEST_ENUM_NAMES[] = { "0", "1" };

KMS_TEST(DI_Value_Base, "Auto", sTest_Base)
{
    // Create
    DI::Object* lObj;
    lObj = DI::Boolean                        ::Create(); delete lObj;
    lObj = DI::Enum<TestEnum, TEST_ENUM_NAMES>::Create(); delete lObj;
    lObj = DI::Float<double>                  ::Create(); delete lObj;
    lObj = DI::File                           ::Create(); delete lObj;
    lObj = DI::Folder                         ::Create(); delete lObj;
    lObj = DI::Int<int32_t>                   ::Create(); delete lObj;
    lObj = DI::String                         ::Create(); delete lObj;
    lObj = DI::String_Expand                  ::Create(); delete lObj;
    lObj = DI::UInt<uint32_t>                 ::Create(); delete lObj;

    // Constructor
    bool                                    lB0I = false;
    DI::Boolean_Ptr                         lB0  (&lB0I);
    DI::Boolean                             lB1  (true);
    TestEnum                                lE0I = TestEnum::TEST_ENUM_0;
    DI::Enum_Ptr<TestEnum, TEST_ENUM_NAMES> lE0  (&lE0I);
    DI::Enum<TestEnum, TEST_ENUM_NAMES>     lE1  (TestEnum::TEST_ENUM_1);
    double                                  lF0I = 0.0;
    DI::Float_Ptr<double>                   lF0  (&lF0I);
    DI::Float<double>                       lF1  (1.0);
    FILE                                  * lFi0I = nullptr;
    DI::File_Ptr                            lFi0 (&lFi0I);
    DI::File                                lFi1 (stdin, "stdin");
    File::Folder                            lFo0I;
    DI::Folder_Ptr                          lFo0 (&lFo0I);
    DI::Folder                              lFo1;
    int32_t                                 lI0I = 0;
    DI::Int_Ptr<int32_t>                    lI0  (&lI0I);
    DI::Int<int32_t>                        lI1  (1);
    std::string                             lS0I ("Test 1");
    DI::String_Ptr                          lS0  (&lS0I);
    DI::String                              lS1  ("Test 2");
    std::string                             lSE0I("TestE 1");
    DI::String_Expand_Ptr                   lSE0 (&lSE0I);
    DI::String_Expand                       lSE1 ("TestE 2");
    uint32_t                                lUI0I(0);
    DI::UInt_Ptr<uint32_t>                  lUI0 (&lUI0I);
    DI::UInt<uint32_t>                      lUI1 (1);
    
    // Get
    char lStr[128];
    lB0 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "false"));
    lB1 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "true"));
    lE0 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "0"));
    lE1 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "1"));
    lF0 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "0.000000"));
    lF1 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "1.000000"));
    lFi0.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, ""));
    lFi1.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "stdin"));
    lFo0.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, ""));
    lFo1.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, ""));
    lI0 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "0"));
    lI1 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "1"));
    lS0 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "Test 1"));
    lS1 .Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "Test 2"));
    lSE0.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "TestE 1"));
    lSE1.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "TestE 2"));
    lUI0.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "0"));
    lUI1.Get(lStr, sizeof(lStr)); KMS_TEST_ASSERT(0 == strcmp(lStr, "1"));

    // Set
    lB0 .Set("false");
    lB1 .Set("true");
    lE0 .Set("1");
    lE1 .Set("0");
    lF0 .Set("2.0");
    lF1 .Set("3.0");
    lFi0.Set(TEST_FILE_0);
    lFi1.Set(TEST_FILE_1);
    lFo0.Set("2");
    lFo1.Set("3");
    lI0 .Set("2");
    lI1 .Set("3");
    lS0 .Set("Test 3");
    lS1 .Set("Test 4");
    lSE0.Set("TestE 3");
    lSE1.Set("TestE 4");
    lUI0.Set("2");
    lUI1.Set("3");

    KMS_TEST_ASSERT(!lB0I);
    KMS_TEST_ASSERT( lE0I  == TestEnum::TEST_ENUM_1);
    KMS_TEST_ASSERT( lF0I  == 2.0);
    KMS_TEST_ASSERT( lFi0I != nullptr);
    KMS_TEST_ASSERT( lI0I  == 2);
    KMS_TEST_ASSERT( lS0I  == "Test 3");
    KMS_TEST_ASSERT( lSE0I == "TestE 3");
    KMS_TEST_ASSERT( lUI0I == 2);

    // Set_Try

    // operator =
    lB0  = true;
    lB1  = true;
    lE0  = TestEnum::TEST_ENUM_1;
    lE1  = TestEnum::TEST_ENUM_1;
    lF0  = 4.0;
    lF1  = 5.0;
    lFi0 = TEST_FILE_0;
    lFi1 = TEST_FILE_1;
    lFo0 = File::Folder("4");
    lFo1 = File::Folder("5");
    lI0  = 4;
    lI1  = 5;
    lS0  = "Test 5";
    lS1  = "Test 6";
    lSE0 = "TestE 5";
    lSE1 = "TestE 6";
    lUI0 = 4;
    lUI1 = 5;

    KMS_TEST_ASSERT(lB0I);
    KMS_TEST_ASSERT(lE0I  == TestEnum::TEST_ENUM_1);
    KMS_TEST_ASSERT(lF0I  == 4.0);
    KMS_TEST_ASSERT(lFi0I != nullptr);
    KMS_TEST_ASSERT(lI0I  == 4);
    KMS_TEST_ASSERT(lS0I  == "Test 5");
    KMS_TEST_ASSERT(lSE0I == "TestE 5");
    KMS_TEST_ASSERT(lUI0I == 4);

    // operator ==
    KMS_TEST_ASSERT(lB0  == true);
    KMS_TEST_ASSERT(lB1  == true);
    KMS_TEST_ASSERT(lE0  == TestEnum::TEST_ENUM_1);
    KMS_TEST_ASSERT(lE1  == TestEnum::TEST_ENUM_1);
    KMS_TEST_ASSERT(lF0  == 4.0);
    KMS_TEST_ASSERT(lF1  == 5.0);
    KMS_TEST_ASSERT(lI0  == 4);
    KMS_TEST_ASSERT(lI1  == 5);
    KMS_TEST_ASSERT(lS0  == "Test 5");
    KMS_TEST_ASSERT(lS1  == "Test 6");
    KMS_TEST_ASSERT(lSE0 == "TestE 5");
    KMS_TEST_ASSERT(lSE1 == "TestE 6");
    KMS_TEST_ASSERT(lUI0 == 4U);
    KMS_TEST_ASSERT(lUI1 == 5U);

    // String_Ptr::GetLenngth
    KMS_TEST_COMPARE(lS0 .GetLength(), 6U);
    KMS_TEST_COMPARE(lS1 .GetLength(), 6U);
    KMS_TEST_COMPARE(lSE0.GetLength(), 7U);
    KMS_TEST_COMPARE(lSE1.GetLength(), 7U);

    // String_Ptr::GetString
    auto lS0S  = lS0 .GetString();
    auto lS1S  = lS1 .GetString();
    auto lSE0S = lSE0.GetString();
    auto lSE1S = lSE1.GetString();

    // ===== Object =========================================================

    // Clear
    KMS_TEST_ASSERT(lB0 .Clear());
    KMS_TEST_ASSERT(lB1 .Clear());
    KMS_TEST_ASSERT(lE0 .Clear());
    KMS_TEST_ASSERT(lE1 .Clear());
    KMS_TEST_ASSERT(lF0 .Clear());
    KMS_TEST_ASSERT(lF1 .Clear());
    KMS_TEST_ASSERT(lFi0.Clear());
    KMS_TEST_ASSERT(lFi1.Clear());
    KMS_TEST_ASSERT(lFo0.Clear());
    KMS_TEST_ASSERT(lFo1.Clear());
    KMS_TEST_ASSERT(lI0 .Clear());
    KMS_TEST_ASSERT(lI1 .Clear());
    KMS_TEST_ASSERT(lS0 .Clear());
    KMS_TEST_ASSERT(lS1 .Clear());
    KMS_TEST_ASSERT(lSE0.Clear());
    KMS_TEST_ASSERT(lSE1.Clear());
    KMS_TEST_ASSERT(lUI0.Clear());
    KMS_TEST_ASSERT(lUI1.Clear());

    KMS_TEST_ASSERT(!lB0 .Clear());
    KMS_TEST_ASSERT(!lB1 .Clear());
    KMS_TEST_ASSERT(!lE0 .Clear());
    KMS_TEST_ASSERT(!lE1 .Clear());
    KMS_TEST_ASSERT(!lF0 .Clear());
    KMS_TEST_ASSERT(!lF1 .Clear());
    KMS_TEST_ASSERT(!lFi0.Clear());
    KMS_TEST_ASSERT(!lFi1.Clear());
    KMS_TEST_ASSERT(!lFo0.Clear());
    KMS_TEST_ASSERT(!lFo1.Clear());
    KMS_TEST_ASSERT(!lI0 .Clear());
    KMS_TEST_ASSERT(!lI1 .Clear());
    KMS_TEST_ASSERT(!lS0 .Clear());
    KMS_TEST_ASSERT(!lS1 .Clear());
    KMS_TEST_ASSERT(!lSE0.Clear());
    KMS_TEST_ASSERT(!lSE1.Clear());
    KMS_TEST_ASSERT(!lUI0.Clear());
    KMS_TEST_ASSERT(!lUI1.Clear());

    KMS_TEST_ASSERT(!lB0I);
    KMS_TEST_ASSERT( lE0I  == TestEnum::TEST_ENUM_0);
    KMS_TEST_ASSERT( lF0I  == 0.0);
    KMS_TEST_ASSERT( lI0I  == 0);
    KMS_TEST_ASSERT( lS0I  == "");
    KMS_TEST_ASSERT( lSE0I == "");
    KMS_TEST_ASSERT( lUI0I == 0);
}

KMS_TEST(DI_Value_Exception, "Auto", sTest_Exception)
{
    DI::Boolean                         lB0;
    DI::Enum<TestEnum, TEST_ENUM_NAMES> lE0(TestEnum::TEST_ENUM_0);
    DI::Float<double>                   lF0;
    DI::File                            lFi0;
    DI::Int<int8_t>                     lI0;
    DI::String                          lS0("Test0");
    DI::String_Expand                   lSE0;
    DI::Int<uint8_t>                    lUI0;

    // Get
    char lBuffer[8];

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lS0.Get(lBuffer, 5);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_OUTPUT_TOO_SHORT);

    // Set
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lB0.Set("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lE0.Set("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.Set("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_FORMAT);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lFi0.Set("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_OPEN_FAILED);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lI0.Set("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lI0.Set("128");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lSE0.Set("{$DoesNotExist}");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_EXPAND_FAILED");

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lUI0.Set("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lUI0.Set("256");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Text_File_UTF16.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Text/File_UTF16.h>

KMS_TEST(Text_File_UTF16_Base, "Text_File_UTF16_Base", "Auto", sTest_Base)
{
    KMS::Text::File_UTF16 lF0;

    // AddLine
    lF0.AddLine(L"0");

    // Clear
    lF0.Clear();

    // GetLineCount
    KMS_TEST_COMPARE(lF0.GetLineCount(), 0L);

    // InsertLine
    lF0.InsertLine(0, L"0");
    KMS_TEST_ASSERT(NULL != lF0.GetLine(0));
    KMS_TEST_COMPARE(lF0.GetLineCount(), 1L);

    // RemoveEmptyLines
    KMS_TEST_COMPARE(lF0.RemoveEmptyLines(), 0L);

    // RemoveLines
    lF0.RemoveLines(0, 1);
    KMS_TEST_COMPARE(lF0.GetLineCount(), 0L);

    // Read
    lF0.Read(KMS::File::Folder(KMS::File::Folder::Id::CURRENT, "KMS-A-Test" SLASH "Tests"), "Test0.txt");
    KMS_TEST_COMPARE(lF0.GetLineCount(), 9737L);

    // Write
    lF0.Write(KMS::File::Folder::CURRENT, "Test.txt");

    // CountOccurrence
    KMS_TEST_COMPARE(lF0.CountOccurrence(L"Start"), 22L);

    // ReplaceLine *
    lF0.ReplaceLine(1, L"Test");

    // ReplaceLines *
    KMS_TEST_COMPARE(lF0.ReplaceLines(L"^Test$", L"Replaced"), 1L);
}

KMS_TEST(Text_File_UTF16_Exception, "Text_File_UTF16_Base", "Auto", sTest_Exception)
{
    KMS::Text::File_UTF16 lF0;

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lF0.GetLine(0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lF0.InsertLine(1, L"");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lF0.RemoveLines(0, 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lF0.ReplaceLine(0, L"");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lF0.Read(KMS::File::Folder::CURRENT, "DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_OPEN_FAILED);

    try
    {
        KMS_TEST_EXPECTED_EXCEPTION();
        lF0.Write(KMS::File::Folder(KMS::File::Folder::Id::CURRENT, "DoesNotExist"), "DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_OPEN_FAILED);
}

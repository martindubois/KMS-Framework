
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Text_File_ASCII.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Text/File_ASCII.h>

using namespace KMS;

KMS_TEST(Text_File_ASCII_Base, "Text_File_ASCII_Base", "Auto", sTest_Base)
{
    Text::File_ASCII lF0;

    // AddLine
    lF0.AddLine("0");

    // Clear
    lF0.Clear();

    // GetLineCount
    KMS_TEST_COMPARE(lF0.GetLineCount(), 0U);

    // InsertLine
    lF0.InsertLine(0, "0");
    KMS_TEST_ASSERT(nullptr != lF0.GetLine(0));
    KMS_TEST_COMPARE(lF0.GetLineCount(), 1U);

    // RemoveEmptyLines
    KMS_TEST_COMPARE(lF0.RemoveEmptyLines(), 0U);

    // RemoveLines
    lF0.RemoveLines(0, 1);
    KMS_TEST_COMPARE(lF0.GetLineCount(), 0U);

    // Read
    lF0.Read(File::Folder(File::Folder::Id::CURRENT, "KMS-A-Test" SLASH "Tests"), "Test5.txt");
    KMS_TEST_COMPARE(lF0.GetLineCount(), 29U);

    // GetUserLineNo *
    KMS_TEST_COMPARE(lF0.GetUserLineNo(0), 0U);

    // Write
    lF0.Write(File::Folder::CURRENT, "Test_Text_File_ASCII_Base_0.txt");

    // CountOccurrence
    KMS_TEST_COMPARE(lF0.CountOccurrence("Start"), 1U);

    // RemoveComments_CPP
    KMS_TEST_COMPARE(lF0.RemoveComments_CPP(), 1U);
    KMS_TEST_COMPARE(lF0.RemoveComments_CPP(), 0U);

    // RemoveComments_Script
    KMS_TEST_COMPARE(lF0.RemoveComments_Script(), 0U);

    // ReplaceLine *
    lF0.ReplaceLine(1, "Test");

    // ReplaceLines *
    KMS_TEST_COMPARE(lF0.ReplaceLines("^Test$", "Replaced"), 1U);

    // ===== Cleanup ========================================================
    File::Folder::CURRENT.Delete("Test_Text_File_ASCII_Base_0.txt");
}

KMS_TEST(Text_File_ASCII_Exception, "Text_File_ASCII_Exception", "Auto", sTest_Exception)
{
    Text::File_ASCII lF0;

    // GetLine
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.GetLine(0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    // GetUserLineNo
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.GetUserLineNo(0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    // InsertLine
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.InsertLine(1, "");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    // RemoveLines
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.RemoveLines(0, 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    // ReplaceLine
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.ReplaceLine(0, "");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_ARGUMENT_INVALID);

    // Read
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.Read(File::Folder::CURRENT, "DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_OPEN_FAILED);

    // Write
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lF0.Write(File::Folder(File::Folder::Id::CURRENT, "DoesNotExist"), "Test_Text_File_ASCII_Excpetion_0");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(TEXT_OPEN_FAILED);
}

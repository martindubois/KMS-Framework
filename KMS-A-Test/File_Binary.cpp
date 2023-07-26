
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Binary.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Dbg/Log.h>
#include <KMS/File/Binary.h>

using namespace KMS;

KMS_TEST(File_Binary_Base, "File_Binary_Base", "Auto", sTest_Base)
{
    uint8_t lBuffer[1];

    // Read
    File::Binary lB0(File::Folder::CURRENT, "Test_File_Binary_Base_0", true);
    KMS_TEST_COMPARE(lB0.Read(lBuffer, sizeof(lBuffer)), 0U);

    // Write
    File::Binary lB1(File::Folder::CURRENT, "Test_File_Binary_Base_1", true);
    lB1.Write(lBuffer, sizeof(lBuffer));
}

KMS_TEST(File_Binary_Exception, "File_Binary_Exception", "Auto", sTest_Exception)
{
    // Constructor
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Binary lB(File::Folder::CURRENT, "DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FILE_OPEN_FAILED);

    // Map
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Binary lB(File::Folder::CURRENT, "Test_File_Binary_Exception_0", true);
        lB.Map(1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FILE_TOO_SHORT);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Binary lB(File::Folder::CURRENT, "Test_File_Binary_Exception_1", true);
        lB.Map(0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FILE_MAPPING_FAILED);

    // Write
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Binary lB(File::Folder::CURRENT, ".gitignore", false);
        lB.Write(" ", 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FILE_WRITE_FAILED);
}

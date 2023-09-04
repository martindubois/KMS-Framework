
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
    {
        uint8_t lBuffer[1];

        // Map
        if (File::Folder::CURRENT.DoesFileExist("Test_File_Binary_Base_0"))
        {
            File::Folder::CURRENT.Delete("Test_File_Binary_Base_0");
        }
        File::Binary lB0(File::Folder::CURRENT, "Test_File_Binary_Base_0", true);
        KMS_TEST_ASSERT(nullptr != lB0.Map(1));

        // Read
        File::Binary lB1(File::Folder::CURRENT, "Test_File_Binary_Base_1", true);
        KMS_TEST_COMPARE(lB1.Read(lBuffer, sizeof(lBuffer)), 0U);

        // Write
        File::Binary lB2(File::Folder::CURRENT, "Test_File_Binary_Base_2", true);
        lB2.Write(lBuffer, sizeof(lBuffer));
    }

    // ===== Cleanup ========================================================
    File::Folder::CURRENT.Delete("Test_File_Binary_Base_0");
    File::Folder::CURRENT.Delete("Test_File_Binary_Base_1");
    File::Folder::CURRENT.Delete("Test_File_Binary_Base_2");
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
    KMS_TEST_CATCH(RESULT_OPEN_FAILED);

    // Map

    // Write
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Binary lB(File::Folder::CURRENT, ".gitignore", false);
        lB.Write(" ", 1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_WRITE_FAILED);
}

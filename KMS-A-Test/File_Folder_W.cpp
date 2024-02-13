
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Folder_W.cpp

// TODO  Make these test work on Linux

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

using namespace KMS;

KMS_TEST(File_Folder_Windows, "Auto", sTest_Windows)
{
    // Constructors
    File::Folder lF0(File::Folder::Id::CURRENT);
    File::Folder lF1(File::Folder::Id::TEMPORARY);

    // Copy
    lF0.Copy(lF1, "DoesNotExist", "Test_File_Folder_Base_2.txt", File::Folder::FLAG_IGNORE_ERROR | File::Folder::FLAG_VERBOSE);

    // DoesFolderExist
    KMS_TEST_ASSERT(!lF0.DoesFolderExist(".gitignore"));
    KMS_TEST_ASSERT(!lF0.DoesFolderExist("DoesNotExist"));
}

KMS_TEST(File_Folder_Windows_Exception, "Auto", sTest_Windows_Exception)
{
    // Compress
    try
    {
        File::Folder lF("DoesNotExist");
        KMS_TEST_OUTPUT_BEGIN();
        lF.Compress(File::Folder::CURRENT, "Test_File_Folder_Exception_0.zip");
        KMS_TEST_OUTPUT_END();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_OUTPUT_END_N("RESULT_COMPRESS_FAILED");

    // Uncompress
    try
    {
        File::Folder lF("Test_File_Folder_Exception_2");
        KMS_TEST_OUTPUT_BEGIN();
        lF.Uncompress(File::Folder::CURRENT, "DoesNotExist.zip");
        KMS_TEST_OUTPUT_END();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_OUTPUT_END_N("RESULT_UNCOMPRESS_FAILED");

    // ===== Cleanup ========================================================
    File::Folder lF("Test_File_Folder_Exception_2");
    lF.Delete();
}

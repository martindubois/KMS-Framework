
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Folder.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

using namespace KMS;

KMS_TEST(File_Folder_Base, "Auto", sTest_Base)
{
    // ChangeCurrentDirectory
    File::Folder::ChangeCurrentDirectory("KMS-A-Test");
    File::Folder::ChangeCurrentDirectory("..");

    // Constructors
    File::Folder lF0(File::Folder::Id::CURRENT);
    File::Folder lF1(File::Folder::Id::TEMPORARY);
    File::Folder lF2(lF1, "Folder");

    // Backup
    lF0.Backup(".gitignore");
    lF0.Backup(".gitignore.000.bak", File::Folder::FLAG_BACKUP_RENAME);

    // Create
    lF2.Create();

    // Copy
    lF0.Copy(lF1, ".gitignore");
    lF0.Copy(lF1, ".gitignore"  , "Test_File_Folder_Base_0.txt");
    lF0.Copy(lF1, ".gitignore"  , "Test_File_Folder_Base_0.txt", File::Folder::FLAG_BACKUP);
    KMS_TEST_OUTPUT_BEGIN();
    {
        lF0.Copy(lF1, ".gitignore"  , "Test_File_Folder_Base_1.txt", File::Folder::FLAG_VERBOSE);
        lF2.Copy(File::Folder(lF1, "Folder2"));
    }
    KMS_TEST_OUTPUT_END();

    // Delete
    lF1.Delete("Test_File_Folder_Base_0.txt");
    lF1.Delete("Test_File_Folder_Base_1.txt");
    lF2.Delete();

    // DeleteFiles
    KMS_TEST_OUTPUT_BEGIN();
    {
        File::Folder::CURRENT.DeleteFiles("DoesNotExist");
    }
    KMS_TEST_OUTPUT_END();

    // DoesFolderExist
    KMS_TEST_ASSERT( lF0.DoesFolderExist("KMS-A-Test"));

    // ===== Cleanup ========================================================
    File::Folder::CURRENT.Delete(".gitignore.000.bak.000.bak");
}

KMS_TEST(File_Folder_Exception, "Auto", sTest_Exception)
{
    // ChangeCurrentDirectory
    try
    {
        File::Folder::ChangeCurrentDirectory("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_FOLDER_CHANGE_FAILED");

    // Compress

    // Delete
    try
    {
        File::Folder::CURRENT.Delete("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_DELETE_FAILED");

    try
    {
        File::Folder lF("DoesNotExist");
        lF.Delete();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_REMOVE_FAILED");

    // Rename
    try
    {
        File::Folder::CURRENT.Rename("DoesNotExist", "Test_File_Folder_Exception_1.zip");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_RENAME_FAILED");

    // Uncompress
}

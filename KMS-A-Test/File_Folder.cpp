
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Folder.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

using namespace KMS;

KMS_TEST(File_Folder_Base, "File_Folder_Base", "Auto", sTest_Base)
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
    lF0.Copy(lF1, ".gitignore"  , "Test_File_Folder_Base_1.txt", File::Folder::FLAG_VERBOSE);
    lF0.Copy(lF1, "DoesNotExist", "Test_File_Folder_Base_2.txt", File::Folder::FLAG_IGNORE_ERROR | File::Folder::FLAG_VERBOSE);
    lF2.Copy(File::Folder(lF1, "Folder2"));
    KMS_TEST_OUTPUT_END();

    // Delete
    lF1.Delete("Test_File_Folder_Base_0.txt");
    lF1.Delete("Test_File_Folder_Base_1.txt");
    lF2.Delete();

    // DeleteFiles
    KMS_TEST_OUTPUT_BEGIN();
    File::Folder::CURRENT.DeleteFiles("DoesNotExist");
    KMS_TEST_OUTPUT_END();

    // DoesFolderExist
    KMS_TEST_ASSERT(!lF0.DoesFolderExist(".gitignore"));
    KMS_TEST_ASSERT(!lF0.DoesFolderExist("DoesNotExist"));
    KMS_TEST_ASSERT( lF0.DoesFolderExist("KMS-A-Test"));
}

KMS_TEST(File_Folder_Exception, "File_Folder_Exception", "Auto", sTest_Exception)
{
    // ChangeCurrentDirectory
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Folder::ChangeCurrentDirectory("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FOLDER_CHANGE_FAILED);

    // Compress
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Folder lF("DoesNotExist");
        KMS_TEST_OUTPUT_BEGIN();
        lF.Compress(File::Folder::CURRENT, "Test_File_Folder_Exception_0.zip");
        KMS_TEST_OUTPUT_END();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_OUTPUT_END(FILE_COMPRESS_FAILED);

    // Delete
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Folder::CURRENT.Delete("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FILE_DELETE_FAILED);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Folder lF("DoesNotExist");
        lF.Delete();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FILE_REMOVE_FAILED);

    // Rename
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Folder::CURRENT.Rename("DoesNotExist", "Test_File_Folder_Exception_1.zip");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(FILE_RENAME_FAILED);

    // Uncompress
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        File::Folder lF("Test_File_Folder_Exception_2");
        KMS_TEST_OUTPUT_BEGIN();
        lF.Uncompress(File::Folder::CURRENT, "DoesNotExist.zip");
        KMS_TEST_OUTPUT_END();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_OUTPUT_END(FILE_UNCOMPRESS_FAILED);
}

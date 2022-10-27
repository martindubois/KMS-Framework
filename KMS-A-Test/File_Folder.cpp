
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Sync.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

KMS_TEST(File_Folder_Main, "File_Folder_Main", "Auto", sTest_Main)
{
    // Constructors
    KMS::File::Folder lF0(KMS::File::Folder::Id::CURRENT);
    KMS::File::Folder lF1(KMS::File::Folder::Id::TEMPORARY);

    // Backup
    lF0.Backup(".gitignore");
    lF0.Backup(".gitignore.000.bak", KMS::File::Folder::FLAG_BACKUP_RENAME);

    // Copy
    lF0.Copy(lF1, ".gitignore");
    lF0.Copy(lF1, ".gitignore", "Test0.txt");
    lF0.Copy(lF1, ".gitignore", "Test0.txt", KMS::File::Folder::FLAG_BACKUP);
    lF0.Copy(lF1, ".gitignore", "Test1.txt", KMS::File::Folder::FLAG_VERBOSE);
}

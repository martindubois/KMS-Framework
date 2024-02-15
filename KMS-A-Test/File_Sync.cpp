
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Sync.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Redirection.h>
#include <KMS/File/Sync.h>

using namespace KMS;

KMS_TEST(File_Sync_Main, "Auto", sTest_Main)
{
    static const char* VECTOR_A[] =
    {
        "KMS-Test.exe",
        "Bidirectional.Default+=DoesNotExist_A",
        "Bidirectional.Default+=DoesNotExist_B",
        "Unknown+=1",
        "Unknown=1",
        "SaveConfig=Test_File_Sync_Main_0.txt",
    };

    KMS_TEST_COMPARE(File::Sync::Main(3, VECTOR_A), 0);

    KMS_TEST_COMPARE(File::Sync::Main(6, VECTOR_A), 0);

    static const char* VECTOR_B[] =
    {
        "KMS-Test.exe",
        "Unidirectional+=DoesNotExist;DoesNotExist",
        "Unidirectional+=DoesNotExist;DoesNotExist",
        "SaveConfig=Test_File_Sync_Main_1.txt",
    };

    int lRet;
    Console::Redirection lR(Console::Redirection::What::WHAT_STDOUT);
    {
        lRet = File::Sync::Main(4, VECTOR_B);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    // ===== Cleanup ========================================================
    File::Folder::CURRENT.Delete("Test_File_Sync_Main_0.txt");
    File::Folder::CURRENT.Delete("Test_File_Sync_Main_1.txt");
}

KMS_TEST(File_Sync_Main_Fail, "Auto", sTest_Fail)
{
    static const char* VECTOR_A[] =
    {
        "KMS-Test.exe",
        "Bidirectional.Default+=DoesNotExist_A",
    };

    KMS_TEST_ASSERT(0 != File::Sync::Main(2, VECTOR_A));
}

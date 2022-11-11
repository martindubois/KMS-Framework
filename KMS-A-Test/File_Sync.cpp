
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Sync.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Sync.h>

KMS_TEST(File_Sync_Main, "File_Sync_Main", "Auto", sTest_Main)
{
    static const char* VECTOR_A[] =
    {
        "KMS-Test.exe",
        "Bidirectional.Default+=DoesNotExist_A",
        "Bidirectional.Default+=DoesNotExist_B",
        "Unknown+=1",
        "Unknown=1",
        "SaveConfig=Test.txt",
    };

    KMS_TEST_COMPARE(KMS::File::Sync::Main(3, VECTOR_A), 0);

    KMS_TEST_EXPECTED_WARNINGS(2);
    KMS_TEST_COMPARE(KMS::File::Sync::Main(6, VECTOR_A), 0);

    static const char* VECTOR_B[] =
    {
        "KMS-Test.exe",
        "Unidirectional+=DoesNotExist;DoesNotExist",
        "Unidirectional+=DoesNotExist;DoesNotExist",
        "SaveConfig=Test.txt",
    };

    KMS_TEST_COMPARE(KMS::File::Sync::Main(4, VECTOR_B), 0);
}

KMS_TEST(File_Sync_Main_Fail, "File_Sync_Main_Fail", "Auto", sTest_Fail)
{
    static const char* VECTOR_A[] =
    {
        "KMS-Test.exe",
        "Bidirectional.Default+=DoesNotExist_A",
    };

    KMS_TEST_EXPECTED_EXCEPTION();
    KMS_TEST_EXPECTED_ERROR();
    KMS_TEST_COMPARE(KMS::File::Sync::Main(2, VECTOR_A), static_cast<int>(KMS::Exception::Code::FILE_CONFIG_INVALID));
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/File_Sync.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>

#include <KMS/File/Sync.h>

KMS_TEST(File_Sync_Main, "File_Sync_Main", "Auto", sTest_Main)
{
    static const char* VECTOR_A[] =
    {
        "KMS-Test.exe",
        "Destinations",
        "Sources",
        "Folders+=DoesNotExist_A",
        "Folders+=DoesNotExist_B",
        "Unknown+=1",
        "Unknown=1",
    };

    KMS_TEST_COMPARE(KMS::File::Sync::Main(2, VECTOR_A), 0);
    KMS_TEST_COMPARE(KMS::File::Sync::Main(3, VECTOR_A), 0);
    KMS_TEST_COMPARE(KMS::File::Sync::Main(7, VECTOR_A), 0);

    static const char* VECTOR_B[] =
    {
        "KMS-Test.exe",
        "Destinations",
        "Sources",
        "Destination=DoesNotExist",
        "Destination=DoesNotExist",
        "Source=DoesNotExist",
        "Source=DoesNotExist",
    };

    KMS_TEST_COMPARE(KMS::File::Sync::Main(6, VECTOR_B), 0);
    KMS_TEST_COMPARE(KMS::File::Sync::Main(7, VECTOR_B), 0);
}

KMS_TEST(File_Sync_Main_Fail, "File_Sync_Main_Fail", "Auto", sTest_Fail)
{
    std::cerr << KMS::Console::Color::BLUE;
    std::cerr << "IMPORTANT : Ignore the following error messages. The current test tests failling tests." << std::endl;
    std::cerr << KMS::Console::Color::WHITE;

    static const char* VECTOR_A[] =
    {
        "KMS-Test.exe",
        "Destinations",
        "Sources",
        "Folders+=DoesNotExist_A",
    };

    KMS_TEST_COMPARE(KMS::File::Sync::Main(4, VECTOR_A), static_cast<int>(KMS::Exception::Code::CONFIG));

    static const char* VECTOR_B[] =
    {
        "KMS-Test.exe",
        "Destinations",
        "Sources",
        "Destination=DoesNotExist",
        "Destination=DoesNotExist",
    };

    KMS_TEST_COMPARE(KMS::File::Sync::Main(4, VECTOR_B), static_cast<int>(KMS::Exception::Code::CONFIG));
    KMS_TEST_COMPARE(KMS::File::Sync::Main(5, VECTOR_B), static_cast<int>(KMS::Exception::Code::CONFIG));

    static const char* VECTOR_C[] =
    {
        "KMS-Test.exe",
        "Destinations",
        "Sources",
        "Source=DoesNotExist",
        "Destination=DoesNotExist",
        "Folders+=DoesNotExist",
    };

    KMS_TEST_COMPARE(KMS::File::Sync::Main(4, VECTOR_C), static_cast<int>(KMS::Exception::Code::CONFIG));
    KMS_TEST_COMPARE(KMS::File::Sync::Main(6, VECTOR_C), static_cast<int>(KMS::Exception::Code::CONFIG));

    std::cerr << KMS::Console::Color::BLUE;
    std::cerr << "IMPORTANT : End of the test of failling tests." << std::endl;
    std::cerr << KMS::Console::Color::WHITE;
}

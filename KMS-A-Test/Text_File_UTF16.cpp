
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Text_File_UTF16.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Text/File_UTF16.h>

KMS_TEST(Text_File_UTF16_Base, "Text_File_UTF16_Base", "Auto", sTest_Base)
{
    KMS::Text::File_UTF16 lF0;

    lF0.Read(KMS::File::Folder(KMS::File::Folder::Id::CURRENT, "KMS-A-Test" SLASH "Tests"), "Test0.txt");

    KMS_TEST_COMPARE(lF0.CountOccurrence(L"Start"), 22L);

    lF0.Write(KMS::File::Folder(KMS::File::Folder::Id::CURRENT), "Test.txt");
}

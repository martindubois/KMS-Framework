
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/FileFormat_ELF.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/FileFormat/ELF.h>

using namespace KMS;

KMS_TEST(FileFormat_ELF_Main, "FileFormat_ELF_Base", "Auto", sTest_Base)
{
    FileFormat::ELF lELF0(File::Folder::CURRENT, "KMS-A-Test/Tests/Test3.elf");

    lELF0.DisplayHeaders(stdout);

    uint8_t lImage[0x4000];

    KMS_TEST_COMPARE(lELF0.CreateFlashImage(0, 0x4000, lImage), static_cast<unsigned int>(14934));
}

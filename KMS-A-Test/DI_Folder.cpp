
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Folder.h>

KMS_TEST(DI_Folder_Base, "DI_Folder_Base", "Auto", sTest_Base)
{
    // Create
    KMS::DI::Object* lO = KMS::DI::Folder::Create();
    KMS_TEST_ASSERT(NULL != lO);
    delete lO;

    // Constructors

    // Operator =
    KMS::DI::Folder lF0;
    lF0 = KMS::File::Folder::CURRENT;

    // Set
    lF0.Set("EXECUTABLE:DoesNotExist");
    lF0.Set("HOME:.vscode");
    lF0.Set("PROGRAM_FILES:DoesNotExist");
    lF0.Set("PROGRAM_FILES_X86:DoesNotExist");
    lF0.Set("KMS-A-Test");
    KMS_TEST_ASSERT(lF0.Get().DoesExist());
}
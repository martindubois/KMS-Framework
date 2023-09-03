
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Folder.h>

using namespace KMS;

KMS_TEST(DI_Folder_Base, "DI_Folder_Base", "Auto", sTest_Base)
{
    // Create
    auto lF0 = DI::Folder::Create();
    KMS_TEST_ASSERT(nullptr != lF0);
    delete lF0;

    // Constructors
    File::Folder lInternal;
    DI::Folder     lF1;
    DI::Folder_Ptr lFP0(&lInternal);

    // Operator =
    lF1  = File::Folder::CURRENT;
    lFP0 = File::Folder::CURRENT;

    // operator const File::Folder&
    File::Folder lF2 = lF1;

    // Set
    lF1.Set("EXECUTABLE:DoesNotExist");
    lF1.Set("HOME:.vscode");
    lF1.Set("PROGRAM_FILES:DoesNotExist");
    lF1.Set("PROGRAM_FILES_X86:DoesNotExist");
    lF1.Set("KMS-A-Test");
    KMS_TEST_ASSERT(lF1.GetFolder().DoesExist());
}

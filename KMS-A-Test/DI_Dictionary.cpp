
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Dictionary.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/String.h>

using namespace KMS;

KMS_TEST(DI_Dictionary_Base, "DI_Dictionary_Base", "Auto", sTest_Base)
{
    DI::Dictionary lD0;
    DI::String     lS0;

    // Create
    DI::Object* lO0 = DI::Dictionary::Create();
    KMS_TEST_ASSERT(nullptr != lO0);
    delete lO0;

    // AddConstEntry
    lD0.AddConstEntry("Test0", &lS0);
    lD0.AddConstEntry("Test0", &lS0);

    // AddEntry
    lD0.AddEntry("Test0", &lS0, false);

    // CreateEntry

    // GetEntry_R
    KMS_TEST_ASSERT(nullptr == lD0.GetEntry_R("DoesNotExist"));
    KMS_TEST_ASSERT(nullptr != lD0.GetEntry_R("Test0"));

    // GetEntry_RW

    // ===== DI::Container ==================================================

    // Clear

    // GetCount
    KMS_TEST_COMPARE(lD0.GetCount(), 1U);

    // IsEmpty
    KMS_TEST_ASSERT(!lD0.IsEmpty());

    // FindObject_RW
}

KMS_TEST(DI_Dictionary_Exception, "DI_Dictionary_Exception", "Auto", sTest_Exception)
{
    DI::Dictionary lD0;

    // ===== DI::Container ==================================================

    // FindObject_RW
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lD0.FindObject_RW("");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(DI_NAME_INVALID);

}

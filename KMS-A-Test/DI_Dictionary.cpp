
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

KMS_TEST(DI_Dictionary_Base, "Auto", sTest_Base)
{
    DI::Dictionary     lD0;
    Ptr_OF<DI::Object> lEntry;
    DI::String         lS0;

    // Create
    DI::Object* lO0 = DI::Dictionary::Create();
    KMS_TEST_ASSERT(nullptr != lO0);
    delete lO0;

    // AddConstEntry
    lEntry.Set(&lS0); lD0.AddEntry("Test0", lEntry);
    lEntry.Set(&lS0); lD0.AddEntry("Test0", lEntry);

    // AddEntry
    lEntry.Set(&lS0, false); lD0.AddEntry("Test0", lEntry);

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

KMS_TEST(DI_Dictionary_Exception, "Auto", sTest_Exception)
{
    DI::Dictionary lD0;

    // ===== DI::Container ==================================================

    // FindObject_RW
    try
    {
        lD0.FindObject_RW("");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

}

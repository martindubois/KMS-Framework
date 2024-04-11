
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/DI_Array.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/String.h>

using namespace KMS;

KMS_TEST(DI_Array_Base, "Auto", sTest_Base)
{
    DI::Array          lA0;
    Ptr_OF<DI::Object> lEntry;
    DI::String         lS0;

    // AddConstEntry
    lEntry.Set(&lS0);
    lA0.AddEntry(lEntry);

    // AddEntry
    lEntry.Set(&lS0, false);
    lA0.AddEntry(lEntry);

    // CreateEntry

    // GetEntry_R
    KMS_TEST_ASSERT(nullptr == lA0.GetEntry_R(2));

    // GetEntry_RW

    // RemoveEntry
    lA0.RemoveEntry(1);

    // SetEntry
    lEntry.Set(&lS0, false); lA0.SetEntry(0, lEntry);
    lEntry.Set(&lS0, false); lA0.SetEntry(1, lEntry);

    // ===== Container ======================================================

    // GetCount
    KMS_TEST_COMPARE(lA0.GetCount(), 2U);

    // IsEmpty
    KMS_TEST_ASSERT(!lA0.IsEmpty());

    // FindObject_RW
    KMS_TEST_ASSERT(nullptr != lA0.FindObject_RW("0"));
    KMS_TEST_ASSERT(nullptr == lA0.FindObject_RW("2"));

    // ===== Object =========================================================

    // Clear
}

KMS_TEST(DI_Array_Exception, "Auto", sTest_Exception)
{
    DI::Array lA0;

    DI::String lS0;

    // RemoveEntry
    try
    {
        lA0.RemoveEntry(0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_INDEX);

    // SetEntry
    try
    {
        Ptr_OF<DI::Object> lEntry(&lS0, false);
        lA0.SetEntry(1, lEntry);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_INDEX);

    // ===== Container ======================================================

    // FindObject_RW
    try
    {
        lA0.FindObject_RW("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    try
    {
        lA0.FindObject_RW("1");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_INDEX);
}

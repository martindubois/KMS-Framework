
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

KMS_TEST(DI_Array_Base, "DI_Array_Base", "Auto", sTest_Base)
{
    DI::Array lA0;

    DI::String lS0;

    // AddConstEntry
    lA0.AddConstEntry(&lS0);

    // AddEntry
    lA0.AddEntry(&lS0, false);

    // CreateEntry

    // GetEntry_R
    KMS_TEST_ASSERT(NULL == lA0.GetEntry_R(2));

    // GetEntry_RW

    // RemoveEntry
    lA0.RemoveEntry(1);

    // SetEntry
    lA0.SetEntry(0, &lS0, false);
    lA0.SetEntry(1, &lS0, false);

    // ===== Container ======================================================

    // GetCount
    KMS_TEST_COMPARE(lA0.GetCount(), 2U);

    // IsEmpty
    KMS_TEST_ASSERT(!lA0.IsEmpty());

    // FindObject_RW
    KMS_TEST_ASSERT(NULL != lA0.FindObject_RW("0"));
    KMS_TEST_ASSERT(NULL == lA0.FindObject_RW("2"));

    // ===== Object =========================================================

    // Clear
}

KMS_TEST(DI_Array_Exception, "DI_Array_Exception", "Auto", sTest_Exception)
{
    DI::Array lA0;

    DI::String lS0;

    // RemoveEntry
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lA0.RemoveEntry(0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(DI_INDEX_INVALID);

    // SetEntry
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lA0.SetEntry(1, &lS0, false);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(DI_INDEX_INVALID);

    // ===== Container ======================================================

    // FindObject_RW
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lA0.FindObject_RW("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(DI_NAME_INVALID);

    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lA0.FindObject_RW("1");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(DI_INDEX_INVALID);
}

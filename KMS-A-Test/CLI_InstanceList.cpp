
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/CLI_InstanceList.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/InstanceList.h>
#include <KMS/Console/Redirection.h>

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(CLI_InstanceList_Base, "Auto", sTest_Base)
{
    // Constructor
    CLI::InstanceList<uint32_t> lIL0;

    // Add
    lIL0.Add("first", new uint32_t);

    // Get
    KMS_TEST_ASSERT(nullptr != lIL0.Get("first"));
    KMS_TEST_ASSERT(nullptr == lIL0.Get("DoesNotExist"));

    // GetSelected
    KMS_TEST_ASSERT(nullptr == lIL0.GetSelected());

    // Delete
    KMS_TEST_COMPARE(lIL0.Delete("first"), 0);
    KMS_TEST_COMPARE(lIL0.Delete("DoesNotExist"), -2);

    // DeleteAll
    lIL0.DeleteAll();
    lIL0.Add("first", new uint32_t);
    lIL0.DeleteAll();

    // ExecuteCommand
    int lRet;

    lIL0.Add("first", new uint32_t);

    Console::Redirection lR(Console::Redirection::What::WHAT_ALL);
    {
        lRet = lIL0.ExecuteCommand("Delete first");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("Delete DoesNotExist");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, -2);

    lIL0.Add("first", new uint32_t);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("Select first");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("Select DoesNotExist");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, -3);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("Delete All");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("List");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lIL0.Add("first", new uint32_t);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("List");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("Invalid");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, -4);

    // List

    // Remove
    KMS_TEST_ASSERT(nullptr == lIL0.Remove("DoesNotExist"));

    // Select
    KMS_TEST_ASSERT(nullptr == lIL0.Select("DoesNotExist"));

    // Destructor
}

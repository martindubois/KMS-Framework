
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

KMS_TEST(CLI_InstanceList_Base, "Auto", sTest_Base)
{
    // Constructor
    CLI::InstanceList<uint32_t> lIL0;

    // GetSelectedName
    lIL0.Add("first", new uint32_t);
    KMS_TEST_ASSERT(0 == strcmp("first", lIL0.GetSelectedName()));

    // IsInstanceSelected
    KMS_TEST_ASSERT(lIL0.IsInstanceSelected());
    lIL0.Unselect();
    KMS_TEST_ASSERT(!lIL0.IsInstanceSelected());

    // SetAllowedCmds
    lIL0.SetAllowedCmds(CLI::InstanceList_Base::CMD_ALL);

    // DeleteAll
    lIL0.DeleteAll();

    // DeleteByName
    lIL0.Add("first", new uint32_t);
    lIL0.DeleteByName("first");

    // DeleteSelected
    lIL0.Add("first", new uint32_t);
    lIL0.DeleteSelected();

    // DeleteAll
    lIL0.DeleteAll();
    lIL0.Add("first", new uint32_t);
    lIL0.DeleteAll();

    // ExecuteCommand
    int lRet;

    lIL0.Add("first", new uint32_t);

    Console::Redirection lR(Console::Redirection::What::WHAT_ALL);
    {
        lRet = lIL0.ExecuteCommand("Delete");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lIL0.Add("first", new uint32_t);

    lR.Redirect();
    {
        lRet = lIL0.ExecuteCommand("Select first");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

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

    // List

    // VerifyInstanceSelected

    // ===== Typed ==========================================================

    // Add

    // GetByName
    KMS_TEST_ASSERT(nullptr != lIL0.GetByName("first"));

    // GetSelected
    KMS_TEST_ASSERT(nullptr != lIL0.GetSelected());

    // Delete

    // Remove
    auto lI0 = lIL0.GetByName("first");
    lIL0.Remove(lI0);
    delete lI0;

    // RemoveByName

    // RemoveSelected

    // SelectByName

    // Destructor
}

KMS_TEST(CLI_InstanceList_Exception, "Auto", sTest_Exception)
{
    CLI::InstanceList<uint32_t> lIL0;

    // Constructor

    // GetSelectedName
    try
    {
        lIL0.GetSelectedName();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    // IsInstanceSelected

    // SetAllowedCmds

    // DeleteAll

    // DeleteByName
    try
    {
        lIL0.DeleteByName("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    // DeleteSelected
    try
    {
        lIL0.DeleteSelected();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    // ExecuteCommand
    try
    {
        lIL0.ExecuteCommand("Delete");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    try
    {
        lIL0.ExecuteCommand("Select DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    try
    {
        lIL0.ExecuteCommand("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    lIL0.SetAllowedCmds(lIL0.CMD_NONE);

    try
    {
        lIL0.ExecuteCommand("Delete");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_DENIED);

    try
    {
        lIL0.ExecuteCommand("Delete All");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_DENIED);

    try
    {
        lIL0.ExecuteCommand("List");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_DENIED);

    try
    {
        lIL0.ExecuteCommand("Select first");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_DENIED);

    // List

    // VerifyInstanceSelected
    try
    {
        lIL0.VerifyInstanceSelected();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    // Unselect

    // ===== Typed ==========================================================

    // Add
    lIL0.Add("first", new uint32_t);
    try
    {
        lIL0.Add("first", new uint32_t);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    // GetByName
    try
    {
        lIL0.GetByName("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    // GetSelected
    lIL0.Unselect();
    try
    {
        lIL0.GetSelected();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    // Delete
    try
    {
        lIL0.Delete(reinterpret_cast<uint32_t*>(0x1));
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    // Remove
    try
    {
        lIL0.Remove(reinterpret_cast<uint32_t*>(0x1));
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_VALUE);

    // RemoveByName
    try
    {
        lIL0.RemoveByName("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    // RemoveSelected
    try
    {
        lIL0.RemoveSelected();
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    // SelectByName
    try
    {
        lIL0.SelectByName("DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);
}

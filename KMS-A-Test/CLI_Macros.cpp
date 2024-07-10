
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/CLI_Macris.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/Macros.h>
#include <KMS/Console/Redirection.h>

using namespace KMS;

KMS_TEST(CLI_Macros_Base, "Auto", sTest_Base)
{
    int lRet;

    // Constructor
    CLI::Macros lM0(&lM0);

    // Add
    lM0.Add("first");

    // ExecuteCommand
    Console::Redirection lR(Console::Redirection::What::WHAT_ALL);
    {
        lRet = lM0.ExecuteCommand("Macro Add second");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("Macro Delete");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("Macro Delete All");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    auto lFirst = lM0.Add("first");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("Macro Display");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lFirst->AddCommand("Command 0");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("Macro Display");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("Macro List");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    KMS_TEST_ASSERT(nullptr != lM0.mMacros.SelectByName("first"));

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("Macro + Command 1");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("first");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, lM0.UNKNOWN_COMMAND);

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand("DoesNotExist");
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, lM0.UNKNOWN_COMMAND);

    // DisplayHelp
    lR.Redirect();
    {
        lM0.DisplayHelp(stdout);
    }
    lR.Restore();
}

KMS_TEST(CLI_Macros_Exception, "Auto", sTest_Exception)
{
    CLI::Macros lM0(&lM0);

    // Add

    // ExecuteCommand
    try
    {
        lM0.ExecuteCommand("Macro Delete");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    try
    {
        lM0.ExecuteCommand("Macro Display");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    try
    {
        lM0.ExecuteCommand("Macro Select DoesNotExist");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    try
    {
        lM0.ExecuteCommand("Macro + Command 1");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);
}

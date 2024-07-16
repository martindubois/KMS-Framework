
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

    CLI::CommandLine lCL0("Macro Add second");

    Console::Redirection lR(Console::Redirection::What::WHAT_ALL);
    {
        lRet = lM0.ExecuteCommand(&lCL0);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    CLI::CommandLine lCL1("Macro Delete");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL1);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    CLI::CommandLine lCL2("Macro Delete All");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL2);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    auto lFirst = lM0.Add("first");

    CLI::CommandLine lCL3("Macro Display");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL3);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    lFirst->AddCommand("Command 0");

    CLI::CommandLine lCL4("Macro Display");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL4);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    CLI::CommandLine lCL5("Macro List");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL5);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    KMS_TEST_ASSERT(nullptr != lM0.mMacros.SelectByName("first"));

    CLI::CommandLine lCL6("Macro + Command 1");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL6);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    CLI::CommandLine lCL7("first");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL7);
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, lM0.UNKNOWN_COMMAND);

    CLI::CommandLine lCL8("DoesNotExist");

    lR.Redirect();
    {
        lRet = lM0.ExecuteCommand(&lCL8);
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

    CLI::CommandLine lCL0("Macro Delete");

    try
    {
        lM0.ExecuteCommand(&lCL0);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    CLI::CommandLine lCL1("Macro Display");

    try
    {
        lM0.ExecuteCommand(&lCL1);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);

    CLI::CommandLine lCL2("Macro Select DoesNotExist");

    try
    {
        lM0.ExecuteCommand(&lCL2);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_NAME);

    CLI::CommandLine lCL3("Macro + Command 1");

    try
    {
        lM0.ExecuteCommand(&lCL3);
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(RESULT_INVALID_STATE);
}

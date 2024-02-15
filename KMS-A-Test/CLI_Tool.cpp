
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/CLI_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>
#include <KMS/Console/Redirection.h>

using namespace KMS;

class Test_Tool : public CLI::Tool
{
};

// Constants
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(CLI_Tool_Base, "Auto", sTest_Base)
{
    // Constructor
    Test_Tool lT;

    // AddCommand
    lT.AddCommand("AbortIfError");
    lT.AddCommand("Config Invalid");
    lT.AddCommand("Echo Message");
    lT.AddCommand("ExecuteScript KMS-A-Test/Tests/Test_CLI_Tool_Base_0.txt");
    lT.AddCommand("ExitIfError");
    lT.AddCommand("Help");
    lT.AddCommand("Repeat 0 Help");
    lT.AddCommand("Repeat 1 Help");
    lT.AddCommand("UntilCtrlC Invalid");
    lT.AddCommand("Exit");

    // ClearError
    lT.ClearError();

    // GetExitCode

    // ExecuteCommands
    lT.ExecuteCommands(stdout);

    // ExecuteScripts

    // DisplayHelp

    // ExecuteCommand

    // Run
    int lRet;
    Console::Redirection lR(Console::Redirection::What::WHAT_STDOUT);
    {
        lRet = lT.Run();
    }
    lR.Restore();
    KMS_TEST_COMPARE(lRet, 0);

    // ===== DI::Container ==================================================

    // Validate
    lT.Validate();
}

KMS_TEST(CLI_Tool_Exception, "Auto", sTest_Exception)
{
    Test_Tool lT;

    // ExecuteCommand
    try
    {
        lT.ExecuteCommand("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_INVALID_COMMAND");
}

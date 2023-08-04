
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/CLI_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>

using namespace KMS;

class Test_Tool : public CLI::Tool
{
};

// Constants
// //////////////////////////////////////////////////////////////////////////

KMS_TEST(CLI_Tool_Base, "CLI_Tool_Base", "Auto", sTest_Base)
{
    // Constructor
    Test_Tool lT;

    lT.mConsole.Set_Null();

    // AddCommand
    lT.AddCommand("Config Invalid");
    lT.AddCommand("Echo Message");
    lT.AddCommand("ExecuteScript KMS-A-Test/Tests/Test_CLI_TOOL_Base_0.txt");
    lT.AddCommand("Help");
    lT.AddCommand("Repeat 0 Help");
    lT.AddCommand("Repeat 1 Help");
    lT.AddCommand("UntilCtrlC Invalid");
    lT.AddCommand("Exit");

    // ExecuteCommands
    lT.ExecuteCommands(stdout);

    // ExecuteScripts

    // DisplayHelp

    // ExecuteCommand

    // Run
    Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
    lT.Run();
}

KMS_TEST(CLI_Tool_Exception, "CLI_Tool_Exception", "Auto", sTest_Exception)
{
    Test_Tool lT;

    // ExecuteCommand
    try
    {
        Dbg::gLog.SetHideCount(Dbg::LogFile::Level::LEVEL_ERROR, 2);
        lT.ExecuteCommand("Invalid");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH(CLI_COMMAND_INVALID);
}

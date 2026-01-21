
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Script_Script.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Script/Script_Cmd.h>
#include <KMS/Script/Script_PS.h>
#include <KMS/Script/Script_Sh.h>

using namespace KMS;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static bool ExecuteTest(Script::Script& aS);

KMS_TEST(Script_Cmd_Base, "Auto", sTest_Cmd_Base)
{
    Script::Script_Cmd lSC("Test_Script_Cmd_Base");

    KMS_TEST_ASSERT(ExecuteTest(lSC));
}

KMS_TEST(Script_PS_Base, "Auto", sTest_PS_Base)
{
    Script::Script_PS lSPS("Test_Script_PS_Base");

    KMS_TEST_ASSERT(ExecuteTest(lSPS));
}

KMS_TEST(Script_Sh_Base, "Auto", sTest_Sh_Base)
{
    Script::Script_Sh lSS("Test_Script_Sh_Base");

    KMS_TEST_ASSERT(ExecuteTest(lSS));
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

bool ExecuteTest(Script::Script& aS)
{
    auto lSC  = dynamic_cast<Script::Script_Cmd*>(&aS);
    auto lSPS = dynamic_cast<Script::Script_PS *>(&aS);
    auto lSS  = dynamic_cast<Script::Script_Sh *>(&aS);

    // ~Script

    // Write_Comment_Section

    // Write_CreateFolder_IfNeeded
    aS.Write_CreateFolder_IfNeeded("Test_Script__Base");

    // Write_Echo_Executing
    aS.Write_Echo_Executing("Executing");

    // Write_Echo_OK

    // Write_EmptyLine

    // Write_Error

    // Write_Instruction
    aS.Write_Instruction("Do nothing");
    aS.Write_Instruction("Do nothing", Script::Script::FLAG_DO_NOT_PAUSE);
    aS.Write_Instruction("Do nothing", Script::Script::FLAG_PAUSE);

    // Write_Warning
    aS.Write_Warning("Test");
    aS.Write_Warning("Test", true);

    // Write_Verify_Exist
    aS.Write_Verify_Exist("Includes/KMS/Script/Script.h");
    aS.Write_Verify_Exist("Includes\\KMS\\Script\\Script.h");
    aS.Write_Verify_Exist("/Windows");

    // Write_Command
    if ((nullptr != lSC) || (nullptr != lSPS))
    {
        aS.Write_Command("dir .gitignore");
    }

    if (nullptr != lSS)
    {
        aS.Write_Command("ls -l .gitignore");
    }

    // Write_Verify_Result - After executing a command
    if (nullptr != lSC)
    {
        aS.Write_Verify_Result("dir failed", "try again", false);
    }

    // Write_Command

    // Write_Comment

    // Write_CreateFolder

    // Write_Echo
    aS.Write_Echo("/Option");

    // Write_Exit

    // Write_If

    // Write_If_End

    // Write_If_Error

    // Write_If_NotExist

    // Write_Pause

    // Write_Variable_Set
    aS.Write_Variable_Set("GITIGNORE", ".gitignore", Script::Script::FLAG_FILE_NAME);
    aS.Write_Echo("%GITIGNORE%", Script::Script::FLAG_FILE_NAME);
    aS.Write_Echo("Value: %GITIGNORE%");

    return true;
}

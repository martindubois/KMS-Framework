
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Command_PreBuild.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Tool.h"

#include "Tool_Command.h"

using namespace KMS;

KMS_RESULT_STATIC(RESULT_COMMAND_FAILED);

class T_Command final : public Tool
{

public:

    T_Command(const char* aCommand, Phase aPhase);

    // ===== Tool ===========================================================
    virtual void Execute(Phase aPhase, Script::Script* aScript) override;

private:

    String_ASCII mCommand;

    Phase mPhase;

};

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

namespace Tool_Command
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void CreateTools(ToolList* aTools, const DI::Array& aCommands, Phase aPhase)
    {
        for (const auto& lCE : aCommands.mInternal)
        {
            auto lC = dynamic_cast<const DI::String*>(lCE.Get());
            assert(nullptr != lC);

            CreateTool(aTools, lC->Get(), aPhase);
        }
    }

    void CreateTool(ToolList* aTools, const char* aCommand, Phase aPhase)
    {
        assert(nullptr != aTools);

        auto lComp = new T_Command(aCommand, aPhase);

        aTools->Add(lComp);
    }

}

// Public
// //////////////////////////////////////////////////////////////////////////

T_Command::T_Command(const char* aCommand, Phase aPhase)
    : Tool("Command"), mCommand(aCommand), mPhase(aPhase)
{
    assert(nullptr != aCommand);
}

// ===== Tool ===============================================================

// SCRIPT  Execute the command
void T_Command::Execute(Phase aPhase, Script::Script* aScript)
{
    if (mPhase == aPhase)
    {
        auto lCmd = mCommand.c_str();
        assert(nullptr != lCmd);

        auto lRet = system(lCmd);

        KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_COMMAND_FAILED, "The command failed", lCmd);

        if (nullptr != aScript)
        {
            aScript->Write_Command(lCmd);
            aScript->Write_Verify_Result("The command failed");
        }
    }
}

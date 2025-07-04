
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Test_L.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Config.h"
#include "Phase.h"
#include "Tool_Executable.h"
#include "Tool_Make.h"

#include "Comp_Test.h"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define ALLOWED_TIME_ms (1000 * 60 *  5) //  5 minutes

namespace Comp_Test
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateTool_OSDep(ToolList* aTools, const Config& aCfg, const char* aTest, const char* aConfiguration, const char* aProcessor)
    {
        if (!aCfg.GetDoNotTest())
        {
            auto lFolder = Tool_Make::GetBinaryOutDir(aConfiguration, aProcessor);

            auto lTool = Tool_Executable::CreateTool(aTools, aCfg, lFolder, aTest, ALLOWED_TIME_ms, Phase::TEST, Phase::TEST);

            lTool->AddArgument("Groups+=Auto");
        }
    }

}

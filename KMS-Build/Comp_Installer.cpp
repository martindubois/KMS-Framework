
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Installer.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp_Installer.h"

using namespace KMS;

namespace Comp_Installer
{
// Functions
// //////////////////////////////////////////////////////////////////////////

    void CreateComponentsAndTools(CompList* aComps, ToolList* aTools, const Config& aCfg, const DI::Array& aProcessors)
    {
        for (const auto& lPE : aProcessors.mInternal)
        {
            auto lP = dynamic_cast<const DI::String*>(lPE.Get());
            assert(nullptr != lP);

            CreateComponentAndTool_OSDep(aComps, aTools, aCfg, lP->Get());
        }
    }

}

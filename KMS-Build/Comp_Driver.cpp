
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Driver.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp_ReadMe.h"
#include "Config.h"
#include "Comp_Driver.h"

using namespace KMS;

namespace Comp_Driver
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateComponentsAndTools(CompList* aComps, ToolList* aTools, const Config& aCfg, const KMS::DI::Array& aDrivers, const KMS::DI::Array& aConfigurations, const KMS::DI::Array& aProcessors)
    {
        for (const auto& lDE : aDrivers.mInternal)
        {
            auto lD = dynamic_cast<const DI::String*>(lDE.Get());
            assert(nullptr != lD);

            Comp_ReadMe::CreateComponent(aComps, aCfg, lD->Get());

            CreateComponentsAndTools_OSDep(aComps, aTools, aCfg, lD->Get(), aConfigurations, aProcessors);
        }
    }

}

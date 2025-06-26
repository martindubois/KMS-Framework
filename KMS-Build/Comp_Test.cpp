
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Test.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp_Test.h"

using namespace KMS;

namespace Comp_Test
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateTools(ToolList* aTools, const Config& aCfg, const DI::Array& aTests, const DI::Array& aConfigurations, const DI::Array& aProcessors)
    {
        for (const auto& lTE : aTests.mInternal)
        {
            auto lT = dynamic_cast<const DI::String*>(lTE.Get());
            assert(nullptr != lT);

            for (const auto& lCE : aConfigurations.mInternal)
            {
                auto lC = dynamic_cast<const DI::String*>(lCE.Get());
                assert(nullptr != lC);

                for (const auto& lPE : aProcessors.mInternal)
                {
                    auto lP = dynamic_cast<const DI::String*>(lPE.Get());
                    assert(nullptr != lC);
        
                    CreateTool_OSDep(aTools, aCfg, lT->Get(), lC->Get(), lP->Get());
                }
            }
        }
    }

}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Binary.cpp

// TEST COVERAGE 2025-04-11 Martin Dubois

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp_ReadMe.h"
#include "Config.h"

#include "Comp_Binary.h"

using namespace KMS;

namespace Comp_Binary
{

    // Function
    // //////////////////////////////////////////////////////////////////////

    void CreateComponents(CompList* aComps, const Config& aCfg, const DI::Array& aBinaries, const DI::Array& aConfigurations, const DI::Array& aProcessors)
    {
        for (const auto& lBE : aBinaries.mInternal)
        {
            auto lB = dynamic_cast<const DI::String*>(lBE.Get());
            assert(nullptr != lB);

            Comp_ReadMe::CreateComponent(aComps, aCfg, lB->Get());

            for (const auto& lCE : aConfigurations.mInternal)
            {
                auto lC = dynamic_cast<const DI::String*>(lCE.Get());
                assert(nullptr != lC);

                for (const auto& lPE : aProcessors.mInternal)
                {
                    auto lP = dynamic_cast<const DI::String*>(lPE.Get());
                    assert(nullptr != lP);
        
                    CreateComponents_OSDep(aComps, aCfg, lB->Get(), lC->Get(), lP->Get());
                }
            }
        }
    }

}

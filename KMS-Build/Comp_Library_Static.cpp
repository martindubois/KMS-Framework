
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Library_Static.cpp

// TEST COVERAGE 2025-04-11 Martin Dubois

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp_ReadMe.h"
#include "Config.h"

#include "Comp_Library_Static.h"

using namespace KMS;

namespace Comp_Library_Static
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    void CreateComponents(CompList* aComps, const Config& aCfg, const DI::Array& aLibraries, const DI::Array& aConfigurations, const DI::Array& aProcessors)
    {
        for (const auto& lLE : aLibraries.mInternal)
        {
            auto lL = dynamic_cast<const DI::String*>(lLE.Get());
            assert(nullptr != lL);

            Comp_ReadMe::CreateComponent(aComps, aCfg, lL->Get());

            for (const auto& lCE : aConfigurations.mInternal)
            {
                auto lC = dynamic_cast<const DI::String*>(lCE.Get());
                assert(nullptr != lC);

                for (const auto& lPE : aProcessors.mInternal)
                {
                    auto lP = dynamic_cast<const DI::String*>(lPE.Get());
                    assert(nullptr != lC);
        
                    CreateComponents_OSDep(aComps, aCfg, lL->Get(), lC->Get(), lP->Get());
                }
            }
        }
    }

}

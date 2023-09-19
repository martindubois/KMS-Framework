
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Scope_Channel_Random.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Scope/Channel_Random.h>

namespace KMS
{
    namespace Scope
    {
        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Channel ====================================================

        void Channel_Random::Sample(unsigned int* aY_px, bool* aTrig)
        {
            SetSample(static_cast<double>(rand()) / 512.0);

            Channel::Sample(aY_px, aTrig);
        }

        // Public
        // //////////////////////////////////////////////////////////////////

        Channel_Random::Channel_Random() {}

    }
}

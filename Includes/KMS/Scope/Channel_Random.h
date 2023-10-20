
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Scope/Channel_Random.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Scope/Channel.h>

namespace KMS
{
    namespace Scope
    {

        class Channel_Random final : public Channel
        {

        public:

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrigger);

        };

    }
}

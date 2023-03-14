
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Embedded_SPI.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Embedded/Assert.h>

namespace KMS
{
    namespace Embedded
    {

        #ifdef DEBUG

            volatile uint16_t gAssertFailure_Line;

            // Functions
            // //////////////////////////////////////////////////////////////

            void AssertFailure(uint16_t aLine)
            {
                gAssertFailure_Line = aLine;

                while (0 != gAssertFailure_Line) {}
            }

        #endif

    }
}

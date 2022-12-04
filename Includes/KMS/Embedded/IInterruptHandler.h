
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/IInterruptHandler.h

#pragma once

namespace KMS
{
    namespace Embedded
    {

        class IInterruptHandler
        {

        public:

            virtual void OnInterrupt(uint8_t aIndex, uint8_t aLevel) = 0;

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Embedded/Assert.h

#pragma once

#ifdef DEBUG

    namespace KMS
    {
        namespace Embedded
        {

            extern void AssertFailure(uint16_t aLine);

        }
    }

    #define Assert(C) if (!(C)) { KMS::Embedded::AssertFailure(__LINE__); }

#else

    #define Assert(C)

#endif


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/DAQ.h

#pragma once

namespace KMS
{
    namespace DAQ
    {

        typedef double AnalogValue;

        typedef int32_t AnalogValue_Raw;

        typedef uint32_t Count;

        typedef uint8_t Id;

        typedef uint32_t Period;

        static const Count INFINITE;

        static const Period DEFAULT_PULSE_WIDE;

    }
}

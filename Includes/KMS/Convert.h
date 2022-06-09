
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Convert.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Types.h>

namespace KMS
{
    namespace Convert
    {

        extern bool     ToBool  (const char* aValue);
        extern FILE   * ToFile  (const char* aValue, const char * aMode = "rb");
        extern uint16_t ToUInt16(const char* aValue, Radix aRadix = Radix::DECIMAL);
        extern uint32_t ToUInt32(const char* aValue, Radix aRadix = Radix::DECIMAL);
        extern uint8_t  ToUInt8 (const char* aValue, Radix aRadix = Radix::DECIMAL);

    }
}

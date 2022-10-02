
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

        extern bool     ToBool  (const char* aASCII);
        extern double   ToDouble(const char* aASCII);
        extern FILE   * ToFile  (const char* aASCII, const char * aMode = "rb");
        extern uint16_t ToUInt16(const char* aASCII, Radix aRadix = Radix::DECIMAL);
        extern uint32_t ToUInt32(const char* aASCII, Radix aRadix = Radix::DECIMAL);
        extern uint8_t  ToUInt8 (const char* aASCII, Radix aRadix = Radix::DECIMAL);

        extern unsigned int ToASCII(const wchar_t* aUTF16, char   * aOut, unsigned int aOutSize_byte);
        extern unsigned int ToUTF16(const char   * aASCII, wchar_t* aOut, unsigned int aOutSize_byte);

    }
}

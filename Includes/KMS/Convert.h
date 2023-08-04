
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Convert.h
// Status    PROD_READY
// Library   KMS-A

// bool
//      0
//      1
//      false
//      true

// double
//      {9}[.{9}]

// UInt16, UInt32, UInt8
//      0x{9}
//      {9}

#pragma once

// ===== Includes ===========================================================
#include <KMS/Types.h>

namespace KMS
{
    namespace Convert
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        // Exception  CONVERT_FORMAT_INVALID
        extern bool ToBool(const char* aASCII);

        // Exception  CONVERT_FORMAT_INVALID
        extern double ToDouble(const char* aASCII);

        // Exception  CONVERT_OPEN_FAILED
        extern FILE* ToFile(const char* aASCII, const char * aMode = "rb");

        // Exception  CONVERT_DATA_TYPE_INVALID
        extern int32_t ToInt32(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        // Exception  CONVERT_DATA_TYPE_INVALID
        //            CONVERT_VALUE_INVALID
        extern uint16_t ToUInt16(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        // Exception  CONVERT_DATA_TYPE_INVALID
        extern uint32_t ToUInt32(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        // Exception  CONVERT_DATA_TYPE_INVALID
        extern uint8_t ToUInt8 (const char* aASCII, Radix aRadix = Radix::DECIMAL);

        // Exception  CONVERT_OUTPUT_TOO_SHORT
        extern unsigned int ToASCII(const wchar_t* aUTF16, char   * aOut, unsigned int aOutSize_byte);

        // Exception  CONVERT_OUTPUT_TOO_SHORT
        extern unsigned int ToUTF16(const char   * aASCII, wchar_t* aOut, unsigned int aOutSize_byte);

        // Exception  CONVERT_OUTPUT_TOO_SHORT
        extern unsigned int ToDisplay(const char* aASCII, unsigned int aInSize_byte, char* aOut, unsigned aOutSize_byte);

        // Exception  CONVERT_OUTPUT_TOO_SHORT
        //            CONVERT_VALUE_INVALID
        extern unsigned int ToUInt8Array(const char* aASCII, const char* aSeparators, const char* aBlank, uint8_t* aOut, unsigned aOutSize_byte);

    }
}

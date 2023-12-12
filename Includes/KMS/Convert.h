
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

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Types.h>

namespace KMS
{
    namespace Convert
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        // Exception  RESULT_INVALID_FORMAT
        extern bool ToBool(const char* aASCII);

        extern bool ToBool_Try(const char* aASCII, bool* aOut);

        // RESULT_INVALID_VALUE
        uint8_t ToDigitValue(char aC);

        bool ToDigitValue_Try(char aC, uint8_t* aOut);

        // Exception  RESULT_INVALID_FORMAT
        extern double ToDouble(const char* aASCII);

        extern bool ToDouble_Try(const char* aASCII, double* aOut);

        // Exception  RESULT_OPEN_FAILED
        extern FILE* ToFile(const char* aASCII, const char * aMode = "rb");

        // Exception  RESULT_INVALID_VALUE
        extern int16_t ToInt16(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        extern bool ToInt16_Try(const char* aASCII, int16_t* aOut, Radix aRadix = Radix::DECIMAL);

        // Exception  RESULT_INVALID_DATA_TYPE
        extern int32_t ToInt32(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        extern bool ToInt32_Try(const char* aASCII, int32_t* aOut, Radix aRadix = Radix::DECIMAL);

        // Exception  RESULT_INVALID_VALUE
        extern int8_t ToInt8(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        extern bool ToInt8_Try(const char* aASCII, int8_t* aOut, Radix aRadix = Radix::DECIMAL);

        // Exception  RESULT_INVALID_VALUE
        extern uint16_t ToUInt16(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        extern bool ToUInt16_Try(const char* aASCII, uint16_t* aOut, Radix aRadix = Radix::DECIMAL);

        // Exception  RESULT_INVALID_DATA_TYPE
        extern uint32_t ToUInt32(const char* aASCII, Radix aRadix = Radix::DECIMAL);

        extern bool ToUInt32_Try(const char* aASCII, uint32_t* aOut, Radix aRadix = Radix::DECIMAL);

        // Exception  RESULT_INVALID_DATA_TYPE
        extern uint8_t ToUInt8 (const char* aASCII, Radix aRadix = Radix::DECIMAL);

        extern bool ToUInt8_Try(const char* aASCII, uint8_t* aOut, Radix aRadix = Radix::DECIMAL);

        // Exception  CONVERT_OUTPUT_TOO_SHORT
        extern unsigned int ToASCII(const wchar_t* aUTF16, char   * aOut, unsigned int aOutSize_byte);

        // Exception  CONVERT_OUTPUT_TOO_SHORT
        extern unsigned int ToUTF16(const char   * aASCII, wchar_t* aOut, unsigned int aOutSize_byte);

        // Exception  CONVERT_OUTPUT_TOO_SHORT
        extern unsigned int ToDisplay(const char* aASCII, unsigned int aInSize_byte, char* aOut, unsigned aOutSize_byte);

        // Exception  RESULT_INVALID_VALUE
        //            RESULT_OUTPUT_TOO_SHORT
        extern unsigned int ToUInt8Array(const char* aASCII, const char* aSeparators, const char* aBlank, uint8_t* aOut, unsigned aOutSize_byte);

        #ifdef _KMS_WINDOWS_

            ::GUID ToGUID(const char* aIn);

            bool ToGUID_Try(const char* aIn, ::GUID* aOut);

        #endif

    }
}

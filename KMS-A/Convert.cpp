
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Convert.cpp

// TEST COVERAGE 2022-07-30 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

namespace KMS
{
    namespace Convert
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        bool ToBool(const char* aASCII)
        {
            assert(NULL != aASCII);

            if ((0 == _stricmp(aASCII, "true")) || (0 == strcmp(aASCII, "1")))
            {
                return true;
            }

            if ((0 == _stricmp(aASCII, "false")) || (0 == strcmp(aASCII, "0")))
            {
                return false;
            }

            KMS_EXCEPTION(CONVERT_FORMAT, "Invalid boolean value", aASCII);
        }

        double ToDouble(const char* aASCII)
        {
            assert(NULL != aASCII);

            char* lPtr;

            double lResult = strtod(aASCII, &lPtr);

            KMS_EXCEPTION_ASSERT('\0' == *lPtr, CONVERT_FORMAT, "Invalid double format", aASCII);

            return lResult;
        }

        FILE* ToFile(const char* aASCII, const char* aMode)
        {
            assert(NULL != aASCII);
            assert(NULL != aMode);

            if (0 == strcmp(aASCII, "stderr")) { return stderr; }
            if (0 == strcmp(aASCII, "stdin" )) { return stdin ; }
            if (0 == strcmp(aASCII, "stdout")) { return stdout; }

            FILE* lResult;

            int lRet = fopen_s(&lResult, aASCII, aMode);
            KMS_EXCEPTION_ASSERT(0 == lRet, FILE_OPEN, "fopen_s failed", aASCII);

            assert(NULL != lResult);

            return lResult;
        }

        uint16_t ToUInt16(const char* aASCII, Radix aRadix)
        {
            uint32_t lResult = ToUInt32(aASCII, aRadix);

            KMS_EXCEPTION_ASSERT(0xffff >= lResult, CONVERT_TYPE, "The value is to large for the expected type (uint16_t)", aASCII);

            return static_cast<uint16_t>(lResult);
        }

        uint32_t ToUInt32(const char* aASCII, Radix aRadix)
        {
            assert(NULL != aASCII);

            const char* lASCII;
            Radix       lRadix;

            if (0 == strncmp("0x", aASCII, 2))
            {
                lASCII = aASCII + 2;
                lRadix = Radix::HEXADECIMAL;
            }
            else
            {
                lASCII = aASCII;
                lRadix = aRadix;
            }

            char* lPtr;

            unsigned long lResult = strtoul(lASCII, &lPtr, static_cast<int>(lRadix));

            KMS_EXCEPTION_ASSERT('\0' == *lPtr, CONVERT_TYPE, "The value is not of the expected type", aASCII);

            return static_cast<uint32_t>(lResult);
        }

        uint8_t ToUInt8(const char* aASCII, Radix aRadix)
        {
            uint32_t lResult = ToUInt32(aASCII, aRadix);
            KMS_EXCEPTION_ASSERT (0xff >= lResult, CONVERT_TYPE, "The value is to large for the expected type (uint8_t)", aASCII);

            return static_cast<uint8_t>(lResult);
        }

        unsigned int ToASCII(const wchar_t* aUTF16, char* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aUTF16);
            assert(NULL != aOut);

            unsigned int lLen = static_cast<unsigned int>(wcslen(aUTF16));

            KMS_EXCEPTION_ASSERT(aOutSize_byte > lLen, OUTPUT_TOO_SHORT, "The output buffer is too short", lLen);

            unsigned int lResult_byte = 0;

            const wchar_t* lUTF16 = aUTF16;
            while (0 != *lUTF16)
            {
                if (128 > *lUTF16)
                {
                    aOut[lResult_byte] = static_cast<char>(*lUTF16); lResult_byte++;
                }

                lUTF16++;
            }

            aOut[lResult_byte] = '\0';

            return lResult_byte;
        }

        unsigned int ToUTF16(const char* aASCII, wchar_t* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aASCII);
            assert(NULL != aOut);

            unsigned int lLen = static_cast<unsigned int>(strlen(aASCII));

            KMS_EXCEPTION_ASSERT(aOutSize_byte > lLen * sizeof(wchar_t), OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);

            unsigned int lResult_word = 0;

            const char* lASCII = aASCII;
            while ('\0' != *lASCII)
            {
                aOut[lResult_word] = *lASCII; lResult_word++;

                lASCII++;
            }

            aOut[lResult_word] = 0;

            return lResult_word * sizeof(wchar_t);
        }

    }
}

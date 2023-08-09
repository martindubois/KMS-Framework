
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Convert.cpp

// TEST COVERAGE  2023-07-28  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static uint8_t ToDigitValue(char aC);

namespace KMS
{
    namespace Convert
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        bool ToBool(const char* aASCII)
        {
            assert(nullptr != aASCII);

            if ((0 == _stricmp(aASCII, "true")) || (0 == strcmp(aASCII, "1")))
            {
                return true;
            }

            if ((0 == _stricmp(aASCII, "false")) || (0 == strcmp(aASCII, "0")))
            {
                return false;
            }

            KMS_EXCEPTION(CONVERT_FORMAT_INVALID, "Invalid boolean value", aASCII);
        }

        double ToDouble(const char* aASCII)
        {
            assert(nullptr != aASCII);

            char* lPtr;

            auto lResult = strtod(aASCII, &lPtr);

            if ('\0' != *lPtr)
            {
                char lMsg[64 + NAME_LENGTH];
                sprintf_s(lMsg, "\"%s\" is not a valid floating point value", aASCII);
                KMS_EXCEPTION(CONVERT_FORMAT_INVALID, lMsg, "");
            }

            return lResult;
        }

        FILE* ToFile(const char* aASCII, const char* aMode)
        {
            assert(nullptr != aASCII);
            assert(nullptr != aMode);

            if (0 == strcmp(aASCII, "stderr")) { return stderr; }
            if (0 == strcmp(aASCII, "stdin" )) { return stdin ; }
            if (0 == strcmp(aASCII, "stdout")) { return stdout; }

            FILE* lResult;

            auto lRet = fopen_s(&lResult, aASCII, aMode);

            if (0 != lRet)
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open \"%s\"", aASCII);
                KMS_EXCEPTION(CONVERT_OPEN_FAILED, lMsg, lRet);
            }

            assert(nullptr != lResult);

            return lResult;
        }

        int32_t ToInt32(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

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

            auto lResult = strtol(lASCII, &lPtr, static_cast<int>(lRadix));

            if ('\0' != *lPtr)
            {
                char lMsg[64 + NAME_LENGTH];
                sprintf_s(lMsg, "\"%s\" is not a valid integer value", lASCII);
                KMS_EXCEPTION(CONVERT_DATA_TYPE_INVALID, lMsg, "");
            }

            return static_cast<int32_t>(lResult);
        }

        uint16_t ToUInt16(const char* aASCII, Radix aRadix)
        {
            auto lResult = ToUInt32(aASCII, aRadix);

            if (0xffff < lResult)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is too large for the expected type (uint16_t)", lResult);
                KMS_EXCEPTION(CONVERT_VALUE_INVALID, lMsg, aASCII);
            }

            return static_cast<uint16_t>(lResult);
        }

        uint32_t ToUInt32(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

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

            auto lResult = strtoul(lASCII, &lPtr, static_cast<int>(lRadix));

            if ('\0' != *lPtr)
            {
                char lMsg[64 + NAME_LENGTH];
                sprintf_s(lMsg, "\"%s\" is not a valid positive integer value", aASCII);
                KMS_EXCEPTION(CONVERT_DATA_TYPE_INVALID, lMsg, "");
            }

            return static_cast<uint32_t>(lResult);
        }

        uint8_t ToUInt8(const char* aASCII, Radix aRadix)
        {
            auto lResult = ToUInt32(aASCII, aRadix);

            if (0xff < lResult)
            {
                char lMsg[64];
                sprintf_s(lMsg, "%u is too large for the expected type (uint8_t)", lResult);
                KMS_EXCEPTION(CONVERT_DATA_TYPE_INVALID, lMsg, aASCII);
            }

            return static_cast<uint8_t>(lResult);
        }

        unsigned int ToASCII(const wchar_t* aUTF16, char* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aUTF16);
            assert(nullptr != aOut);

            auto lLen = static_cast<unsigned int>(wcslen(aUTF16));

            KMS_EXCEPTION_ASSERT(aOutSize_byte > lLen, CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", lLen);

            unsigned int lResult_byte = 0;

            auto lUTF16 = aUTF16;
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
            assert(nullptr != aASCII);
            assert(nullptr != aOut);

            auto lLen = static_cast<unsigned int>(strlen(aASCII));

            KMS_EXCEPTION_ASSERT(aOutSize_byte > lLen * sizeof(wchar_t), CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);

            unsigned int lResult_word = 0;

            auto lASCII = aASCII;
            while ('\0' != *lASCII)
            {
                aOut[lResult_word] = *lASCII; lResult_word++;

                lASCII++;
            }

            aOut[lResult_word] = 0;

            return lResult_word * sizeof(wchar_t);
        }

        unsigned int ToDisplay(const char* aASCII, unsigned int aInSize_byte, char* aOut, unsigned aOutSize_byte)
        {
            KMS_EXCEPTION_ASSERT(aInSize_byte < aOutSize_byte, CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", aOutSize_byte);

            for (unsigned int i = 0; i < aInSize_byte; i++)
            {
                if ((31 >= aASCII[i]) || (127 <= aASCII[i])) { aOut[i] = '.'; }
                else
                {
                    aOut[i] = aASCII[i];
                }
            }

            aOut[aInSize_byte] = '\0';

            return aInSize_byte;
        }

        // --> HIGH <-------+
        //      |           |
        //      +--> LOW    |
        //            |     |
        //            +--> SEP
        #define STATE_HIGH (0)
        #define STATE_LOW  (1)
        #define STATE_SEP  (2)

        extern unsigned int ToUInt8Array(const char* aASCII, const char* aSeparators, const char* aBlanks, uint8_t* aOut, unsigned aOutSize_byte)
        {
            auto         lASCII       = aASCII;
            uint8_t      lByte;
            unsigned int lResult_byte = 0;
            unsigned int lState       = STATE_HIGH;

            for (;;)
            {
                switch (lState)
                {
                case STATE_HIGH:
                    if ('\0' == *lASCII) { return lResult_byte; }

                    if      (nullptr != strchr(aBlanks, *lASCII)) {}
                    else if (nullptr != strchr(aSeparators, *lASCII))
                    {
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = 0; lResult_byte++;
                    }
                    else
                    {
                        lByte = ToDigitValue(*lASCII);
                        lState = STATE_LOW;
                    }
                    break;

                case STATE_LOW:
                    if ('\0' == *lASCII)
                    {
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        return lResult_byte;
                    }

                    if (nullptr != strchr(aBlanks, *lASCII))
                    {
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        lState = STATE_SEP;
                    }
                    else if (nullptr != strchr(aSeparators, *lASCII))
                    {
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        lState = STATE_HIGH;
                    }
                    else
                    {
                        lByte <<= 4;
                        lByte |= ToDigitValue(*lASCII);
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, CONVERT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        lState = STATE_SEP;
                    }
                    break;

                case STATE_SEP:
                    if ('\0' == *lASCII) { return lResult_byte; }

                    if (nullptr != strchr(aBlanks, *lASCII)) {}
                    else if (nullptr != strchr(aSeparators, *lASCII)) { lState = STATE_HIGH; }
                    else { KMS_EXCEPTION(CONVERT_VALUE_INVALID, "Invalid charactere", aASCII); }
                    break;

                default: assert(false);
                }

                lASCII++;
            }
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

uint8_t ToDigitValue(char aC)
{
    uint8_t lResult;

    switch (aC)
    {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': lResult = aC - '0'; break;

    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F': lResult = aC - 'A' + 10; break;

    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f': lResult = aC - 'a' + 10; break;

    default: KMS_EXCEPTION(CONVERT_VALUE_INVALID, "Invalid digit", aC);
    }

    return lResult;
}

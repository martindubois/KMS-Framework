
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Convert.cpp

// TEST COVERAGE  2023-12-11  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static const char* DetectHex(const char* aASCII, KMS::Radix* aRadix);

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

            bool lResult;

            if (!ToBool_Try(aASCII, &lResult))
            {
                char lMsg[64 + NAME_LENGTH];
                sprintf_s(lMsg, "\"%s\" is not a valid boolean value", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_FORMAT, lMsg, "");
            }

            return lResult;
        }

        bool ToBool_Try(const char* aASCII, bool* aOut)
        {
            assert(nullptr != aASCII);
            assert(nullptr != aOut);

            auto lResult = false;

            if ((0 == _stricmp(aASCII, "true")) || (0 == strcmp(aASCII, "1")))
            {
                *aOut = true;
                lResult = true;
            }
            else if ((0 == _stricmp(aASCII, "false")) || (0 == strcmp(aASCII, "0")))
            {
                *aOut = false;
                lResult = true;
            }

            return lResult;
        }

        uint8_t ToDigitValue(char aC)
        {
            uint8_t lResult;

            if (!ToDigitValue_Try(aC, &lResult))
            {
                KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid digit", aC);
            }

            return lResult;
        }

        bool ToDigitValue_Try(char aC, uint8_t* aOut)
        {
            bool lResult = true;

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
            case '9': *aOut = aC - '0'; break;

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F': *aOut = aC - 'A' + 10; break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f': *aOut = aC - 'a' + 10; break;

            default: lResult = false;
            }

            return lResult;
        }

        double ToDouble(const char* aASCII)
        {
            assert(nullptr != aASCII);

            double lResult;

            if (!ToDouble_Try(aASCII, &lResult))
            {
                char lMsg[64 + NAME_LENGTH];
                sprintf_s(lMsg, "\"%s\" is not a valid floating point value", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_FORMAT, lMsg, "");
            }

            return lResult;
        }

        bool ToDouble_Try(const char* aASCII, double* aOut)
        {
            assert(nullptr != aASCII);
            assert(nullptr != aOut);

            char* lPtr;

            auto lOut = strtod(aASCII, &lPtr);

            auto lResult = ('\0' == *lPtr);
            if (lResult)
            {
                *aOut = lOut;
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
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, lRet);
            }

            assert(nullptr != lResult);

            return lResult;
        }

        int16_t ToInt16(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

            int16_t lResult;

            if (!ToInt16_Try(aASCII, &lResult, aRadix))
            {
                char lMsg[64];
                sprintf_s(lMsg, "\"%s\" is not valid for the expected type (int16_t)", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_VALUE, lMsg, "");
            }

            return lResult;
        }

        bool ToInt16_Try(const char* aASCII, int16_t* aOut, Radix aRadix)
        {
            assert(nullptr != aOut);

            int32_t lOut;

            auto lResult = ToInt32_Try(aASCII, &lOut, aRadix);
            if (lResult)
            {
                lResult = (0x7fff >= lOut) && (-32768 <= lOut);
                if (lResult)
                {
                    *aOut = lOut;
                }
            }

            return lResult;
        }

        int32_t ToInt32(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

            int32_t lResult;

            if (!ToInt32_Try(aASCII, &lResult, aRadix))
            {
                char lMsg[64 + NAME_LENGTH];
                sprintf_s(lMsg, "\"%s\" is not a valid integer value", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_DATA_TYPE, lMsg, "");
            }

            return lResult;
        }

        bool ToInt32_Try(const char* aASCII, int32_t* aOut, Radix aRadix)
        {
            assert(nullptr != aASCII);
            assert(nullptr != aOut);

            auto lRadix = aRadix;

            auto lASCII = DetectHex(aASCII, &lRadix);

            char* lPtr;

            auto lOut = strtol(lASCII, &lPtr, static_cast<int>(lRadix));

            auto lResult = ('\0' == *lPtr);
            if (lResult)
            {
                *aOut = lOut;
            }

            return lResult;
        }

        int8_t ToInt8(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

            int8_t lResult;

            if (!ToInt8_Try(aASCII, &lResult, aRadix))
            {
                char lMsg[64];
                sprintf_s(lMsg, "\"%s\" is not valid for the expected type (int8_t)", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_VALUE, lMsg, aASCII);
            }

            return lResult;
        }

        bool ToInt8_Try(const char* aASCII, int8_t* aOut, Radix aRadix)
        {
            assert(nullptr != aOut);

            int32_t lOut;

            auto lResult = ToInt32_Try(aASCII, &lOut, aRadix);
            if (lResult)
            {
                lResult = (0x7f >= lOut) && (-128 <= lOut);
                if (lResult)
                {
                    *aOut = lOut;
                }
            }

            return lResult;
        }

        uint16_t ToUInt16(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

            uint16_t lResult;

            if (!ToUInt16_Try(aASCII, &lResult, aRadix))
            {
                char lMsg[64];
                sprintf_s(lMsg, "\"%s\" is not valid for the expected type (uint16_t)", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_VALUE, lMsg, "");
            }

            return lResult;
        }

        bool ToUInt16_Try(const char* aASCII, uint16_t* aOut, Radix aRadix)
        {
            assert(nullptr != aOut);

            uint32_t lOut;

            auto lResult = ToUInt32_Try(aASCII, &lOut, aRadix);
            if (lResult)
            {
                lResult = 0xffff >= lOut;
                if (lResult)
                {
                    *aOut = lOut;
                }
            }

            return lResult;
        }

        uint32_t ToUInt32(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

            uint32_t lResult;

            if (!ToUInt32_Try(aASCII, &lResult, aRadix))
            {
                char lMsg[64 + NAME_LENGTH];
                sprintf_s(lMsg, "\"%s\" is not a valid positive integer value", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_DATA_TYPE, lMsg, "");
            }

            return lResult;
        }

        bool ToUInt32_Try(const char* aASCII, uint32_t* aOut, Radix aRadix)
        {
            assert(nullptr != aASCII);
            assert(nullptr != aOut);

            auto lRadix = aRadix;

            auto lASCII = DetectHex(aASCII, &lRadix);

            char* lPtr;

            auto lOut = strtoul(lASCII, &lPtr, static_cast<int>(lRadix));

            auto lResult = '\0' == *lPtr;
            if (lResult)
            {
                *aOut = lOut;
            }

            return lResult;
        }

        uint8_t ToUInt8(const char* aASCII, Radix aRadix)
        {
            assert(nullptr != aASCII);

            uint8_t lResult;

            if (!ToUInt8_Try(aASCII, &lResult, aRadix))
            {
                char lMsg[64];
                sprintf_s(lMsg, "\"%s\" is not valid for the expected type (uint8_t)", aASCII);
                KMS_EXCEPTION(RESULT_INVALID_DATA_TYPE, lMsg, "");
            }

            return lResult;
        }

        bool ToUInt8_Try(const char* aASCII, uint8_t* aOut, Radix aRadix)
        {
            assert(nullptr != aOut);

            uint32_t lOut;

            auto lResult = ToUInt32_Try(aASCII, &lOut, aRadix);
            if (lResult)
            {
                lResult = 0xff >= lOut;
                if (lResult)
                {
                    *aOut = lOut;
                }
            }

            return lResult;
        }

        unsigned int ToASCII(const wchar_t* aUTF16, char* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aUTF16);
            assert(nullptr != aOut);

            auto lLen = static_cast<unsigned int>(wcslen(aUTF16));

            KMS_EXCEPTION_ASSERT(aOutSize_byte > lLen, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", lLen);

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

            KMS_EXCEPTION_ASSERT(aOutSize_byte > lLen * sizeof(wchar_t), RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);

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
            KMS_EXCEPTION_ASSERT(aInSize_byte < aOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aOutSize_byte);

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
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
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
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        return lResult_byte;
                    }

                    if (nullptr != strchr(aBlanks, *lASCII))
                    {
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        lState = STATE_SEP;
                    }
                    else if (nullptr != strchr(aSeparators, *lASCII))
                    {
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        lState = STATE_HIGH;
                    }
                    else
                    {
                        lByte <<= 4;
                        lByte |= ToDigitValue(*lASCII);
                        KMS_EXCEPTION_ASSERT(lResult_byte < aOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aASCII);
                        aOut[lResult_byte] = lByte; lResult_byte++;
                        lState = STATE_SEP;
                    }
                    break;

                case STATE_SEP:
                    if ('\0' == *lASCII) { return lResult_byte; }

                    if (nullptr != strchr(aBlanks, *lASCII)) {}
                    else if (nullptr != strchr(aSeparators, *lASCII)) { lState = STATE_HIGH; }
                    else { KMS_EXCEPTION(RESULT_INVALID_VALUE, "Invalid charactere", aASCII); }
                    break;

                default: assert(false);
                }

                lASCII++;
            }
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

const char* DetectHex(const char* aASCII, Radix* aRadix)
{
    assert(nullptr != aASCII);
    assert(nullptr != aRadix);

    const char* lResult;

    if (0 == strncmp("0x", aASCII, 2))
    {
        lResult = aASCII + 2;
        *aRadix = Radix::HEXADECIMAL;
    }
    else
    {
        lResult = aASCII;
    }

    return lResult;
}

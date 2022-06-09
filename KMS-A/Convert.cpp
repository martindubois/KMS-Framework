
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Convert.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

namespace KMS
{
    namespace Convert
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        FILE* ToFile(const char* aValue, const char* aMode)
        {
            assert(NULL != aValue);
            assert(NULL != aMode);

            if (0 == strcmp(aValue, "stderr")) { return stderr; }
            if (0 == strcmp(aValue, "stdin" )) { return stdin ; }
            if (0 == strcmp(aValue, "stdout")) { return stdout; }

            FILE* lResult;

            int lRet = fopen_s(&lResult, aValue, aMode);
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FILE_OPEN, "fopen_s failed", aValue);
            }

            assert(NULL != lResult);

            return lResult;
        }

        bool ToBool(const char* aValue)
        {
            assert(NULL != aValue);

            if ((0 == _stricmp(aValue, "true")) || (0 == strcmp(aValue, "1")))
            {
                return true;
            }

            if ((0 == _stricmp(aValue, "false")) || (0 == strcmp(aValue, "0")))
            {
                return false;
            }

            KMS_EXCEPTION_WITH_INFO(CONVERT_FORMAT, "Invalid boolean value", aValue);
        }

        uint16_t ToUInt16(const char* aValue, Radix aRadix)
        {
            uint32_t lResult = ToUInt32(aValue, aRadix);
            if (0x10000 <= lResult)
            {
                KMS_EXCEPTION_WITH_INFO(CONVERT_TYPE, "The value is to large for the expected type (uint16_t)", aValue);
            }

            return static_cast<uint16_t>(lResult);
        }

        uint32_t ToUInt32(const char* aValue, Radix aRadix)
        {
            assert(NULL != aValue);

            char* lPtr;

            unsigned long lResult = strtoul(aValue, &lPtr, static_cast<int>(aRadix));

            if ('\0' != *lPtr)
            {
                KMS_EXCEPTION_WITH_INFO(CONVERT_TYPE, "The value is not of the expected type", aValue);
            }

            return static_cast<uint32_t>(lResult);
        }

        uint8_t ToUInt8(const char* aValue, Radix aRadix)
        {
            uint32_t lResult = ToUInt32(aValue, aRadix);
            if (0x100 <= lResult)
            {
                KMS_EXCEPTION_WITH_INFO(CONVERT_TYPE, "The value is to large for the expected type (uint8_t)", aValue);
            }

            return static_cast<uint8_t>(lResult);
        }

    }
}

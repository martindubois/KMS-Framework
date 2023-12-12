
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Convert_W.cpp

// TEST COVERAGE  2023-12-11  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Convert.h>

namespace KMS
{
    namespace Convert
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        GUID ToGUID(const char* aIn)
        {
            GUID lResult;

            if (!ToGUID_Try(aIn, &lResult))
            {
                KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid GUID format", aIn);
            }

            return lResult;
        }

        bool ToGUID_Try(const char* aIn, ::GUID* aOut)
        {
            assert(nullptr != aIn);

            memset(aOut, 0, sizeof(*aOut));

            // {00000000-0000-0000-0000-000000000000}
            // 01234567890123456789012345678901234567
            //           1         2         3

            uint32_t lValue = 0;

            for (unsigned int i = 1; i <= 36; i++)
            {
                lValue <<= 8;

                uint8_t lValue2;

                if (ToDigitValue_Try(aIn[i], &lValue2))
                {
                    lValue |= lValue2;
                }
                else
                {
                    if ('-' != aIn[i])
                    {
                        return false;
                    }
                }

                switch (i)
                {
                case  8: aOut->Data1 = lValue; lValue = 0; break;
                case 13: aOut->Data2 = lValue; lValue = 0; break;
                case 18: aOut->Data3 = lValue; lValue = 0; break;

                case 21:
                case 23: aOut->Data4[(i - 21) / 2] = lValue; lValue = 0; break;

                case 26:
                case 28:
                case 30:
                case 32:
                case 34:
                case 36: aOut->Data4[2 + (i - 26) / 2] = lValue; lValue = 0; break;
                }
            }

            return true;
        }

    }
}

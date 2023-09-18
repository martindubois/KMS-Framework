
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Saturated.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Saturated.h>

namespace KMS
{
    namespace Saturated
    {

        uint8_t Add(uint8_t aA, uint8_t aB)
        {
            unsigned int lResult = aA;

            lResult += aB;

            if (0xff < lResult)
            {
                lResult = 0xff;
            }

            return static_cast<uint8_t>(lResult);
        }

        uint8_t Div(uint8_t aA, uint8_t aB)
        {
            uint8_t lResult;

            if (0 >= aB)
            {
                lResult = 0xff;
            }
            else
            {
                lResult = aA / aB;
            }

            return lResult;
        }

        uint8_t Mul(uint8_t aA, uint8_t aB)
        {
            unsigned int lResult = aA;

            lResult *= aB;

            if (0xff < lResult)
            {
                lResult = 0xff;
            }

            return static_cast<uint8_t>(lResult);
        }

        uint8_t Sub(uint8_t aA, uint8_t aB)
        {
            uint8_t lResult;

            if (aA <= aB)
            {
                lResult = 0;
            }
            else
            {
                lResult = aA - aB;
            }

            return lResult;
        }

    }
}

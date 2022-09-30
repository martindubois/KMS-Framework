
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_GUID.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/DI/GUID.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////

static ::GUID ToGUID(const char* aIn);

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        GUID::GUID() { memset(&mInternal, 0, sizeof(mInternal)); }

        void GUID::Set(const ::_GUID& aIn) { mInternal = aIn; }

        // ===== Value ======================================================
        
        void GUID::Set(const char* aIn) { mInternal = ToGUID(aIn); String::Set(aIn); }

        // ===== Object =====================================================

        GUID::~GUID() {}

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////

GUID ToGUID(const char* aIn)
{
    assert(NULL != aIn);

    GUID lResult;

    memset(&lResult, 0, sizeof(lResult));

    // {00000000-0000-0000-0000-000000000000}
    // 01234567890123456789012345678901234567

    uint32_t lValue = 0;

    for (unsigned int i = 1; i <= 36; i++)
    {
        lValue <<= 8;

        switch (aIn[i])
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
        case '9': lValue |= aIn[i] - '0'; break;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f': lValue |= aIn[i] - 'a' + 10; break;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F': lValue |= aIn[i] - 'A' + 10; break;

        case '-': break;

        default: KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid GUID format", aIn);
        }

        switch (i)
        {
        case  8: lResult.Data1 = lValue; lValue = 0; break;
        case 13: lResult.Data2 = lValue; lValue = 0; break;
        case 18: lResult.Data3 = lValue; lValue = 0; break;

        case 21:
        case 23: lResult.Data4[(i - 21) / 2] = lValue; lValue = 0; break;

        case 26:
        case 28:
        case 30:
        case 32:
        case 34:
        case 36: lResult.Data4[2 + (i - 26) / 2] = lValue; lValue = 0; break;
        }
    }

    return lResult;
}

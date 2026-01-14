
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_String_Expand.cpp

// TEST COVERAGE 2023-08-30 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Environment.h>

#include <KMS/DI/String_Expand.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Object* String_Expand::Create() { return new String_Expand; }

        String_Expand_Ptr::String_Expand_Ptr(String_ASCII* aPtr) : String_Ptr(aPtr) {}

        String_Expand::String_Expand(const char* aIn)
        {
            Set(aIn);
        }

        // ===== String =====================================================

        void String_Expand::operator = (const char* aIn)
        {
            char lStr[LINE_LENGTH];

            Env::Expand(aIn, lStr, sizeof(lStr));

            String::Set(lStr);
        }

        // ===== String_Ptr =================================================

        void String_Expand_Ptr::operator = (const char* aIn)
        {
            char lStr[LINE_LENGTH];

            Env::Expand(aIn, lStr, sizeof(lStr));

            String_Ptr::Set(lStr);
        }

        // ===== Value ======================================================

        void String_Expand_Ptr::Set(const char* aIn)
        {
            char lStr[LINE_LENGTH];

            Env::Expand(aIn, lStr, sizeof(lStr));

            String_Ptr::Set(lStr);
        }

        void String_Expand::Set(const char* aIn)
        {
            char lStr[LINE_LENGTH];

            Env::Expand(aIn, lStr, sizeof(lStr));

            String::Set(lStr);
        }

    }
}

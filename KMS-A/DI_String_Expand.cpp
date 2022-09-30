
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_String_Expand.cpp

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

        String_Expand::String_Expand(const char* aIn) : String(aIn) {}

        // ===== Value ======================================================

        void String_Expand::Set(const char* aIn)
        {
            char lStr[LINE_LENGTH];

            Env::Expand(aIn, lStr, sizeof(lStr));

            String::Set(lStr);
        }

        // ===== Object =====================================================

        String_Expand::~String_Expand() {}

    }
}

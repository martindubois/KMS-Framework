
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/String_Expand.h
// Status    PROD_READY (Windows)
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/String.h>

namespace KMS
{
    namespace DI
    {

        class String_Expand : public String
        {

        public:

            static DI::Object* Create();

            // Exception  ENV_EXPAND_FAILED
            String_Expand(const char* aIn = "");

            // ===== Value ==================================================
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual ~String_Expand();

        };

    }
}

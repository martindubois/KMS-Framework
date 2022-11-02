
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/GUID.h
// Library   KMS-A

#pragma once

// ===== C ==================================================================
#include <stdio.h>

// ===== Includes ===========================================================
#include <KMS/DI/String.h>

namespace KMS
{
    namespace DI
    {

        class GUID : public String
        {

        public:

            static DI::Object* Create();

            GUID();

            GUID(const ::GUID& aG, const char* aIn);

            void Set(const ::GUID& aIn);

            // ===== Object =================================================
            virtual ~GUID();

        // Internal

            // ===== Object =================================================
            virtual void Send_OnChanged(void* aData);

            ::GUID mInternal;

        };

    }
}

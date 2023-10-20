
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/GUID.h
// Library   KMS-A

// External type : GUID (const char*)
// Internal type : GUID (std::string)

#pragma once

// ===== C ==================================================================
#include <stdio.h>

// ===== Includes ===========================================================
#include <KMS/DI/String.h>

namespace KMS
{
    namespace DI
    {

        class GUID_Ptr : public Value
        {

        public:

            static const ::GUID DEFAULT_VALUE;

            GUID_Ptr(::GUID* aPtr);

            void operator = (const ::GUID& aIn);

            operator const ::GUID& () const;

            bool IsValid() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual bool Clear();

        private:

            GUID* mPtr;

        };

        class GUID final : public GUID_Ptr
        {

        public:

            static DI::Object* Create();

            GUID(const ::GUID& aG = DEFAULT_VALUE);

            // ===== GUID_Ptr ===============================================
            using GUID_Ptr::operator =;
            using GUID_Ptr::operator const ::GUID&;

        private:
        
            ::GUID mInternal;

        };

    }
}

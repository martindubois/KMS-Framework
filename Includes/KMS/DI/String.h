
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/String.h
// Status    PROD_READY
// Library   KMS-A

// External type : const char*
// Internal type : String_ASCII

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/String.h>

namespace KMS
{
    namespace DI
    {

        class String_Ptr : public Value
        {

        public:

            static const char* DEFAULT_VALUE;

            String_Ptr(String_ASCII* aPtr);

            void operator = (const char* aIn);

            operator const char* () const;

            bool operator == (const char* aIn) const;

            const char* Get() const;

            unsigned int GetLength() const;

            const String_ASCII& GetString() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);
            virtual bool         Set_Try(const char* aIn);

            // ===== Object =================================================
            virtual bool Clear();

        private:

            String_ASCII* mPtr;

        };

        class String : public String_Ptr
        {

        public:

            static DI::Object* Create();

            String(const char* aIn = "");

            // ===== String_Ptr =============================================
            using String_Ptr::operator =;
            using String_Ptr::operator const char*;
            using String_Ptr::operator ==;

        private:

            String_ASCII mInternal;

        };

    }
}

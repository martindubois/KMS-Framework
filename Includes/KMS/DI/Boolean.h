
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Boolean.h
// Status    PROD_READY
// Library   KMS-A

// External type : bool
// Internal type : bool

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>

namespace KMS
{
    namespace DI
    {

        class Boolean_Ptr : public Value
        {

        public:

            static const bool DEFAULT_VALUE;

            Boolean_Ptr(bool* aPtr);

            void operator = (bool aIn);

            operator bool () const;

            bool Get() const;

            void Set(bool aIn = true);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual bool Clear();

        private:

            bool* mPtr;

        };

        class Boolean final : public Boolean_Ptr
        {

        public:

            static DI::Object* Create();

            Boolean(bool aIn = DEFAULT_VALUE);

            // ===== Boolean_Ptr ============================================
            using Boolean_Ptr::operator =;
            using Boolean_Ptr::operator bool;

        private:

            bool mInternal;

        };

    }
}

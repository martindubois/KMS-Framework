
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/String.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>

namespace KMS
{
    namespace DI
    {

        class String : public Value
        {

        public:

            static DI::Object* Create();

            String(const char* aIn = "");

            void operator = (const char* aIn);

            operator const char* () const;

            bool operator == (const char* aIn) const;

            const char* Get() const;

            unsigned int GetLength() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~String();
            virtual void Clear();

        // Internal

            std::string mInternal;

        };

    }
}

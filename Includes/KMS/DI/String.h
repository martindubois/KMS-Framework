
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/String.h

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

            String(const char* aIn = "", const MetaData* aMD = NULL);

            const String& operator = (const char* aIn);

            operator const char* () const;

            void operator += (const char* aIn);

            bool operator == (const char* aIn);

            void Clear();

            unsigned int GetLength() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual ~String();

        private:

            std::string mInternal;

        };

    }
}

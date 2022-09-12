
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/String.h

#pragma once

// ===== C ==================================================================
#include <string>

// ===== Includes ===========================================================
#include <KMS/JSON/Object.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace JSON
    {

        class String : public Object
        {

        public:

            String();

            String(const String& aV);

            String(const char* aValue);

            const String& operator = (const char* aValue);

            const char* Get() const;

            void Get(char* aOut, unsigned int aOutSize_byte) const;

            void GetWithDefault(char* aOut, unsigned int aOutSize_byte, const char* aDefault) const;

            void Set(const char* aValue);

            // ===== Object =================================================

            virtual ~String();

            virtual void Clear();

            virtual bool IsEmpty() const;

            virtual unsigned int HTTP_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int HTTP_Set(const char* aIn , unsigned int aInSize_byte);

            virtual unsigned int JSON_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int JSON_Set(const char* aIn , unsigned int aInSize_byte);

        private:

            std::string mValue;

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/Dictionary.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

namespace KMS
{
    namespace JSON
    {

        class Base
        {

        public:

            virtual ~Base();

            virtual void Clear() = 0;

            virtual Base* Copy() const = 0;

            virtual bool IsEmpty() const = 0;

            virtual unsigned int HTTP_Get(      char* aOut, unsigned int aOutSize_byte) const = 0;
            virtual unsigned int HTTP_Set(const char* aIn , unsigned int aInSize_byte) = 0;

            virtual unsigned int JSON_Get(      char* aOut, unsigned int aOutSize_byte) const = 0;
            virtual unsigned int JSON_Set(const char* aIn , unsigned int aInSize_byte) = 0;

        protected:

            Base();

        private:

            Base(const Base&);

            const Base & operator == (const Base&);

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Base& aD);

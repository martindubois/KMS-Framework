
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/Object.h

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/RefCount.h>

namespace KMS
{
    namespace JSON
    {

        class Object : public RefCount
        {

        public:

            virtual void Clear() = 0;

            virtual bool IsEmpty() const = 0;

            virtual unsigned int HTTP_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int HTTP_Set(const char* aIn , unsigned int aInSize_byte);

            virtual unsigned int JSON_Get(      char* aOut, unsigned int aOutSize_byte) const = 0;
            virtual unsigned int JSON_Set(const char* aIn , unsigned int aInSize_byte) = 0;

            // ===== RefCount ===============================================
            virtual ~Object();

        protected:

            Object();

        private:

            Object(const Object&);

            const Object & operator == (const Object&);

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Object& aO);

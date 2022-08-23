
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/Array.h

#pragma once

// ===== C ==================================================================
#include <ostream>
#include <vector>

// ===== Includes ===========================================================
#include <KMS/JSON/Base.h>

namespace KMS
{
    namespace JSON
    {

        class Array : public Base
        {

        public:

            Array();

            Array(const Array& aA);

            const Array& operator = (const Array& aA);

            void operator += (const Array& aA);

            void operator += (Base* aE);

            Base* GetEntry(unsigned int aIndex);

            const Base* GetEntry(unsigned int aIndex) const;

            void RemoveEntry(unsigned int aIndex);

            void SetEntry(unsigned int aIndex, Base* aE);

            // ===== Base ===================================================

            virtual ~Array();

            virtual void Clear();

            virtual Base* Copy() const;

            virtual bool IsEmpty() const;

            virtual unsigned int HTTP_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int HTTP_Set(const char* aIn , unsigned int aInSize_byte);

            virtual unsigned int JSON_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int JSON_Set(const char* aIn , unsigned int aInSize_byte);

        // Internal

            typedef std::vector<Base *> Internal;

            const Internal& GetInternal() const;

        private:

            Internal mEntries;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Array& aA);

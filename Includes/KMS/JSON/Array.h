
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
#include <KMS/JSON/Object.h>

namespace KMS
{
    namespace JSON
    {

        class Array : public Object
        {

        public:

            Array();

            Array(const Array& aA);

            const Array& operator = (const Array& aA);

            void operator += (const Array& aA);

            void operator += (Object* aE);

            Object* GetEntry(unsigned int aIndex);

            const Object* GetEntry(unsigned int aIndex) const;

            void RemoveEntry(unsigned int aIndex);

            void SetEntry(unsigned int aIndex, Object* aE);

            // ===== Object =================================================

            virtual ~Array();

            virtual void Clear();

            virtual Object* Copy() const;

            virtual bool IsEmpty() const;

            virtual unsigned int JSON_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int JSON_Set(const char* aIn , unsigned int aInSize_byte);

        // Internal

            typedef std::vector<Object *> Internal;

            const Internal& GetInternal() const;

        private:

            Internal mEntries;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Array& aA);

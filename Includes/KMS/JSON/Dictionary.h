
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/Dictionary.h

#pragma once

// ===== C++ ================================================================
#include <map>
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/JSON/Object.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace JSON
    {

        class Dictionary : public Object
        {

        public:

            Dictionary();

            Dictionary(const Dictionary& aD);

            const Dictionary& operator = (const Dictionary& aD);

            void operator += (const Dictionary& aD);

            const Object* GetEntry(const char* aKey) const;

            bool IsEntryPresent(const char* aKey) const;

            void RemoveEntry(const char* aKey);

            void SetEntry(const char* aKey, Object *aE);

            // ===== Object =================================================

            virtual ~Dictionary();

            virtual void Clear();

            virtual Object* Copy() const;

            virtual bool IsEmpty() const;

            virtual unsigned int HTTP_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int HTTP_Set(const char* aIn , unsigned int aInSize_byte);

            virtual unsigned int JSON_Get(      char* aOut, unsigned int aOutSize_byte) const;
            virtual unsigned int JSON_Set(const char* aIn , unsigned int aInSize_byte);

        // Internal

            typedef std::map<std::string, Object*> Internal;

            const Internal& GetInternal() const;

        private:

            Internal mEntries;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::JSON::Dictionary& aD);
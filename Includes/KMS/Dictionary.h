
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dictionary.h

#pragma once

// ===== C++ ================================================================
#include <ostream>
#include <map>

// ===== Includes ===========================================================
#include <KMS/Types.h>

namespace KMS
{

    class Dictionary
    {

    public:

        void Clear();

        void Get(const char* aKey, char    * aOut, unsigned int aOutSize_byte) const;
        void Get(const char* aKey, double  * aOut) const;
        void Get(const char* aKey,  int32_t* aOut, Radix aRadix = Radix::DECIMAL) const;
        void Get(const char* aKey, uint32_t* aOut, Radix aRadix = Radix::DECIMAL) const;

        void GetWithDefault(const char* aKey, char    * aOut, unsigned int aOutSize_byte, const char* aDefault) const;
        void GetWithDefault(const char* aKey, double  * aOut, double   aDefault) const;
        void GetWithDefault(const char* aKey,  int32_t* aOut,  int32_t aDefault, Radix aRadix = Radix::DECIMAL) const;
        void GetWithDefault(const char* aKey, uint32_t* aOut, uint32_t aDefault, Radix aRadix = Radix::DECIMAL) const;

        bool IsEmpty() const;

        bool IsPresent(const char* aKey) const;

        void Remove(const char* aKey);

        void Set(const char* aKey, const char* aValue);
        void Set(const char* aKey, double      aValue);
        void Set(const char* aKey,  int32_t    aValue, Radix aRadix = Radix::DECIMAL);
        void Set(const char* aKey, uint32_t    aValue, Radix aRadix = Radix::DECIMAL);

        unsigned int HTTP_Get(char* aOut, unsigned int aOutSize_byte);
        void         HTTP_Set(const char* aHTTP);

        unsigned int JSON_Get(char* aOut, unsigned int aOutSize_byte);
        void         JSON_Set(const char* aJSON);

    // Internal

        typedef std::map<std::string, std::string> Internal;

        const Internal& GetInternal() const;

    private:

        Internal mEntries;

    };

}

std::ostream& operator << (std::ostream& aOut, const KMS::Dictionary& aD);

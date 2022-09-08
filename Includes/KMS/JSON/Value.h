
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/JSON/Value.h

#pragma once

// ===== C ==================================================================
#include <ostream>
#include <string>

// ===== Includes ===========================================================
#include <KMS/JSON/Object.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace JSON
    {

        class Value : public Object
        {

        public:

            Value();

            Value(const Value& aV);

            Value(const char* aValue);
            Value(double      aValue);
            Value( int32_t    aValue, Radix aRadix = Radix::DECIMAL);
            Value(uint32_t    aValue, Radix aRadix = Radix::DECIMAL);

            const char* Get() const;

            void Get(char    * aOut, unsigned int aOutSize_byte) const;
            void Get(double  * aOut) const;
            void Get( int32_t* aOut, Radix aRadix = Radix::DECIMAL) const;
            void Get(uint32_t* aOut, Radix aRadix = Radix::DECIMAL) const;

            void GetWithDefault(char    * aOut, unsigned int aOutSize_byte, const char* aDefault) const;
            void GetWithDefault(double  * aOut, double   aDefault) const;
            void GetWithDefault( int32_t* aOut,  int32_t aDefault, Radix aRadix = Radix::DECIMAL) const;
            void GetWithDefault(uint32_t* aOut, uint32_t aDefault, Radix aRadix = Radix::DECIMAL) const;

            void Set(const char* aValue);
            void Set(double      aValue);
            void Set( int32_t    aValue, Radix aRadix = Radix::DECIMAL);
            void Set(uint32_t    aValue, Radix aRadix = Radix::DECIMAL);

            // ===== Object =================================================

            virtual ~Value();

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

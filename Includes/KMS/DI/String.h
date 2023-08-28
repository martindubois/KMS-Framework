
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/String.h
// Status    PROD_READY
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

        class String_Base : public Value
        {

        public:

            void operator = (const char* aIn);

            bool operator == (const char* aIn) const;

            operator const char* () const;

            const char* Get() const;

            unsigned int GetLength() const;

            const std::string& GetString() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~String_Base();
            virtual bool Clear();

        protected:

            String_Base();

            virtual const std::string* Internal_Get() const = 0;
            virtual       std::string* Internal_Get()       = 0;

        };

        class String : public String_Base
        {

        public:

            static DI::Object* Create();

            String(const char* aIn = "");

            // ===== Object =================================================
            virtual ~String();

        protected:

            // ===== String_Base ============================================
            virtual const std::string* Internal_Get() const;
            virtual       std::string* Internal_Get();

        private:

            std::string mInternal;

        };

        class String_Ptr : public String_Base
        {

        public:

            String_Ptr(std::string* aString);

            // ===== Object =================================================
            virtual ~String_Ptr();

        protected:
        
            // ===== String_Base ============================================
            virtual const std::string* Internal_Get() const;
            virtual       std::string* Internal_Get();

        private:

            std::string* mInternal;

        };

    }
}

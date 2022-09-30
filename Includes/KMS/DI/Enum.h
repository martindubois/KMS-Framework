
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Boolean.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/Exception.h>

namespace KMS
{
    namespace DI
    {

        template <typename T, const char** N>
        class Enum : public Value
        {

        public:

            static DI::Object* Create();

            Enum(T aIn);

            void operator = (T aIn);

            operator T () const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Enum();

        private:

            T mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T, const char** N>
        inline Enum<T, N>::Enum(T aIn) : mInternal(aIn) {}

        template <typename T, const char** N>
        inline void Enum<T, N>::operator = (T aIn) { mInternal = aIn; }
        
        template <typename T, const char** N>
        Enum<T, N>::operator T () const { return mInternal; }

        // ===== Value ======================================================

        template <typename T, const char** N>
        unsigned int Enum<T, N>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            unsigned int lResult_byte = static_cast<unsigned int>(strlen(N[static_cast<unsigned int>(mInternal)]));

            if (aOutSize_byte <= lResult_byte + 1)
            {
                throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::OUTPUT_TOO_SHORT, "The output buffer is too short");
            }

            strcpy_s(aOut SizeInfoV(aOutSize_byte), N[static_cast<unsigned int>(mInternal)]);

            return lResult_byte;
        }

        template <typename T, const char** N>
        void Enum<T, N>::Set(const char* aIn)
        {
            for (unsigned int i = 0; i < static_cast<unsigned int>(T::QTY); i++)
            {
                if (0 == strcmp(aIn, N[i]))
                {
                    mInternal = static_cast<T>(i);
                    return;
                }
            }

            throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::CONFIG_VALUE, "Invalid enumeration value");
        }

        // ===== Object =====================================================
        template <typename T, const char** N>
        Enum<T, N>::~Enum() {}

    }
}
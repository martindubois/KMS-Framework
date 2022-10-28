
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Float.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI/Value.h>

namespace KMS
{
    namespace DI
    {

        template <typename T>
        class Float : public Value
        {

        public:

            static DI::Object* Create();

            Float(T aIn = 0.0);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn);

            T Get() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Float();
            virtual void Clear();

        // Internal

            T mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        DI::Object* Float<T>::Create() { return new Float<T>; }

        template <typename T>
        Float<T>::Float(T aIn) : mInternal(aIn) {}

        template <typename T>
        void Float<T>::operator = (T aIn) { mInternal = aIn; }

        template <typename T>
        Float<T>::operator T () const { return mInternal; }

        template <typename T>
        bool Float<T>::operator == (T aIn) { return mInternal == aIn; }

        template <typename T>
        T Float<T>::Get() const { return mInternal; }

        // ===== Value ======================================================

        template <typename T>
        unsigned int Float<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            return sprintf_s(aOut SizeInfoV(aOutSize_byte), "%f", mInternal);
        }

        template <typename T>
        void Float<T>::Set(const char* aIn) { mInternal = Convert::ToDouble(aIn); }

        // ===== Object =====================================================

        template <typename T>
        Float<T>::~Float() {}

        template <typename T>
        void Float<T>::Clear() { mInternal = 0.0; }

    }
}

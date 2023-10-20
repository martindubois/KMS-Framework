
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Float.h

// External type : T (floating point value)
// Internal type : T (floating point value)

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI/Value.h>

namespace KMS
{
    namespace DI
    {

        template <typename T>
        class Float_Ptr : public Value
        {

        public:

            static const T DEFAULT_VALUE;

            Float_Ptr(T* aPtr);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn) const;

            T Get() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual bool Clear();

        private:

            T* mPtr;

        };

        template <typename T>
        class Float final : public Float_Ptr<T>
        {

        public:

            static DI::Object* Create();

            Float(T aIn = Float_Ptr<T>::DEFAULT_VALUE);

            // ===== Float_Ptr ==============================================
            using Float_Ptr<T>::operator =;
            using Float_Ptr<T>::operator T;
            using Float_Ptr<T>::operator ==;

        private:

            T mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        const T Float_Ptr<T>::DEFAULT_VALUE = 0.0;

        template <typename T>
        DI::Object* Float<T>::Create() { return new Float<T>; }

        template <typename T>
        Float_Ptr<T>::Float_Ptr(T* aPtr) : mPtr(aPtr)
        {
            assert(nullptr != aPtr);
        }

        template <typename T>
        Float<T>::Float(T aIn) : Float_Ptr<T>(&mInternal), mInternal(aIn) {}

        template <typename T>
        void Float_Ptr<T>::operator = (T aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        template <typename T>
        Float_Ptr<T>::operator T () const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        template <typename T>
        bool Float_Ptr<T>::operator == (T aIn) const
        {
            assert(nullptr != mPtr);

            return *mPtr == aIn;
        }

        template <typename T>
        T Float_Ptr<T>::Get() const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        // ===== Value ======================================================

        template <typename T>
        unsigned int Float_Ptr<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);

            assert(nullptr != mPtr);

            return sprintf_s(aOut SizeInfoV(aOutSize_byte), "%f", *mPtr);
        }

        template <typename T>
        void Float_Ptr<T>::Set(const char* aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = Convert::ToDouble(aIn);
        }

        // ===== Object =====================================================

        template <typename T>
        bool Float_Ptr<T>::Clear()
        {
            assert(nullptr != mPtr);

            bool lResult = *mPtr != 0.0;

            *mPtr = 0.0;

            return lResult;
        }

    }
}

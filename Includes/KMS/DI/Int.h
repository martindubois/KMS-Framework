
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Int.h

// External type : T (signed integer)
// Internal type : T (signed integer)

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>
#include <KMS/DI/Value.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace DI
    {

        template <typename T>
        class Int_Ptr : public Value
        {

        public:

            static const T DEFAULT_VALUE;

            Int_Ptr(T* aPtr);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn) const;

            T Get() const;

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual bool Clear();

        private:

            T* mPtr;

            Radix mRadix;

        };
        template <typename T>
        class Int : public Int_Ptr<T>
        {

        public:

            static DI::Object* Create();

            Int(T aIn = Int_Ptr<T>::DEFAULT_VALUE);

            // ===== Int_Ptr ================================================
            using Int_Ptr<T>::operator =;
            using Int_Ptr<T>::operator T;
            using Int_Ptr<T>::operator ==;

        private:

            T mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        const T Int_Ptr<T>::DEFAULT_VALUE = 0;

        template <typename T>
        DI::Object* Int<T>::Create() { return new Int<T>; }

        template <typename T>
        Int_Ptr<T>::Int_Ptr(T* aPtr) : mPtr(aPtr), mRadix(Radix::DECIMAL)
        {
            assert(nullptr != aPtr);
        }

        template <typename T>
        Int<T>::Int(T aIn) : Int_Ptr<T>(&mInternal), mInternal(aIn) {}

        template <typename T>
        void Int_Ptr<T>::operator = (T aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        template <typename T>
        Int_Ptr<T>::operator T () const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        template <typename T>
        bool Int_Ptr<T>::operator == (T aIn) const
        {
            assert(nullptr != mPtr);

            return *mPtr == aIn;
        }

        template <typename T>
        T Int_Ptr<T>::Get() const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        template <typename T>
        void Int_Ptr<T>::SetRadix(Radix aRadix) { mRadix = aRadix; }

        // ===== Value ======================================================

        template <typename T>
        unsigned int Int_Ptr<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);

            assert(nullptr != mPtr);

            unsigned int lResult_byte = 0;

            switch (mRadix)
            {
            case Radix::DECIMAL    : lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%d", *mPtr); break;
            case Radix::HEXADECIMAL: lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%x", *mPtr); break;

            default: assert(false);
            }

            return lResult_byte;
        }

        template <typename T>
        void Int_Ptr<T>::Set(const char* aIn)
        {
            assert(nullptr != mPtr);

            int32_t lValue = Convert::ToInt32(aIn, mRadix);

            switch (sizeof(T))
            {
            case 1:
                if ((0x7f < lValue) || (-128 > lValue))
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, RESULT_INVALID_VALUE, "Invalid int8_t value" , aIn);
                }
                break;
            case 2:
                if ((0x7fff < lValue) || (-32768 < lValue))
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, RESULT_INVALID_VALUE, "Invalid int16_t value", aIn);
                }
                break;
            case 4: break;
            default: assert(false);
            }

            *mPtr = lValue;
        }

        // ===== Object =====================================================

        template <typename T>
        bool Int_Ptr<T>::Clear()
        {
            assert(nullptr != mPtr);

            bool lResult = *mPtr != 0;

            *mPtr = 0;

            return lResult;
        }

    }
}

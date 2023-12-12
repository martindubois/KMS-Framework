
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/UInt.h

// External type : T (unsigned integer)
// Internal type : T (unsigned integer)

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
        class UInt_Ptr : public Value
        {

        public:

            static const T DEFAULT_VALUE;

            UInt_Ptr(T* aPtr);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn) const;

            T Get() const;

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);
            virtual bool         Set_Try(const char* aIn);

            // ===== Object =================================================
            virtual bool Clear();

        private:

            static bool IsValueValid(uint32_t aIn);

            T* mPtr;

            Radix mRadix;

        };

        template <typename T>
        class UInt final : public UInt_Ptr<T>
        {

        public:

            static DI::Object* Create();

            UInt(T aIn = 0);

            // ===== UInt_Ptr ===============================================
            using UInt_Ptr<T>::operator =;
            using UInt_Ptr<T>::operator T;
            using UInt_Ptr<T>::operator ==;

        private:

            T mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        const T UInt_Ptr<T>::DEFAULT_VALUE = 0;

        template <typename T>
        DI::Object* UInt<T>::Create() { return new UInt<T>; }

        template <typename T>
        UInt_Ptr<T>::UInt_Ptr(T* aPtr) : mPtr(aPtr), mRadix(Radix::DECIMAL)
        {
            assert(nullptr != aPtr);
        }

        template <typename T>
        UInt<T>::UInt(T aIn) : UInt_Ptr<T>(&mInternal), mInternal(aIn) {}

        template <typename T>
        void UInt_Ptr<T>::operator = (T aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        template <typename T>
        UInt_Ptr<T>::operator T () const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        template <typename T>
        bool UInt_Ptr<T>::operator == (T aIn) const
        {
            assert(nullptr != mPtr);

            return *mPtr == aIn;
        }

        template <typename T>
        T UInt_Ptr<T>::Get() const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        template <typename T>
        void UInt_Ptr<T>::SetRadix(Radix aRadix) { mRadix = aRadix; }

        // ===== Value ======================================================

        template <typename T>
        unsigned int UInt_Ptr<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != aOut);

            assert(nullptr != mPtr);

            unsigned int lResult_byte = 0;

            switch (mRadix)
            {
            case Radix::DECIMAL    : lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u", *mPtr); break;
            case Radix::HEXADECIMAL: lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%x", *mPtr); break;

            default: assert(false);
            }

            return lResult_byte;
        }

        template <typename T>
        void UInt_Ptr<T>::Set(const char* aIn)
        {
            assert(nullptr != mPtr);

            #pragma warning( push )
            #pragma warning( disable : 4244 )
            switch (sizeof(T))
                {
                case 1: *mPtr = Convert::ToUInt8 (aIn, mRadix); break;
                case 2: *mPtr = Convert::ToUInt16(aIn, mRadix); break;
                case 4: *mPtr = Convert::ToUInt32(aIn, mRadix); break;

                default: assert(false);
                }
            #pragma warning(pop)
        }

        template <typename T>
        bool UInt_Ptr<T>::Set_Try(const char* aIn)
        {
            assert(nullptr != mPtr);

            auto lResult = false;

            switch (sizeof(T))
            {
            case 1: lResult = Convert::ToUInt8_Try (aIn, reinterpret_cast<uint8_t *>(mPtr), mRadix); break;
            case 2: lResult = Convert::ToUInt16_Try(aIn, reinterpret_cast<uint16_t*>(mPtr), mRadix); break;
            case 4: lResult = Convert::ToUInt32_Try(aIn, reinterpret_cast<uint32_t*>(mPtr), mRadix); break;

            default: assert(false);
            }

            return lResult;
        }

        // ===== Object =====================================================

        template <typename T>
        bool UInt_Ptr<T>::Clear()
        {
            assert(nullptr != mPtr);

            auto lResult = *mPtr != 0;

            *mPtr = 0;

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        bool UInt_Ptr<T>::IsValueValid(uint32_t aIn)
        {
            bool lResult = false;

            switch (sizeof(T))
            {
            case 1: lResult = 0xff   >= aIn; break;
            case 2: lResult = 0xffff >= aIn; break;
            case 4: lResult = true; break;

            default: assert(false);
            }

            return lResult;
        }

    }
}

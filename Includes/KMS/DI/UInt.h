
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/UInt.h

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
        class UInt_Base : public Value
        {

        public:

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn);

            void SetRadix(Radix aRadix);

            virtual T Get() const = 0;

            virtual void Set(T aIn) = 0;

            // ===== Value ==================================================
            virtual bool         Clear();
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~UInt_Base();

        protected:

            UInt_Base();

            Radix mRadix;

        };

        template <typename T>
        class UInt : public UInt_Base<T>
        {

        public:

            static DI::Object* Create();

            UInt(T aIn = 0);

            // ===== UInt_Base ==============================================
            virtual T    Get() const;
            virtual void Set(T aIn);

            // ===== Object =================================================
            virtual ~UInt();

        // Internal

            T mInternal;

        };

        template <typename T>
        class UInt_Ptr : public UInt_Base<T>
        {

        public:

            UInt_Ptr(T* aInternal);

            // ===== UInt_Base ==============================================
            virtual T    Get() const;
            virtual void Set(T aIn);

            // ===== Object =================================================
            virtual ~UInt_Ptr();

        // Internal

            T* mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        DI::Object* UInt<T>::Create() { return new UInt<T>; }

        template <typename T>
        UInt<T>::UInt(T aIn) : mInternal(aIn) {}

        template <typename T>
        UInt_Ptr<T>::UInt_Ptr(T* aInternal) : mInternal(aInternal) {}

        template <typename T>
        void UInt_Base<T>::operator = (T aIn) { Set(aIn); }

        template <typename T>
        UInt_Base<T>::operator T () const { return Get(); }

        template <typename T>
        bool UInt_Base<T>::operator == (T aIn) { return Get() == aIn; }

        template <typename T>
        void UInt_Base<T>::SetRadix(Radix aRadix) { mRadix = aRadix; }

        // ===== UInt_Base ==================================================

        template <typename T>
        T UInt<T>::Get() const { return mInternal; }

        template <typename T>
        T UInt_Ptr<T>::Get() const { return *mInternal; }

        template <typename T>
        void UInt<T>::Set(T aIn) { mInternal = aIn; };

        template <typename T>
        void UInt_Ptr<T>::Set(T aIn) { *mInternal = aIn; };

        // ===== Value ======================================================

        template <typename T>
        bool UInt_Base<T>::Clear()
        {
            bool lResult = Get() != 0;

            Set(static_cast<T>(0));

            return lResult;
        }

        template <typename T>
        unsigned int UInt_Base<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            switch (mRadix)
            {
            case Radix::DECIMAL    : lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u", Get()); break;
            case Radix::HEXADECIMAL: lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%x", Get()); break;

            default: assert(false);
            }

            return lResult_byte;
        }

        template <typename T>
        void UInt_Base<T>::Set(const char* aIn)
        {
            uint32_t lValue = Convert::ToUInt32(aIn, mRadix);

            switch (sizeof(T))
            {
            case 1:
                if (0xff < lValue)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, Exception::Code::MODBUS_CONFIG_INVALID, "Invalid uint8_t value" , aIn);
                }
                break;
            case 2:
                if (0xffff < lValue)
                {
                    throw Exception(__FILE__, __FUNCTION__, __LINE__, Exception::Code::MODBUS_CONFIG_INVALID, "Invalid uint16_t value", aIn);
                }
                break;
            case 4: break;
            default: assert(false);
            }

            Set(lValue);
        }

        // ===== Object =====================================================

        template <typename T>
        UInt_Base<T>::~UInt_Base() {}

        template <typename T>
        UInt<T>::~UInt() {}

        template <typename T>
        UInt_Ptr<T>::~UInt_Ptr() {}

        // Protected
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        UInt_Base<T>::UInt_Base() : mRadix(Radix::DECIMAL) {}

    }
}

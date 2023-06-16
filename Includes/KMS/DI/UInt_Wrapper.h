
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/UInt_Wrapper.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Convert.h>

namespace KMS
{
    namespace DI
    {

        template <typename T>
        class UInt_Wrapper : public Value
        {

        public:

            UInt_Wrapper(T* aInternal);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn);

            T Get() const;

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual bool         Clear();
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~UInt_Wrapper();

        // Internal

            T* mInternal;

        private:

            Radix mRadix;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        UInt_Wrapper<T>::UInt_Wrapper(T* aInternal) : mInternal(aInternal), mRadix(Radix::DECIMAL) {}

        template <typename T>
        void UInt_Wrapper<T>::operator = (T aIn) { *mInternal = aIn; }

        template <typename T>
        UInt_Wrapper<T>::operator T () const { return *mInternal; }

        template <typename T>
        bool UInt_Wrapper<T>::operator == (T aIn) { return *mInternal == aIn; }

        template <typename T>
        T UInt_Wrapper<T>::Get() const { return *mInternal; }

        template <typename T>
        void UInt_Wrapper<T>::SetRadix(Radix aRadix) { mRadix = aRadix; }

        // ===== Value ======================================================

        template <typename T>
        bool UInt_Wrapper<T>::Clear()
        {
            bool lResult = *mInternal != 0;

            *mInternal = 0;

            return lResult;
        }

        template <typename T>
        unsigned int UInt_Wrapper<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            switch (mRadix)
            {
            case Radix::DECIMAL    : lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u", *mInternal); break;
            case Radix::HEXADECIMAL: lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%x", *mInternal); break;

            default: assert(false);
            }

            return lResult_byte;
        }

        template <typename T>
        void UInt_Wrapper<T>::Set(const char* aIn)
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

            *mInternal = lValue;
        }

        // ===== Object =====================================================

        template <typename T>
        UInt_Wrapper<T>::~UInt_Wrapper() {}

    }
}

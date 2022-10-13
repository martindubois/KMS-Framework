
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/UInt.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace DI
    {

        template <typename T>
        class UInt : public Value
        {

        public:

            static DI::Object* Create();

            UInt(T aIn = 0);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn);

            T Get() const;

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~UInt();

        // Internal

            T mInternal;

        private:

            Radix mRadix;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        DI::Object* UInt<T>::Create() { return new UInt<T>; }

        template <typename T>
        UInt<T>::UInt(T aIn) : mInternal(aIn), mRadix(KMS::Radix::DECIMAL) {}

        template <typename T>
        void UInt<T>::operator = (T aIn) { mInternal = aIn; }

        template <typename T>
        UInt<T>::operator T () const { return mInternal; }

        template <typename T>
        bool UInt<T>::operator == (T aIn) { return mInternal == aIn; }

        template <typename T>
        T UInt<T>::Get() const { return mInternal; }

        template <typename T>
        void UInt<T>::SetRadix(Radix aRadix) { mRadix = aRadix; }

        // ===== Value ======================================================

        template <typename T>
        unsigned int UInt<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            switch (mRadix)
            {
            case KMS::Radix::DECIMAL    : lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%u", mInternal); break;
            case KMS::Radix::HEXADECIMAL: lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%x", mInternal); break;

            default: assert(false);
            }

            return lResult_byte;
        }

        template <typename T>
        void UInt<T>::Set(const char* aIn)
        {
            assert(NULL != aIn);

            int lRet;

            unsigned int lValue;

            switch (mRadix)
            {
            case KMS::Radix::DECIMAL    : lRet = sscanf_s(aIn, "%u", &lValue); break;
            case KMS::Radix::HEXADECIMAL: lRet = sscanf_s(aIn, "%x", &lValue); break;

            default: assert(false);
            }

            if (1 != lRet)
            {
                throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::CONFIG_FORMAT, "Invalid unsigned integer format", aIn);
            }

            switch (sizeof(T))
            {
            case 1:
                if (0xff < lValue)
                {
                    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::CONFIG_VALUE, "Invalid uint8_t value" , aIn);
                }
                break;
            case 2:
                if (0xffff < lValue)
                {
                    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::CONFIG_VALUE, "Invalid uint16_t value", aIn);
                }
                break;
            case 4: break;
            default: assert(false);
            }

            mInternal = lValue;
        }

        // ===== Object =====================================================

        template <typename T>
        UInt<T>::~UInt() {}

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Int.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace DI
    {

        template <typename T>
        class Int : public Value
        {

        public:

            static DI::Object* Create();

            Int(T aIn = 0);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn);

            T Get() const;

            void SetRadix(Radix aRadix);

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Int();

        // Internal

            T mInternal;

        private:

            Radix mRadix;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        DI::Object* Int<T>::Create() { return new Int<T>; }

        template <typename T>
        Int<T>::Int(T aIn) : mInternal(aIn), mRadix(KMS::Radix::DECIMAL) {}

        template <typename T>
        void Int<T>::operator = (T aIn) { mInternal = aIn; }

        template <typename T>
        Int<T>::operator T () const { return mInternal; }

        template <typename T>
        bool Int<T>::operator == (T aIn) { return mInternal == aIn; }

        template <typename T>
        T Int<T>::Get() const { return mInternal; }

        template <typename T>
        void Int<T>::SetRadix(Radix aRadix) { mRadix = aRadix; }

        // ===== Value ======================================================

        template <typename T>
        unsigned int Int<T>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = 0;

            switch (mRadix)
            {
            case KMS::Radix::DECIMAL    : lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%d", mInternal); break;
            case KMS::Radix::HEXADECIMAL: lResult_byte = sprintf_s(aOut SizeInfoV(aOutSize_byte), "%x", mInternal); break;

            default: assert(false);
            }

            return lResult_byte;
        }

        template <typename T>
        void Int<T>::Set(const char* aIn)
        {
            assert(NULL != aIn);

            int lRet;

            int lValue;

            switch (mRadix)
            {
            case KMS::Radix::DECIMAL    : lRet = sscanf_s(aIn, "%d", &lValue); break;
            case KMS::Radix::HEXADECIMAL: lRet = sscanf_s(aIn, "%x", &lValue); break;

            default: assert(false);
            }

            if (1 != lRet)
            {
                throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::CONFIG_FORMAT, "Invalid integer format", aIn);
            }

            switch (sizeof(T))
            {
            case 1:
                if ((0x7f < lValue) || (-128 > lValue))
                {
                    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::CONFIG_VALUE, "Invalid int8_t value" , aIn);
                }
                break;
            case 2:
                if ((0x7fff < lValue) || (-32768 < lValue))
                {
                    throw KMS::Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::CONFIG_VALUE, "Invalid int16_t value", aIn);
                }
                break;
            case 4: break;
            default: assert(false);
            }

            mInternal = lValue;
        }

        // ===== Object =====================================================

        template <typename T>
        Int<T>::~Int() {}

    }
}

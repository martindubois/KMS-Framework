
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Enum.h

// External type : T (enum class)
// Internal type : T (enum class)

#pragma once

#include <KMS/Base.h>

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/Enum.h>
#include <KMS/Exception.h>

namespace KMS
{
    namespace DI
    {

        template <typename T, const char** N>
        class Enum_Ptr : public Value
        {

        public:

            static const T DEFAULT_VALUE;

            Enum_Ptr(T* aPtr);

            void operator = (T aIn);

            operator T () const;

            bool operator == (T aIn) const;

            T Get() const;

            const char* GetName() const;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual bool Clear();

        private:

            T* mPtr;

        };

        template <typename T, const char** N>
        class Enum : public Enum_Ptr<T, N>
        {

        public:

            static DI::Object* Create();

            Enum(T aIn = DEFAULT_VALUE);

            // ===== Enum_Ptr ===============================================
            using Enum_Ptr<T, N>::operator =;
            using Enum_Ptr<T, N>::operator T;
            using Enum_Ptr<T, N>::operator ==;

        private:

            T mInternal;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T, const char** N>
        const T Enum_Ptr<T, N>::DEFAULT_VALUE = static_cast<T>(0);

        template <typename T, const char** N>
        DI::Object* Enum<T, N>::Create() { return new Enum<T, N>(static_cast<T>(0)); }

        template <typename T, const char** N>
        inline Enum_Ptr<T, N>::Enum_Ptr(T* aPtr) : mPtr(aPtr)
        {
            assert(nullptr != aPtr);
        }

        template <typename T, const char** N>
        inline Enum<T, N>::Enum(T aIn) : Enum_Ptr<T, N>(&mInternal), mInternal(aIn) {}

        template <typename T, const char** N>
        void Enum_Ptr<T, N>::operator = (T aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;
        }

        template <typename T, const char** N>
        Enum_Ptr<T, N>::operator T () const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        template <typename T, const char** N>
        bool Enum_Ptr<T, N>::operator == (T aIn) const
        {
            assert(nullptr != mPtr);

            return *mPtr == aIn;
        }

        template <typename T, const char** N>
        T Enum_Ptr<T, N>::Get() const { return *mPtr; }

        template <typename T, const char** N>
        const char* Enum_Ptr<T, N>::GetName() const
        {
            assert(nullptr != mPtr);

            KMS::Enum<T, N> lInternal(*mPtr);
            
            return lInternal.GetName();
        }

        // ===== Value ======================================================

        template <typename T, const char** N>
        unsigned int Enum_Ptr<T, N>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(nullptr != mPtr);

            KMS::Enum<T, N> lInternal(*mPtr);

            const char* lName = lInternal.GetName();
            assert(nullptr != lName);

            unsigned int lResult_byte = static_cast<unsigned int>(strlen(lName));

            if (aOutSize_byte <= lResult_byte + 1)
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short");
            }

            strcpy_s(aOut SizeInfoV(aOutSize_byte), lName);

            return lResult_byte;
        }

        template <typename T, const char** N>
        void Enum_Ptr<T, N>::Set(const char* aIn)
        {
            assert(nullptr != mPtr);

            KMS::Enum<T, N> lInternal(*mPtr);

            if (lInternal.SetName(aIn))
            {
                *mPtr = lInternal;

                Send_OnChanged(const_cast<char*>(aIn));
            }
        }

        // ===== Object =====================================================

        template <typename T, const char** N>
        bool Enum_Ptr<T, N>::Clear()
        {
            assert(nullptr != mPtr);

            bool lResult = *mPtr != static_cast<T>(0);

            *mPtr = static_cast<T>(0);

            return lResult;
        }

    }
}

template <typename T, const char** N>
std::ostream& operator << (std::ostream& aOut, const KMS::DI::Enum<T, N>& aE)
{
    aOut << aE.GetName();

    return aOut;
}

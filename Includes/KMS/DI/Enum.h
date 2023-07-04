
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Enum.h

#pragma once

#include <KMS/Base.h>

// ===== Includes ===========================================================
#include <KMS/DI/Value.h>
#include <KMS/Enum.h>

namespace KMS
{
    namespace DI
    {

        template <typename T, const char** N>
        class Enum_Base : public Value
        {

        public:

            void operator = (T aIn);

            operator T () const;

            const char* GetName() const;

            virtual T Get() const = 0;

            // ===== Value ==================================================
            virtual unsigned int Get(char* aOut, unsigned int aOutSize_byte) const;
            virtual void         Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Enum_Base();
            virtual bool Clear();

        protected:

            Enum_Base();

            virtual bool Internal_Set(T aIn) = 0;

        };

        template <typename T, const char** N>
        class Enum : public Enum_Base<T, N>
        {

        public:

            static DI::Object* Create();

            Enum(T aIn);

            // ===== Enum_Base ==============================================
            virtual T    Get() const;

            // ===== Object =================================================
            virtual ~Enum();

        // Internal

            KMS::Enum<T, N> mInternal;

        protected:

            // ===== Enum_Base ==============================================
            virtual bool Internal_Set(T aIn);

        };

        template <typename T, const char** N>
        class Enum_Ptr : public Enum_Base<T, N>
        {

        public:

            Enum_Ptr(T* aIn);

            // ===== Enum_Base ==============================================
            virtual T Get() const;
            
            // ===== Object =================================================
            virtual ~Enum_Ptr();

        // internal

            KMS::Enum<T, N>* mInternal;

        protected:

            // ===== Enum_Base ==============================================
            virtual bool Internal_Set(T aIn);

        };

    }
}

// ===== Includes ===========================================================
// The KMS/Exception.h header file use the Enum template.
#include <KMS/Exception.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T, const char** N>
        inline Enum<T, N>::Enum(T aIn) : Enum_Base<T, N>(), mInternal(aIn) {}

        template <typename T, const char** N>
        inline Enum_Ptr<T, N>::Enum_Ptr(T* aIn) : Enum_Base<T, N>(), mInternal(aIn) {}

        template <typename T, const char** N>
        inline void Enum_Base<T, N>::operator = (T aIn) { Set(aIn); }
        
        template <typename T, const char** N>
        Enum_Base<T, N>::operator T () const { return Get(); }

        template <typename T, const char** N>
        const char* Enum_Base<T, N>::GetName() const
        {
            KMS::Enum<T, N> lIn(Get());
            
            return lIn.GetName();
        }

        // ===== Enum_Base ==================================================

        template <typename T, const char** N>
        T Enum<T, N>::Get() const { return mInternal; }

        template <typename T, const char** N>
        T Enum_Ptr<T, N>::Get() const { return *mInternal; }

        // ===== Value ======================================================

        template <typename T, const char** N>
        unsigned int Enum_Base<T, N>::Get(char* aOut, unsigned int aOutSize_byte) const
        {
            const char* lName = GetName();

            unsigned int lResult_byte = static_cast<unsigned int>(strlen(lName));

            if (aOutSize_byte <= lResult_byte + 1)
            {
                throw Exception(__FILE__, __FUNCTION__, __LINE__, Exception::Code::DI_OUTPUT_TOO_SHORT, "The output buffer is too short");
            }

            strcpy_s(aOut SizeInfoV(aOutSize_byte), lName);

            return lResult_byte;
        }

        template <typename T, const char** N>
        void Enum_Base<T, N>::Set(const char* aIn)
        {
            KMS::Enum<T, N> lIn(0);

            lIn.SetName(aIn);

            if (Internal_Set(lIn))
            {
                Send_OnChanged(const_cast<char*>(aIn));
            }
        }

        // ===== Object =====================================================

        template <typename T, const char** N>
        Enum_Base<T, N>::~Enum_Base() {}

        template <typename T, const char** N>
        Enum<T, N>::~Enum() {}

        template <typename T, const char** N>
        Enum_Ptr<T, N>::~Enum_Ptr() {}

        template <typename T, const char** N>
        bool Enum_Base<T, N>::Clear() { return Internal_Set(static_cast<T>(0)); }

        // Protected
        // //////////////////////////////////////////////////////////////////

        template <typename T, const char** N>
        inline Enum_Base<T, N>::Enum_Base() {}

        // ===== Enum_Base ==================================================

        template <typename T, const char** N>
        bool Enum<T, N>::Internal_Set(T aIn) { return mInternal.Set(aIn); }

        template <typename T, const char** N>
        bool Enum_Ptr<T, N>::Internal_Set(T aIn) { return mInternal->Set(aIn); }

    }
}

template <typename T, const char** N>
std::ostream& operator << (std::ostream& aOut, const KMS::DI::Enum<T, N>& aE)
{
    aOut << aE.mInternal.GetName();

    return aOut;
}

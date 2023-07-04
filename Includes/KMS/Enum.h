
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Enum.h

#pragma once

namespace KMS
{

    template <typename T, const char** N>
    class Enum
    {

    public:

        Enum(T aValue);

        Enum(const char* aName);

        void operator = (T aValue);

        operator T () const;

        T Get() const;

        const char* GetName() const;

        bool Set(T aValue);

        bool SetName(const char* aName);

    private:

        T mValue;

    };

}

// ===== Includes ===========================================================
// The KMS/Exception.h header file use the Enum template.
#include <KMS/Exception.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    template <typename T, const char** N>
    inline Enum<T, N>::Enum(T aValue) { Set(aValue); }

    template <typename T, const char** N>
    inline Enum<T, N>::Enum(const char* aName) { SetName(aName); }

    template <typename T, const char** N>
    inline void Enum<T, N>::operator = (T aValue) { Set(aValue); }

    template <typename T, const char** N>
    Enum<T, N>::operator T () const { return mValue; }

    template <typename T, const char** N>
    T Enum<T, N>::Get() const { return mValue; }

    template <typename T, const char** N>
    const char* Enum<T, N>::GetName() const
    {
        assert(T::QTY > mValue);

        return N[static_cast<unsigned int>(mValue)];
    }

    template <typename T, const char** N>
    bool Enum<T, N>::SetName(const char* aName)
    {
        assert(NULL != aName);

        for (unsigned int i = 0; i < static_cast<unsigned int>(T::QTY); i++)
        {
            if (0 == strcmp(aName, N[i]))
            {
                return Set(static_cast<T>(i));
            }
        }

        throw Exception(__FILE__, __FUNCTION__, __LINE__, KMS::Exception::Code::ENUM_INVALID_VALUE_NAME, "Invalid enumeration value name");
    }

    template <typename T, const char** N>
    bool Enum<T, N>::Set(T aValue)
    {
        assert(T::QTY > mValue);

        bool lResult = mValue != aValue;

        mValue = aValue;

        return lResult;
    }

}

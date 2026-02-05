
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework
// Library KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2023-2026 KMS
/// \file      Includes/KMS/Enum.h

#pragma once

namespace KMS
{

    /// \brief Use this class to associate usable text name to a enum
    /// \note This is what, here at KMS, we call a value class.
    ///       - copy allowed
    ///       - limited size
    ///       - no virtual method
    template <typename T, const char** N>
    class Enum final
    {

    public:

        /// \param aValue enum value used to initialize the instance
        Enum(T aValue);

        /// \param aName Value name used to initialize the instance
        Enum(const char* aName);

        /// \param aValue enum value used to assign the instance
        void operator = (T aValue);

        /// \return The enum value
        operator T () const;

        /// \return The enum value
        T Get() const;

        /// \return The enum value name
        const char* GetName() const;

        /// \param aValue The enum value
        bool Set(T aValue);

        /// \param aName The enum value name
        /// \exception Exception RESULT_INVALID_NAME
        bool SetName(const char* aName);

        /// \param aName    The enum value name
        /// \param aChanged Set to true if the value changed, set to false if
        ///                 the value did not change
        /// \retval false The name is not valid
        /// \retval true  The name is valid
        bool SetName_Try(const char* aName, bool* aChanged = nullptr);

    private:

        T mValue;

    };

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
    bool Enum<T, N>::Set(T aValue)
    {
        assert(T::QTY > aValue);

        bool lResult = mValue != aValue;

        mValue = aValue;

        return lResult;
    }

}

#include <KMS/Exception.h>

namespace KMS
{

    template <typename T, const char** N>
    bool Enum<T, N>::SetName(const char* aName)
    {
        assert(nullptr != aName);

        for (unsigned int i = 0; i < static_cast<unsigned int>(T::QTY); i++)
        {
            if (0 == strcmp(aName, N[i]))
            {
                return Set(static_cast<T>(i));
            }
        }

        throw Exception(__FILE__, __FUNCTION__, __LINE__, RESULT_INVALID_NAME, "Invalid enumeration value name");
    }

    template <typename T, const char** N>
    bool Enum<T, N>::SetName_Try(const char* aName, bool* aChanged)
    {
        assert(nullptr != aName);

        for (unsigned int i = 0; i < static_cast<unsigned int>(T::QTY); i++)
        {
            if (0 == strcmp(aName, N[i]))
            {
                auto lChanged = Set(static_cast<T>(i));

                if (nullptr != aChanged)
                {
                    *aChanged = lChanged;
                }

                return true;
            }
        }

        return false;
    }

}

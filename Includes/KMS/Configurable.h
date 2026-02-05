
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// Library   KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2025-2026 KMS
/// \file      Includes/KMS/Configurable.h

#pragma once

namespace KMS
{
    
    /// \brief Base class for configurable instance
    template <class T>
    class Configurable
    {

    public:

        /// \brief Set the configuration to default configuration
        virtual void Config_Default();

        /// \brief Get the current configuration
        /// \param aOut Output buffer
        virtual void Config_Get(T* aOut) const;

        /// \brief Set the current configuration
        /// \param aIn Configuration
        /// \exception Exception
        virtual void Config_Set(const T& aIn);

    protected:

        T mConfig;

    };

    // Public
    // //////////////////////////////////////////////////////////////////////

    template <class T>
    void Configurable<T>::Config_Default() { mConfig.SetDefault(); }

    template <class T>
    void Configurable<T>::Config_Get(T* aOut) const { *aOut = mConfig; };

    template <class T>
    void Configurable<T>::Config_Set(const T& aIn)
    {
        aIn.Validate();

        mConfig = aIn;
    }

}

// Macros
// //////////////////////////////////////////////////////////////////////////

/// \brief Define common method for all configuration structure
#define KMS_Config_METHODS(T)                                                 \
    T() { SetDefault(); };                                                    \
    void IsEqual(const T& aB) const;                                          \
    bool IsValid(char* aMsg = nullptr, unsigned int aMsgSize_byte = 0) const; \
    void SetDefault();                                                        \
    void Validate() const;

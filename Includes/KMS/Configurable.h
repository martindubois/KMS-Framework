
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Configurable.h

#pragma once

namespace KMS
{
    
    template <class T>
    class Configurable
    {

    public:

        virtual void Config_Default();

        virtual void Config_Get(T* aOut) const;

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

#define KMS_Config_METHODS(T)                                                 \
    T() { SetDefault(); };                                                    \
    void IsEqual(const T& aB) const;                                          \
    bool IsValid(char* aMsg = nullptr, unsigned int aMsgSize_byte = 0) const; \
    void SetDefault();                                                        \
    void Validate() const;

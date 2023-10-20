
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Callback.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

namespace KMS
{

    class ICallback
    {

    public:

        // Action
        static const unsigned int FLAG_ACTION_CLEAR = 0x01000000;
        static const unsigned int FLAG_ACTION_QUEUE = 0x02000000;
        static const unsigned int FLAG_ACTION_STOP  = 0x04000000;
        static const unsigned int FLAGS_ACTIONS     = 0xff000000;

        // Errors
        static const unsigned int FLAG_ERROR             = 0x00000001;
        static const unsigned int FLAG_EXCEPTION         = 0x00000002;
        static const unsigned int FLAG_EXCEPTION_STD     = 0x00000004;
        static const unsigned int FLAG_EXCEPTION_UNKNOWN = 0x00000008;
        static const unsigned int FLAGS_ERRORS           = 0x000000ff;

        // Warnings
        static const unsigned int FLAG_IGNORED   = 0x00010000;
        static const unsigned int FLAG_NOT_SET   = 0x00020000;
        static const unsigned int FLAGS_WARNINGS = 0x00ff0000;

        virtual unsigned int Send(void* aSender, void* aData = nullptr) = 0;

    };

    class Callback_Ptr final
    {

    public:

        Callback_Ptr();

        void operator = (const ICallback *aPtr);

        void Clear();

        bool IsSet();

        unsigned int Send(void* aSender, void* aData = nullptr);

    private:

        ICallback* mCallback;

    };

    template < class T >
    class Callback final : public ICallback
    {

    public:

        typedef unsigned int (T::*Method)(void* aSender, void* aData);

        Callback(T* aInstance, Method aMethod);

        // ===== ICallback ==============================================
        virtual unsigned int Send(void* aSender, void* aData = nullptr);

    private:

        T    * mInstance;
        Method mMethod;

    };

    // Public
    // //////////////////////////////////////////////////////////////////

    template < class T >
    Callback< T >::Callback(T* aInstance, Method aMethod) : mInstance(aInstance), mMethod(aMethod)
    {}

    // ===== ICallback ==================================================

    template < class T >
    unsigned int Callback< T >::Send(void* aSender, void* aData)
    {
        return (mInstance->*mMethod)(aSender, aData);
    }

}

#define CALLBACK_SUCCESS(R)            (0 == ((R) & (ICallback::FLAGS_ERRORS|ICallback::FLAGS_WARNINGS)))
#define CALLBACK_SUCCESS_OR_WARNING(R) (0 == ((R) &  ICallback::FLAGS_ERRORS))

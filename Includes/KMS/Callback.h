
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// Library   KMS-A

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2023-2026 KMS
/// \file      Includes/KMS/Callback.h

#pragma once

namespace KMS
{

    /// \brief Interface to receive callback
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

        /// \param aSender The instance calling the callback
        /// \param aData   Data passed to the callback
        /// \return See `FLAG_...`
        virtual unsigned int Send(void* aSender, void* aData = nullptr) = 0;

    };

    /// \brief Pointer to a ICallback interface
    /// \note This is what, here at KMS, we call a value class.
    ///       - copy allowed
    ///       - limited size
    ///       - no virtual method
    class Callback_Ptr final
    {

    public:

        /// \brief Constructor
        Callback_Ptr();

        /// \param aPtr Instance to assign
        void operator = (const ICallback *aPtr);

        /// \param Set the internal pointer to nullptr
        void Clear();

        /// \retval false The pointer is `nullptr`
        /// \retval true  The pointer is set
        bool IsSet();

        /// \param aSender The instance calling the calback
        /// \param aData   Data passed to the callback
        /// \return See `FLAG_...`
        unsigned int Send(void* aSender, void* aData = nullptr);

    private:

        ICallback* mCallback;

    };

    /// \brief Use to attach a ICallback interface to a method
    template < class T >
    class Callback final : public ICallback
    {

    public:

        /// \param aSender The instance calling the callback
        /// \param aData   Data passed to the callback
        /// \return See `FLAG_...`
        typedef unsigned int (T::*Method)(void* aSender, void* aData);

        /// \brief Constructor
        /// \param aInstance The instance to receive the callback
        /// \param aMethod   The methode called to process the callback
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

#define CALLBACK_SUCCESS(R)            (0 == ((R) & (KMS::ICallback::FLAGS_ERRORS|KMS::ICallback::FLAGS_WARNINGS)))
#define CALLBACK_SUCCESS_OR_WARNING(R) (0 == ((R) &  KMS::ICallback::FLAGS_ERRORS))

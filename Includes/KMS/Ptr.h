
// License http://www.apache.org/licenses/LICENSE-2.0
// Product KMS-Framework

/// \author    KMS - Martin Dubois, P. Eng.
/// \copyright Copyright &copy; 2024-2026 KMS
/// \file      Includes/KMS/Ptr.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Exception.h>

namespace KMS
{

    /// \brief Mandatory dynamic pointer
    template <typename T>
    class Ptr_M
    {

    public:

        /// \brief Constructor
        /// \param aInit The initial value
        Ptr_M(T* aInit);

        /// \brief Destructor - Delete the pointed `T` instance
        ~Ptr_M();

        /// \return The `T` instance pointer
        const T* operator -> () const;

        /// \return The `T` instance pointer
        T* operator -> ();

        /// \brief Cast operator
        operator const T* () const;

        /// \brief Cast operator
        operator T* ();

    private:

        Ptr_M(const Ptr_M &);
        void operator = (const Ptr_M&);

        T* mInternal;

    };

    /// \brief Optional dynamic pointer
    template <typename T>
    class Ptr_O
    {

    public:

        /// \brief Default constructor
        /// \param aInit The initial value
        Ptr_O(T* aInit = nullptr);

        /// \brief Destructor - Delete the pointed `T` instance
        ~Ptr_O();

        /// \return The `T` instance pointer
        const T* operator -> () const;

        /// \return The `T` instance pointer
        T* operator -> ();

        /// \brief Cast operator
        operator const T* () const;

        /// \brief Cast operator
        operator T* ();

        /// \brief Assignment operator
        /// \param aSet The new pointer
        void operator = (T* aSet);

        /// \return The `T` instance pointer
        T* Detach();

        /// \brief Delete the pointed `T` instance
        void Release();

    private:

        Ptr_O(const Ptr_O &);
        void operator = (const Ptr_O&);

        T* mInternal;

    };

    /// \brief Optional pointer with flags
    template <typename T>
    class Ptr_OF
    {

    public:

        /// \brief Default constructor
        Ptr_OF();

        /// \brief Constructor
        /// \param aInit The instance keep the pointer, protect the data
        ///              against modification (const) and is not responsible
        ///              for deleting the `T` instance.
        Ptr_OF(const T* aInit);

        /// \brief Constructor
        /// \param aInit   The instance keep the pointer.
        /// \param aDelete false  The instance is not responsible for
        ///                       deleting the `T` instance
        ///                true   The instance is responsible for deleting
        ///                       the `T` instance
        Ptr_OF(T* aInit, bool aDelete);

        /// \brief Destructor
        /// \note Release the `T` instance if the current instance is
        ///       responsible for doing that.
        ~Ptr_OF();

        /// \return The `T` instance pointer
        const T* operator -> () const;
        
        /// \return The `T` instance pointer
        /// \exception Exception  RESULT_DENIED
        T* operator -> ();

        /// \brief Cast operator
        operator const T* () const;

        /// \brief Cast operator
        /// \exception Exception RESULT_DENIED
        operator T* ();

        /// \brief Assignment operator
        /// \param aSet The instance keep the pointer, protect the data
        ///             against modification (const) and is not responsible
        ///             for deleting the `T` instance.
        /// \note This method can cause previous pointed `T` instance to be
        ///       deleted.
        void operator = (const T* aSet);

        /// \return The `T` instance pointer
        const T* Get() const;

        /// \return The `T` instance pointer
        /// \exception Exception  RESULT_DENIED
        T* Get();

        /// \retval false The `T` instance is not const
        /// \retval true  The `T` instance is const
        bool IsConst() const;

        /// \param aSet The value passed here is reset to nullptr when the
        ///             method returns.
        ///             If `aSet` is responsible for deleting the `T`
        ///             instance, the current instance become responsible for
        ///             deleting the `T` instance.
        /// \note This method can cause previous pointed `T` instance to be
        ///       deleted.
        void Set(Ptr_OF<T>& aSet);

        /// \param aSet The instance keep the pointer, protect the data
        ///             against modification (const) and is not responsible
        ///             for deleting the `T` instance.
        /// \note This method can cause previous pointed `T` instance to be
        ///       deleted.
        void Set(const T* aSet);

        /// \param aSet    The instance keep the pointer.
        /// \param aDelete false  The instance is not responsible for
        ///                       deleting the `T` instance
        ///                true   The instance is responsible for deleting
        ///                       the `T` instance
        /// \note This method can cause previous pointed `T` instance to be
        ///       deleted.
        void Set(T* aSet, bool aDelete);

        /// \return The `T` instance pointer
        /// \note This method reset the instance to `nullptr`.
        T* Detach();

        /// \brief Release the `T` instance
        /// \note If the current instance is responsible for doing that.
        /// \note This method reset the instance to `nullptr`.
        void Release();

    // Internal

        /// \param aSet The value passed here is reset to `nullptr` when the
        ///             method returns.
        ///             If aSet is responsible for deleting the `T` instance,
        ///             the current instance become responsible for deleting
        ///             the `T` instance.
        /// \note This method can cause previous pointed `T` instance to be
        ///       deleted.
        Ptr_OF(const Ptr_OF& aIn);

        /// \param aIn Must contain `nullptr`.
        /// \exception Exception RESULT_NOT_IMPLEMENTED
        void operator = (Ptr_OF& aIn);

    private:

        struct
        {
            unsigned mConst  : 1;
            unsigned mDelete : 1;
        }
        mFlags;
        
        T* mInternal;

    };

    // Public
    // //////////////////////////////////////////////////////////////////////

    template <typename T>
    Ptr_M<T>::Ptr_M(T* aInit) : mInternal(aInit) { assert(nullptr != aInit); }

    template <typename T>
    Ptr_M<T>::~Ptr_M() { assert(nullptr != mInternal); delete mInternal; }

    template <typename T>
    const T* Ptr_M<T>::operator -> () const { assert(nullptr != mInternal); return mInternal; }

    template <typename T>
    T* Ptr_M<T>::operator -> () { assert(nullptr != mInternal); return mInternal; }

    template <typename T>
    Ptr_M<T>::operator const T * () const { assert(nullptr != mInternal); return mInternal; }

    template <typename T>
    Ptr_M<T>::operator T * () { assert(nullptr != mInternal); return mInternal; }

    template <typename T>
    Ptr_O<T>::Ptr_O(T* aInit) : mInternal(aInit) {}

    template <typename T>
    Ptr_O<T>::~Ptr_O() { Release(); }

    template <typename T>
    const T* Ptr_O<T>::operator -> () const { return mInternal; }

    template <typename T>
    T* Ptr_O<T>::operator -> () { return mInternal; }

    template <typename T>
    Ptr_O<T>::operator const T* () const { return mInternal; }

    template <typename T>
    Ptr_O<T>::operator T* () { return mInternal; }

    template <typename T>
    void Ptr_O<T>::operator = (T* aSet) { Release(); mInternal = aSet; }

    template <typename T>
    T* Ptr_O<T>::Detach()
    {
        auto lResult = mInternal;

        mInternal = nullptr;

        return lResult;
    }

    template <typename T>
    void Ptr_O<T>::Release()
    {
        if (nullptr != mInternal)
        {
            delete mInternal;
            mInternal = nullptr;
        }
    }

    template <typename T>
    Ptr_OF<T>::Ptr_OF() : mInternal(nullptr)
    {
        mFlags.mConst  = false;
        mFlags.mDelete = false;
    }

    template <typename T>
    Ptr_OF<T>::Ptr_OF(const T* aInit) : mInternal(const_cast<T*>(aInit))
    {
        mFlags.mConst  = (nullptr != aInit);
        mFlags.mDelete = false;
    }

    template <typename T>
    Ptr_OF<T>::Ptr_OF(T* aInit, bool aDelete) : mInternal(aInit)
    {
        mFlags.mConst  = false;
        mFlags.mDelete = aDelete && (nullptr != aInit);
    }

    template <typename T>
    Ptr_OF<T>::~Ptr_OF() { Release(); }

    template <typename T>
    const T* Ptr_OF<T>::operator -> () const { return mInternal; }

    template <typename T>
    T* Ptr_OF<T>::operator -> () { return Get(); }

    template <typename T>
    Ptr_OF<T>::operator const T* () const { return mInternal; }

    template <typename T>
    Ptr_OF<T>::operator T* () { return Get(); }

    template <typename T>
    void Ptr_OF<T>::operator = (const T* aSet)
    {
        Release();
        Set(aSet);
    }

    template <typename T>
    const T* Ptr_OF<T>::Get() const { return mInternal; }

    template <typename T>
    T* Ptr_OF<T>::Get()
    {
        if (mFlags.mConst)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, KMS::RESULT_DENIED, "Pointer is const", "");
        }

        return mInternal;
    }

    template <typename T>
    bool Ptr_OF<T>::IsConst() const { return mFlags.mConst; }

    template <typename T>
    void Ptr_OF<T>::Set(Ptr_OF<T>& aSet)
    {
        Release();
        
        mFlags.mConst = aSet.mFlags.mConst;
        mFlags.mDelete = aSet.mFlags.mDelete;
        mInternal = aSet.Detach();
    }

    template <typename T>
    void Ptr_OF<T>::Set(const T* aSet)
    {
        Release();

        mFlags.mConst = true;
        mFlags.mDelete = false;
        mInternal = const_cast<T*>(aSet);
    }

    template <typename T>
    void Ptr_OF<T>::Set(T* aSet, bool aDelete)
    {
        Release();

        mFlags.mConst = false;
        mFlags.mDelete = aDelete;
        mInternal = aSet;
    }

    template <typename T>
    T* Ptr_OF<T>::Detach()
    {
        T* lResult = mInternal;

        mFlags.mConst = false;
        mFlags.mDelete = false;
        mInternal = nullptr;

        return lResult;
    }

    template <typename T>
    void Ptr_OF<T>::Release()
    {
        if (mFlags.mDelete && (nullptr != mInternal))
        {
            delete mInternal;
        }

        mFlags.mConst = false;
        mFlags.mDelete = false;
        mInternal = nullptr;
    }

    // Internal
    // //////////////////////////////////////////////////////////////////////

    template <typename T>
    Ptr_OF<T>::Ptr_OF(const Ptr_OF& aIn)
    {
        mFlags.mConst = aIn.mFlags.mConst;
        mFlags.mDelete = aIn.mFlags.mDelete;

        auto lIn = const_cast<Ptr_OF*>(&aIn);

        mInternal = lIn->Detach();
    }

    template <typename T>
    void Ptr_OF<T>::operator = (Ptr_OF& aIn)
    {
        if (nullptr != aIn.mInternal)
        {
            throw Exception(__FILE__, __FUNCTION__, __LINE__, KMS::RESULT_NOT_IMPLEMENTED, "Assigning not null Ptr_OF", "");
        }

        mFlags.mConst = false;
        mFlags.mDelete = false;
        mInternal = nullptr;
    }

}

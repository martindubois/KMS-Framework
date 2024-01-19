
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Ptr.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Exception.h>

namespace KMS
{

    template <typename T>
    class Ptr_M
    {

    public:

        Ptr_M(T* aInit);

        ~Ptr_M();

        const T* operator -> () const;

        T* operator -> ();

        operator const T* () const;

        operator T* ();

    private:

        Ptr_M(const Ptr_M &);
        void operator = (const Ptr_M&);

        T* mInternal;

    };

    template <typename T>
    class Ptr_O
    {

    public:

        Ptr_O(T* aInit = nullptr);

        ~Ptr_O();

        const T* operator -> () const;

        T* operator -> ();

        operator const T* () const;

        operator T* ();

        void operator = (T* aSet);

        T* Detach();

        void Release();

    private:

        Ptr_O(const Ptr_O &);
        void operator = (const Ptr_O&);

        T* mInternal;

    };

    template <typename T>
    class Ptr_OF
    {

    public:

        Ptr_OF();

        Ptr_OF(const T* aInit);

        Ptr_OF(T* aInit, bool aDelete);

        ~Ptr_OF();

        const T* operator -> () const;
        
        T* operator -> ();

        operator const T* () const;

        operator T* ();

        void operator = (const T* aSet);

        const T* Get() const;

        T* Get();

        bool IsConst() const;

        void Set(const T* aSet);

        void Set(T* aSet, bool aDelete);

        T* Detach();

        void Release();

    // Internal

        Ptr_OF(const Ptr_OF& aIn);

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
    void Ptr_OF<T>::Set(const T* aSet)
    {
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
        if (mFlags.mDelete)
        {
            assert(nullptr != mInternal);

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

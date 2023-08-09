
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Container.cpp

// TEST COVERAGE  2023-07-28  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Container::Container() : mCreator(nullptr) {}

        void Container::SetCreator(Creator aC) { mCreator = aC; }

        // ===== Object =====================================================

        Container::~Container() {}

        // Internal
        // //////////////////////////////////////////////////////////////////

        Container::Entry::Entry(const Object* aObject) : mObject(const_cast<Object*>(aObject))
        {
            assert(nullptr != aObject);

            mFlags.mConst  = true;
            mFlags.mDelete = false;
        }

        Container::Entry::Entry(Object* aObject, bool aDelete) : mObject(aObject)
        {
            assert(nullptr != aObject);

            mFlags.mConst  = false;
            mFlags.mDelete = aDelete;
        }

        Container::Entry::operator const Object* () const { return Get(); }

        Container::Entry::operator Object* () { return Get(); }

        const Object* Container::Entry::Get() const { assert(nullptr != mObject); return mObject; }

        Object* Container::Entry::Get()
        {
            assert(nullptr != mObject);

            KMS_EXCEPTION_ASSERT(!mFlags.mConst, DI_DENIED, "Trying to modify a constant Object", "");

            return mObject;
        }

        bool Container::Entry::IsConst() const { return mFlags.mConst; }

        // NOT TESTED
        void Container::Entry::Set(const Object* aObject)
        {
            assert(nullptr != aObject);

            Release();

            mFlags.mConst  = true;
            mFlags.mDelete = false;
            mObject        = const_cast<Object*>(aObject);
        }

        void Container::Entry::Set(Object* aObject, bool aDelete)
        {
            assert(nullptr != aObject);

            Release();

            mFlags.mConst  = false;
            mFlags.mDelete = aDelete;
            mObject        = aObject;
        }

        void Container::Entry::Release()
        {
            assert(nullptr != mObject);

            if (mFlags.mDelete)
            {
                delete mObject;
            }

            mObject = nullptr;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        bool Container::IsDynamic() { return nullptr != mCreator; }

        Object* Container::CallCreator()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mCreator, DI_NOT_DYNAMIC, "The container is not dynamic", "");

            return mCreator();
        }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Container.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Container::Container() : mCreator(NULL) {}

        void Container::SetCreator(Creator aC) { mCreator = aC; }

        // ===== Object =====================================================

        Container::~Container() {}

        // Internal
        // //////////////////////////////////////////////////////////////////

        Container::Entry::Entry(const Object* aObject) : mObject(const_cast<Object*>(aObject))
        {
            assert(NULL != aObject);

            mFlags.mConst  = true;
            mFlags.mDelete = false;
        }

        Container::Entry::Entry(Object* aObject, bool aDelete) : mObject(aObject)
        {
            assert(NULL != aObject);

            mFlags.mConst  = false;
            mFlags.mDelete = aDelete;
        }

        Container::Entry::operator const Object* () const { return Get(); }

        Container::Entry::operator Object* () { return Get(); }

        const Object* Container::Entry::Get() const { assert(NULL != mObject); return mObject; }

        Object* Container::Entry::Get()
        {
            assert(NULL != mObject);

            KMS_EXCEPTION_ASSERT(!mFlags.mConst, DI_DENIED, "Trying to modify a constant Object", "");

            return mObject;
        }

        void Container::Entry::Set(const Object* aObject)
        {
            assert(NULL != aObject);

            Release();

            mFlags.mConst  = true;
            mFlags.mDelete = false;
            mObject        = const_cast<Object*>(aObject);
        }

        void Container::Entry::Set(Object* aObject, bool aDelete)
        {
            assert(NULL != aObject);

            Release();

            mFlags.mConst  = false;
            mFlags.mDelete = aDelete;
            mObject        = aObject;
        }

        void Container::Entry::Release()
        {
            assert(NULL != mObject);

            if (mFlags.mDelete)
            {
                delete mObject;
            }

            mObject = NULL;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Object* Container::CallCreator()
        {
            KMS_EXCEPTION_ASSERT(NULL != mCreator, DI_NOT_DYNAMIC, "The container is not dynamic", "");

            return mCreator();
        }

    }
}

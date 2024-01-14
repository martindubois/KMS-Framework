
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Container.cpp

// TEST COVERAGE  2023-08-09  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>

KMS_RESULT_STATIC(RESULT_NOT_DYNAMIC);

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

        // Protected
        // //////////////////////////////////////////////////////////////////

        bool Container::IsDynamic() { return nullptr != mCreator; }

        Object* Container::CallCreator()
        {
            KMS_EXCEPTION_ASSERT(nullptr != mCreator, RESULT_NOT_DYNAMIC, "The container is not dynamic", "");

            return mCreator();
        }

    }
}

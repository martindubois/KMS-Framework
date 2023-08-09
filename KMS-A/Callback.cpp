
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Callback.cpp

// TEST COVERAGE  2023-08-05  KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Callback.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Callback_Ptr::Callback_Ptr() : mCallback(nullptr) {}

    void Callback_Ptr::operator = (const ICallback* aCallback) { mCallback = const_cast<ICallback*>(aCallback); }

    // NOT TESTED
    void Callback_Ptr::Clear() { mCallback = nullptr; }

    bool Callback_Ptr::IsSet() { return nullptr != mCallback; }

    unsigned int Callback_Ptr::Send(void* aSender, void* aData)
    {
        assert(nullptr != aSender);

        unsigned int lResult = ICallback::FLAG_NOT_SET;

        if (IsSet())
        {
            assert(nullptr != mCallback);
            
            lResult = mCallback->Send(aSender, aData);
        }

        return lResult;
    }

}

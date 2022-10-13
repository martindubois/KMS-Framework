
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Folder.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Folder.h>

namespace KMS
{
    namespace DI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Object* Folder::Create() { return new Folder; }

        Folder::Folder() {}

        Folder::Folder(const File::Folder& aIn) : String_Expand(aIn.GetPath()), mInternal(aIn) {}

        void Folder::operator = (const File::Folder& aIn)
        {
            mInternal = aIn;

            String_Expand::Set(aIn.GetPath());
        }

        Folder::operator const File::Folder& () const { return mInternal; }

        const File::Folder& Folder::Get() const { return mInternal; }

        // ===== Value ======================================================

        void Folder::Set(const char* aIn)
        {
            String_Expand::Set(aIn);

            mInternal = String_Expand::Get();
        }

        // ===== Object =====================================================
        Folder::~Folder() {}

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Folder.cpp

// TEST COVERAGE 2022-10-26 KMS - Martin Dubois, P. Eng.

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

        // ===== Object =====================================================
        Folder::~Folder() {}

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Object =====================================================

        void Folder::Send_OnChanged(void* aData)
        {
            auto lIn = String_Expand::Get();

            if      (0 == strncmp("EXECUTABLE:"       , lIn, 11)) { mInternal = File::Folder(File::Folder::EXECUTABLE       , lIn + 11); }
            else if (0 == strncmp("HOME:"             , lIn,  5)) { mInternal = File::Folder(File::Folder::HOME             , lIn +  5); }
            else if (0 == strncmp("PROGRAM_FILES:"    , lIn, 14)) { mInternal = File::Folder(File::Folder::PROGRAM_FILES    , lIn + 14); }
            else if (0 == strncmp("PROGRAM_FILES_X86:", lIn, 19)) { mInternal = File::Folder(File::Folder::PROGRAM_FILES_X86, lIn + 19); }
            else
            {
                mInternal = File::Folder(lIn);
            }

            String_Expand::Send_OnChanged(aData);
        }

    }
}

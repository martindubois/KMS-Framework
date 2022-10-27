
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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

        // ===== Value ======================================================

        void Folder::Set(const char* aIn)
        {
            String_Expand::Set(aIn);

            const char* lStr = String_Expand::Get();

            if      (0 == strncmp("EXECUTABLE:"       , lStr, 11)) { mInternal = File::Folder(File::Folder::EXECUTABLE       , lStr + 11); }
            else if (0 == strncmp("HOME:"             , lStr,  5)) { mInternal = File::Folder(File::Folder::HOME             , lStr +  5); }
            else if (0 == strncmp("PROGRAM_FILES:"    , lStr, 14)) { mInternal = File::Folder(File::Folder::PROGRAM_FILES    , lStr + 14); }
            else if (0 == strncmp("PROGRAM_FILES_X86:", lStr, 19)) { mInternal = File::Folder(File::Folder::PROGRAM_FILES_X86, lStr + 19); }
            else
            {
                mInternal = File::Folder(lStr);
            }
        }

        // ===== Object =====================================================
        Folder::~Folder() {}

    }
}

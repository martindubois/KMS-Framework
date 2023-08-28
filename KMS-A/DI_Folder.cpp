
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Folder.cpp

// TEST COVERAGE

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

        Folder::Folder(const File::Folder& aIn) : Folder_Base(aIn.GetPath()), mInternal(aIn) {}

        Folder_Ptr::Folder_Ptr(File::Folder* aFolder) : mInternal(aFolder) {}

        void Folder_Base::operator = (const File::Folder& aIn)
        {
            Internal_Set(aIn);

            String_Expand::Set(aIn.GetPath());
        }

        Folder_Base::operator const File::Folder& () const { return GetFolder(); }

        // ===== Folder_Base ================================================

        const File::Folder& Folder    ::GetFolder() const { return  mInternal; }
        const File::Folder& Folder_Ptr::GetFolder() const { return *mInternal; }

        // ===== Object =====================================================

        Folder_Base::~Folder_Base() {}

        Folder::~Folder() {}

        Folder_Ptr::~Folder_Ptr() {}

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Object =====================================================

        void Folder_Base::Send_OnChanged(void* aData)
        {
            auto lIn = String_Expand::Get();

            if      (0 == strncmp("EXECUTABLE:"       , lIn, 11)) { Internal_Set(File::Folder(File::Folder::EXECUTABLE       , lIn + 11)); }
            else if (0 == strncmp("HOME:"             , lIn,  5)) { Internal_Set(File::Folder(File::Folder::HOME             , lIn +  5)); }

            #ifdef _KMS_WINDOWS_
                else if (0 == strncmp("PROGRAM_FILES:"    , lIn, 14)) { Internal_Set(File::Folder(File::Folder::PROGRAM_FILES    , lIn + 14)); }
                else if (0 == strncmp("PROGRAM_FILES_X86:", lIn, 19)) { Internal_Set(File::Folder(File::Folder::PROGRAM_FILES_X86, lIn + 19)); }
            #endif

            else
            {
                Internal_Set(File::Folder(lIn));
            }

            String_Expand::Send_OnChanged(aData);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Folder_Base::Folder_Base() {}

        Folder_Base::Folder_Base(const char* aIn) : String_Expand(aIn) {}

        // ===== Folder_Base ================================================

        void Folder::Internal_Set(const File::Folder& aIn) { mInternal = aIn; }

        void Folder_Ptr::Internal_Set(const File::Folder& aIn) { *mInternal = aIn; }

    }
}

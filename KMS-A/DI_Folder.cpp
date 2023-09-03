
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DI_Folder.cpp

// TEST COVERAGE  2023-08-30  KMS - Martin Dubois, P. Eng.

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

        Folder_Ptr::Folder_Ptr(File::Folder* aPtr) : String_Expand(aPtr->GetPath()), mPtr(aPtr)
        {
            assert(nullptr != aPtr);
        }

        Folder::Folder() : Folder_Ptr(&mInternal) {}

        Folder::Folder(const File::Folder& aIn) : Folder_Ptr(&mInternal, aIn.GetPath()), mInternal(aIn) {}

        void Folder_Ptr::operator = (const File::Folder& aIn)
        {
            assert(nullptr != mPtr);

            *mPtr = aIn;

            String_Expand::Set(aIn.GetPath());
        }

        Folder_Ptr::operator const File::Folder& () const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        const File::Folder& Folder_Ptr::GetFolder() const
        {
            assert(nullptr != mPtr);

            return *mPtr;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        // ===== Object =====================================================

        void Folder_Ptr::Send_OnChanged(void* aData)
        {
            assert(nullptr != mPtr);

            auto lIn = String_Expand::Get();

            if      (0 == strncmp("EXECUTABLE:", lIn, 11)) { *mPtr = File::Folder(File::Folder::EXECUTABLE, lIn + 11); }
            else if (0 == strncmp("HOME:"      , lIn,  5)) { *mPtr = File::Folder(File::Folder::HOME      , lIn +  5); }

            #ifdef _KMS_WINDOWS_
                else if (0 == strncmp("PROGRAM_FILES:"    , lIn, 14)) { *mPtr = File::Folder(File::Folder::PROGRAM_FILES    , lIn + 14); }
                else if (0 == strncmp("PROGRAM_FILES_X86:", lIn, 19)) { *mPtr = File::Folder(File::Folder::PROGRAM_FILES_X86, lIn + 19); }
            #endif

            else
            {
                *mPtr = File::Folder(lIn);
            }

            String_Expand::Send_OnChanged(aData);
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Folder_Ptr::Folder_Ptr(File::Folder* aPtr, const char* aIn) : String_Expand(aIn), mPtr(aPtr)
        {
            assert(nullptr != aPtr);
        }

    }
}

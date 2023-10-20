
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Folder.h
// Library   KMS-A

// External type : KMS::File::Folder (const char*)
// Internal type : KMS::File::Folder (str::string)

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace DI
    {

        class Folder_Ptr : public String_Expand
        {

        public:

            Folder_Ptr(KMS::File::Folder* aPtr);

            void operator = (const KMS::File::Folder& aIn);

            operator const KMS::File::Folder& () const;

            const KMS::File::Folder& GetFolder() const;

        // Internal

            Folder_Ptr();

            void Init(KMS::File::Folder* aPtr);

            // ===== Object =================================================
            virtual void Send_OnChanged(void* aData);

        protected:

            Folder_Ptr(KMS::File::Folder* aPtr, const char* aIn);

        private:

            KMS::File::Folder* mPtr;

        };

        class Folder final : public Folder_Ptr
        {

        public:

            static DI::Object* Create();

            Folder();

            Folder(const KMS::File::Folder& aFolder);

            // ===== Folder_Ptr =============================================
            using Folder_Ptr::operator =;
            using Folder_Ptr::operator const KMS::File::Folder&;

        private:

            KMS::File::Folder mInternal;

        };

    }
}

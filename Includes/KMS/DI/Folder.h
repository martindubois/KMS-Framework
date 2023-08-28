
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/Folder.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace DI
    {

        class Folder_Base : public String_Expand
        {

        public:

            void operator = (const KMS::File::Folder& aIn);

            operator const KMS::File::Folder& () const;

            virtual const KMS::File::Folder& GetFolder() const = 0;

            // ===== Object =================================================
            virtual ~Folder_Base();

        // Internal

            // ===== Object =================================================
            virtual void Send_OnChanged(void* aData);

        protected:

            Folder_Base();

            Folder_Base(const char* aIn);

            virtual void Internal_Set(const KMS::File::Folder& aIn) = 0;

        };

        class Folder : public Folder_Base
        {

        public:

            static DI::Object* Create();

            Folder();

            Folder(const KMS::File::Folder& aFolder);

            // ===== Folder_Base ============================================
            virtual const KMS::File::Folder& GetFolder() const;

            // ===== Object =================================================
            virtual ~Folder();

        // Internal

            KMS::File::Folder mInternal;

        protected:

            // ===== Folder_Base ============================================
            virtual void Internal_Set(const KMS::File::Folder& aIn);

        };

        class Folder_Ptr : public Folder_Base
        {

        public:

            Folder_Ptr(KMS::File::Folder* aFolder);

            // ===== Folder_Base ============================================
            virtual const KMS::File::Folder& GetFolder() const;

            // ===== Object =================================================
            virtual ~Folder_Ptr();

        // Internal

            KMS::File::Folder* mInternal;

        protected:

            // ===== Folder_Base ============================================
            virtual void Internal_Set(const KMS::File::Folder& aIn);

        };

    }
}

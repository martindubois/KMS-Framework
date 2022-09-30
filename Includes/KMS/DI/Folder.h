
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
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

        class Folder : public String_Expand
        {

        public:

            static DI::Object* Create();

            Folder();

            Folder(const File::Folder& aFolder);

            void operator = (const File::Folder& aIn);

            operator const File::Folder& () const;

            const File::Folder& Get() const;

            // ===== Value ==================================================
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual ~Folder();

        private:

            File::Folder mInternal;

        };

    }
}

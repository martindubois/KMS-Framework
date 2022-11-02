
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

            Folder(const KMS::File::Folder& aFolder);

            void operator = (const KMS::File::Folder& aIn);

            operator const KMS::File::Folder& () const;

            const KMS::File::Folder& Get() const;

            // ===== Object =================================================
            virtual ~Folder();

        // Internal

            // ===== Object =================================================
            virtual void Send_OnChanged(void* aData);

            KMS::File::Folder mInternal;

        };

    }
}

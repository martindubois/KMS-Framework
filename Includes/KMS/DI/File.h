
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/File.h

#pragma once

// ===== C ==================================================================
#include <stdio.h>

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>

namespace KMS
{
    namespace DI
    {

        class File : public String_Expand
        {

        public:

            static DI::Object* Create(KMS::DI::MetaData* aMD);

            File(FILE* aFile, const char* aIn, const MetaData* aMD);

            operator FILE * ();

            void SetMode(const char* aMode);

            void Close();

            // ===== Value ==================================================
            virtual void Set(const char* aIn);

            // ===== Object =================================================
            virtual ~File();

        private:

            FILE* mInternal;

            const char* mMode;

        };

    }
}

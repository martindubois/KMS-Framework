
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/File.h
// Library   KMS-A

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

            static DI::Object* Create();

            File();

            File(FILE* aFile, const char* aIn);

            operator FILE * ();

            bool IsOpen() const;

            void SetMode(const char* aMode);

            void Close();

            // ===== Object =================================================
            virtual ~File();

        // Internal

            // ===== Object =================================================
            virtual void Send_OnChanged(void* aData);

            FILE* mInternal;

        private:

            const char* mMode;

        };

    }
}

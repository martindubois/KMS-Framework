
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DI/File.h
// Library   KMS-A

// External type : FILE* (const char*)
// Internal type : FILE* (str::string)

#pragma once

// ===== C ==================================================================
#include <stdio.h>

// ===== Includes ===========================================================
#include <KMS/DI/String_Expand.h>

namespace KMS
{
    namespace DI
    {

        class File_Ptr : public String_Expand
        {

        public:

            static const char* DEFAULT_MODE;
            static const char* DEFAULT_VALUE;

            File_Ptr(FILE** aPtr, const char* aIn = DEFAULT_VALUE);

            operator FILE * ();

            bool IsOpen() const;

            void SetMode(const char* aMode);

            void Close();

            // ===== String_Expand ==========================================
            using String_Expand::operator =;

            // ===== Object =================================================
            virtual ~File_Ptr();
            virtual bool Clear();

        // Internal

            // ===== Object =================================================
            virtual void Send_OnChanged(void* aData);

        private:

            FILE** mPtr;

            const char* mMode;

        };

        class File : public File_Ptr
        {

        public:

            static DI::Object* Create();

            File();

            File(FILE* aFile, const char* aIn);

            // ===== File_Ptr ===============================================
            using File_Ptr::operator =;
            using File_Ptr::operator FILE*;

        private:

            FILE* mInternal;

        };

    }
}

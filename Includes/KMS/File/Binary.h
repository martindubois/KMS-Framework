
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/Binary.h

#pragma once

// ===== Windows ============================================================
#include <Windows.h>

namespace KMS
{
    namespace File
    {

        class Folder;

        class Binary
        {

        public:

            Binary(const Folder& aFolder, const char* aFile, bool aWrite = false);

            ~Binary();

            unsigned int GetMappedSize() const;

            unsigned int GetSize();

            void* Map(unsigned int aMinSize_byte = 0, unsigned int aMaxSize_byte = 0xffffffff);

            unsigned int Read(void* aOut, unsigned int aOutSize_byte);

            void Write(const void* aIn, unsigned int aInSize_byte);

        private:

            HANDLE       mHandle;
            unsigned int mMappedSize_byte;
            HANDLE       mMapping;
            void       * mView;
            bool         mWrite;

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/Binary.h
// Status    PROD_READY (Windows)
// Library   KMS-A

#pragma once

#ifdef _KMS_WINDOWS_
    // ===== Windows ========================================================
    #include <Windows.h>
#endif

namespace KMS
{
    namespace File
    {

        class Folder;

        class Binary final
        {

        public:

            // Exception  FILE_OPEN_FAILED
            Binary(const Folder& aFolder, const char* aFile, bool aWrite = false);

            ~Binary();

            unsigned int GetMappedSize() const;

            // Exception  FILE_ACCESS_FAILED
            unsigned int GetSize();

            void Close();

            // Exception  FILE_TOO_SHORT       Trying to map longer than the file
            //            FILE_MAPPING_FAILED
            void* Map(unsigned int aMinSize_byte = 0, unsigned int aMaxSize_byte = 0xffffffff);

            // Exception  FILE_READ_FAILED
            unsigned int Read(void* aOut, unsigned int aOutSize_byte);

            // Exception  FILE_WRITE_FAILED
            void Write(const void* aIn, unsigned int aInSize_byte);

        private:

            void Construct_OSDep(const char* aPath);

            unsigned int mMappedSize_byte;
            void       * mView;
            bool         mWrite;

            #ifdef _KMS_LINUX_
                int mHandle;
            #endif

            #ifdef _KMS_WINDOWS_
                HANDLE mHandle;
                HANDLE mMapping;
            #endif

        };

    }
}

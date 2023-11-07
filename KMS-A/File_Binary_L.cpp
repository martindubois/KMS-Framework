
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/File_Binary_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <fcntl.h>
#include <unistd.h>

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Binary::Close()
        {
            if(-1 != mHandle);
            {
                auto lRet = close(mHandle);
                assert(0 == lRet);

                mHandle = -1;
            }
        }

        unsigned int Binary::Read(void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);

            assert(-1 != mHandle);

            auto lRet = read(mHandle, aOut, aOutSize_byte);
            KMS_EXCEPTION_ASSERT(0 <= lRet, RESULT_READ_FAILED, "Cannot read the binary file (NOT TESTED)", lRet);

            return lRet;
        }

        void Binary::Write(const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);

            assert(-1 != mHandle);

            auto lRet = write(mHandle, aIn, aInSize_byte);
            KMS_EXCEPTION_ASSERT(aInSize_byte == lRet, RESULT_WRITE_FAILED, "Cannot write the binary file", lRet);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Binary::Construct_OSDep(const char* aPath)
        {
            int lFlags = O_NOCTTY;

            if (mWrite)
            {
                lFlags |= O_CREAT | O_RDWR;
            }
            else
            {
                lFlags |= O_RDONLY;
            }

            mHandle = open(aPath, lFlags);
            if (-1 == mHandle)
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open %s", aPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, lFlags);
            }
        }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/File_Binary_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <fcntl.h>
#include <unistd.h>

// ===== System =============================================================
#include <sys/mman.h>
#include <sys/stat.h>

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>

KMS_RESULT_STATIC(RESULT_ACCESS_FAILED);
KMS_RESULT_STATIC(RESULT_MAPPING_FAILED);
KMS_RESULT_STATIC(RESULT_TOO_SHORT);

// Constants
// //////////////////////////////////////////////////////////////////////////

#define INVALID_HANDLE_VALUE (-1)

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        unsigned int Binary::GetSize()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            struct stat lStat;

            auto lRet = fstat(mHandle, &lStat);
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_ACCESS_FAILED, "gstat failed", lRet);

            return lStat.st_size;
        }

        void Binary::Close()
        {
            if (INVALID_HANDLE_VALUE != mHandle)
            {
                if (nullptr != mView)
                {
                    auto lRet = munmap(mView, mMappedSize_byte);
                    assert(0 == lRet);

                    mView = nullptr;
                }

                auto lRet = close(mHandle);
                assert(0 == lRet);

                mHandle = -1;
            }
        }

        void* Binary::Map(unsigned int aMinSize_byte, unsigned int aMaxSize_byte)
        {
            assert(aMinSize_byte <= aMaxSize_byte);

            assert(INVALID_HANDLE_VALUE != mHandle);
            assert(nullptr == mView);

            int lProt = PROT_READ;

            auto lSize_byte = GetSize();

            mMappedSize_byte = lSize_byte;

            if (mWrite)
            {
                lProt |= PROT_WRITE;

                if (mMappedSize_byte < aMinSize_byte)
                {
                    mMappedSize_byte = aMinSize_byte;

                    auto lRet = ftruncate(mHandle, mMappedSize_byte);
                    KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_MAPPING_FAILED, "ftruncate failed", lRet);
                }
            }
            else
            {
                KMS_EXCEPTION_ASSERT(aMinSize_byte <= lSize_byte, RESULT_TOO_SHORT, "The file is too short", lSize_byte);
            }

            if (mMappedSize_byte > aMaxSize_byte)
            {
                // NOT TESTED
                mMappedSize_byte = aMaxSize_byte;
            }

            mView = mmap(nullptr, mMappedSize_byte, lProt, MAP_SHARED, mHandle, 0);
            KMS_EXCEPTION_ASSERT(MAP_FAILED != mView, RESULT_MAPPING_FAILED, "mmap failed", "");
            assert(nullptr != mView);

            return mView;
        }

        unsigned int Binary::Read(void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);

            assert(INVALID_HANDLE_VALUE != mHandle);

            auto lRet = read(mHandle, aOut, aOutSize_byte);
            KMS_EXCEPTION_ASSERT(0 <= lRet, RESULT_READ_FAILED, "Cannot read the binary file (NOT TESTED)", lRet);

            return lRet;
        }

        void Binary::Write(const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);

            assert(INVALID_HANDLE_VALUE != mHandle);

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

                mHandle = open(aPath, lFlags, S_IRUSR | S_IWUSR | S_IRGRP);
            }
            else
            {
                lFlags |= O_RDONLY;

                mHandle = open(aPath, lFlags);
            }

            if (INVALID_HANDLE_VALUE == mHandle)
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open %s", aPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, lFlags);
            }
        }

    }
}

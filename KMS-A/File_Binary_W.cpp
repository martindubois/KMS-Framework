
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/File_Binary_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Binary.h>

namespace KMS
{
    namespace File
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Binary::Construct_OSDep(const char* aPath)
        {
            assert(nullptr != aPath);

            mMapping = nullptr;

            DWORD lAccess = GENERIC_READ;
            DWORD lDispo;
            DWORD lShare = FILE_SHARE_READ;

            if (mWrite)
            {
                lAccess |= GENERIC_WRITE;
                lDispo = CREATE_ALWAYS;
            }
            else
            {
                lDispo = OPEN_EXISTING;
                lShare |= FILE_SHARE_WRITE;
            }

            mHandle = CreateFile(aPath, lAccess, FILE_SHARE_READ, nullptr, lDispo, 0, nullptr);
            if (INVALID_HANDLE_VALUE == mHandle)
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open %s", aPath);
                KMS_EXCEPTION(RESULT_OPEN_FAILED, lMsg, lAccess);
            }
        }

        unsigned int Binary::GetSize()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            LARGE_INTEGER lResult_byte;

            if (!GetFileSizeEx(mHandle, &lResult_byte))
            {
                KMS_EXCEPTION(RESULT_ACCESS_FAILED, "Cannot retrieve the size of the binary file (NOT TESTED)", "");
            }

            assert(0 == lResult_byte.HighPart);

            return lResult_byte.LowPart;
        }

        void Binary::Close()
        {
            if (INVALID_HANDLE_VALUE != mHandle)
            {
                BOOL lRet;

                if (nullptr != mMapping)
                {
                    if (nullptr != mView)
                    {
                        lRet = UnmapViewOfFile(mView);
                        assert(lRet);

                        mView = nullptr;
                    }

                    lRet = CloseHandle(mMapping);
                    assert(lRet);

                    mMapping = nullptr;
                }

                lRet = CloseHandle(mHandle);
                assert(lRet);

                mHandle = INVALID_HANDLE_VALUE;
            }
        }

        void* Binary::Map(unsigned int aMinSize_byte, unsigned int aMaxSize_byte)
        {
            DWORD lAccess = FILE_MAP_READ;
            DWORD lProtect;

            auto lSize_byte = GetSize();

            mMappedSize_byte = lSize_byte;

            if (mWrite)
            {
                lAccess |= FILE_MAP_WRITE;
                lProtect = PAGE_READWRITE;

                if (mMappedSize_byte < aMinSize_byte)
                {
                    mMappedSize_byte = aMinSize_byte;
                }
            }
            else
            {
                lProtect = PAGE_READONLY;

                KMS_EXCEPTION_ASSERT(aMinSize_byte <= lSize_byte, RESULT_TOO_SHORT, "The file is too short", lSize_byte);
            }

            if (mMappedSize_byte > aMaxSize_byte)
            {
                // NOT TESTED
                mMappedSize_byte = aMaxSize_byte;
            }

            mMapping = CreateFileMapping(mHandle, nullptr, lProtect, 0, mMappedSize_byte, nullptr);
            KMS_EXCEPTION_ASSERT(nullptr != mMapping, RESULT_MAPPING_FAILED, "Cannot map the file", aMaxSize_byte);

            mView = MapViewOfFile(mMapping, lAccess, 0, 0, mMappedSize_byte);
            KMS_EXCEPTION_ASSERT(nullptr != mView, RESULT_MAPPING_FAILED, "Cannot map the file", mMappedSize_byte);

            return mView;
        }

        unsigned int Binary::Read(void* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lResult_byte = aOutSize_byte;

            if (!ReadFile(mHandle, aOut, aOutSize_byte, &lResult_byte, nullptr))
            {
                KMS_EXCEPTION(RESULT_READ_FAILED, "Cannot read the binary file (NOT TESTED)", aOutSize_byte);
            }

            return lResult_byte;
        }

        void Binary::Write(const void* aIn, unsigned int aInSize_byte)
        {
            assert(nullptr != aIn);

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lInfo_byte;

            if (!WriteFile(mHandle, aIn, aInSize_byte, &lInfo_byte, nullptr))
            {
                KMS_EXCEPTION(RESULT_WRITE_FAILED, "Cannot write the binary file", aInSize_byte);
            }

            KMS_EXCEPTION_ASSERT(aInSize_byte == lInfo_byte, RESULT_WRITE_FAILED, "Cannot write the binary file", lInfo_byte);
        }

    }
}

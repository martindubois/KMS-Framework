
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/File_Binary.cpp

// TEST COVERAGE 2023-07-22 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

#include <KMS/File/Binary.h>

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Binary::Binary(const Folder& aFolder, const char* aFile, bool aWrite)
            : mMappedSize_byte(0), mMapping(NULL), mView(NULL), mWrite(aWrite)
        {
            char lPath[PATH_LENGTH];

            aFolder.GetPath(aFile, lPath, sizeof(lPath));

            DWORD lAccess = GENERIC_READ;
            DWORD lDispo = OPEN_EXISTING;

            if (aWrite)
            {
                lAccess |= GENERIC_WRITE;
                lDispo = CREATE_ALWAYS;
            }

            mHandle = CreateFile(lPath, lAccess, 0, NULL, lDispo, 0, NULL);
            if (INVALID_HANDLE_VALUE == mHandle)
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot open %s", lPath);
                KMS_EXCEPTION(FILE_OPEN_FAILED, lMsg, lAccess);
            }
        }

        Binary::~Binary()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            BOOL lRet;

            if (NULL != mMapping)
            {
                if (NULL != mView)
                {
                    lRet = UnmapViewOfFile(mView);
                    assert(lRet);
                }

                lRet = CloseHandle(mMapping);
                assert(lRet);
            }

            lRet = CloseHandle(mHandle);
            assert(lRet);
        }

        unsigned int Binary::GetMappedSize() const { return mMappedSize_byte; }

        unsigned int Binary::GetSize()
        {
            assert(INVALID_HANDLE_VALUE != mHandle);

            LARGE_INTEGER lResult_byte;

            if (!GetFileSizeEx(mHandle, &lResult_byte))
            {
                // NOT TESTED
                KMS_EXCEPTION(FILE_ACCESS_FAILED, "Cannot retrieve the size of the binary file", "");
            }

            assert(0 == lResult_byte.HighPart);

            return lResult_byte.LowPart;
        }

        void* Binary::Map(unsigned int aMinSize_byte, unsigned int aMaxSize_byte)
        {
            auto lSize_byte = GetSize();

            KMS_EXCEPTION_ASSERT(aMinSize_byte <= lSize_byte, FILE_TOO_SHORT, "The file is too short", lSize_byte);

            mMappedSize_byte = (lSize_byte > aMaxSize_byte) ? aMaxSize_byte : lSize_byte;

            mMapping = CreateFileMapping(mHandle, NULL, PAGE_READONLY, 0, mMappedSize_byte, NULL);
            KMS_EXCEPTION_ASSERT(NULL != mMapping, FILE_MAPPING_FAILED, "Cannot map the file", aMaxSize_byte);

            mView = MapViewOfFile(mMapping, FILE_MAP_READ, 0, 0, mMappedSize_byte);
            KMS_EXCEPTION_ASSERT(NULL != mView, FILE_MAPPING_FAILED, "Cannot map the file", mMappedSize_byte);

            return mView;
        }

        unsigned int Binary::Read(void* aOut, unsigned int aOutSize_byte)
        {
            assert(NULL != aOut);

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lResult_byte = aOutSize_byte;

            if (!ReadFile(mHandle, aOut, aOutSize_byte, &lResult_byte, NULL))
            {
                // NOT TESTED
                KMS_EXCEPTION(FILE_READ_FAILED, "Cannot read the binary file", aOutSize_byte);
            }

            return lResult_byte;
        }

        void Binary::Write(const void* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            assert(INVALID_HANDLE_VALUE != mHandle);

            DWORD lInfo_byte;

            if (!WriteFile(mHandle, aIn, aInSize_byte, &lInfo_byte, NULL))
            {
                KMS_EXCEPTION(FILE_WRITE_FAILED, "Cannot write the binary file", aInSize_byte);
            }

            KMS_EXCEPTION_ASSERT(aInSize_byte == lInfo_byte, FILE_WRITE_FAILED, "Cannot write the binary file", lInfo_byte);
        }

    }
}

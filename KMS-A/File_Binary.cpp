
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/File_Binary.cpp

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
                KMS_EXCEPTION(FILE_OPEN_FAILED, "Cannot open binary file", lPath);
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

            mMapping = CreateFileMapping(mHandle, NULL, mWrite ? PAGE_READWRITE : PAGE_READONLY, 0, mMappedSize_byte, NULL);
            KMS_EXCEPTION_ASSERT(NULL != mMapping, FILE_MAPPING_FAILED, "Cannot map the file", aMaxSize_byte);

            DWORD lAccess = FILE_MAP_READ;

            if (mWrite)
            {
                lAccess |= FILE_MAP_WRITE;
            }

            mView = MapViewOfFile(mMapping, lAccess, 0, 0, mMappedSize_byte);
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
                KMS_EXCEPTION(FILE_READ_FAILED, "Cannot read the binary file", aOutSize_byte);
            }

            return lResult_byte;
        }

    }
}

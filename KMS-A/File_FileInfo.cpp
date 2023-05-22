
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_FileInfo.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/File/FileInfo.h>

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        FileInfo::~FileInfo()
        {
            assert(NULL != mData);

            auto lData = reinterpret_cast<WIN32_FIND_DATA*>(mData);

            delete lData;
        }

        bool FileInfo::IsNewer(const FileInfo& aA) const
        {
            return GetLastWrite() > aA.GetLastWrite();
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        FileInfo::FileInfo(const void* aData)
        {
            Allocate();

            SetData(aData);
        }

        FileInfo::FileInfo(const FileInfo& aFI)
        {
            Allocate();

            SetData(aFI.GetData());
        }

        void FileInfo::operator = (const FileInfo& aA) { SetData(aA.GetData()); }

        const void* FileInfo::GetData() const { assert(NULL != mData); return mData; }

        uint64_t FileInfo::GetLastWrite() const
        {
            assert(NULL != mData);

            auto lData = reinterpret_cast<const WIN32_FIND_DATA*>(mData);

            uint64_t lResult = lData->ftLastWriteTime.dwLowDateTime;

            lResult <<= 32;

            lResult |= lData->ftLastWriteTime.dwLowDateTime;

            return lResult;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void FileInfo::SetData(const void* aData)
        {
            assert(NULL != aData);

            assert(NULL != mData);

            memcpy(mData, aData, sizeof(WIN32_FIND_DATA));
        }

        void FileInfo::Allocate()
        {
            auto lData = new WIN32_FIND_DATA;

            mData = lData;
        }

    }
}

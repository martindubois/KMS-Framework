
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/FileInfo.h

#pragma once

namespace KMS
{
    namespace File
    {

        class FileInfo final
        {

        public:

            ~FileInfo();

            bool IsNewer(const FileInfo& aA) const;

        // Internal

            FileInfo(const void * aData);

            FileInfo(const FileInfo &);

            void operator = (const FileInfo& aA);

            const void* GetData() const;

            uint64_t GetLastWrite() const;

        private:

            void SetData(const void* aData);

            void Allocate();

            void * mData;

        };

    }
}

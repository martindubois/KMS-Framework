
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/FileList.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace File
    {

        class FileList
        {

        public:

            FileList(const Folder& aRoot, const char* aPattern, bool aRecurse);

            void Copy(const Folder& aNewRoot);

            unsigned int GetCount() const;

        private:

            FileList(const FileList &);

            const FileList & operator = (const FileList &);

            void Find(const char * aPattern, bool aRecurse);
            void FindInFolder(const char * aFullFolder, const char * aFolder, const char * aPattern);

            StringSet mFiles;
            StringSet mFolders;
            Folder    mRoot;

        };

    }
}

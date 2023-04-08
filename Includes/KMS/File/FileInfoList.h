
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/FileInfoList.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace File
    {

        class FileInfo;

        class FileInfoList
        {

        public:

            class Counters
            {

            public:

                Counters();

            // Internal

                void Display(std::ostream& aOut) const;

                void Increment(unsigned int aId);

            private:

                unsigned int mCounters[6];

            };

            static const unsigned int FLAG_IF_DOES_NOT_EXIST;
            static const unsigned int FLAG_IF_NEWER;

            FileInfoList(const Folder& aRoot, const char* aPattern, bool aRecurse);

            ~FileInfoList();

            unsigned int GetCount() const;

            const Folder& GetRoot() const;

            void Add(const char* aRelPath, FileInfo* aInfo);

            void Copy(const Folder& aNewRoot, unsigned int aFlags = 0);

            void Copy(FileInfoList* aList, unsigned int aFlags = 0, Counters* aCounters = NULL);

            FileInfo* Find(const char* aRelPath);

        // Internal

            void Display(std::ostream& aOut) const;

        private:

            NO_COPY(FileInfoList);

            void CreateFolders(const Folder& aNewToot);

            void Find(const char * aPattern, bool aRecurse);
            void FindInFolder(const char * aFullFolder, const char * aFolder, const char * aPattern);

            typedef std::map<std::string, FileInfo *> FileInfoMap;

            FileInfoMap mFiles;
           
            StringSet_ASCII mFolders;
            Folder          mRoot;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::File::FileInfoList          & aFIL);
std::ostream& operator << (std::ostream& aOut, const KMS::File::FileInfoList::Counters& aC);

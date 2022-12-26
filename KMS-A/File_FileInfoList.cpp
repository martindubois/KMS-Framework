
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_FileInfoList.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/File/FileInfo.h>

#include <KMS/File/FileInfoList.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define COUNTER_COPIED       (0)
#define COUNTER_EXISTING     (1)
#define COUNTER_NEW          (2)
#define COUNTER_NEW_COPIED   (3)
#define COUNTER_NEWER        (4)
#define COUNTER_NEWER_COPIED (5)

#define COUNTER_QTY (sizeof(FileInfoList::Counters) / sizeof(unsigned int))

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // The flag must be differenent than the one defined in
        // File_Folder.cpp.
        const unsigned int FileInfoList::FLAG_IF_DOES_NOT_EXIST = 0x00000001;
        const unsigned int FileInfoList::FLAG_IF_NEWER          = 0x00000002;

        FileInfoList::FileInfoList(const File::Folder& aRoot, const char* aPattern, bool aRecurse) : mRoot(aRoot)
        {
            assert(NULL != aPattern);

            Find(aPattern, aRecurse);
        }

        FileInfoList::~FileInfoList()
        {
            for (FileInfoMap::value_type lInfo : mFiles)
            {
                assert(NULL != lInfo.second);

                delete lInfo.second;
            }
        }

        unsigned int FileInfoList::GetCount() const { return static_cast<unsigned int>(mFiles.size()); }

        const Folder& FileInfoList::GetRoot() const { return mRoot; }

        void FileInfoList::Add(const char* aRelPath, FileInfo* aInfo)
        {
            assert(NULL != aRelPath);
            assert(NULL != aInfo);

            mFiles.insert(FileInfoMap::value_type(aRelPath, aInfo));
        }

        void FileInfoList::Copy(const Folder& aNewRoot, unsigned int aFlags)
        {
            CreateFolders(aNewRoot);

            for (FileInfoMap::value_type lInfo : mFiles)
            {
                mRoot.Copy(aNewRoot, lInfo.first.c_str(), aFlags);
            }
        }

        void FileInfoList::Copy(FileInfoList* aList, unsigned int aFlags, Counters* aCounters)
        {
            assert(NULL != aList);
            assert(0 != aFlags);

            const Folder& lNewRoot = aList->GetRoot();

            CreateFolders(lNewRoot);

            for (FileInfoMap::value_type& lPair : mFiles)
            {
                bool lCopy = false;

                FileInfo * lFI = aList->Find(lPair.first.c_str());
                if (NULL == lFI)
                {
                    if (aCounters) { aCounters->Increment(COUNTER_NEW); }

                    lCopy = (FLAG_IF_DOES_NOT_EXIST == (aFlags & FLAG_IF_DOES_NOT_EXIST));
                    if (lCopy)
                    {
                        if (aCounters) { aCounters->Increment(COUNTER_NEW_COPIED); }

                        aList->Add(lPair.first.c_str(), new FileInfo(*lPair.second));
                    }
                }
                else
                {
                    if (aCounters) { aCounters->Increment(COUNTER_EXISTING); }

                    if (lPair.second->IsNewer(*lFI))
                    {
                        if (aCounters) { aCounters->Increment(COUNTER_NEWER); }

                        lCopy = (FLAG_IF_NEWER == (aFlags & FLAG_IF_NEWER));
                        if (lCopy)
                        {
                            if (aCounters) { aCounters->Increment(COUNTER_NEWER_COPIED); }

                            *lFI = *lPair.second;
                        }
                    }
                }

                if (lCopy)
                {
                    if (aCounters) { aCounters->Increment(COUNTER_COPIED); }

                    mRoot.Copy(lNewRoot, lPair.first.c_str(), aFlags);
                }
            }
        }

        FileInfo* FileInfoList::Find(const char* aRelPath)
        {
            assert(NULL != aRelPath);

            FileInfo* lResult = NULL;

            FileInfoMap::iterator lIt = mFiles.find(aRelPath);
            if (mFiles.end() != lIt)
            {
                assert(NULL != lIt->second);

                lResult = lIt->second;
            }

            return lResult;
        }

        FileInfoList::Counters::Counters()
        {
            memset(&mCounters, 0, sizeof(mCounters));
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        void FileInfoList::Display(std::ostream& aOut) const
        {
            aOut << mRoot.GetPath() << "\n";
            aOut << "    " << mFolders.size() << " folders\n";
            aOut << "    " << mFiles  .size() << " files\n";
            aOut << std::endl;
        }

        void FileInfoList::Counters::Display(std::ostream& aOut) const
        {
            aOut << "    " << mCounters[COUNTER_NEW         ] << " new files\n";
            aOut << "    " << mCounters[COUNTER_NEW_COPIED  ] << " created files\n";
            aOut << "    " << mCounters[COUNTER_EXISTING    ] << " existing files\n";
            aOut << "    " << mCounters[COUNTER_NEWER       ] << " newer files\n";
            aOut << "    " << mCounters[COUNTER_NEWER_COPIED] << " replaced files\n";
            aOut << "    " << mCounters[COUNTER_COPIED      ] << " copied files\n";

            assert( mCounters[COUNTER_COPIED  ] == mCounters[COUNTER_NEW_COPIED] + mCounters[COUNTER_NEWER_COPIED]);
            assert((mCounters[COUNTER_EXISTING] >= mCounters[COUNTER_NEWER]));

            assert((mCounters[COUNTER_NEWER] == mCounters[COUNTER_NEWER_COPIED]) || (0 == mCounters[COUNTER_NEWER_COPIED]));
            assert((mCounters[COUNTER_NEW  ] == mCounters[COUNTER_NEW_COPIED  ]) || (0 == mCounters[COUNTER_NEW_COPIED  ]));
        }

        void FileInfoList::Counters::Increment(unsigned int aId)
        {
            assert(COUNTER_QTY > aId);

            mCounters[aId]++;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void FileInfoList::CreateFolders(const Folder& aNewRoot)
        {
            for (std::string lFolder : mFolders)
            {
                Folder lF(aNewRoot, lFolder.c_str());
                if (!lF.DoesExist())
                {
                    lF.Create();
                }
            }
        }

        void FileInfoList::Find(const char* aPattern, bool aRecurse)
        {
            assert(NULL != aPattern);

            char lPattern[PATH_LENGTH];
            mRoot.GetPath(aPattern, lPattern, sizeof(lPattern));

            WIN32_FIND_DATA lData;

            HANDLE lHandle = FindFirstFile(lPattern, &lData);
            if (INVALID_HANDLE_VALUE != lHandle)
            {
                do
                {
                    if (FILE_ATTRIBUTE_DIRECTORY == (lData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        if (aRecurse)
                        {
                            FindInFolder(lData.cFileName, lData.cFileName, aPattern);
                        }
                    }
                    else
                    {
                        Add(lData.cFileName, new FileInfo(&lData));
                    }
                }
                while (FindNextFile(lHandle, &lData));
            }
        }

        void FileInfoList::FindInFolder(const char* aFullFolder, const char* aFolder, const char* aPattern)
        {
            assert(NULL != aFullFolder);
            assert(NULL != aFolder);
            assert(NULL != aPattern);

            if ((0 == strcmp(".", aFolder)) || (0 == strcmp("..", aFolder)))
            {
                return;
            }

            File::Folder lFolder(mRoot, aFullFolder);

            mFolders.insert(aFullFolder);

            char lPattern[PATH_LENGTH];
            lFolder.GetPath(aPattern, lPattern, sizeof(lPattern));

            WIN32_FIND_DATA lData;

            HANDLE lHandle = FindFirstFile(lPattern, &lData);
            if (INVALID_HANDLE_VALUE != lHandle)
            {
                do
                {
                    std::string lFileName = aFullFolder;

                    lFileName += "\\";
                    lFileName += lData.cFileName;

                    if (0 == (lData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        Add(lFileName.c_str(), new FileInfo(&lData));
                    }
                    else
                    {
                        FindInFolder(lFileName.c_str(), lData.cFileName, aPattern);
                    }
                }
                while (FindNextFile(lHandle, &lData));
            }
        }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const File::FileInfoList& aFIL)
{
    aFIL.Display(aOut);

    return aOut;
}

std::ostream& operator << (std::ostream& aOut, const File::FileInfoList::Counters& aC)
{
    aC.Display(aOut);

    return aOut;
}

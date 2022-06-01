
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

        void FileInfoList::Copy(const File::Folder& aNewRoot, unsigned int aFlags)
        {
            for (std::string lFolder : mFolders)
            {
                Folder lF(aNewRoot, lFolder.c_str());

                lF.Create();
            }

            for (FileInfoMap::value_type lInfo : mFiles)
            {
                mRoot.Copy(aNewRoot, lInfo.first.c_str(), aFlags);
            }
        }

        void FileInfoList::Copy(FileInfoList* aList, unsigned int aFlags)
        {
            assert(NULL != aList);
            assert(0 != aFlags);

            const Folder& lNewRoot = aList->GetRoot();

            for (FileInfoMap::value_type& lPair : mFiles)
            {
                bool lCopy = false;

                FileInfo * lFI = aList->Find(lPair.first.c_str());
                if (NULL == lFI)
                {
                    lCopy = (FLAG_IF_DOES_NOT_EXIST == (aFlags & FLAG_IF_DOES_NOT_EXIST));
                    if (lCopy)
                    {
                        aList->Add(lPair.first.c_str(), new FileInfo(*lPair.second));
                    }
                }
                else
                {
                    lCopy = (FLAG_IF_NEWER == (aFlags & FLAG_IF_NEWER)) && lPair.second->IsNewer(*lFI);
                    if (lCopy)
                    {
                        *lFI = *lPair.second;
                    }
                }

                if (lCopy)
                {
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

        // Private
        // //////////////////////////////////////////////////////////////////

        void FileInfoList::Find(const char* aPattern, bool aRecurse)
        {
            assert(NULL != aPattern);

            char lPattern[MAX_PATH];
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

            char lPattern[MAX_PATH];
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

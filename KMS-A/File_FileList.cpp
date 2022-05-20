
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_FileList.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/File/FileList.h>

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        FileList::FileList(const File::Folder& aRoot, const char* aPattern, bool aRecurse) : mRoot(aRoot)
        {
            assert(NULL != aPattern);

            Find(aPattern, aRecurse);
        }

        void FileList::Copy(const File::Folder& aNewRoot)
        {
            for (std::string lFolder : mFolders)
            {
                Folder lF(aNewRoot, lFolder.c_str());

                lF.Create();
            }

            for (std::string lFile : mFiles)
            {
                mRoot.Copy(aNewRoot, lFile.c_str());
            }
        }

        unsigned int FileList::GetCount() const { return static_cast<unsigned int>(mFiles.size()); }

        // Private
        // //////////////////////////////////////////////////////////////////

        void FileList::Find(const char* aPattern, bool aRecurse)
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
                        mFiles.insert(lData.cFileName);
                    }
                }
                while (FindNextFile(lHandle, &lData));
            }
        }

        void FileList::FindInFolder(const char* aFullFolder, const char* aFolder, const char* aPattern)
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
                        mFiles.insert(lFileName);
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


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Folder_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Process.h>

#include <KMS/File/Folder.h>

// Constants
/////////////////////////////////////////////////////////////////////////////

#define FILE_ATTR   (0)
#define FILE_MASK   (FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)

#define FOLDER_ATTR (FILE_ATTRIBUTE_DIRECTORY)
#define FOLDER_MASK (FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_DIRECTORY|FILE_ATTRIBUTE_REPARSE_POINT)

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool Folder::DoesExist() const
        {
            DWORD lAttributes = GetFileAttributes(mPath.c_str());
            if (INVALID_FILE_ATTRIBUTES == lAttributes) { return false; }

            return FOLDER_ATTR == (lAttributes & FOLDER_MASK);
        }

        bool Folder::DoesFileExist(const char* aFile) const
        {
            char lPath[MAX_PATH];

            GetPath(aFile, lPath, sizeof(lPath));

            DWORD lAttributes = GetFileAttributes(lPath);
            if (INVALID_FILE_ATTRIBUTES == lAttributes) { return false; }

            return FILE_ATTR == (lAttributes & FILE_MASK);
        }

        bool Folder::DoesFolderExist(const char* aFolder) const
        {
            char lPath[MAX_PATH];

            GetPath(aFolder, lPath, sizeof(lPath));

            DWORD lAttributes = GetFileAttributes(lPath);
            if (INVALID_FILE_ATTRIBUTES == lAttributes) { return false; }

            return FOLDER_ATTR == (lAttributes & FOLDER_MASK);
        }

        void Folder::Compress(const Folder& aFolder, const char* aFile)
        {
            char lDst[MAX_PATH];

            aFolder.GetPath(aFile, lDst, sizeof(lDst));

            Process lProcess(Folder(Id::NONE), "PowerShell.exe");

            lProcess.AddArgument("-NoLogo");
            lProcess.AddArgument("-Command");
            lProcess.AddArgument("Compress-Archive");
            lProcess.AddArgument("-Path");
            lProcess.AddArgument((mPath + "\\*").c_str());
            lProcess.AddArgument("-DestinationPath");
            lProcess.AddArgument(lDst);

            int lRet = lProcess.Run();
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_COMPRESS, "Cannot compress the folder's elements", lProcess.GetCmdLine());
            }
        }

        void Folder::Uncompress(const Folder& aFolder, const char* aFile)
        {
            char lSrc[MAX_PATH];

            aFolder.GetPath(aFile, lSrc, sizeof(lSrc));

            Process lProcess(Folder(Id::NONE), "PowerShell.exe");

            lProcess.AddArgument("-NoLogo");
            lProcess.AddArgument("-Command");
            lProcess.AddArgument("Expand-Archive");
            lProcess.AddArgument("-Path");
            lProcess.AddArgument(lSrc);
            lProcess.AddArgument("-DestinationPath");
            lProcess.AddArgument(mPath.c_str());
            lProcess.AddArgument("-Force");

            int lRet = lProcess.Run();
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_UNCOMPRESS, "Cannot uncompress the elements", lProcess.GetCmdLine());
            }
        }

        void Folder::Create()
        {
            if (!CreateDirectory(mPath.c_str(), NULL))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_CREATE, "CreateDirectory failed", mPath.c_str());
            }
        }

        void Folder::Delete()
        {
            if (!RemoveDirectory(mPath.c_str()))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_REMOVE, "RemoveDirectory failed", mPath.c_str());
            }
        }

        void Folder::Rename(const char* aSrc, const char* aDst, unsigned int aFlags) const
        {
            BackupIfNeeded(aDst, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[MAX_PATH];
            char lSrc[MAX_PATH];

            GetPath(aDst, lDst, sizeof(lDst));
            GetPath(aSrc, lSrc, sizeof(lSrc));

            if (MoveFile(lSrc, lDst))
            {
                Verbose(lSrc, "renamed to", lDst, aFlags);
            }
            else
            {
                Verbose(lSrc, "not renamed to", lDst, aFlags | FLAG_RED);

                KMS_EXCEPTION_WITH_INFO(FILE_RENAME, "MoveFile failed", lSrc);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Folder::Copy_Internal(const char* aDst, const char* aSrc, unsigned int aFlags) const
        {
            assert(NULL != aDst);
            assert(NULL != aSrc);

            if (CopyFile(aSrc, aDst, (FLAG_OVERWRITE == (aFlags & FLAG_OVERWRITE)) ? FALSE : TRUE))
            {
                Verbose(aSrc, "copied to", aDst, aFlags);
            }
            else
            {
                Verbose(aSrc, "not copied to", aDst, aFlags | FLAG_RED);

                if (FLAG_IGNORE_ERROR != (aFlags & FLAG_IGNORE_ERROR))
                {
                    KMS_EXCEPTION_WITH_INFO(FILE_COPY, "CopyFile failed", aSrc);
                }
            }
        }

        void Folder::Init_Executable()
        {
            char lModule[MAX_PATH];

            DWORD lRet = GetModuleFileName(NULL, lModule, sizeof(lModule));
            if ((0 >= lRet) || (sizeof(lModule) <= lRet))
            {
                KMS_EXCEPTION(FOLDER_INIT, "GetModuleFileName failed");
            }

            char* lPtr = strrchr(lModule, '\\');
            if (NULL == lPtr)
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_INIT, "Invalid executable name", lModule);
            }

            *lPtr = '\0';

            mPath = lModule;
        }

        void Folder::Init_Temporary()
        {
            char lRoot[MAX_PATH];

            unsigned int lLength = sizeof(lRoot) / sizeof(lRoot[0]);

            DWORD lRet = GetTempPath(lLength, lRoot);
            if ((0 >= lRet) || (lLength <= lRet))
            {
                KMS_EXCEPTION(FOLDER_INIT, "GetTempPath failed");
            }

            char lFolder[MAX_PATH];

            if (0 == GetTempFileName(lRoot, "KMS_", 0, lFolder))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_INIT, "GetTempFileName failed", lRoot);
            }

            BOOL lRetB = DeleteFile(lFolder);
            assert(lRetB);

            mPath = lFolder;

            Create();
        }

    }
}

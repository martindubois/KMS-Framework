
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Folder_W.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

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

        const Folder PROGRAM_FILES    (Folder::Id::PROGRAM_FILES);
        const Folder PROGRAM_FILES_X86(Folder::Id::PROGRAM_FILES_X86);

        bool Folder::DoesExist() const
        {
            DWORD lAttributes = GetFileAttributes(mPath.c_str());
            if (INVALID_FILE_ATTRIBUTES == lAttributes) { return false; }

            return FOLDER_ATTR == (lAttributes & FOLDER_MASK);
        }

        bool Folder::DoesFileExist(const char* aFile) const
        {
            char lPath[PATH_LENGTH];

            GetPath(aFile, lPath, sizeof(lPath));

            DWORD lAttributes = GetFileAttributes(lPath);
            if (INVALID_FILE_ATTRIBUTES == lAttributes) { return false; }

            return FILE_ATTR == (lAttributes & FILE_MASK);
        }

        bool Folder::DoesFolderExist(const char* aFolder) const
        {
            char lPath[PATH_LENGTH];

            GetPath(aFolder, lPath, sizeof(lPath));

            DWORD lAttributes = GetFileAttributes(lPath);
            if (INVALID_FILE_ATTRIBUTES == lAttributes) { return false; }

            return FOLDER_ATTR == (lAttributes & FOLDER_MASK);
        }

        void Folder::Compress(const Folder& aFolder, const char* aFile)
        {
            char lDst[PATH_LENGTH];

            aFolder.GetPath(aFile, lDst, sizeof(lDst));

            Proc::Process lProcess(Folder(Id::NONE), "PowerShell.exe");

            lProcess.AddArgument("-NoLogo");
            lProcess.AddArgument("-Command");
            lProcess.AddArgument("Compress-Archive");
            lProcess.AddArgument("-Path");
            lProcess.AddArgument((mPath + "\\*").c_str());
            lProcess.AddArgument("-DestinationPath");
            lProcess.AddArgument(lDst);

            lProcess.Run(1000 * 60 * 5);

            if (0 != lProcess.GetExitCode())
            {
                KMS_EXCEPTION(FOLDER_COMPRESS, "Cannot compress the folder's elements", lProcess.GetCmdLine());
            }
        }

        void Folder::Uncompress(const Folder& aFolder, const char* aFile)
        {
            char lSrc[PATH_LENGTH];

            aFolder.GetPath(aFile, lSrc, sizeof(lSrc));

            Proc::Process lProcess(Folder(Id::NONE), "PowerShell.exe");

            lProcess.AddArgument("-NoLogo");
            lProcess.AddArgument("-Command");
            lProcess.AddArgument("Expand-Archive");
            lProcess.AddArgument("-Path");
            lProcess.AddArgument(lSrc);
            lProcess.AddArgument("-DestinationPath");
            lProcess.AddArgument(mPath.c_str());
            lProcess.AddArgument("-Force");

            lProcess.Run(1000 * 60 * 5);

            if (0 != lProcess.GetExitCode())
            {
                KMS_EXCEPTION(FOLDER_UNCOMPRESS, "Cannot uncompress the elements", lProcess.GetCmdLine());
            }
        }

        void Folder::Copy(const Folder& aDst) const
        {
            Proc::Process lProcess(Folder(Id::NONE), "xcopy");

            lProcess.AddArgument("/CEKIV");
            lProcess.AddArgument(mPath.c_str());
            lProcess.AddArgument(aDst.GetPath());

            lProcess.Run(1000 * 60 * 5);

            if (0 != lProcess.GetExitCode())
            {
                KMS_EXCEPTION(FOLDER_UNCOMPRESS, "Cannot copy folder", lProcess.GetCmdLine());
            }
        }

        void Folder::Create()
        {
            if (!CreateDirectory(mPath.c_str(), NULL))
            {
                KMS_EXCEPTION(FOLDER_CREATE, "Cannot create the folder", mPath.c_str());
            }
        }

        void Folder::Delete()
        {
            // TODO Allow to remove none empty folder.

            if (!RemoveDirectory(mPath.c_str()))
            {
                KMS_EXCEPTION(FOLDER_REMOVE, "Cannot remove the folder", mPath.c_str());
            }
        }

        void Folder::Rename(const char* aSrc, const char* aDst, unsigned int aFlags) const
        {
            BackupIfNeeded(aDst, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[PATH_LENGTH];
            char lSrc[PATH_LENGTH];

            GetPath(aDst, lDst, sizeof(lDst));
            GetPath(aSrc, lSrc, sizeof(lSrc));

            if (MoveFile(lSrc, lDst))
            {
                Verbose(lSrc, "renamed to", lDst, aFlags);
            }
            else
            {
                Verbose(lSrc, "not renamed to", lDst, aFlags | FLAG_RED);

                KMS_EXCEPTION(FILE_RENAME, "Cannot move the file", lSrc);
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
                    KMS_EXCEPTION(FILE_COPY, "Cannot copy the file", aSrc);
                }
            }
        }

        void Folder::Init_Current()
        {
            char lPath[PATH_LENGTH];

            DWORD lRet = GetCurrentDirectory(sizeof(lPath), lPath);
            assert(0 < lRet);
            assert(sizeof(lPath) > lRet);

            mPath = lPath;
        }

        void Folder::Init_Executable()
        {
            char lModule[PATH_LENGTH];

            DWORD lRet = GetModuleFileName(NULL, lModule, sizeof(lModule));
            KMS_EXCEPTION_ASSERT((0 < lRet) && (sizeof(lModule) > lRet), FOLDER_INIT, "GetModuleFileName failed", lRet);

            char* lPtr = strrchr(lModule, '\\');
            KMS_EXCEPTION_ASSERT(NULL != lPtr, FOLDER_INIT, "Invalid executable name", lModule);

            *lPtr = '\0';

            mPath = lModule;
        }

        void Folder::Init_Temporary()
        {
            char lRoot[PATH_LENGTH];

            unsigned int lLength = sizeof(lRoot) / sizeof(lRoot[0]);

            DWORD lRet = GetTempPath(lLength, lRoot);
            KMS_EXCEPTION_ASSERT((0 < lRet) && (lLength > lRet), FOLDER_INIT, "GetTempPath failed", lRet);

            char lFolder[PATH_LENGTH];

            if (0 == GetTempFileName(lRoot, "KMS_", 0, lFolder))
            {
                KMS_EXCEPTION(FOLDER_INIT, "GetTempFileName failed", lRoot);
            }

            BOOL lRetB = ::DeleteFile(lFolder);
            assert(lRetB);

            mPath = lFolder;

            Create();
        }

    }
}

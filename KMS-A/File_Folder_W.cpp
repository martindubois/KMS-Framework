
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Folder_W.cpp

// TEST COVERAGE 2023-07-24 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

#include <KMS/File/Folder.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CMD_ALLOWED_TIME_ms         (1000 * 60 *  2) //  2 minutes
#define POWER_SHELL_ALLOWER_TIME_ms (1000 * 60 * 10) // 10 minutes
#define XCOPY_ALLOWER_TIME_ms       (1000 * 60 *  5) //  5 minutes

// Constants
// //////////////////////////////////////////////////////////////////////////

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

        const Folder Folder::PROGRAM_FILES    (Folder::Id::PROGRAM_FILES);
        const Folder Folder::PROGRAM_FILES_X86(Folder::Id::PROGRAM_FILES_X86);

        void Folder::ChangeCurrentDirectory(const Folder& aNew)
        {
            const char* lNew = aNew.GetPath();

            if (!SetCurrentDirectory(lNew))
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot change the current directory for %s", lNew);
                KMS_EXCEPTION(FOLDER_CHANGE_FAILED, lMsg, "");
            }

            CURRENT.Init_Current();
        }

        bool Folder::DoesExist() const
        {
            auto lAttributes = GetFileAttributes(mPath.c_str());
            if (INVALID_FILE_ATTRIBUTES == lAttributes)
            {
                return false;
            }

            return FOLDER_ATTR == (lAttributes & FOLDER_MASK);
        }

        bool Folder::DoesFileExist(const char* aFile) const
        {
            char lPath[PATH_LENGTH];

            GetPath(aFile, lPath, sizeof(lPath));

            auto lAttributes = GetFileAttributes(lPath);
            if (INVALID_FILE_ATTRIBUTES == lAttributes)
            {
                return false;
            }

            return FILE_ATTR == (lAttributes & FILE_MASK);
        }

        bool Folder::DoesFolderExist(const char* aFolder) const
        {
            char lPath[PATH_LENGTH];

            GetPath(aFolder, lPath, sizeof(lPath));

            auto lAttributes = GetFileAttributes(lPath);
            if (INVALID_FILE_ATTRIBUTES == lAttributes)
            {
                return false;
            }

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

            lProcess.Run(POWER_SHELL_ALLOWER_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lProcess.GetExitCode(), FILE_COMPRESS_FAILED, "Cannot compress the folder's elements", lProcess.GetCmdLine());
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

            lProcess.Run(POWER_SHELL_ALLOWER_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lProcess.GetExitCode(), FILE_UNCOMPRESS_FAILED, "Cannot uncompress the elements", lProcess.GetCmdLine());
        }

        void Folder::Copy(const Folder& aDst) const
        {
            Proc::Process lProcess(Folder(Id::NONE), "xcopy");

            lProcess.AddArgument("/CEKIV");
            lProcess.AddArgument(mPath.c_str());
            lProcess.AddArgument(aDst.GetPath());

            lProcess.Run(XCOPY_ALLOWER_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lProcess.GetExitCode(), FILE_UNCOMPRESS_FAILED, "Cannot copy folder", lProcess.GetCmdLine());
        }

        void Folder::Create()
        {
            if (!CreateDirectory(mPath.c_str(), nullptr))
            {
                // NOT TESTED
                KMS_EXCEPTION(FILE_CREATE_FAILED, "Cannot create the folder", mPath.c_str());
            }
        }

        void Folder::Delete()
        {
            // TODO Allow to remove none empty folder.

            if (!RemoveDirectory(mPath.c_str()))
            {
                KMS_EXCEPTION(FILE_REMOVE_FAILED, "Cannot remove the folder", mPath.c_str());
            }
        }

        void Folder::Delete(const char* aFile)
        {
            char lPath[PATH_LENGTH];

            GetPath(aFile, lPath, sizeof(lPath));

            auto lAttr = GetFileAttributes(lPath);

            char lMsg[64 + PATH_LENGTH];

            sprintf_s(lMsg, "Cannot retrieve attributes of \"%s\"", lPath);
            KMS_EXCEPTION_ASSERT(INVALID_FILE_ATTRIBUTES != lAttr, FILE_DELETE_FAILED, lMsg, "");

            if (0 != (lAttr & FILE_ATTRIBUTE_READONLY))
            {
                // NOT TESTED
                lAttr &= ~FILE_ATTRIBUTE_READONLY;
                if (!SetFileAttributes(lPath, lAttr))
                {
                    sprintf_s(lMsg, "Cannot change attributes of \"%s\"", lPath);
                    KMS_EXCEPTION(FILE_DELETE_FAILED, lMsg, "");
                }
            }

            if (!DeleteFile(lPath))
            {
                // NOT TESTED
                sprintf_s(lMsg, "Cannot delete \"%s\"", lPath);
                KMS_EXCEPTION(FILE_DELETE_FAILED, lMsg, "");
            }
        }

        void Folder::DeleteFiles(const char* aPattern)
        {
            char lPattern[PATH_LENGTH];

            GetPath(aPattern, lPattern, sizeof(lPattern));

            Proc::Process lP(Folder(Id::NONE), "cmd.exe");

            lP.AddArgument("/C");
            lP.AddArgument("del");
            lP.AddArgument("/F");
            lP.AddArgument("/Q");
            lP.AddArgument(lPattern);

            lP.Run(CMD_ALLOWED_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), FILE_DELETE_FAILED, "Cannot delete files", lP.GetCmdLine());
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

                KMS_EXCEPTION(FILE_RENAME_FAILED, "Cannot move the file", lSrc);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Folder::Copy_Internal(const char* aDst, const char* aSrc, unsigned int aFlags) const
        {
            assert(nullptr != aDst);
            assert(nullptr != aSrc);

            if (CopyFile(aSrc, aDst, (FLAG_OVERWRITE == (aFlags & FLAG_OVERWRITE)) ? FALSE : TRUE))
            {
                Verbose(aSrc, "copied to", aDst, aFlags);
            }
            else
            {
                Verbose(aSrc, "not copied to", aDst, aFlags | FLAG_RED);

                KMS_EXCEPTION_ASSERT(FLAG_IGNORE_ERROR == (aFlags & FLAG_IGNORE_ERROR), FILE_COPY_FAILED, "Cannot copy the file", aSrc);
            }
        }

        void Folder::Init_Current()
        {
            char lPath[PATH_LENGTH];

            auto lRet = GetCurrentDirectory(sizeof(lPath), lPath);
            assert(0 < lRet);
            assert(sizeof(lPath) > lRet);

            mPath = lPath;
        }

        void Folder::Init_Executable()
        {
            char lModule[PATH_LENGTH];

            auto lRet = GetModuleFileName(NULL, lModule, sizeof(lModule));
            KMS_EXCEPTION_ASSERT((0 < lRet) && (sizeof(lModule) > lRet), FILE_INIT_FAILED, "GetModuleFileName failed", lRet);

            char* lPtr = strrchr(lModule, '\\');
            KMS_EXCEPTION_ASSERT(nullptr != lPtr, FILE_INIT_FAILED, "Invalid executable name", lModule);

            *lPtr = '\0';

            mPath = lModule;
        }

        void Folder::Init_Temporary()
        {
            char lRoot[PATH_LENGTH];

            unsigned int lLength = sizeof(lRoot) / sizeof(lRoot[0]);

            auto lRet = GetTempPath(lLength, lRoot);
            KMS_EXCEPTION_ASSERT((0 < lRet) && (lLength > lRet), FILE_INIT_FAILED, "GetTempPath failed", lRet);

            char lFolder[PATH_LENGTH];

            KMS_EXCEPTION_ASSERT(0 != GetTempFileName(lRoot, "KMS_", 0, lFolder), FILE_INIT_FAILED, "GetTempFileName failed", lRoot);

            auto lRetB = DeleteFile(lFolder);
            assert(lRetB);

            mPath = lFolder;

            Create();
        }

    }
}

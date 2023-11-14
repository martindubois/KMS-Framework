
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Folder_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== System =============================================================
#include <sys/stat.h>

// ===== Includes ===========================================================
#include <KMS/Proc/Process.h>

#include <KMS/File/Folder.h>

KMS_RESULT_STATIC(RESULT_COPY_FAILED);
KMS_RESULT_STATIC(RESULT_CREATE_FAILED);
KMS_RESULT_STATIC(RESULT_COMPRESS_FAILED);
KMS_RESULT_STATIC(RESULT_DELETE_FAILED);
KMS_RESULT_STATIC(RESULT_FOLDER_CHANGE_FAILED);
KMS_RESULT_STATIC(RESULT_INIT_FAILED);
KMS_RESULT_STATIC(RESULT_REMOVE_FAILED);
KMS_RESULT_STATIC(RESULT_RENAME_FAILED);
KMS_RESULT_STATIC(RESULT_UNCOMPRESS_FAILED);

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CP_ALLOWED_TIME_ms    (1000 * 60 * 2) // 2 minutes
#define RM_ALLOWED_TIME_ms    (1000 * 60 * 2) // 2 minutes
#define UNZIP_ALLOWED_TIME_ms (1000 * 60 * 5) // 5 minutes
#define ZIP_ALLOWED_TIME_ms   (1000 * 60 * 5) // 5 minutes

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Folder::ChangeCurrentDirectory(const Folder& aNew)
        {
            auto lNew = aNew.GetPath();

            auto lRet = chdir(lNew);
            if (0 != lRet)
            {
                char lMsg[64 + PATH_LENGTH];
                sprintf_s(lMsg, "Cannot change the current directory for %s", lNew);
                KMS_EXCEPTION(RESULT_FOLDER_CHANGE_FAILED, lMsg, lRet);
            }

            CURRENT.Init_Current();
        }

        bool Folder::DoesExist() const
        {
            struct stat lStat;

            return (0 == stat(mPath.c_str(), &lStat)) && S_ISDIR(lStat.st_mode);
        }

        bool Folder::DoesFileExist(const char* aFile) const
        {
            char lPath[PATH_LENGTH];

            GetPath(aFile, lPath, sizeof(lPath));

            struct stat lStat;

            return (0 == stat(lPath, &lStat)) && S_ISREG(lStat.st_mode);
        }

        bool Folder::DoesFolderExist(const char* aFolder) const
        {
            char lPath[PATH_LENGTH];

            GetPath(aFolder, lPath, sizeof(lPath));

            struct stat lStat;

            return (0 == stat(mPath.c_str(), &lStat)) && S_ISDIR(lStat.st_mode);
        }

        void Folder::Compress(const Folder& aFolder, const char* aFile)
        {
            char lDst[PATH_LENGTH];

            aFolder.GetPath(aFile, lDst, sizeof(lDst));

            Proc::Process lP(Folder(Id::NONE), "zip");

            lP.AddArgument("-r");
            lP.AddArgument(lDst);
            lP.AddArgument("*");

            lP.SetWorkingDirectory(mPath.c_str());

            lP.Run(ZIP_ALLOWED_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), RESULT_COMPRESS_FAILED, "Cannot compress the folder's elements", lP.GetCmdLine());
        }

        void Folder::Uncompress(const Folder& aFolder, const char* aFile)
        {
            char lSrc[PATH_LENGTH];

            aFolder.GetPath(aFile, lSrc, sizeof(lSrc));

            Proc::Process lP(Folder(Id::NONE), "unzip");

            lP.AddArgument(lSrc);
            lP.AddArgument("-d");
            lP.AddArgument(mPath.c_str());

            lP.Run(UNZIP_ALLOWED_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), RESULT_UNCOMPRESS_FAILED, "Cannot uncompress the elements", lP.GetCmdLine());
        }

        void Folder::Copy(const Folder& aDst) const
        {
            Proc::Process lP(Folder(Id::NONE), "cp");

            lP.AddArgument("-R");
            lP.AddArgument(mPath.c_str());
            lP.AddArgument(aDst.GetPath());

            lP.Run(CP_ALLOWED_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), RESULT_COPY_FAILED, "Cannot copy folder", lP.GetCmdLine());
        }

        void Folder::Create()
        {
            if (0 != mkdir(mPath.c_str(), S_IRWXU))
            {
                KMS_EXCEPTION(RESULT_CREATE_FAILED, "mkdir failed", mPath.c_str());
            }
        }

        void Folder::Delete()
        {
            if (0 != rmdir(mPath.c_str()))
            {
                KMS_EXCEPTION(RESULT_REMOVE_FAILED, "rmdir failed", mPath.c_str());
            }
        }

        void Folder::Delete(const char* aFile)
        {
            char lPath[PATH_LENGTH];

            GetPath(aFile, lPath, sizeof(lPath));

            if (0 != unlink(lPath))
            {
                KMS_EXCEPTION(RESULT_DELETE_FAILED, "unlink failed", lPath);
            }
        }

        void Folder::DeleteFiles(const char* aPattern)
        {
            char lPattern[PATH_LENGTH];

            GetPath(aPattern, lPattern, sizeof(lPattern));

            Proc::Process lP(Folder(Id::NONE), "rm");

            lP.AddArgument("-f");
            lP.AddArgument(lPattern);

            lP.Run(RM_ALLOWED_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), RESULT_DELETE_FAILED, "Cannot delete files", lP.GetCmdLine());
        }

        void Folder::Rename(const char* aSrc, const char* aDst, unsigned int aFlags) const
        {
            BackupIfNeeded(aDst, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[PATH_LENGTH];
            char lSrc[PATH_LENGTH];

            GetPath(aDst, lDst, sizeof(lDst));
            GetPath(aSrc, lSrc, sizeof(lSrc));

            if (0 == rename(lSrc, lDst))
            {
                Verbose(lSrc, "renamed to", lDst, aFlags);
            }
            else
            {
                Verbose(lSrc, "not renamed to", lDst, aFlags | FLAG_RED);

                KMS_EXCEPTION(RESULT_RENAME_FAILED, "rename failed", lSrc);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Folder::Copy_Internal(const char* aDst, const char* aSrc, unsigned int aFlags) const
        {
            assert(nullptr != aDst);
            assert(nullptr != aSrc);

            Proc::Process lP(Folder(Id::NONE), "cp");

            if (FLAG_OVERWRITE == (aFlags & FLAG_OVERWRITE))
            {
                lP.AddArgument("-f");
            }

            lP.AddArgument(aSrc);
            lP.AddArgument(aDst);

            lP.Run(CP_ALLOWED_TIME_ms);

            KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), RESULT_COPY_FAILED, "Cannot copy the file", lP.GetCmdLine());
        }

        void Folder::Init_Current()
        {
            char lPath[PATH_LENGTH];

            auto lRet = getcwd(lPath, sizeof(lPath));
            assert(lPath == lRet);

            mPath = lPath;
        }

        void Folder::Init_Executable()
        {
            char lModule[PATH_LENGTH];

            auto lRet = readlink("/proc/self/exe", lModule, sizeof(lModule));
            KMS_EXCEPTION_ASSERT((0 < lRet) && (sizeof(lModule) > lRet), RESULT_INIT_FAILED, "readlink failed", lRet);

            auto lPtr = strrchr(lModule, '/');
            KMS_EXCEPTION_ASSERT(nullptr != lPtr, RESULT_INIT_FAILED, "Invalid executable name", lModule);

            *lPtr = '\0';

            mPath = lModule;
        }

        void Folder::Init_Temporary()
        {
            char lPath[PATH_LENGTH];

            strcpy(lPath, "/tmp/KMSXXXXXX");

            auto lRet = mkdtemp(lPath);
            KMS_EXCEPTION_ASSERT(nullptr != lRet, RESULT_INIT_FAILED, "mkdtemp failed", lPath);

            mPath = lPath;
        }

    }
}

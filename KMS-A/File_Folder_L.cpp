
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Folder_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== System =============================================================
#include <sys/stat.h>

// ===== Includes ===========================================================
#include <KMS/Process.h>

#include <KMS/File/Folder.h>

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool Folder::DoesExist() const
        {
            struct stat lStat;

            return (0 == stat(mPath.c_str(), &lStat)) && S_ISDIR(lStat.st_mode);
        }

        bool Folder::DoesFileExist(const char* aFile) const
        {
            char lPath[MAX_PATH];

            GetPath(aFile, lPath, sizeof(lPath));

            struct stat lStat;

            return (0 == stat(lPath, &lStat)) && S_ISREG(lStat.st_mode);
        }

        bool Folder::DoesFolderExist(const char* aFolder) const
        {
            char lPath[MAX_PATH];

            GetPath(aFolder, lPath, sizeof(lPath));

            struct stat lStat;

            return (0 == stat(mPath.c_str(), &lStat)) && S_ISDIR(lStat.st_mode);
        }

        void Folder::Compress(const Folder& aFolder, const char* aFile)
        {
            char lDst[MAX_PATH];

            aFolder.GetPath(aFile, lDst, sizeof(lDst));

            Process lProcess(Folder(Id::NONE), "zip");

            lProcess.AddArgument("-r");
            lProcess.AddArgument(lDst);
            lProcess.AddArgument((mPath + SLASH + "*").c_str());

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

            Process lProcess(Folder(Id::NONE), "unzip");

            lProcess.AddArgument(lSrc);
            lProcess.AddArgument("-d");
            lProcess.AddArgument(mPath.c_str());

            int lRet = lProcess.Run();
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_UNCOMPRESS, "Cannot uncompress the elements", lProcess.GetCmdLine());
            }
        }

        void Folder::Create()
        {
            if (0 != mkdir(mPath.c_str(), S_IRWXU))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_CREATE, "mkdir failed", mPath.c_str());
            }
        }

        void Folder::Delete()
        {
            if (0 != rmdir(mPath.c_str()))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_REMOVE, "rmdir failed", mPath.c_str());
            }
        }

        void Folder::DeleteFile(const char* aFile)
        {
            char lPath[MAX_PATH];

            GetPath(aFile, lPath, sizeof(lPath));

            if (0 != unlink(lPath))
            {
                KMS_EXCEPTION_WITH_INFO(FILE_DELETE, "unlink failed", lPath);
            }
        }

        void Folder::DeleteFiles(const char* aPattern)
        {
            char lPattern[MAX_PATH];

            GetPath(aPattern, lPattern, sizeof(lPattern));

            Process lProcess(Folder(Id::NONE), "rm");

            lProcess.AddArgument("-f");
            lProcess.AddArgument(lPattern);

            int lRet = lProcess.Run();
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_UNCOMPRESS, "Cannot delete files", lProcess.GetCmdLine());
            }
        }

        void Folder::Rename(const char* aSrc, const char* aDst, unsigned int aFlags) const
        {
            BackupIfNeeded(aDst, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[MAX_PATH];
            char lSrc[MAX_PATH];

            GetPath(aDst, lDst, sizeof(lDst));
            GetPath(aSrc, lSrc, sizeof(lSrc));

            if (0 == rename(lSrc, lDst))
            {
                Verbose(lSrc, "renamed to", lDst, aFlags);
            }
            else
            {
                Verbose(lSrc, "not renamed to", lDst, aFlags | FLAG_RED);

                KMS_EXCEPTION_WITH_INFO(FILE_RENAME, "rename failed", lSrc);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Folder::Copy_Internal(const char* aDst, const char* aSrc, unsigned int aFlags) const
        {
            assert(NULL != aDst);
            assert(NULL != aSrc);

            Process lProcess(Folder(Id::NONE), "cp");

            if (FLAG_OVERWRITE == (aFlags & FLAG_OVERWRITE))
            {
                lProcess.AddArgument("-f");
            }

            lProcess.AddArgument(aSrc);
            lProcess.AddArgument(aDst);

            int lRet = lProcess.Run();
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_UNCOMPRESS, "Cannot copy the file", lProcess.GetCmdLine());
            }
        }

        void Folder::Init_Executable()
        {
            char lModule[MAX_PATH];

            ssize_t lRet = readlink("/proc/self/exe", lModule, sizeof(lModule));
            if ((0 >= lRet) || (sizeof(lModule) <= lRet))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_INIT, "readlink failed", lRet);
            }

            char* lPtr = strrchr(lModule, '/');
            if (NULL == lPtr)
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_INIT, "Invalid executable name", lModule);
            }

            *lPtr = '\0';

            mPath = lModule;
        }

        void Folder::Init_Temporary()
        {
            char lPath[MAX_PATH];

            strcpy(lPath, "/tmp/KMSXXXXXX");

            char* lRet = mkdtemp(lPath);
            if (NULL == lRet)
            {
                KMS_EXCEPTION(FOLDER_INIT, "mkdtemp failed");
            }

            mPath = lPath;
        }

    }
}

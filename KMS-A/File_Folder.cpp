
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_FileList.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>
#include <KMS/Process.h>

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

        // The flag must be differenent than the one defined in
        // File_FileInfoList.cpp.
        const unsigned int Folder::FLAG_BACKUP        = 0x01000000;
        const unsigned int Folder::FLAG_BACKUP_RENAME = 0x02000000;
        const unsigned int Folder::FLAG_IGNORE_ERROR  = 0x04000000;
        const unsigned int Folder::FLAG_OVERWRITE     = 0x08000000;
        const unsigned int Folder::FLAG_RED           = 0x10000000;
        const unsigned int Folder::FLAG_VERBOSE       = 0x20000000;

        Folder::Folder(Id aId)
        {
            switch (aId)
            {
            case Id::CURRENT: mPath = "."; break;
            case Id::NONE   : mPath = "" ; break;

            case Id::EXECUTABLE: Init_Executable(); break;
            case Id::TEMPORARY : Init_Temporary (); break;

            case Id::HOME         : Init_Env("USERPROFILE" ); break;
            case Id::PROGRAM_FILES: Init_Env("ProgramFiles"); break;

            default: assert(false);
            }
        }

        Folder::Folder(const char* aPath) : mPath(aPath)
        {
            assert(NULL != aPath);
        }

        Folder::Folder(const Folder& aRoot, const char* aFolder)
        {
            char lPath[MAX_PATH];

            aRoot.GetPath(aFolder, lPath, sizeof(lPath));

            mPath = lPath;
        }

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

        const char* Folder::GetPath() const { return mPath.c_str(); }

        void Folder::GetPath(const char* aFile, char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aFile);
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);

            if (0 < mPath.size())
            {
                sprintf_s(aOut, aOutSize_byte, "%s\\%s", mPath.c_str(), aFile);
            }
            else
            {
                sprintf_s(aOut, aOutSize_byte, "%s", aFile);
            }
        }

        void Folder::Backup(const char* aFile, unsigned int aFlags) const
        {
            assert(NULL != aFile);

            for (unsigned int i = 0; i < 1000; i++)
            {
                char lFile[MAX_PATH];

                sprintf_s(lFile, "%s.%03u.bak", aFile, i);

                if (!DoesFileExist(lFile))
                {
                    unsigned int lFlags = aFlags & ~(FLAG_BACKUP | FLAG_BACKUP_RENAME | FLAG_OVERWRITE);

                    if (FLAG_BACKUP_RENAME == (aFlags & FLAG_BACKUP_RENAME))
                    {
                        Rename(aFile, lFile, lFlags);
                    }
                    else
                    {
                        Copy(aFile, lFile, lFlags);
                    }

                    return;
                }
            }

            KMS_EXCEPTION_WITH_INFO(FILE_BACKUP, "Too many backup", aFile);
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

        void Folder::Copy(const char* aSrc, const char* aDst, unsigned int aFlags) const
        {
            BackupIfNeeded(aDst, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[MAX_PATH];
            char lSrc[MAX_PATH];

            GetPath(aDst, lDst, sizeof(lDst));
            GetPath(aSrc, lSrc, sizeof(lSrc));

            Copy_Internal(lDst, lSrc, aFlags);
        }

        void Folder::Copy(const Folder& aDst, const char* aFile, unsigned int aFlags) const
        {
            aDst.BackupIfNeeded(aFile, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[MAX_PATH];
            char lSrc[MAX_PATH];

            aDst.GetPath(aFile, lDst, sizeof(lDst));
                 GetPath(aFile, lSrc, sizeof(lSrc));

            Copy_Internal(lDst, lSrc, aFlags);
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

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Folder::BackupIfNeeded(const char* aFile, unsigned int aFlags) const
        {
            if ((FLAG_BACKUP == (aFlags & FLAG_BACKUP)) && DoesFileExist(aFile))
            {
                Backup(aFile, aFlags | FLAG_BACKUP_RENAME | FLAG_RED);
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

        void Folder::Init_Env(const char * aVariable)
        {
            assert(NULL != aVariable);

            char lPath[MAX_PATH];

            DWORD lRet = GetEnvironmentVariable(aVariable, lPath, sizeof(lPath));
            if ((0 >= lRet) || (sizeof(lPath) <= lRet))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_INIT, "GetEnvironmentVariable failed", aVariable);
            }

            mPath = lPath;
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

        void Folder::Verbose(const char* aSrc, const char* aOp, const char* aDst, unsigned int aFlags) const
        {
            if (FLAG_VERBOSE == (aFlags & FLAG_VERBOSE))
            {
                bool lRed = FLAG_RED == (aFlags & FLAG_RED);

                if (lRed)
                {
                    std::cout << Console::Color::RED;
                }

                std::cout << aSrc << " " << aOp << " " << aDst << std::endl;

                if (lRed)
                {
                    std::cout << Console::Color::WHITE;
                }
            }
        }

    }
}

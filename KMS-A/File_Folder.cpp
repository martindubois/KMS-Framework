
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Folder.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Environment.h>

#include <KMS/File/Folder.h>

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

        Folder::Folder() {}

        Folder::Folder(Id aId)
        {
            switch (aId)
            {
            case Id::CURRENT   : Init_Current   (); break;
            case Id::EXECUTABLE: Init_Executable(); break;
            case Id::TEMPORARY : Init_Temporary (); break;

            case Id::NONE: mPath = ""; break;

            #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
                case Id::HOME: Init_Env("HOME"); break;
            #endif

            #ifdef _KMS_WINDOWS_
                case Id::HOME             : Init_Env("USERPROFILE"      ); break;
                case Id::PROGRAM_FILES    : Init_Env("ProgramFiles"     ); break;
                case Id::PROGRAM_FILES_X86: Init_Env("ProgramFiles(x86)"); break;
            #endif

            default: assert(false);
            }
        }

        Folder::Folder(const char* aPath) : mPath(aPath)
        {
            assert(NULL != aPath);
        }

        Folder::Folder(const Folder& aRoot, const char* aFolder)
        {
            char lPath[PATH_LENGTH];

            aRoot.GetPath(aFolder, lPath, sizeof(lPath));

            mPath = lPath;
        }

        const char* Folder::GetPath() const { return mPath.c_str(); }

        void Folder::GetPath(const char* aFile, char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aFile);
            assert(NULL != aOut);
            assert(0 < aOutSize_byte);

            if (0 < mPath.size())
            {
                sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s" SLASH "%s", mPath.c_str(), aFile);
            }
            else
            {
                sprintf_s(aOut SizeInfoV(aOutSize_byte), "%s", aFile);
            }
        }

        void Folder::Backup(const char* aFile, unsigned int aFlags) const
        {
            assert(NULL != aFile);

            for (unsigned int i = 0; i < 1000; i++)
            {
                char lFile[PATH_LENGTH];

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

        void Folder::Copy(const char* aSrc, const char* aDst, unsigned int aFlags) const
        {
            BackupIfNeeded(aDst, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[PATH_LENGTH];
            char lSrc[PATH_LENGTH];

            GetPath(aDst, lDst, sizeof(lDst));
            GetPath(aSrc, lSrc, sizeof(lSrc));

            Copy_Internal(lDst, lSrc, aFlags);
        }

        void Folder::Copy(const Folder& aDst, const char* aFile, unsigned int aFlags) const
        {
            aDst.BackupIfNeeded(aFile, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[PATH_LENGTH];
            char lSrc[PATH_LENGTH];

            aDst.GetPath(aFile, lDst, sizeof(lDst));
                 GetPath(aFile, lSrc, sizeof(lSrc));

            Copy_Internal(lDst, lSrc, aFlags);
        }

        void Folder::Copy(const Folder& aDst, const char* aFileSrc, const char* aFileDst, unsigned int aFlags) const
        {
            aDst.BackupIfNeeded(aFileDst, aFlags & ~FLAG_IGNORE_ERROR);

            char lDst[PATH_LENGTH];
            char lSrc[PATH_LENGTH];

            aDst.GetPath(aFileDst, lDst, sizeof(lDst));
                 GetPath(aFileSrc, lSrc, sizeof(lSrc));

            Copy_Internal(lDst, lSrc, aFlags);
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

        void Folder::Init_Env(const char * aVariable)
        {
            assert(NULL != aVariable);

            char lPath[PATH_LENGTH];

            Environment::GetVariableValue(aVariable, lPath, sizeof(lPath));

            mPath = lPath;
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

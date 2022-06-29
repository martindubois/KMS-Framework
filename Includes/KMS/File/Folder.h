
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/File/Folder.h

#pragma once

// ===== C++ ================================================================
#include <string>

namespace KMS
{
    namespace File
    {

        class Folder
        {

        public:

            enum class Id
            {
                CURRENT,
                EXECUTABLE,
                HOME,
                NONE,
                PROGRAM_FILES,
                PROGRAM_FILES_X86,
                TEMPORARY,
            };

            static const unsigned int FLAG_BACKUP;
            static const unsigned int FLAG_BACKUP_RENAME;
            static const unsigned int FLAG_IGNORE_ERROR;
            static const unsigned int FLAG_OVERWRITE;
            static const unsigned int FLAG_RED;
            static const unsigned int FLAG_VERBOSE;

            Folder();
            Folder(Id aId);
            Folder(const char * aPath);
            Folder(const Folder & aRoot, const char * aFolder);

            bool DoesExist() const;

            bool DoesFileExist  (const char* aFile  ) const;
            bool DoesFolderExist(const char* aFolder) const;

            const char * GetPath() const;

            void GetPath(const char * aFile, char * aOut, unsigned int aOutSize_byte) const;

            // aFlags FLAG_BACKUP_RENAME (see Copy and Rename)
            void Backup(const char * aFile, unsigned int aFlags = 0) const;

            void Compress  (const Folder &aFolder, const char * aFile);
            void Uncompress(const Folder &aFolder, const char * aFile);

            // aFlags FLAG_BACKUP, FLAG_IGNORE_ERROR, FLAG_OVERWRITE, FLAG_RED, FLAG_VERBOSE
            void Copy(const char* aSrc, const char* aDst, unsigned int aFlags = 0) const;

            void Copy(const Folder& aDst) const;

            // aFlags FLAG_BACKUP, FLAG_INGORE_ERROR, FLAG_OVERWRITE, FLAG_RED, FLAG_VERBOSE
            void Copy(const Folder & aDst, const char * aFile, unsigned int aFlags = 0) const;

            // aFlags FLAG_BACKUP, FLAG_INGORE_ERROR, FLAG_OVERWRITE, FLAG_RED, FLAG_VERBOSE
            void Copy(const Folder& aDst, const char* aFileSrc, const char* aFileDst, unsigned int aFlags = 0) const;

            void Create();

            void Delete();

            void DeleteFile(const char* aFile);

            void DeleteFiles(const char* aPattern);

            // aFlags FLAG_BACKUP, FLAG_RED, FLAG_VERBOSE
            void Rename(const char * aSrc, const char * aDst, unsigned int aFlags = 0) const;

        // internal

            // aFlags FLAG_BACKUP, FLAG_VERBOSE
            void BackupIfNeeded(const char * aFile, unsigned int aFlags) const;

        private:

            // aFlags FLAG_IGNORE_ERROR, FLAG_OVERWRITE, FLAG_RED, FLAG_VERBOSE
            void Copy_Internal(const char* aDst, const char* aSrc, unsigned int aFlags) const;

            void Init_Env(const char* aVariable);

            void Init_Current   ();
            void Init_Executable();
            void Init_Temporary ();

            // aFlags FLAG_VERBOSE
            void Verbose(const char * aSrc, const char * aOp, const char * aDst, unsigned int aFlags) const;

            std::string mPath;

        };

    }
}

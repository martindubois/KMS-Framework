
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
                TEMPORARY,
            };

            Folder(Id aId);
            Folder(const char * aPath);
            Folder(const Folder & aRoot, const char * aFolder);

            bool DoesExist() const;

            bool DoesFileExist  (const char* aFile  ) const;
            bool DoesFolderExist(const char* aFolder) const;

            void Compress  (const Folder &aFolder, const char * aFile);
            void Uncompress(const Folder &aFolder, const char * aFile);

            void Copy(const Folder & aDst, const char * aFile);

            void Create();

            void Delete();

            void GetPath(const char * aFile, char * aOut, unsigned int aOutSize_byte) const;

        private:

            void Init_Env(const char* aVariable);

            void Init_Executable();
            void Init_Temporary ();

            std::string mPath;

        };

    }
}

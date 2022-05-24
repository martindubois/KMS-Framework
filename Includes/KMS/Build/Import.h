
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Import.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    namespace Build
    {
        
        class Import : public Config::Configurable
        {

        public:

            static int Main(int aCount, const char** aVector);

            Import();

            void AddDependency(const char *aD);
            void AddRepository(const char *aR);

            void ImportDependency(const char* aDependency);

            int Run();

            // ===== Config::Configurable ===================================
            virtual ~Import();
            virtual bool AddAttribute(const char* aA, const char* aV);

        private:

            typedef std::list<File::Folder> FolderList;

            Import(const Import &);

            const Import & operator = (const Import &);

            File::Folder mImport;

            // ===== Configurable attributes ================================
            StringSet  mDependencies;
            FolderList mRepositories;

        };

    }
}

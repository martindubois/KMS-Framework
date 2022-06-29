
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

            ~Import();

            void AddDependency       (const char* aD);
            void AddOSIndependentDeps(const char* aD);
            void AddRepository       (const char* aR);

            void ImportDependency(const char* aDependency, bool aOSIndependent);

            int Run();

            // ===== Config::Configurable ===================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual void DisplayHelp(FILE* aOut) const;

        private:

            typedef std::list<File::Folder> FolderList;

            Import(const Import &);

            const Import & operator = (const Import &);

            File::Folder mImport;

            // ===== Configurable attributes ================================
            StringSet  mDependencies;
            StringSet  mOSIndependentDeps;
            FolderList mRepositories;

        };

    }
}

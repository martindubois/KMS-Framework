
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Build.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>

namespace KMS
{
    class Version;

    namespace Build
    {
        
        class Build : public Config::Configurable
        {

        public:

            static int Main(int aCount, const char** aVector);

            Build();

            ~Build();

            void AddBinary       (const char * aB);
            void AddConfiguration(const char * aC);
            void AddEditOperation(const char * aC);
            void AddLibrary      (const char * aL);
            void AddProcessor    (const char * aP);
            void AddTest         (const char * aT);

            void SetDoNotCompile(bool aDNC = true);
            void SetDoNotExport (bool aDNE = true);
            void SetDoNotPackage(bool aDNP = true);

            void SetProduct     (const char *aP );
            void SetProductShort(const char *aPS);
            void SetVersionFile (const char* aVF);

            int Run();

            // ===== Config::Configurable ===================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual bool SetAttribute(const char *aA);
            virtual bool SetAttribute(const char* aA, const char* aV);

        private:

            Build(const Build &);

            const Build & operator = (const Build &);

            void Compile();
            void Edit  (const Version & aVersion);
            void Export(const Version & aVersion);
            void Package();
            void Package_Component();
            void Package_Header();
            void Package_ReadMe();
            void Test();
            void VerifyConfig();

            File::Folder mTempFolder;

            // ===== Configurable attributes ================================
            StringSet    mBinaries;
            StringSet    mConfigurations;
            bool         mDoNotCompile;
            bool         mDoNotExport;
            bool         mDoNotPackage;
            StringSet    mEditOperations;
            File::Folder mExportFolder;
            StringSet    mLibraries;
            StringSet    mProcessors;
            std::string  mProduct;
            std::string  mProductShort;
            StringSet    mTests;
            std::string  mVersionFile;

        };

    }
}

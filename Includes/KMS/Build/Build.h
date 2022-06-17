
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
            void AddTest         (const char * aT);

            void SetDoNotCompile(bool aDNC = true);
            void SetDoNotExport (bool aDNE = true);
            void SetDoNotPackage(bool aDNP = true);

            void SetProduct     (const char *aP );
            void SetProductShort(const char *aPS);
            void SetVersionFile (const char* aVF);

            int Run();

            #ifdef _KMS_WINDOWS_
                void AddProcessor(const char * aP);
            #endif

            // ===== Config::Configurable ===================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual bool SetAttribute(const char* aA, const char* aV);
            virtual void DisplayHelp(FILE* aOut) const;

        private:

            Build(const Build &);

            const Build & operator = (const Build &);

            void Compile();
            void Compile(const char* aC);
            void Edit  (const Version & aVersion);
            void Export(const Version & aVersion);
            void Package();
            void Package_Component();
            void Package_Component(const char* aC);
            void Package_Header();
            void Package_ReadMe();
            void Test();
            void Test(const char* aC);
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
            std::string  mProduct;
            std::string  mProductShort;
            StringSet    mTests;
            std::string  mVersionFile;

            #ifdef _KMS_WINDOWS_
                StringSet mProcessors;
            #endif

        };

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Build/Build.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/Folder.h>
#include <KMS/DI/String.h>
#include <KMS/File/Folder.h>
#include <KMS/Types.h>
#include <KMS/Version.h>

namespace KMS
{

    namespace Build
    {
        
        class Build : public DI::Dictionary
        {

        public:

            static int Main(int aCount, const char** aVector);

            Build();

            ~Build();

            void AddBinary       (const char* aB);
            void AddConfiguration(const char* aC);
            void AddEditOperation(const char* aC);
            void AddFile         (const char* aF);
            void AddFolder       (const char* aF);
            void AddLibrary      (const char* aL);
            void AddPreBuildCmd  (const char* aC);
            void AddTest         (const char* aT);

            int Run();

            #ifdef _KMS_WINDOWS_
                void AddProcessor(const char * aP);
            #endif

            // ===== Configurable attributes ================================
            DI::Boolean mDoNotCompile;
            DI::Boolean mDoNotExport;
            DI::Boolean mDoNotPackage;
            DI::String  mEmbedded;
            DI::Folder  mExportFolder;
            DI::Boolean mOSIndependent;
            DI::String  mProduct;
            DI::String  mVersionFile;

        private:

            Build(const Build &);

            const Build & operator = (const Build &);

            bool IsEmbedded() const;

            void Compile();
            void Compile_Make(const char* aC);
            void Compile_VisualStudio(const char* aC);
            void Edit();
            void ExecuteCommands(const DI::Array& aCommands);
            void Export();
            void Package();
            void Package_Components();
            void Package_Components(const char* aC);
            void Package_Components_Embedded(const char* aC);
            void Package_Files();
            void Package_Folders();
            void Package_Libraries(const char* aC, const char* aExtension);
            void Test();
            void Test(const char* aC);
            void VerifyConfig();

            #ifdef _KMS_WINDOWS_
                void Export_WindowsFile_MSI();
            #endif

            File::Folder mProductFolder;
            File::Folder mTempFolder;
            Version      mVersion;

            // ===== Configurable attributes ================================
            DI::Array    mBinaries;
            DI::Array    mConfigurations;
            DI::Array    mEditOperations;
            DI::Array    mFiles;
            DI::Array    mFolders;
            DI::Array    mLibraries;
            DI::Array    mPreBuildCmds;
            DI::Array    mTests;

            #ifdef _KMS_WINDOWS_
                DI::String mWindowsFile_MSI;
                DI::Array  mWindowsProcessors;
            #endif

        };

    }
}

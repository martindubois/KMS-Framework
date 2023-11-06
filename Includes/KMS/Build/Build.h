
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
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
        
        class Build final : public DI::Dictionary
        {

        public:

            static const bool  DO_NOT_COMPILE_DEFAULT;
            static const bool  DO_NOT_EXPORT_DEFAULT;
            static const bool  DO_NOT_PACKAGE_DEFAULT;
            static const bool  DO_NOT_TEST_DEFAULT;
            static const char* EMBEDDED_DEFAULT;
            static const char* EXPORT_FOLDER_DEFAULT;
            static const bool  OS_INDEPENDENT_DEFAULT;
            static const char* PRODUCT_DEFAULT;
            static const char* VERSION_FILE_DEFAULT;

            DI::Array   mBinaries;
            DI::Array   mConfigurations;
            DI::Boolean mDoNotCompile;
            DI::Boolean mDoNotExport;
            DI::Boolean mDoNotPackage;
            DI::Boolean mDoNotTest;
            DI::Array   mDrivers;
            DI::Array   mEditOperations;
            DI::String  mEmbedded;
            DI::Folder  mExportFolder;
            DI::Array   mFiles;
            DI::Array   mFolders;
            DI::Array   mLibraries;
            DI::Boolean mOSIndependent;
            DI::Array   mPreBuildCmds;
            DI::Array   mProcessors;
            DI::String  mProduct;
            DI::String  mVersionFile;
            DI::Array   mTests;

            #ifdef _KMS_WINDOWS_
                static const char* CERTIFICAT_SHA1_DEFAULT;

                DI::String_Expand mCertificatSHA1;
            #endif

        public:

            static int Main(int aCount, const char** aVector);

            Build();

            int Run();

            // ===== DI::Container ==========================================
            virtual void Validate() const;

        private:

            NO_COPY(Build);

            bool IsEmbedded() const;

            void Compile();
            void Compile_Make(const char* aC);
            void Compile_Make(const char* aC, const char* aP);
            void Compile_VisualStudio(const char* aC);
            void Compile_VisualStudio(const char* aC, const char* aP);
            void CreateInstaller();
            void Edit();
            void ExecuteCommands(const DI::Array& aCommands);
            void Export();
            void Package();
            void Package_Components();
            void Package_Components(const char* aC);
            void Package_Components(const char* aC, const char* aP);
            void Package_Components_Embedded(const char* aC);
            void Package_Files();
            void Package_Folders();
            void Package_Libraries(const char* aC, const char* aExtension);
            void Test();
            void Test(const char* aC);
            void Test(const char* aC, const char* aP);

            #ifdef _KMS_WINDOWS_
                void CreateDriverCab(const char* aC, const char* aCabFile);
                void CreateInstaller(const char* aP);
            #endif

            File::Folder mProductFolder;
            Version      mVersion;

            // ===== Folders ================================================
            File::Folder mTmp_Binaries;
            File::Folder mTmp_Drivers;
            File::Folder mTmp_Libraries;
            File::Folder mTmp_Root;

        };

    }
}

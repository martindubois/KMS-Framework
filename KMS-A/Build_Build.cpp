
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Config/Configurator.h>
#include <KMS/File/FileInfoList.h>
#include <KMS/Process.h>
#include <KMS/Version.h>

#include <KMS/Build/Build.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Build.cfg")

#define DEFAULT_DO_NOT_COMPILE (false)
#define DEFAULT_DO_NOT_EXPORT  (false)
#define DEFAULT_EXPORT_FOLDER  ("K:\\Export")

#define MSBUILD_FOLDER ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        int Build::Main(int aCount, const char ** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            try
            {
                KMS::Build::Build         lB;
                KMS::Config::Configurator lC;

                lB.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lResult = lB.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Build::Build()
            : mDoNotCompile(DEFAULT_DO_NOT_COMPILE)
            , mDoNotExport(DEFAULT_DO_NOT_EXPORT)
            , mExportFolder(DEFAULT_EXPORT_FOLDER)
            , mTempFolder(File::Folder::Id::TEMPORARY)
        {
        }

        void Build::AddBinary       (const char* aB) { assert(NULL != aB); mBinaries      .insert(aB); }
        void Build::AddConfiguration(const char* aC) { assert(NULL != aC); mConfigurations.insert(aC); }
        void Build::AddLibrary      (const char* aL) { assert(NULL != aL); mLibraries     .insert(aL); }
        void Build::AddProcessor    (const char* aP) { assert(NULL != aP); mProcessors    .insert(aP); }
        void Build::AddTest         (const char* aT) { assert(NULL != aT); mTests         .insert(aT); }

        void Build::SetDoNotCompile(bool aDNC) { mDoNotCompile = aDNC; }
        void Build::SetDoNotExport (bool aDNE) { mDoNotExport  = aDNE; }

        void Build::SetProduct     (const char* aP ) { assert(NULL != aP ); mProduct      = aP ; }
        void Build::SetProductShort(const char* aPS) { assert(NULL != aPS); mProductShort = aPS; }

        int Build::Run()
        {
            // TODO Verify the attributes

            if (!mDoNotCompile)
            {
                Compile();
            }

            Test();

            Package();

            if (!mDoNotExport)
            {
                Export();
            }

            return 0;
        }

        // ===== Config::Configurable =======================================

        Build::~Build() {}

        bool Build::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            if (0 == strcmp(aA, "Binaries"      )) { AddBinary       (aV); return true; }
            if (0 == strcmp(aA, "Configurations")) { AddConfiguration(aV); return true; }
            if (0 == strcmp(aA, "Libraries"     )) { AddLibrary      (aV); return true; }
            if (0 == strcmp(aA, "Processors"    )) { AddProcessor    (aV); return true; }
            if (0 == strcmp(aA, "Tests"         )) { AddTest         (aV); return true; }

            return Configurable::AddAttribute(aA, aV);
        }

        bool Build::SetAttribute(const char* aA)
        {
            assert(NULL != aA);

            if (0 == strcmp(aA, "DoNotCompile")) { SetDoNotCompile(); return true; }
            if (0 == strcmp(aA, "DoNotExport" )) { SetDoNotExport (); return true; }

            return Configurable::SetAttribute(aA);
        }

        bool Build::SetAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            if (0 == strcmp(aA, "Product"     )) { SetProduct     (aV); return true; }
            if (0 == strcmp(aA, "ProductShort")) { SetProductShort(aV); return true; }

            return Configurable::SetAttribute(aA, aV);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile()
        {
            for (std::string lC : mConfigurations)
            {
                for (std::string lP : mProcessors)
                {
                    File::Folder lProgramFiles(File::Folder::Id::PROGRAM_FILES);
                    File::Folder lBin(lProgramFiles, MSBUILD_FOLDER);

                    KMS::Process lProcess(lBin, "MSBuild.exe");

                    lProcess.AddArgument("Solution.sln");
                    lProcess.AddArgument("/target:rebuild");

                    lProcess.AddArgument(("/Property:Configuration=" + lC).c_str());
                    lProcess.AddArgument(("/property:Platform="      + lP).c_str());

                    int lRet = lProcess.Run();
                    if (0 != lRet)
                    {
                        KMS_EXCEPTION_WITH_INFO(BUILD_COMPILE, "The compilation failed", lProcess.GetCmdLine());
                    }
                }
            }
        }

        void Build::Export()
        {
            Version lVersion(File::Folder("Common"), "Version.h");

            File::Folder lProduct(mExportFolder, mProduct.c_str());

            char lPackage[1024];

            lVersion.GetPackageName(mProduct.c_str(), lPackage, sizeof(lPackage));

            char lZip[1024];

            sprintf_s(lZip, "%s.zip", lPackage);

            mTempFolder.Compress(lProduct, lZip);
        }

        void Build::Package()
        {
            Package_Component();
            Package_Header();
            Package_ReadMe();
        }

        void Build::Package_Component()
        {
            File::Folder lBinaries(mTempFolder, "Binaries");
            File::Folder lLibraries(mTempFolder, "Libraries");

            lBinaries.Create();
            lLibraries.Create();

            for (std::string lC : mConfigurations)
            {
                for (std::string lP : mProcessors)
                {
                    std::string lCfg = lC;

                    lCfg += "_";
                    lCfg += lP;

                    File::Folder lBin(lBinaries, lCfg.c_str());
                    File::Folder lLib(lLibraries, lCfg.c_str());
                    File::Folder lOut_Src((lP + "\\" + lC).c_str());

                    lBin.Create();
                    lLib.Create();

                    for (std::string lB : mBinaries)
                    {
                        lOut_Src.Copy(lBin, (lB + ".exe").c_str());
                        lOut_Src.Copy(lBin, (lB + ".pdb").c_str());
                    }

                    for (std::string lB : mLibraries)
                    {
                        lOut_Src.Copy(lLib, (lB + ".lib").c_str());
                        lOut_Src.Copy(lLib, (lB + ".pdb").c_str());
                    }
                }
            }
        }

        void Build::Package_Header()
        {
            File::Folder lIncludes_Src("Includes");

            File::FileInfoList lHeaders(lIncludes_Src, "*", true);

            if (0 < lHeaders.GetCount())
            {
                File::Folder lIncludes(mTempFolder, "Includes");

                lIncludes.Create();

                lHeaders.Copy(lIncludes);
            }
        }

        void Build::Package_ReadMe()
        {
            File::Folder("_DocUser").Copy(mTempFolder, (mProductShort + ".ReadMe.txt").c_str());

            for (std::string lB : mBinaries)
            {
                File::Folder(lB.c_str(), "_DocUser").Copy(mTempFolder, (mProductShort + "." + lB + ".ReadMe.txt").c_str());
            }

            for (std::string lL : mLibraries)
            {
                File::Folder(lL.c_str(), "_DocUser").Copy(mTempFolder, (mProductShort + "." + lL + ".ReadMe.txt").c_str());
            }
        }

        void Build::Test()
        {
            for (std::string lC : mConfigurations)
            {
                for (std::string lP : mProcessors)
                {
                    for (std::string lT : mTests)
                    {
                        KMS::Process lProcess((lP + "\\" + lC).c_str(), (lT + ".exe").c_str());

                        int lRet = lProcess.Run();
                        if (0 != lRet)
                        {
                            KMS_EXCEPTION_WITH_INFO(BUILD_TEST, "The test failed", lProcess.GetCmdLine());
                        }
                    }
                }
            }
        }

    }
}

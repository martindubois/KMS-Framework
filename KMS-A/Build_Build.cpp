
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
#include <KMS/SafeAPI.h>
#include <KMS/Text/TextFile.h>
#include <KMS/Version.h>

#include <KMS/Build/Build.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Build.cfg")

#define DEFAULT_DO_NOT_COMPILE (false)
#define DEFAULT_DO_NOT_EXPORT  (false)
#define DEFAULT_DO_NOT_PACKAGE (false)
#define DEFAULT_EXPORT_FOLDER  ("K:\\Export")
#define DEFAULT_VERSION_FILE   ("Common\\Version.h")

#define MSBUILD_FOLDER ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")

// Constants
// //////////////////////////////////////////////////////////////////////////

#define STRING_LEN (1024)

// Static function declarataions
// //////////////////////////////////////////////////////////////////////////

static void operator += (std::string& aString, unsigned int aValue);

static std::string ProcessReplaceLine(const char * aIn, const KMS::Version & aVersion);

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
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE, false);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lResult = lB.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Build::Build()
            : mDoNotCompile(DEFAULT_DO_NOT_COMPILE)
            , mDoNotExport(DEFAULT_DO_NOT_EXPORT)
            , mDoNotPackage(DEFAULT_DO_NOT_PACKAGE)
            , mExportFolder(DEFAULT_EXPORT_FOLDER)
            , mTempFolder(File::Folder::Id::TEMPORARY)
            , mVersionFile(DEFAULT_VERSION_FILE)
        {
        }

        Build::~Build() {}

        void Build::AddBinary       (const char* aB) { assert(NULL != aB); mBinaries      .insert(aB); }
        void Build::AddConfiguration(const char* aC) { assert(NULL != aC); mConfigurations.insert(aC); }
        void Build::AddEditOperation(const char* aC) { assert(NULL != aC); mEditOperations.insert(aC); }
        void Build::AddLibrary      (const char* aL) { assert(NULL != aL); mLibraries     .insert(aL); }
        void Build::AddProcessor    (const char* aP) { assert(NULL != aP); mProcessors    .insert(aP); }
        void Build::AddTest         (const char* aT) { assert(NULL != aT); mTests         .insert(aT); }

        void Build::SetDoNotCompile(bool aDNC) { mDoNotCompile = aDNC; }
        void Build::SetDoNotExport (bool aDNE) { mDoNotExport  = aDNE; }
        void Build::SetDoNotPackage(bool aDNP) { mDoNotPackage = aDNP; }

        void Build::SetProduct     (const char* aP ) { assert(NULL != aP ); mProduct      = aP ; }
        void Build::SetProductShort(const char* aPS) { assert(NULL != aPS); mProductShort = aPS; }
        void Build::SetVersionFile (const char* aVF) { assert(NULL != aVF); mVersionFile  = aVF; }

        int Build::Run()
        {
            VerifyConfig();

            Version lVersion(File::Folder(File::Folder::Id::CURRENT), mVersionFile.c_str());

            Edit(lVersion);

            if (!mDoNotCompile)
            {
                Compile();
            }

            Test();

            if (!mDoNotPackage)
            {
                Package();
            }

            if (!mDoNotExport)
            {
                Export(lVersion);
            }

            return 0;
        }

        // ===== Config::Configurable =======================================

        bool Build::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            if (0 == strcmp(aA, "Binaries"      )) { AddBinary       (aV); return true; }
            if (0 == strcmp(aA, "Configurations")) { AddConfiguration(aV); return true; }
            if (0 == strcmp(aA, "EditOperations")) { AddEditOperation(aV); return true; }
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
            if (0 == strcmp(aA, "DoNotPackage")) { SetDoNotPackage(); return true; }

            return Configurable::SetAttribute(aA);
        }

        bool Build::SetAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            if (0 == strcmp(aA, "Product"     )) { SetProduct     (aV); return true; }
            if (0 == strcmp(aA, "ProductShort")) { SetProductShort(aV); return true; }
            if (0 == strcmp(aA, "VersionFile" )) { SetVersionFile (aV); return true; }

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

        void Build::Edit(const Version& aVersion)
        {
            File::Folder lCurrent(File::Folder::Id::CURRENT);

            for (std::string lOp : mEditOperations)
            {
                char lFile[1024];
                char lRegEx[1024];
                char lReplace[STRING_LEN];

                if (3 != sscanf_s(lOp.c_str(), "%[^;];%[^;];%[^\n\r]", lFile SizeInfo(lFile), lRegEx SizeInfo(lRegEx), lReplace SizeInfo(lReplace)))
                {
                    KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid edit operation", lOp.c_str());
                }

                std::string lReplaceStr = ProcessReplaceLine(lReplace, aVersion);

                Text::TextFile lText;

                lText.Read(lCurrent, lFile);

                lText.ReplaceLines(lRegEx, lReplaceStr.c_str());

                lCurrent.Backup(lFile);

                lText.Write(lCurrent, lFile);
            }
        }

        void Build::Export(const Version& aVersion)
        {
            File::Folder lProduct(mExportFolder, mProduct.c_str());

            char lPackage[1024];

            aVersion.GetPackageName(mProduct.c_str(), lPackage, sizeof(lPackage));

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

        void Build::VerifyConfig()
        {
            if (!mDoNotCompile)
            {
                if (0 >= mConfigurations.size())
                {
                    KMS_EXCEPTION(CONFIG, "No configuration");
                }

                if (0 >= mProcessors.size())
                {
                    KMS_EXCEPTION(CONFIG, "No processor");
                }
            }

            if (!mDoNotExport)
            {
                if (!mExportFolder.DoesExist())
                {
                    KMS_EXCEPTION_WITH_INFO(CONFIG, "Invalid export folder", mExportFolder.GetPath());
                }

                if (0 >= mProduct.size())
                {
                    KMS_EXCEPTION(CONFIG, "Invalid product name");
                }
            }

            if (!mDoNotPackage)
            {
                if (0 >= mProductShort.size())
                {
                    KMS_EXCEPTION(CONFIG, "Invalid product short name");
                }
            }

            if (0 >= mVersionFile.size())
            {
                KMS_EXCEPTION(CONFIG, "Invalid version file");
            }
        }

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void operator += (std::string& aString, unsigned int aValue)
{
    char lValue[32];

    sprintf_s(lValue, "%u", aValue);

    aString += lValue;
}

std::string ProcessReplaceLine(const char * aIn, const KMS::Version & aVersion)
{
    // --> INIT <--+
    //      |      |
    //      +--> PROCESS
    static const unsigned int STATE_INIT    = 0;
    static const unsigned int STATE_PROCESS = 1;

    assert(NULL != aIn);

    const char * lIn = aIn;
    std::string  lResult;
    unsigned int lState = STATE_INIT;
    const char * lType = aVersion.GetType();

    if (0 >= strlen(lType))
    {
        lType = NULL;
    }

    for (;;)
    {
        switch (lState)
        {
        case STATE_INIT:
            switch (*lIn)
            {
            case '\0': return lResult;
            case '{': lState = STATE_PROCESS; break;
            default: lResult += *lIn;
            }
            break;

        case STATE_PROCESS:
            switch (*lIn)
            {
            case '\0': KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid edition operation", aIn);
            case '}': lState = STATE_INIT; break;
            case 'M': lResult += aVersion.GetMajor(); break;
            case 'm': lResult += aVersion.GetMinor(); break;
            case 'B': lResult += aVersion.GetBuild(); break;
            case 'C': lResult += aVersion.GetCompat(); break;

            case 'T':
                if (NULL != lType)
                {
                    lResult += "_";
                    lResult += aVersion.GetType();
                }
                break;

            default: lResult += *lIn;
            }
            break;

        default: assert(false);
        }

        lIn++;
    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Build/Make.h>
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/Color.h>
#include <KMS/Dbg/Log_Cfg.h>
#include <KMS/Dbg/Stats.h>
#include <KMS/Dbg/Stats_Timer.h>
#include <KMS/Installer.h>
#include <KMS/Text/File_ASCII.h>
#include <KMS/Version.h>

#include <KMS/Build/Build.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Build.cfg")

#define DEFAULT_DO_NOT_COMPILE (false)
#define DEFAULT_DO_NOT_EXPORT  (false)
#define DEFAULT_DO_NOT_PACKAGE (false)
#define DEFAULT_DO_NOT_TEST    (false)
#define DEFAULT_OS_INDEPENDENT (false)
#define DEFAULT_VERSION_FILE   ("Common" SLASH "Version.h")

#define MSBUILD_FOLDER ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_BINARIES       ("Binaries += {Name}");
static const KMS::Cfg::MetaData MD_CONFIGURATIONS ("Configurations += {Name}");
static const KMS::Cfg::MetaData MD_DO_NOT_COMPILE ("DoNotCompile = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_EXPORT  ("DoNotExport = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_PACKAGE ("DoNotPackage = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_TEST    ("DoNotTest = false | true");
static const KMS::Cfg::MetaData MD_EDIT_OPERATIONS("EditOperations += {Operation}");
static const KMS::Cfg::MetaData MD_EMBEDDED       ("Embedded = false | true");
static const KMS::Cfg::MetaData MD_EXPORT_FOLDER  ("ExportFolder = {Path}");
static const KMS::Cfg::MetaData MD_FILES          ("Files += {Path}");
static const KMS::Cfg::MetaData MD_FOLDERS        ("Folders += {Path}");
static const KMS::Cfg::MetaData MD_LIBRARIES      ("Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_INDEPENDENT ("OSIntependent = false | true");
static const KMS::Cfg::MetaData MD_PRE_BUILD_CMDS ("PreBuildCmds += {Command}");
static const KMS::Cfg::MetaData MD_PROCESSORS     ("Processors += x64 | x86 | ...");
static const KMS::Cfg::MetaData MD_PRODUCT        ("Product = {Name}");
static const KMS::Cfg::MetaData MD_TESTS          ("Tests += {Name}");
static const KMS::Cfg::MetaData MD_VERSION_FILE   ("VersionFile = {Path}");

#ifdef _KMS_DARWIN_
    #define NAME_OS "Darwin"
    #define NO_OS_0 "Linux"
    #define NO_OS_1 "Windows"
#endif

#ifdef _KMS_LINUX_
    #define NAME_OS "Linux"
    #define NO_OS_0 "Darwin"
    #define NO_OS_1 "Windows"
#endif

#ifdef _KMS_WINDOWS_
    #define NAME_OS "Windows"
    #define NO_OS_0 "Darwin"
    #define NO_OS_1 "Linux"

    static const KMS::Cfg::MetaData MD_WINDOWS_FILE_MSI  ("WindowsFile_MSI = {Path}");
#endif

static const KMS::Cfg::MetaData MD_OS_BINARIES      (NAME_OS "Binaries += {Name}");
static const KMS::Cfg::MetaData MD_OS_CONFIGURATIONS(NAME_OS "Configurations += {Name}");
static const KMS::Cfg::MetaData MD_OS_FILES         (NAME_OS "Files += {Path}");
static const KMS::Cfg::MetaData MD_OS_FOLDERS       (NAME_OS "Folders += {Path}");
static const KMS::Cfg::MetaData MD_OS_LIBRARIES     (NAME_OS "Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_PRE_BUILD_CMDS(NAME_OS "PreBuildCmds += {Command}");
static const KMS::Cfg::MetaData MD_OS_PROCESSORS    (NAME_OS "Processors += x64 | x86 | ...");
static const KMS::Cfg::MetaData MD_OS_TESTS         (NAME_OS "Tests += {Name}");

static const char* SILENCE[] =
{
    NO_OS_0 "Binaries"      , NO_OS_1 "Binaries"      ,
    NO_OS_0 "Configurations", NO_OS_1 "Configurations",
    NO_OS_0 "Files"         , NO_OS_1 "Files"         ,
    NO_OS_0 "Folders"       , NO_OS_1 "Folders"       ,
    NO_OS_0 "Libraries"     , NO_OS_1 "Libraries"     ,
    NO_OS_0 "PreBuildCmds"  , NO_OS_1 "PreBuildCmds"  ,
    NO_OS_0 "Processors"    , NO_OS_1 "Processors"    ,
    NO_OS_0 "Tests"         , NO_OS_1 "Tests"         ,

    #if defined(_KMS_DARWIN_) || defined(_KMS_LINUX_)
        "WindowsFile_MSI"  ,
    #endif

    nullptr
};

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
            assert(nullptr != aVector);
            assert(nullptr != aVector[0]);

            int lResult = __LINE__;

            auto lET = new Dbg::Stats_Timer("ExecutionTime");
            lET->Start();

            try
            {
                Build             lB;
                Cfg::Configurator lC;
                Installer         lInstaller;
                Dbg::Log_Cfg      lLogCfg(&Dbg::gLog);

                lC.SetSilence(SILENCE);

                lC.AddConfigurable(&lB);
                lC.AddConfigurable(&lInstaller);

                lC.AddConfigurable(&lLogCfg);
                lC.AddConfigurable(&Dbg::gStats);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE, true);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lInstaller.Run();

                lResult = lB.Run();
            }
            KMS_CATCH_RESULT(lResult)

            lET->Stop();

            return lResult;
        }

        Build::Build()
            : mDoNotCompile (DEFAULT_DO_NOT_COMPILE)
            , mDoNotExport  (DEFAULT_DO_NOT_EXPORT)
            , mDoNotPackage (DEFAULT_DO_NOT_PACKAGE)
            , mDoNotTest    (DEFAULT_DO_NOT_TEST)
            , mOSIndependent(DEFAULT_OS_INDEPENDENT)
            , mVersionFile  (DEFAULT_VERSION_FILE)
            , mTmp_Root(File::Folder::Id::TEMPORARY)
            #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
                , mExportFolder(File::Folder(File::Folder::Id::HOME, "Export"))
            #endif
            #ifdef _KMS_WINDOWS_
                , mExportFolder("K:\\Export")
            #endif
        {
            mBinaries      .SetCreator(DI::String::Create);
            mConfigurations.SetCreator(DI::String::Create);
            mEditOperations.SetCreator(DI::String::Create);
            mFiles         .SetCreator(DI::String::Create);
            mFolders       .SetCreator(DI::String::Create);
            mLibraries     .SetCreator(DI::String::Create);
            mPreBuildCmds  .SetCreator(DI::String::Create);
            mProcessors    .SetCreator(DI::String::Create);
            mTests         .SetCreator(DI::String::Create);

            AddEntry("Binaries"      , &mBinaries      , false, &MD_BINARIES);
            AddEntry("Configurations", &mConfigurations, false, &MD_CONFIGURATIONS);
            AddEntry("DoNotCompile"  , &mDoNotCompile  , false, &MD_DO_NOT_COMPILE);
            AddEntry("DoNotExport"   , &mDoNotExport   , false, &MD_DO_NOT_EXPORT);
            AddEntry("DoNotPackage"  , &mDoNotPackage  , false, &MD_DO_NOT_PACKAGE);
            AddEntry("DoNotTest"     , &mDoNotTest     , false, &MD_DO_NOT_TEST);
            AddEntry("EditOperations", &mEditOperations, false, &MD_EDIT_OPERATIONS);
            AddEntry("Embedded"      , &mEmbedded      , false, &MD_EMBEDDED);
            AddEntry("ExportFolder"  , &mExportFolder  , false, &MD_EXPORT_FOLDER);
            AddEntry("Files"         , &mFiles         , false, &MD_FILES);
            AddEntry("Folders"       , &mFolders       , false, &MD_FOLDERS);
            AddEntry("Libraries"     , &mLibraries     , false, &MD_LIBRARIES);
            AddEntry("OSIndependent" , &mOSIndependent , false, &MD_OS_INDEPENDENT);
            AddEntry("PreBuildCmds"  , &mPreBuildCmds  , false, &MD_PRE_BUILD_CMDS);
            AddEntry("Processors"    , &mProcessors    , false, &MD_PROCESSORS);
            AddEntry("Product"       , &mProduct       , false, &MD_PRODUCT);
            AddEntry("Tests"         , &mTests         , false, &MD_TESTS);
            AddEntry("VersionFile"   , &mVersionFile   , false, &MD_VERSION_FILE);

            AddEntry(NAME_OS "Binaries"      , &mBinaries      , false, &MD_OS_BINARIES);
            AddEntry(NAME_OS "Configurations", &mConfigurations, false, &MD_OS_CONFIGURATIONS);
            AddEntry(NAME_OS "Files"         , &mFiles         , false, &MD_OS_FILES);
            AddEntry(NAME_OS "Folders"       , &mFolders       , false, &MD_OS_FOLDERS);
            AddEntry(NAME_OS "Libraries"     , &mLibraries     , false, &MD_OS_LIBRARIES);
            AddEntry(NAME_OS "PreBuildCmds"  , &mPreBuildCmds  , false, &MD_OS_PRE_BUILD_CMDS);
            AddEntry(NAME_OS "Processors"    , &mProcessors    , false, &MD_OS_PROCESSORS);
            AddEntry(NAME_OS "Tests"         , &mTests         , false, &MD_OS_TESTS);

            #ifdef _KMS_WINDOWS_
                AddEntry("WindowsFile_MSI"  , &mWindowsFile_MSI  , false, &MD_WINDOWS_FILE_MSI);
            #endif

            mTmp_Binaries  = File::Folder(mTmp_Root, "Binaries" );
            mTmp_Libraries = File::Folder(mTmp_Root, "Libraries");
        }

        Build::~Build() {}

        void Build::AddBinary       (const char* aB) { assert(nullptr != aB); mBinaries      .AddEntry(new DI::String(aB), true); }
        void Build::AddConfiguration(const char* aC) { assert(nullptr != aC); mConfigurations.AddEntry(new DI::String(aC), true); }
        void Build::AddEditOperation(const char* aE) { assert(nullptr != aE); mEditOperations.AddEntry(new DI::String(aE), true); }
        void Build::AddFile         (const char* aF) { assert(nullptr != aF); mFiles         .AddEntry(new DI::String(aF), true); }
        void Build::AddFolder       (const char* aF) { assert(nullptr != aF); mFolders       .AddEntry(new DI::String(aF), true); }
        void Build::AddLibrary      (const char* aL) { assert(nullptr != aL); mLibraries     .AddEntry(new DI::String(aL), true); }
        void Build::AddPreBuildCmd  (const char* aC) { assert(nullptr != aC); mPreBuildCmds  .AddEntry(new DI::String(aC), true); }
        void Build::AddProcessor    (const char* aP) { assert(nullptr != aP); mProcessors    .AddEntry(new DI::String(aP), true); }
        void Build::AddTest         (const char* aT) { assert(nullptr != aT); mTests         .AddEntry(new DI::String(aT), true); }

        int Build::Run()
        {
            VerifyConfig();

            mVersion = Version(File::Folder::CURRENT, mVersionFile);

            Edit();

            ExecuteCommands(mPreBuildCmds);

            if (!mDoNotCompile) { Compile(); }
            if (!mDoNotTest   ) { Test   (); }
            if (!mDoNotPackage) { Package(); }
            if (!mDoNotExport ) { Export (); }

            return 0;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Build::IsEmbedded() const { return 0 < mEmbedded.GetLength(); }

        void Build::Compile()
        {
            auto lCT = new Dbg::Stats_Timer("CompileTime.Configuration");

            for (const auto& lEntry : mConfigurations.mInternal)
            {
                lCT->Start();

                auto lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lC);

                if (IsEmbedded())
                {
                    Compile_Make(*lC);
                }
                else
                {
                    #ifdef _KMS_LINUX_
                        Compile_MaKe(*lC);
                    #endif

                    #ifdef _KMS_WINDOWS_
                        Compile_VisualStudio(*lC);
                    #endif
                }

                lCT->Stop();
            }
        }

        void Build::Compile_Make(const char* aC)
        {
            auto lCT = new Dbg::Stats_Timer("CompileTime.Processor");

            for (const auto& lEntry : mProcessors.mInternal)
            {
                lCT->Start();

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                Compile_Make(aC, *lP);

                lCT->Stop();
            }
        }

        void Build::Compile_Make(const char* aC, const char* aP)
        {
            Cfg::Configurator lC;
            Make              lM;
            Dbg::Log_Cfg      lLogCfg(&Dbg::gLog);

            lC.SetSilence(Make::SILENCE);

            lC.AddConfigurable(&lM);

            lC.AddConfigurable(&lLogCfg);

            lC.ParseFile(File::Folder::CURRENT, "KMS-Build.cfg");
            lC.ParseFile(File::Folder::CURRENT, "KMS-Make.cfg");

            lM.mConfiguration.Set(aC);
            lM.mProcessor    .Set(aP);

            lM.AddCommand("Clean");
            lM.AddCommand("Make");

            auto lRet = lM.Run();
            KMS_EXCEPTION_ASSERT(0 == lRet, BUILD_COMPILE_FAILED, "KMS::Build::Make::Run failed", lRet);
        }

        void Build::Edit()
        {
            unsigned int lOpIndex = 0;

            for (const auto& lEntry : mEditOperations.mInternal)
            {
                assert(nullptr != lEntry);

                auto lOp = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lOp);

                char lFile   [FILE_LENGTH];
                char lRegEx  [LINE_LENGTH];
                char lReplace[LINE_LENGTH];

                if (3 != sscanf_s(*lOp, "%[^;];%[^;];%[^\n\r]", lFile SizeInfo(lFile), lRegEx SizeInfo(lRegEx), lReplace SizeInfo(lReplace)))
                {
                    KMS_EXCEPTION(BUILD_CONFIG_INVALID, "Invalid edit operation", *lOp);
                }

                auto lReplaceStr = ProcessReplaceLine(lReplace, mVersion);

                Text::File_ASCII lText;

                lText.Read(File::Folder::CURRENT, lFile);

                auto lCount = lText.ReplaceLines(lRegEx, lReplaceStr.c_str());
                if (0 == lCount)
                {
                    std::cout << Console::Color::RED;
                    {
                        std::cout << "Edit operation " << lOpIndex << " - 0 line replaced";
                    }
                    std::cout << Console::Color::WHITE;
                }
                else
                {
                    std::cout << "Edit operation " << lOpIndex << " - " << lCount << " line replaced";

                    File::Folder::CURRENT.Backup(lFile);

                    lText.Write(File::Folder::CURRENT, lFile);
                }

                std::cout << std::endl;

                lOpIndex++;
            }
        }

        void Build::ExecuteCommands(const DI::Array& aCommands)
        {
            for (const auto& lEntry : aCommands.mInternal)
            {
                assert(nullptr != lEntry);

                auto lCommand = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lCommand);

                if (0 != system(*lCommand))
                {
                    KMS_EXCEPTION(BUILD_COMMAND_FAILED, "The command failed", *lCommand);
                }
            }
        }

        void Build::Export()
        {
            mProductFolder = File::Folder(mExportFolder, mProduct);
            if (!mProductFolder.DoesExist())
            {
                mProductFolder.Create();
            }

            unsigned int lFlags = (IsEmbedded() || mOSIndependent) ? Version::FLAG_OS_INDEPENDENT : 0;

            char lPackage[FILE_LENGTH];

            mVersion.GetPackageName(mProduct, lPackage, sizeof(lPackage), lFlags);

            mTmp_Root.Compress(mProductFolder, lPackage);

            #ifdef _KMS_WINDOWS_
                Export_WindowsFile_MSI();
            #endif
        }

        void Build::Package()
        {
            Package_Components();
            Package_Files();
            Package_Folders();
        }

        void Build::Package_Components()
        {
            if (!mBinaries .IsEmpty()) { mTmp_Binaries .Create(); }
            if (!mLibraries.IsEmpty()) { mTmp_Libraries.Create(); }

            for (const auto& lEntry : mConfigurations.mInternal)
            {
                auto lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lC);

                if (IsEmbedded())
                {
                    Package_Components_Embedded(*lC);
                }
                else
                {
                    Package_Components(*lC);
                }
            }
        }

        void Build::Package_Components_Embedded(const char* aC)
        {
            for (const auto& lEntryP : mProcessors.mInternal)
            {
                auto lP = dynamic_cast<const DI::String*>(lEntryP.Get());
                assert(nullptr != lP);

                std::string lFolder = std::string(aC) + "_" + lP->Get();

                if (!mBinaries.IsEmpty())
                {
                    File::Folder lBin(mTmp_Binaries, lFolder.c_str());
                    File::Folder lBin_Src((std::string("Binaries/") + lFolder).c_str());

                    lBin.Create();

                    for (const auto& lEntry : mBinaries.mInternal)
                    {
                        auto lB = dynamic_cast<const DI::String*>(lEntry.Get());

                        lBin_Src.Copy(lBin, (lB->mInternal + ".elf").c_str());
                    }
                }

                if (!mLibraries.IsEmpty())
                {
                    File::Folder lLib(mTmp_Libraries, lFolder.c_str());
                    File::Folder lLib_Src((std::string("Libraries/") + lFolder).c_str());

                    lLib.Create();

                    for (const auto& lEntry : mLibraries.mInternal)
                    {
                        auto lL = dynamic_cast<const DI::String*>(lEntry.Get());

                        lLib_Src.Copy(lLib, (lL->mInternal + ".a").c_str());
                    }
                }
            }
        }

        void Build::Package_Files()
        {
            for (const auto& lEntry : mFiles.mInternal)
            {
                assert(nullptr != lEntry);

                auto lF = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lF);

                auto lPtr = strrchr(*lF, '/');
                if (nullptr == lPtr)
                {
                    lPtr = *lF;
                }
                else
                {
                    lPtr++;
                }

                File::Folder::CURRENT.Copy(mTmp_Root, *lF, lPtr);
            }
        }

        void Build::Package_Folders()
        {
            for (const auto& lEntry : mFolders.mInternal)
            {
                assert(nullptr != lEntry);

                auto lF = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lF);

                char lDst[NAME_LENGTH];
                char lSrc[NAME_LENGTH];

                if (2 != sscanf_s(lF->Get(), "%[^;];%[^\n\r]", lSrc SizeInfo(lSrc), lDst SizeInfo(lDst)))
                {
                    KMS_EXCEPTION(BUILD_CONFIG_INVALID, "Invalid folder copy operation", *lF);
                }

                File::Folder lFD(mTmp_Root, lDst);
                File::Folder lFS(lSrc);

                lFS.Copy(lFD);
            }
        }

        void Build::Test()
        {
            for (const auto& lEntry : mConfigurations.mInternal)
            {
                assert(nullptr != lEntry);

                auto lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lC);

                Test(*lC);
            }
        }

        void Build::VerifyConfig()
        {
            if ((!mBinaries.IsEmpty()) || (!mLibraries.IsEmpty()))
            {
                if (!mDoNotCompile)
                {
                    KMS_EXCEPTION_ASSERT(!mConfigurations.IsEmpty(), BUILD_CONFIG_INVALID, "No configuration", "");
                }

                if (!mDoNotExport)
                {
                    char lMsg[64 + PATH_LENGTH];
                    sprintf_s(lMsg, "\"%s\" is not a valid export folder", mExportFolder.Get().GetPath());
                    KMS_EXCEPTION_ASSERT(mExportFolder.Get().DoesExist(), BUILD_CONFIG_INVALID, lMsg, "");

                    KMS_EXCEPTION_ASSERT(0 < mProduct.GetLength(), BUILD_CONFIG_INVALID, "Invalid product name", "");
                }
            }

            KMS_EXCEPTION_ASSERT(!mProcessors.IsEmpty()       , BUILD_CONFIG_INVALID, "No processor"        , "");
            KMS_EXCEPTION_ASSERT(0 < mVersionFile.GetLength (), BUILD_CONFIG_INVALID, "Invalid version file", "");
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void operator += (std::string& aString, unsigned int aValue)
{
    char lValue[32];

    sprintf_s(lValue, "%u", aValue);

    aString += lValue;
}

std::string ProcessReplaceLine(const char * aIn, const Version & aVersion)
{
    // --> INIT <--+
    //      |      |
    //      +--> PROCESS
    static const unsigned int STATE_INIT    = 0;
    static const unsigned int STATE_PROCESS = 1;

    assert(nullptr != aIn);

    auto         lIn = aIn;
    std::string  lResult;
    unsigned int lState = STATE_INIT;
    auto         lType = aVersion.GetType();

    if (0 >= strlen(lType))
    {
        lType = nullptr;
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
            case '\0': KMS_EXCEPTION(BUILD_CONFIG_INVALID, "Invalid edition operation", aIn);
            case '}': lState = STATE_INIT; break;
            case 'M': lResult += aVersion.GetMajor(); break;
            case 'm': lResult += aVersion.GetMinor(); break;
            case 'B': lResult += aVersion.GetBuild(); break;
            case 'C': lResult += aVersion.GetCompat(); break;

            case 'T':
                if (nullptr != lType)
                {
                    lResult += "-";
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

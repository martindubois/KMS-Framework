
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
#include <KMS/Build/Package.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Console/Color.h>
#include <KMS/Main.h>
#include <KMS/Text/File_ASCII.h>
#include <KMS/Version.h>

#include <KMS/Build/Build.h>

KMS_RESULT_STATIC(RESULT_COMMAND_FAILED);
KMS_RESULT_STATIC(RESULT_COMPILATION_FAILED);

#define FILE_EXT_A   ".a"
#define FILE_EXT_CFG ".cfg"
#define FILE_EXT_ELF ".elf"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Build" FILE_EXT_CFG)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_BINARIES       ("Binaries += {Name}");
static const KMS::Cfg::MetaData MD_CONFIGURATIONS ("Configurations += {Name}");
static const KMS::Cfg::MetaData MD_DO_NOT_CLEAN   ("DoNotRebuild = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_COMPILE ("DoNotCompile = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_EXPORT  ("DoNotExport = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_PACKAGE ("DoNotPackage = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_TEST    ("DoNotTest = false | true");
static const KMS::Cfg::MetaData MD_DRIVERS        ("Drivers += {Name}");
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
#endif

static const KMS::Cfg::MetaData MD_OS_BINARIES       (NAME_OS "Binaries += {Name}");
static const KMS::Cfg::MetaData MD_OS_CONFIGURATIONS (NAME_OS "Configurations += {Name}");
static const KMS::Cfg::MetaData MD_OS_DRIVERS        (NAME_OS "Drivers += {Name}");
static const KMS::Cfg::MetaData MD_OS_EDIT_OPERATIONS(NAME_OS "EditOperations += {Operation}");
static const KMS::Cfg::MetaData MD_OS_FILES          (NAME_OS "Files += {Path}");
static const KMS::Cfg::MetaData MD_OS_FOLDERS        (NAME_OS "Folders += {Path}");
static const KMS::Cfg::MetaData MD_OS_LIBRARIES      (NAME_OS "Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_PRE_BUILD_CMDS (NAME_OS "PreBuildCmds += {Command}");
static const KMS::Cfg::MetaData MD_OS_PROCESSORS     (NAME_OS "Processors += x64 | x86 | ...");
static const KMS::Cfg::MetaData MD_OS_TESTS          (NAME_OS "Tests += {Name}");

static const char* SILENCE[] =
{
    NO_OS_0 "Binaries"      , NO_OS_1 "Binaries"      ,
    NO_OS_0 "Configurations", NO_OS_1 "Configurations",
    NO_OS_0 "Drivers"       , NO_OS_1 "Drivers"       ,
    NO_OS_0 "EditOperations", NO_OS_1 "EditOperations",
    NO_OS_0 "Files"         , NO_OS_1 "Files"         ,
    NO_OS_0 "Folders"       , NO_OS_1 "Folders"       ,
    NO_OS_0 "Libraries"     , NO_OS_1 "Libraries"     ,
    NO_OS_0 "Packages"      , NO_OS_1 "Packages"      ,
    NO_OS_0 "PreBuildCmds"  , NO_OS_1 "PreBuildCmds"  ,
    NO_OS_0 "Processors"    , NO_OS_1 "Processors"    ,
    NO_OS_0 "Tests"         , NO_OS_1 "Tests"         ,

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

        const bool  Build::DO_NOT_CLEAN_DEFAULT   = false;
        const bool  Build::DO_NOT_COMPILE_DEFAULT = false;
        const bool  Build::DO_NOT_EXPORT_DEFAULT  = false;
        const bool  Build::DO_NOT_PACKAGE_DEFAULT = false;
        const bool  Build::DO_NOT_TEST_DEFAULT    = false;
        const char* Build::EMBEDDED_DEFAULT       = "";
        const bool  Build::OS_INDEPENDENT_DEFAULT = false;
        const char* Build::PRODUCT_DEFAULT        = "";
        const char* Build::VERSION_FILE_DEFAULT   = "Common" SLASH "Version.h";

        int Build::Main(int aCount, const char ** aVector)
        {
            KMS_MAIN_BEGIN;
            {
                Build lB;

                lConfigurator.SetSilence(SILENCE);

                lConfigurator.AddConfigurable(&lB);

                lConfigurator.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::CURRENT   , CONFIG_FILE, true);

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                lResult = lB.Run();
            }
            KMS_MAIN_END;

            KMS_MAIN_RETURN;
        }

        Build::Build()
            : mDoNotClean   (DO_NOT_CLEAN_DEFAULT)
            , mDoNotCompile (DO_NOT_COMPILE_DEFAULT)
            , mDoNotExport  (DO_NOT_EXPORT_DEFAULT)
            , mDoNotPackage (DO_NOT_PACKAGE_DEFAULT)
            , mDoNotTest    (DO_NOT_TEST_DEFAULT)
            , mEmbedded     (EMBEDDED_DEFAULT)
            , mOSIndependent(OS_INDEPENDENT_DEFAULT)
            , mProduct      (PRODUCT_DEFAULT)
            , mVersionFile  (VERSION_FILE_DEFAULT)
            , mTmp_Root(File::Folder::Id::TEMPORARY)
        {
            mBinaries      .SetCreator(DI::String::Create);
            mConfigurations.SetCreator(DI::String::Create);
            mDrivers       .SetCreator(DI::String::Create);
            mEditOperations.SetCreator(DI::String::Create);
            mFiles         .SetCreator(DI::String::Create);
            mFolders       .SetCreator(DI::String::Create);
            mLibraries     .SetCreator(DI::String::Create);
            mPreBuildCmds  .SetCreator(DI::String::Create);
            mProcessors    .SetCreator(DI::String::Create);
            mTests         .SetCreator(DI::String::Create);

            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mBinaries      , false); AddEntry("Binaries"      , lEntry, &MD_BINARIES);
            lEntry.Set(&mConfigurations, false); AddEntry("Configurations", lEntry, &MD_CONFIGURATIONS);
            lEntry.Set(&mDrivers       , false); AddEntry("Drivers"       , lEntry, &MD_DRIVERS);
            lEntry.Set(&mDoNotClean    , false); AddEntry("DoNotClean"    , lEntry, &MD_DO_NOT_CLEAN);
            lEntry.Set(&mDoNotCompile  , false); AddEntry("DoNotCompile"  , lEntry, &MD_DO_NOT_COMPILE);
            lEntry.Set(&mDoNotExport   , false); AddEntry("DoNotExport"   , lEntry, &MD_DO_NOT_EXPORT);
            lEntry.Set(&mDoNotPackage  , false); AddEntry("DoNotPackage"  , lEntry, &MD_DO_NOT_PACKAGE);
            lEntry.Set(&mDoNotTest     , false); AddEntry("DoNotTest"     , lEntry, &MD_DO_NOT_TEST);
            lEntry.Set(&mEditOperations, false); AddEntry("EditOperations", lEntry, &MD_EDIT_OPERATIONS);
            lEntry.Set(&mEmbedded      , false); AddEntry("Embedded"      , lEntry, &MD_EMBEDDED);
            lEntry.Set(&mExportFolder  , false); AddEntry("ExportFolder"  , lEntry, &MD_EXPORT_FOLDER);
            lEntry.Set(&mFiles         , false); AddEntry("Files"         , lEntry, &MD_FILES);
            lEntry.Set(&mFolders       , false); AddEntry("Folders"       , lEntry, &MD_FOLDERS);
            lEntry.Set(&mLibraries     , false); AddEntry("Libraries"     , lEntry, &MD_LIBRARIES);
            lEntry.Set(&mOSIndependent , false); AddEntry("OSIndependent" , lEntry, &MD_OS_INDEPENDENT);
            lEntry.Set(&mPreBuildCmds  , false); AddEntry("PreBuildCmds"  , lEntry, &MD_PRE_BUILD_CMDS);
            lEntry.Set(&mProcessors    , false); AddEntry("Processors"    , lEntry, &MD_PROCESSORS);
            lEntry.Set(&mProduct       , false); AddEntry("Product"       , lEntry, &MD_PRODUCT);
            lEntry.Set(&mTests         , false); AddEntry("Tests"         , lEntry, &MD_TESTS);
            lEntry.Set(&mVersionFile   , false); AddEntry("VersionFile"   , lEntry, &MD_VERSION_FILE);

            lEntry.Set(&mBinaries      , false); AddEntry(NAME_OS "Binaries"      , lEntry, &MD_OS_BINARIES);
            lEntry.Set(&mConfigurations, false); AddEntry(NAME_OS "Configurations", lEntry, &MD_OS_CONFIGURATIONS);
            lEntry.Set(&mDrivers       , false); AddEntry(NAME_OS "Drivers"       , lEntry, &MD_OS_DRIVERS);
            lEntry.Set(&mEditOperations, false); AddEntry(NAME_OS "EditOperations", lEntry, &MD_OS_EDIT_OPERATIONS);
            lEntry.Set(&mFiles         , false); AddEntry(NAME_OS "Files"         , lEntry, &MD_OS_FILES);
            lEntry.Set(&mFolders       , false); AddEntry(NAME_OS "Folders"       , lEntry, &MD_OS_FOLDERS);
            lEntry.Set(&mLibraries     , false); AddEntry(NAME_OS "Libraries"     , lEntry, &MD_OS_LIBRARIES);
            lEntry.Set(&mPreBuildCmds  , false); AddEntry(NAME_OS "PreBuildCmds"  , lEntry, &MD_OS_PRE_BUILD_CMDS);
            lEntry.Set(&mProcessors    , false); AddEntry(NAME_OS "Processors"    , lEntry, &MD_OS_PROCESSORS);
            lEntry.Set(&mTests         , false); AddEntry(NAME_OS "Tests"         , lEntry, &MD_OS_TESTS);

            mTmp_Binaries  = File::Folder(mTmp_Root, "Binaries" );
            mTmp_Drivers   = File::Folder(mTmp_Root, "Drivers"  );
            mTmp_Libraries = File::Folder(mTmp_Root, "Libraries");

            Construct_OSDep();
        }

        int Build::Run()
        {
            mVersion = Version(File::Folder::CURRENT, mVersionFile);

            Edit();

            ExecuteCommands(mPreBuildCmds);

            if (!mDoNotCompile) { Compile(); }
            if (!mDoNotTest   ) { Test   (); }
            if (!mDoNotPackage) { Package(); }
            if (!mDoNotExport ) { Export (); }

            CreateInstaller();

            return 0;
        }

        // ===== DI::Container ==============================================

        void Build::Validate() const
        {
            DI::Dictionary::Validate();

            if ((!mBinaries.IsEmpty()) || (!mDrivers.IsEmpty()) || (!mLibraries.IsEmpty()) || (!mTests.IsEmpty()))
            {
                if (!mDoNotCompile)
                {
                    KMS_EXCEPTION_ASSERT(!mConfigurations.IsEmpty(), RESULT_INVALID_CONFIG, "No configuration", "");
                }

                if (!mDoNotExport)
                {
                    char lMsg[64 + PATH_LENGTH];
                    sprintf_s(lMsg, "\"%s\" is not a valid export folder", mExportFolder.GetFolder().GetPath());
                    KMS_EXCEPTION_ASSERT(mExportFolder.GetFolder().DoesExist(), RESULT_INVALID_CONFIG, lMsg, "");

                    KMS_EXCEPTION_ASSERT(0 < mProduct.GetLength(), RESULT_INVALID_CONFIG, "Invalid product name", "");
                }

                KMS_EXCEPTION_ASSERT(!mOSIndependent, RESULT_INVALID_CONFIG, "OS independent binary, driver, library or test are not supported", "");
            }

            if (!mDrivers.IsEmpty())
            {
                KMS_EXCEPTION_ASSERT(0 == mEmbedded.GetLength(), RESULT_INVALID_CONFIG, "Embedded driver are not supported", "");
            }

            if (!mOSIndependent)
            {
                KMS_EXCEPTION_ASSERT(!mProcessors.IsEmpty(), RESULT_INVALID_CONFIG, "No processor", "");
            }

            KMS_EXCEPTION_ASSERT(0 < mVersionFile.GetLength(), RESULT_INVALID_CONFIG, "Invalid version file", "");
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
                        Compile_Make(*lC);
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

            lC.ParseFile(File::Folder::CURRENT, CONFIG_FILE);
            lC.ParseFile(File::Folder::CURRENT, "KMS-Make" FILE_EXT_CFG);

            lM.mConfiguration.Set(aC);
            lM.mProcessor    .Set(aP);

            if (!mDoNotClean.Get())
            {
                lM.AddCommand("Clean");
            }

            lM.AddCommand("Make");

            lC.Validate();

            auto lRet = lM.Run();
            KMS_EXCEPTION_ASSERT(0 == lRet, RESULT_COMPILATION_FAILED, "KMS::Build::Make::Run failed", lRet);
        }

        void Build::CreateInstaller()
        {
            auto lCT = new Dbg::Stats_Timer("CreateInstallerTime.Processor");

            for (const auto& lEntry : mProcessors.mInternal)
            {
                lCT->Start();

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                CreateInstaller(*lP);

                lCT->Stop();
            }
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
                    KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid edit operation", *lOp);
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
                    KMS_EXCEPTION(RESULT_COMMAND_FAILED, "The command failed", *lCommand);
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

            KMS::Build::Package lPackage(IsEmbedded() || mOSIndependent);

            lPackage.Set(mProduct.Get(), mVersion);

            char lFileName[FILE_LENGTH];

            lPackage.GetFileName(lFileName, sizeof(lFileName));

            mTmp_Root.Compress(mProductFolder, lFileName);
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
            if (!mDrivers  .IsEmpty()) { mTmp_Drivers  .Create(); }
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

        void Build::Package_Components(const char* aC)
        {
            for (const auto& lEntry : mProcessors.mInternal)
            {
                assert(nullptr != lEntry);

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                Package_Components(aC, lP->Get());
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

                        lBin_Src.Copy(lBin, (lB->GetString() + FILE_EXT_ELF).c_str());
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

                        lLib_Src.Copy(lLib, (lL->GetString() + FILE_EXT_A).c_str());
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
                    KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid folder copy operation", *lF);
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

        void Build::Test(const char* aC)
        {
            for (const auto& lEntry : mProcessors.mInternal)
            {
                assert(nullptr != lEntry);

                auto lP = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lP);

                Test(aC, lP->Get());
            }
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
            case '\0': KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid edition operation", aIn);
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

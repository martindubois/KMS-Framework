
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
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
static const KMS::Cfg::MetaData MD_EDIT_OPERATIONS("EditOperations += {Operation}");
static const KMS::Cfg::MetaData MD_EXPORT_FOLDER  ("ExportFolder = {Path}");
static const KMS::Cfg::MetaData MD_FILES          ("Files += {Path}");
static const KMS::Cfg::MetaData MD_FOLDERS        ("Folders += {Path}");
static const KMS::Cfg::MetaData MD_LIBRARIES      ("Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_INDEPENDENT ("OSIntependent = false | true");
static const KMS::Cfg::MetaData MD_PRE_BUILD_CMDS ("PreBuildCmds += {Command}");
static const KMS::Cfg::MetaData MD_PRODUCT        ("Product = {Name}");
static const KMS::Cfg::MetaData MD_TESTS          ("Tests += {Name}");
static const KMS::Cfg::MetaData MD_VERSION_FILE   ("VersionFile = {Path}");

#ifdef _KMS_DARWIN_
    #define NAME_OS "Darwin"

    static const KMS::Cfg::MetaData MD_OS_BINARIES      ("DarwinBinaries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_CONFIGURATIONS("DarwinConfigurations += {Name}");
    static const KMS::Cfg::MetaData MD_OS_FILES         ("DarwinFiles += {Path}");
    static const KMS::Cfg::MetaData MD_OS_FILES         ("DarwinFolders += {Path}");
    static const KMS::Cfg::MetaData MD_OS_LIBRARIES     ("DarwinLibraries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_PRE_BUILD_CMDS("DarwinPreBuildCmds += {Command}");
    static const KMS::Cfg::MetaData MD_OS_TESTS         ("DarwinTests += {Name}");
#endif

#ifdef _KMS_LINUX_
    #define NAME_OS "Linux"

    static const KMS::Cfg::MetaData MD_OS_BINARIES      ("LinuxBinaries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_CONFIGURATIONS("LinuxConfigurations += {Name}");
    static const KMS::Cfg::MetaData MD_OS_FILES         ("LinuxFiles += {Path}");
    static const KMS::Cfg::MetaData MD_OS_FOLDERS       ("LinuxFolders += {Path}");
    static const KMS::Cfg::MetaData MD_OS_LIBRARIES     ("LinuxLibraries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_PRE_BUILD_CMDS("LinuxPreBuildCmds += {Command}");
    static const KMS::Cfg::MetaData MD_OS_TESTS         ("LinuxTests += {Name}");
#endif

#ifdef _KMS_WINDOWS_
    #define NAME_OS "Windows"

    static const KMS::Cfg::MetaData MD_OS_BINARIES      ("WindowsBinaries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_CONFIGURATIONS("WindowsConfigurations += {Name}");
    static const KMS::Cfg::MetaData MD_OS_FILES         ("WindowsFiles += {Path}");
    static const KMS::Cfg::MetaData MD_OS_FOLDERS       ("WindowsFolders += {Path}");
    static const KMS::Cfg::MetaData MD_OS_LIBRARIES     ("WindowsLibraries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_PRE_BUILD_CMDS("WindowsPreBuildCmds += {Command}");
    static const KMS::Cfg::MetaData MD_OS_TESTS         ("WindowsTests += {Name}");

    static const KMS::Cfg::MetaData MD_WINDOWS_FILE_MSI  ("WindowsFile_MSI = {Path}");
    static const KMS::Cfg::MetaData MD_WINDOWS_PROCESSORS("WindowsProcessors += x64 | x86");
#endif

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
                KMS::Build::Build      lB;
                KMS::Cfg::Configurator lC;
                KMS::Installer         lInstaller;

                lC.AddConfigurable(&lB);
                lC.AddConfigurable(&lInstaller);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE, true);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lInstaller.Run();

                lResult = lB.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Build::Build()
            : mDoNotCompile (DEFAULT_DO_NOT_COMPILE)
            , mDoNotExport  (DEFAULT_DO_NOT_EXPORT)
            , mDoNotPackage (DEFAULT_DO_NOT_PACKAGE)
            , mOSIndependent(DEFAULT_OS_INDEPENDENT)
            , mVersionFile  (DEFAULT_VERSION_FILE)
            , mTempFolder(File::Folder::Id::TEMPORARY)
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
            mTests         .SetCreator(DI::String::Create);

            AddEntry("Binaries"      , &mBinaries      , false, &MD_BINARIES);
            AddEntry("Configurations", &mConfigurations, false, &MD_CONFIGURATIONS);
            AddEntry("DoNotCompile"  , &mDoNotCompile  , false, &MD_DO_NOT_COMPILE);
            AddEntry("DoNotExport"   , &mDoNotExport   , false, &MD_DO_NOT_EXPORT);
            AddEntry("DoNotPackage"  , &mDoNotPackage  , false, &MD_DO_NOT_PACKAGE);
            AddEntry("EditOperations", &mEditOperations, false, &MD_EDIT_OPERATIONS);
            AddEntry("Files"         , &mFiles         , false, &MD_FILES);
            AddEntry("Folders"       , &mFolders       , false, &MD_FOLDERS);
            AddEntry("Libraries"     , &mLibraries     , false, &MD_LIBRARIES);
            AddEntry("OSIndependent" , &mOSIndependent , false, &MD_OS_INDEPENDENT);
            AddEntry("PreBuildCmds"  , &mPreBuildCmds  , false, &MD_PRE_BUILD_CMDS);
            AddEntry("Product"       , &mProduct       , false, &MD_PRODUCT);
            AddEntry("Tests"         , &mTests         , false, &MD_TESTS);
            AddEntry("VersionFile"   , &mVersionFile   , false, &MD_VERSION_FILE);
            AddEntry("ExportFolder"  , &mExportFolder  , false, &MD_EXPORT_FOLDER);

            AddEntry(NAME_OS "Binaries"      , &mBinaries      , false, &MD_OS_BINARIES);
            AddEntry(NAME_OS "Configurations", &mConfigurations, false, &MD_OS_CONFIGURATIONS);
            AddEntry(NAME_OS "Files"         , &mFiles         , false, &MD_OS_FILES);
            AddEntry(NAME_OS "Folders"       , &mFolders       , false, &MD_OS_FOLDERS);
            AddEntry(NAME_OS "Libraries"     , &mLibraries     , false, &MD_OS_LIBRARIES);
            AddEntry(NAME_OS "PreBuildCmds"  , &mPreBuildCmds  , false, &MD_OS_PRE_BUILD_CMDS);
            AddEntry(NAME_OS "Tests"         , &mTests         , false, &MD_OS_TESTS);

            #ifdef _KMS_WINDOWS_
                mWindowsProcessors.SetCreator(DI::String::Create);

                AddEntry("WindowsFile_MSI"  , &mWindowsFile_MSI  , false, &MD_WINDOWS_FILE_MSI);
                AddEntry("WindowsProcessors", &mWindowsProcessors, false, &MD_WINDOWS_PROCESSORS);
            #endif

        }

        Build::~Build() {}

        void Build::AddBinary       (const char* aB) { assert(NULL != aB); mBinaries      .AddEntry(new DI::String(aB), true); }
        void Build::AddConfiguration(const char* aC) { assert(NULL != aC); mConfigurations.AddEntry(new DI::String(aC), true); }
        void Build::AddEditOperation(const char* aE) { assert(NULL != aE); mEditOperations.AddEntry(new DI::String(aE), true); }
        void Build::AddFile         (const char* aF) { assert(NULL != aF); mFiles         .AddEntry(new DI::String(aF), true); }
        void Build::AddFolder       (const char* aF) { assert(NULL != aF); mFolders       .AddEntry(new DI::String(aF), true); }
        void Build::AddLibrary      (const char* aL) { assert(NULL != aL); mLibraries     .AddEntry(new DI::String(aL), true); }
        void Build::AddPreBuildCmd  (const char* aC) { assert(NULL != aC); mPreBuildCmds  .AddEntry(new DI::String(aC), true); }
        void Build::AddTest         (const char* aT) { assert(NULL != aT); mTests         .AddEntry(new DI::String(aT), true); }

        void Build::SetDoNotCompile (bool aDNC) { mDoNotCompile  = aDNC; }
        void Build::SetDoNotExport  (bool aDNE) { mDoNotExport   = aDNE; }
        void Build::SetDoNotPackage (bool aDNP) { mDoNotPackage  = aDNP; }
        void Build::SetOSIndependent(bool aOSI) { mOSIndependent = aOSI; }

        void Build::SetProduct    (const char* aP ) { assert(NULL != aP ); mProduct      = aP ; }
        void Build::SetVersionFile(const char* aVF) { assert(NULL != aVF); mVersionFile  = aVF; }

        int Build::Run()
        {
            VerifyConfig();

            mVersion = Version(File::Folder::CURRENT, mVersionFile);

            Edit();

            ExecuteCommands(mPreBuildCmds);

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
                Export();
            }

            return 0;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile()
        {
            for (const DI::Container::Entry& lEntry : mConfigurations.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lC);

                Compile(*lC);
            }
        }

        void Build::Edit()
        {
            for (const DI::Container::Entry& lEntry : mEditOperations.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lOp = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lOp);

                char lFile   [FILE_LENGTH];
                char lRegEx  [LINE_LENGTH];
                char lReplace[LINE_LENGTH];

                if (3 != sscanf_s(*lOp, "%[^;];%[^;];%[^\n\r]", lFile SizeInfo(lFile), lRegEx SizeInfo(lRegEx), lReplace SizeInfo(lReplace)))
                {
                    KMS_EXCEPTION(BUILD_CONFIG_INVALID, "Invalid edit operation", *lOp);
                }

                std::string lReplaceStr = ProcessReplaceLine(lReplace, mVersion);

                Text::File_ASCII lText;

                lText.Read(File::Folder::CURRENT, lFile);

                lText.ReplaceLines(lRegEx, lReplaceStr.c_str());

                File::Folder::CURRENT.Backup(lFile);

                lText.Write(File::Folder::CURRENT, lFile);
            }
        }

        void Build::ExecuteCommands(const DI::Array& aCommands)
        {
            for (const DI::Container::Entry& lEntry : aCommands.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lCommand = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lCommand);

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

            unsigned int lFlags = mOSIndependent ? Version::FLAG_OS_INDEPENDENT : 0;

            char lPackage[FILE_LENGTH];

            mVersion.GetPackageName(mProduct, lPackage, sizeof(lPackage), lFlags);

            mTempFolder.Compress(mProductFolder, lPackage);

            #ifdef _KMS_WINDOWS_
                Export_WindowsFile_MSI();
            #endif
        }

        void Build::Package()
        {
            Package_Component();
            Package_Files();
            Package_Folders();
        }

        void Build::Package_Component()
        {
            File::Folder lBin(mTempFolder, "Binaries" );
            File::Folder lLib(mTempFolder, "Libraries");

            if (!mBinaries .IsEmpty()) { lBin.Create(); }
            if (!mLibraries.IsEmpty()) { lLib.Create(); }

            for (const DI::Container::Entry& lEntry : mConfigurations.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lC);

                Package_Component(*lC);
            }
        }

        void Build::Package_Files()
        {
            for (const DI::Container::Entry& lEntry : mFiles.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lF = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lF);

                const char* lPtr = strrchr(*lF, '/');
                if (NULL == lPtr)
                {
                    lPtr = *lF;
                }
                else
                {
                    lPtr++;
                }

                File::Folder::CURRENT.Copy(mTempFolder, *lF, lPtr);
            }
        }

        void Build::Package_Folders()
        {
            for (const DI::Container::Entry& lEntry : mFolders.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lF = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lF);

                char lDst[NAME_LENGTH];
                char lSrc[NAME_LENGTH];

                if (2 != sscanf_s(lF->Get(), "%[^;];%[^\n\r]", lSrc SizeInfo(lSrc), lDst SizeInfo(lDst)))
                {
                    KMS_EXCEPTION(BUILD_CONFIG_INVALID, "Invalid folder copy operation", *lF);
                }

                File::Folder lFD(mTempFolder, lDst);
                File::Folder lFS(lSrc);

                lFS.Copy(lFD);
            }
        }

        void Build::Test()
        {
            for (const DI::Container::Entry& lEntry : mConfigurations.mInternal)
            {
                assert(NULL != lEntry);

                const DI::String* lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lC);

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

                    #ifdef _KMS_WINDOWS_
                        KMS_EXCEPTION_ASSERT(!mWindowsProcessors.IsEmpty(), BUILD_CONFIG_INVALID, "No processor", "");
                    #endif
                }

                if (!mDoNotExport)
                {
                    KMS_EXCEPTION_ASSERT(mExportFolder.Get().DoesExist(), BUILD_CONFIG_INVALID, "Invalid export folder", *mExportFolder);

                    KMS_EXCEPTION_ASSERT(0 < mProduct.GetLength(), BUILD_CONFIG_INVALID, "Invalid product name", "");
                }
            }

            KMS_EXCEPTION_ASSERT(0 < mVersionFile.GetLength(), BUILD_CONFIG_INVALID, "Invalid version file", "");
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
            case '\0': KMS_EXCEPTION(BUILD_CONFIG_INVALID, "Invalid edition operation", aIn);
            case '}': lState = STATE_INIT; break;
            case 'M': lResult += aVersion.GetMajor(); break;
            case 'm': lResult += aVersion.GetMinor(); break;
            case 'B': lResult += aVersion.GetBuild(); break;
            case 'C': lResult += aVersion.GetCompat(); break;

            case 'T':
                if (NULL != lType)
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

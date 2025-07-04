
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Build.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Main.h>

// ===== Local ==============================================================
#include "Comp_Archive.h"
#include "Comp_Binary.h"
#include "Comp_Driver.h"
#include "Comp_File_ToPackage.h"
#include "Comp_Folder_ToPackage.h"
#include "Comp_Library_Dynamic.h"
#include "Comp_Library_Static.h"
#include "Comp_Test.h"
#include "Phase.h"
#include "Config.h"
#include "Tool_Command.h"
#include "Tool_Doxygen.h"
#include "Tool_Editor.h"
#include "Tool_Export.h"
#include "Tool_Make.h"

#include "Build.h"

using namespace KMS;

#define FILE_EXT_CFG ".cfg"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Build" FILE_EXT_CFG)

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_BINARIES         ("Binaries += {Name}");
static const KMS::Cfg::MetaData MD_CONFIGURATIONS   ("Configurations += {Name}");
static const KMS::Cfg::MetaData MD_DYNAMIC_LIBRARIES("DynamicLibraries += {Name}");
static const KMS::Cfg::MetaData MD_DO_NOT_CLEAN     ("DoNotRebuild = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_COMPILE   ("DoNotCompile = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_EXPORT    ("DoNotExport = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_PACKAGE   ("DoNotPackage = false | true");
static const KMS::Cfg::MetaData MD_DO_NOT_TEST      ("DoNotTest = false | true");
static const KMS::Cfg::MetaData MD_DRIVERS          ("Drivers += {Name}");
static const KMS::Cfg::MetaData MD_EDIT_OPERATIONS  ("EditOperations += {Operation}");
static const KMS::Cfg::MetaData MD_EMBEDDED         ("Embedded = {}");
static const KMS::Cfg::MetaData MD_EXPORT_FOLDER    ("ExportFolder = {Path}");
static const KMS::Cfg::MetaData MD_FILES            ("Files += {Path}");
static const KMS::Cfg::MetaData MD_FOLDERS          ("Folders += {Path}");
static const KMS::Cfg::MetaData MD_LIBRARIES        ("Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_INDEPENDENT   ("OSIntependent = false | true");
static const KMS::Cfg::MetaData MD_PRE_BUILD_CMDS   ("PreBuildCmds += {Command}");
static const KMS::Cfg::MetaData MD_PROCESSORS       ("Processors += x64 | x86 | ...");
static const KMS::Cfg::MetaData MD_PRODUCT          ("Product = {Name}");
static const KMS::Cfg::MetaData MD_TESTS            ("Tests += {Name}");
static const KMS::Cfg::MetaData MD_VERSION_FILE     ("VersionFile = {Path}");

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

static const KMS::Cfg::MetaData MD_OS_BINARIES         (NAME_OS "Binaries += {Name}");
static const KMS::Cfg::MetaData MD_OS_CONFIGURATIONS   (NAME_OS "Configurations += {Name}");
static const KMS::Cfg::MetaData MD_OS_DYNAMIC_LIBRARIES(NAME_OS "Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_DRIVERS          (NAME_OS "Drivers += {Name}");
static const KMS::Cfg::MetaData MD_OS_EDIT_OPERATIONS  (NAME_OS "EditOperations += {Operation}");
static const KMS::Cfg::MetaData MD_OS_FILES            (NAME_OS "Files += {Path}");
static const KMS::Cfg::MetaData MD_OS_FOLDERS          (NAME_OS "Folders += {Path}");
static const KMS::Cfg::MetaData MD_OS_LIBRARIES        (NAME_OS "Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_PRE_BUILD_CMDS   (NAME_OS "PreBuildCmds += {Command}");
static const KMS::Cfg::MetaData MD_OS_PROCESSORS       (NAME_OS "Processors += x64 | x86 | ...");
static const KMS::Cfg::MetaData MD_OS_TESTS            (NAME_OS "Tests += {Name}");

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

// Public
// //////////////////////////////////////////////////////////////////////////

const bool  ::Build::DO_NOT_CLEAN_DEFAULT   = false;
const bool  ::Build::DO_NOT_COMPILE_DEFAULT = false;
const bool  ::Build::DO_NOT_EXPORT_DEFAULT  = false;
const bool  ::Build::DO_NOT_PACKAGE_DEFAULT = false;
const bool  ::Build::DO_NOT_TEST_DEFAULT    = false;
const char* ::Build::EMBEDDED_DEFAULT       = "";
const bool  ::Build::OS_INDEPENDENT_DEFAULT = false;
const char* ::Build::PRODUCT_DEFAULT        = "";
const char* ::Build::VERSION_FILE_DEFAULT   = "Common" SLASH "Version.h";

int ::Build::Main(int aCount, const char ** aVector)
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

::Build::Build()
    : mDoNotClean   (DO_NOT_CLEAN_DEFAULT)
    , mDoNotCompile (DO_NOT_COMPILE_DEFAULT)
    , mDoNotExport  (DO_NOT_EXPORT_DEFAULT)
    , mDoNotPackage (DO_NOT_PACKAGE_DEFAULT)
    , mDoNotTest    (DO_NOT_TEST_DEFAULT)
    , mEmbedded     (EMBEDDED_DEFAULT)
    , mOSIndependent(OS_INDEPENDENT_DEFAULT)
    , mProduct      (PRODUCT_DEFAULT)
    , mVersionFile  (VERSION_FILE_DEFAULT)
{
    mBinaries        .SetCreator(DI::String::Create);
    mConfigurations  .SetCreator(DI::String::Create);
    mDynamicLibraries.SetCreator(DI::String::Create);
    mDrivers         .SetCreator(DI::String::Create);
    mEditOperations  .SetCreator(DI::String::Create);
    mFiles           .SetCreator(DI::String::Create);
    mFolders         .SetCreator(DI::String::Create);
    mLibraries       .SetCreator(DI::String::Create);
    mPreBuildCmds    .SetCreator(DI::String::Create);
    mProcessors      .SetCreator(DI::String::Create);
    mTests           .SetCreator(DI::String::Create);

    Ptr_OF<DI::Object> lEntry;

    lEntry.Set(&mBinaries        , false); AddEntry("Binaries"        , lEntry, &MD_BINARIES);
    lEntry.Set(&mConfigurations  , false); AddEntry("Configurations"  , lEntry, &MD_CONFIGURATIONS);
    lEntry.Set(&mDynamicLibraries, false); AddEntry("DynamicLibraries", lEntry, &MD_DYNAMIC_LIBRARIES);
    lEntry.Set(&mDrivers         , false); AddEntry("Drivers"         , lEntry, &MD_DRIVERS);
    lEntry.Set(&mDoNotClean      , false); AddEntry("DoNotClean"      , lEntry, &MD_DO_NOT_CLEAN);
    lEntry.Set(&mDoNotCompile    , false); AddEntry("DoNotCompile"    , lEntry, &MD_DO_NOT_COMPILE);
    lEntry.Set(&mDoNotExport     , false); AddEntry("DoNotExport"     , lEntry, &MD_DO_NOT_EXPORT);
    lEntry.Set(&mDoNotPackage    , false); AddEntry("DoNotPackage"    , lEntry, &MD_DO_NOT_PACKAGE);
    lEntry.Set(&mDoNotTest       , false); AddEntry("DoNotTest"       , lEntry, &MD_DO_NOT_TEST);
    lEntry.Set(&mEditOperations  , false); AddEntry("EditOperations"  , lEntry, &MD_EDIT_OPERATIONS);
    lEntry.Set(&mEmbedded        , false); AddEntry("Embedded"        , lEntry, &MD_EMBEDDED);
    lEntry.Set(&mExportFolder    , false); AddEntry("ExportFolder"    , lEntry, &MD_EXPORT_FOLDER);
    lEntry.Set(&mFiles           , false); AddEntry("Files"           , lEntry, &MD_FILES);
    lEntry.Set(&mFolders         , false); AddEntry("Folders"         , lEntry, &MD_FOLDERS);
    lEntry.Set(&mLibraries       , false); AddEntry("Libraries"       , lEntry, &MD_LIBRARIES);
    lEntry.Set(&mOSIndependent   , false); AddEntry("OSIndependent"   , lEntry, &MD_OS_INDEPENDENT);
    lEntry.Set(&mPreBuildCmds    , false); AddEntry("PreBuildCmds"    , lEntry, &MD_PRE_BUILD_CMDS);
    lEntry.Set(&mProcessors      , false); AddEntry("Processors"      , lEntry, &MD_PROCESSORS);
    lEntry.Set(&mProduct         , false); AddEntry("Product"         , lEntry, &MD_PRODUCT);
    lEntry.Set(&mTests           , false); AddEntry("Tests"           , lEntry, &MD_TESTS);
    lEntry.Set(&mVersionFile     , false); AddEntry("VersionFile"     , lEntry, &MD_VERSION_FILE);

    lEntry.Set(&mBinaries        , false); AddEntry(NAME_OS "Binaries"        , lEntry, &MD_OS_BINARIES);
    lEntry.Set(&mConfigurations  , false); AddEntry(NAME_OS "Configurations"  , lEntry, &MD_OS_CONFIGURATIONS);
    lEntry.Set(&mDynamicLibraries, false); AddEntry(NAME_OS "DynamicLibraries", lEntry, &MD_OS_DYNAMIC_LIBRARIES);
    lEntry.Set(&mDrivers         , false); AddEntry(NAME_OS "Drivers"         , lEntry, &MD_OS_DRIVERS);
    lEntry.Set(&mEditOperations  , false); AddEntry(NAME_OS "EditOperations"  , lEntry, &MD_OS_EDIT_OPERATIONS);
    lEntry.Set(&mFiles           , false); AddEntry(NAME_OS "Files"           , lEntry, &MD_OS_FILES);
    lEntry.Set(&mFolders         , false); AddEntry(NAME_OS "Folders"         , lEntry, &MD_OS_FOLDERS);
    lEntry.Set(&mLibraries       , false); AddEntry(NAME_OS "Libraries"       , lEntry, &MD_OS_LIBRARIES);
    lEntry.Set(&mPreBuildCmds    , false); AddEntry(NAME_OS "PreBuildCmds"    , lEntry, &MD_OS_PRE_BUILD_CMDS);
    lEntry.Set(&mProcessors      , false); AddEntry(NAME_OS "Processors"      , lEntry, &MD_OS_PROCESSORS);
    lEntry.Set(&mTests           , false); AddEntry(NAME_OS "Tests"           , lEntry, &MD_OS_TESTS);

    Construct_OSDep();
}

int ::Build::Run()
{
    mVersion = Version(File::Folder::CURRENT, mVersionFile);

    CreateLists();

    for (auto lPhase = Phase::VERIFY; lPhase != Phase::NONE; ++lPhase)
    {
        mComps.Verify (lPhase);
        mTools.Execute(lPhase);
    }

    return 0;
}

// ===== DI::Container ==============================================

void ::Build::Validate() const
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
        KMS_EXCEPTION_ASSERT(!IsEmbedded(), RESULT_INVALID_CONFIG, "Embedded driver are not supported", "");
    }

    if (!mOSIndependent)
    {
        KMS_EXCEPTION_ASSERT(!mProcessors.IsEmpty(), RESULT_INVALID_CONFIG, "No processor", "");
    }

    KMS_EXCEPTION_ASSERT(0 < mVersionFile.GetLength(), RESULT_INVALID_CONFIG, "Invalid version file", "");
}

// Private
// //////////////////////////////////////////////////////////////////////////

bool ::Build::IsEmbedded() const { return 0 < mEmbedded.GetLength(); }

void ::Build::CreateLists()
{
    #ifdef _KMS_LINUX_
        Config lCfg(mDoNotClean, mDoNotCompile, mDoNotExport, mDoNotPackage, mDoNotTest, IsEmbedded(), mExportFolder, mOSIndependent, mProduct, mVersion);
    #endif

    #ifdef _KMS_WINDOWS_
        Config lCfg(mCertificatSHA1, mDoNotClean, mDoNotCompile, mDoNotExport, mDoNotPackage, mDoNotTest, IsEmbedded(), mExportFolder, mOSIndependent, mProduct, mVersion, mVisualStudioVersion);
    #endif
    
    Comp_Archive         ::CreateComponentAndTool(&mComps, &mTools, lCfg);
    Comp_Binary          ::CreateComponents(&mComps, lCfg, mBinaries, mConfigurations, mProcessors);
    Comp_Driver          ::CreateComponentsAndTools(&mComps, &mTools, lCfg, mDrivers, mConfigurations, mProcessors);
    Comp_File_ToPackage  ::CreateComponents(&mComps, lCfg, mFiles);
    Comp_Folder_ToPackage::CreateComponents(&mComps, lCfg, mFolders);
    Comp_Library_Dynamic ::CreateComponents(&mComps, lCfg, mDynamicLibraries, mConfigurations, mProcessors);
    Comp_Library_Static  ::CreateComponents(&mComps, lCfg, mLibraries, mConfigurations, mProcessors);
    Comp_Test            ::CreateTools(&mTools, lCfg, mTests, mConfigurations, mProcessors);

    Tool_Command::CreateTools(&mTools, mPreBuildCmds, Phase::PRE_BUILD);
    Tool_Doxygen::CreateTools(&mTools, lCfg);
    Tool_Editor ::CreateTools(&mTools, lCfg, mEditOperations);
    Tool_Export ::CreateTool (&mTools, lCfg, &mComps);

    if (IsEmbedded())
    {
        Tool_Make::CreateTools(&mTools, lCfg, mConfigurations, mProcessors);
    }

    CreateLists_OSDep();
}

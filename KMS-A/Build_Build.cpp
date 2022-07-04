
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
#include <KMS/Convert.h>
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
#define DEFAULT_OS_INDEPENDENT (false)
#define DEFAULT_VERSION_FILE   ("Common" SLASH "Version.h")

#define MSBUILD_FOLDER ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")

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

                lB.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE, true);
                lC.ParseArguments(aCount - 1, aVector + 1);

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
            , mTempFolder(File::Folder::Id::TEMPORARY)
            , mVersionFile(DEFAULT_VERSION_FILE)
            #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
                , mExportFolder(File::Folder(File::Folder::Id::HOME), "Export")
            #endif
            #ifdef _KMS_WINDOWS_
                , mExportFolder("K:\\Export")
            #endif
        {
        }

        Build::~Build() {}

        void Build::AddBinary       (const char* aB) { assert(NULL != aB); mBinaries      .insert(aB); }
        void Build::AddConfiguration(const char* aC) { assert(NULL != aC); mConfigurations.insert(aC); }
        void Build::AddEditOperation(const char* aC) { assert(NULL != aC); mEditOperations.insert(aC); }
        void Build::AddFile         (const char* aF) { assert(NULL != aF); mFiles         .insert(aF); }
        void Build::AddFolder       (const char* aF) { assert(NULL != aF); mFolders       .insert(aF); }
        void Build::AddLibrary      (const char* aL) { assert(NULL != aL); mLibraries     .insert(aL); }
        void Build::AddPreBuildCmd  (const char* aC) { assert(NULL != aC); mPreBuildCmds  .insert(aC); }
        void Build::AddTest         (const char* aT) { assert(NULL != aT); mTests         .insert(aT); }

        void Build::SetDoNotCompile (bool aDNC) { mDoNotCompile  = aDNC; }
        void Build::SetDoNotExport  (bool aDNE) { mDoNotExport   = aDNE; }
        void Build::SetDoNotPackage (bool aDNP) { mDoNotPackage  = aDNP; }
        void Build::SetOSIndependent(bool aOSI) { mOSIndependent = aOSI; }

        void Build::SetProduct    (const char* aP ) { assert(NULL != aP ); mProduct      = aP ; }
        void Build::SetVersionFile(const char* aVF) { assert(NULL != aVF); mVersionFile  = aVF; }

        int Build::Run()
        {
            VerifyConfig();

            Version lVersion(File::Folder(File::Folder::Id::CURRENT), mVersionFile.c_str());

            Edit(lVersion);

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
                Export(lVersion);
            }

            return 0;
        }

        // ===== Cfg::Configurable ==========================================

        bool Build::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            CFG_CALL("Binaries"      , AddBinary       );
            CFG_CALL("Configurations", AddConfiguration);
            CFG_CALL("EditOperations", AddEditOperation);
            CFG_CALL("Files"         , AddFile         );
            CFG_CALL("Folders"       , AddFolder       );
            CFG_CALL("Libraries"     , AddLibrary      );
            CFG_CALL("PreBuildCmds"  , AddPreBuildCmd  );
            CFG_CALL("Tests"         , AddTest         );

            #ifdef _KMS_DARWIN_
                CFG_CALL("DarwinBinaries" , AddBinary);
                CFG_CALL("DarwinFiles"    , AddFile);
                CFG_CALL("DarwinLibraries", AddLibrary);
                CFG_CALL("DarwinTests"    , AddTest);
            #endif

            #ifdef _KMS_LINUX_
                CFG_CALL("LinuxBinaries" , AddBinary);
                CFG_CALL("LinuxFiles"    , AddFile);
                CFG_CALL("LinuxLibraries", AddLibrary);
                CFG_CALL("LinuxTests"    , AddTest);
            #endif

            #ifdef _KMS_WINDOWS_
                CFG_CALL("WindowsBinaries"  , AddBinary);
                CFG_CALL("WindowsFiles"     , AddFile);
                CFG_CALL("WindowsLibraries" , AddLibrary);
                CFG_CALL("WindowsProcessors", AddProcessor);
                CFG_CALL("WindowsTests"     , AddTest);
            #endif

            return Configurable::AddAttribute(aA, aV);
        }

        bool Build::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("DoNotCompile" ) { SetDoNotCompile (); return true; }
                CFG_IF("DoNotExport"  ) { SetDoNotExport  (); return true; }
                CFG_IF("DoNotPackage" ) { SetDoNotPackage (); return true; }
                CFG_IF("OSIndependent") { SetOSIndependent(); return true; }
                CFG_IF("VersionFile"  ) { SetVersionFile(DEFAULT_VERSION_FILE); return true; }
            }
            else
            {
                CFG_CALL("Product"    , SetProduct    );
                CFG_CALL("VersionFile", SetVersionFile);

                CFG_CONVERT("DoNotCompile" , SetDoNotCompile , Convert::ToBool);
                CFG_CONVERT("DoNotExport"  , SetDoNotExport  , Convert::ToBool);
                CFG_CONVERT("DoNotPackage" , SetDoNotPackage , Convert::ToBool);
                CFG_CONVERT("OSIndependent", SetOSIndependent, Convert::ToBool);
            }

            return Configurable::SetAttribute(aA, aV);
        }

        void Build::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "===== KMS::Build::Build =====\n"
                "Binaries += {Name}\n"
                "    Add a builded binary\n"
                "Configurations += {Name}\n"
                "    Add a build configuration\n"
                "DoNotCompile\n"
                "    Set the do not compile flag\n"
                "DoNotCompile = {Boolean}\n"
                "    Set or clear the do not compile flag\n"
                "DoNotExport\n"
                "    Set the do not export flag\n"
                "DoNotExport = {Boolean}\n"
                "    Set or clear the do not compile flag\n"
                "DoNotPackage\n"
                "    Set the do not package flag\n"
                "DoNotPackage = {Boolean}\n"
                "    Set or clear the do not package flag\n"
                "EditOperations += {Path};{RegEx};{Line}\n"
                "    Add an edit operations to change the version in a text file\n"
                "Files += {File}\n"
                "    Add a file to package\n"
                "Folders += {Source};{Destination}\n"
                "    Add a folder to package\n"
                "Libraries += {Name}\n"
                "    Add a builded library\n"
                "OSIndependend\n"
                "    Set the OS independent flag\n"
                "OSIndependent = {Boolean}\n"
                "    Set or clear the OS independent flag\n"
                "PreBuildCmds += {Command Arg0 Arg1 ...}\n"
                "    Add a pre-build command\n"
                "Product = {Name}\n"
                "    Set the product name\n"
                "    Mandatory\n"
                "Tests += {Name}\n"
                "    Add a builded and executed test program\n"
                "VersionFile\n"
                "    Default: %s\n"
                "VersionFile = {Path}\n"
                "    Set the version file.\n",
                DEFAULT_VERSION_FILE);

            #ifdef _KMS_DARWIN_
                fprintf(aOut,
                    "DarwinBinaries += {Name}\n"
                    "    See Bionaries\n"
                    "DarwinFiles += {Name}\n"
                    "    See Files\n"
                    "DarwinLibraries += {Name}\n"
                    "    See Libraries\n"
                    "DarwinTests += {Name}\n"
                    "    See Tests\n");
            #endif

            #ifdef _KMS_LINUX_
                fprintf(aOut,
                    "LinuxBinaries += {Name}\n"
                    "    See Bionaries\n"
                    "LinuxFiles += {Name}\n"
                    "    See Files\n"
                    "LinuxLibraries += {Name}\n"
                    "    See Libraries\n"
                    "LinuxTests += {Name}\n"
                    "    See Tests\n");
            #endif

            #ifdef _KMS_WINDOWS_
                fprintf(aOut,
                    "WindowsBinaries += {Name}\n"
                    "    See Bionaries\n"
                    "WindowsFiles += {Name}\n"
                    "    See Files\n"
                    "WindowsLibraries += {Name}\n"
                    "    See Libraries\n"
                    "WindowsTests += {Name}\n"
                    "    See Tests\n");
            #endif

            Configurable::DisplayHelp(aOut);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile()
        {
            for (std::string lC : mConfigurations)
            {
                Compile(lC.c_str());
            }
        }

        void Build::Edit(const Version& aVersion)
        {
            File::Folder lCurrent(File::Folder::Id::CURRENT);

            for (std::string lOp : mEditOperations)
            {
                char lFile   [FILE_LENGTH];
                char lRegEx  [LINE_LENGTH];
                char lReplace[LINE_LENGTH];

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

        void Build::ExecuteCommands(const StringSet& aCommands)
        {
            for (const std::string& lCommand : aCommands)
            {
                if (0 != system(lCommand.c_str()))
                {
                    KMS_EXCEPTION_WITH_INFO(BUILD_COMMAND, "The command failed", lCommand.c_str());
                }
            }
        }

        void Build::Export(const Version& aVersion)
        {
            File::Folder lProduct(mExportFolder, mProduct.c_str());

            if (!lProduct.DoesExist())
            {
                lProduct.Create();
            }

            unsigned int lFlags = mOSIndependent ? Version::FLAG_OS_INDEPENDENT : 0;

            char lPackage[FILE_LENGTH];

            aVersion.GetPackageName(mProduct.c_str(), lPackage, sizeof(lPackage), lFlags);

            mTempFolder.Compress(lProduct, lPackage);
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

            if (0 < mBinaries .size()) { lBin.Create(); }
            if (0 < mLibraries.size()) { lLib.Create(); }

            for (std::string lC : mConfigurations)
            {
                Package_Component(lC.c_str());
            }
        }

        void Build::Package_Files()
        {
            File::Folder lCurrent(File::Folder::Id::CURRENT);

            for (const std::string& lF : mFiles)
            {
                const char* lPtr = strrchr(lF.c_str(), '/');
                if (NULL == lPtr)
                {
                    lPtr = lF.c_str();
                }
                else
                {
                    lPtr++;
                }

                lCurrent.Copy(mTempFolder, lF.c_str(), lPtr);
            }
        }

        void Build::Package_Folders()
        {
            for (std::string lF : mFolders)
            {
                char lDst[NAME_LENGTH];
                char lSrc[NAME_LENGTH];

                if (2 != sscanf_s(lF.c_str(), "%[^;];%[^\n\r]", lSrc SizeInfo(lSrc), lDst SizeInfo(lDst)))
                {
                    KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid folder operation", lF.c_str());
                }

                File::Folder lFD(mTempFolder, lDst);
                File::Folder lFS(lSrc);

                lFS.Copy(lFD);
            }
        }

        void Build::Test()
        {
            for (std::string lC : mConfigurations)
            {
                Test(lC.c_str());
            }
        }

        void Build::VerifyConfig()
        {
            if ((0 < mBinaries.size()) || (0 < mLibraries.size()))
            {
                if (!mDoNotCompile)
                {
                    if (0 >= mConfigurations.size())
                    {
                        KMS_EXCEPTION(CONFIG, "No configuration");
                    }

                    #ifdef _KMS_WINDOWS_
                        if (0 >= mProcessors.size())
                        {
                            KMS_EXCEPTION(CONFIG, "No processor");
                        }
                    #endif
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

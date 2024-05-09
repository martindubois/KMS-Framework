
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Make.cpp

#include "Component.h"

#ifdef _KMS_LINUX
    // ===== C ==============================================================
    #include <unistd.h>
#endif

// ===== Includes ===========================================================
#include <KMS/Build/Depend.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Main.h>
#include <KMS/Proc/Process.h>

#include <KMS/Build/Make.h>

KMS_RESULT_STATIC(RESULT_COMPILATION_FAILED);

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_CLEAN_EXCENTION "*.o"

#define DEFAULT_INCLUDE "Includes"

#define MAKE_ALLOWED_TIME_ms (1000 * 60 * 5) // 5 minutes

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MAKE_FILE_NAME "makefile"

static const KMS::Cfg::MetaData MD_CLEAN_EXTENSIONS("CleanExtensions += {Name}");
static const KMS::Cfg::MetaData MD_COMPONENT       ("Component = {Name}");
static const KMS::Cfg::MetaData MD_COMPONENT_TYPE  ("ComponentType = BINARY | LIBRARY | NONE | TEST");
static const KMS::Cfg::MetaData MD_CONFIGURATION   ("Configuration = {Name}");
static const KMS::Cfg::MetaData MD_INCLUDES        ("Includes += {Name}");
static const KMS::Cfg::MetaData MD_MAKE            ("Make = {Path}");
static const KMS::Cfg::MetaData MD_PROCESSOR       ("Processor = {Name}");

// ----- Build --------------------------------------------------------------

static const KMS::Cfg::MetaData MD_BINARIES  ("Binaries += {Name}");
static const KMS::Cfg::MetaData MD_LIBRARIES ("Libraries += {Name}");
static const KMS::Cfg::MetaData MD_TESTS     ("Tests += {Name}");

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

static const KMS::Cfg::MetaData MD_OS_BINARIES (NAME_OS "Binaries += {Name}");
static const KMS::Cfg::MetaData MD_OS_LIBRARIES(NAME_OS "Libraries += {Name}");
static const KMS::Cfg::MetaData MD_OS_TESTS    (NAME_OS "Tests += {Name}");

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static bool DoesContain(const KMS::DI::Array& aArray, const char* aStr);

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Make::COMPONENT_DEFAULT      = "";
        const char* Make::COMPONENT_TYPE_DEFAULT = "NONE";
        const char* Make::CONFIGURATION_DEFAULT  = "Debug";
        const char* Make::MAKE_DEFAULT           = "make";
        const char* Make::PROCESSOR_DEFAULT      = "";

        const char* Make::SILENCE[]
        {
            NAME_OS "Drivers",
            NAME_OS "EditOperations",
            NAME_OS "Folders",
            NAME_OS "Packages",
            NAME_OS "Processors",

            NO_OS_0 "Binaries"      , NO_OS_1 "Binaries"      ,
            NO_OS_0 "Configurations", NO_OS_1 "Configurations",
            NO_OS_0 "Drivers"       , NO_OS_1 "Drivers"       ,
            NO_OS_0 "EditOperations", NO_OS_1 "EditOperations",
            NO_OS_0 "Files"         , NO_OS_1 "Files"         ,
            NO_OS_0 "Folders"       , NO_OS_1 "Folders"       ,
            NO_OS_0 "Libraries"     , NO_OS_1 "Libraries"     ,
            NO_OS_0 "Packages"      , NO_OS_1 "Packages"      ,
            NO_OS_0 "Processors"    , NO_OS_1 "Processors"    ,
            NO_OS_0 "Tests"         , NO_OS_1 "Tests"         ,

            "Configurations"  ,
            "EditOperations"  ,
            "Drivers"         ,
            "Embedded"        ,
            "Files"           ,
            "Folders"         ,
            "Product"         ,
            "Stats_Console"   ,
            "VersionFile"     ,

            nullptr
        };

        const char* Make::COMPONENT_TYPE_NAMES[] = { "BINARY", "DRIVER", "LIBRARY", "NONE", "TEST" };

        int Make::Main(int aCount, const char ** aVector)
        {
            KMS_MAIN_BEGIN;
            {
                Build::Make lM;

                lConfigurator.SetSilence(SILENCE);

                lConfigurator.AddConfigurable(&lM);

                lConfigurator.ParseFile(File::Folder::CURRENT, "KMS-Build.cfg");
                lConfigurator.ParseFile(File::Folder::CURRENT, "KMS-Make.cfg");

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                lResult = lM.Run();
            }
            KMS_MAIN_END;

            KMS_MAIN_RETURN;
        }

        Make::Make()
            : mComponent    (COMPONENT_DEFAULT)
            , mComponentType(ComponentType::NONE)
            , mConfiguration(CONFIGURATION_DEFAULT)
            , mMake         (MAKE_DEFAULT)
            , mProcessor    (PROCESSOR_DEFAULT)
            , mF_Product(File::Folder::Id::CURRENT)
        {
            mCleanExtensions.SetCreator(DI::String::Create);
            mIncludes       .SetCreator(DI::String_Expand::Create);

            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mCleanExtensions, false); AddEntry("CleanExtensions", lEntry, &MD_CLEAN_EXTENSIONS);
            lEntry.Set(&mComponent      , false); AddEntry("Component"      , lEntry, &MD_COMPONENT);
            lEntry.Set(&mComponentType  , false); AddEntry("ComponentType"  , lEntry, &MD_COMPONENT_TYPE);
            lEntry.Set(&mConfiguration  , false); AddEntry("Configuration"  , lEntry, &MD_CONFIGURATION);
            lEntry.Set(&mIncludes       , false); AddEntry("Includes"       , lEntry, &MD_INCLUDES);
            lEntry.Set(&mMake           , false); AddEntry("Make"           , lEntry, &MD_MAKE);
            lEntry.Set(&mProcessor      , false); AddEntry("Processor"      , lEntry, &MD_PROCESSOR);

            lEntry.Set(new DI::String(DEFAULT_CLEAN_EXCENTION), true); mCleanExtensions.AddEntry(lEntry);
            lEntry.Set(new DI::String(DEFAULT_INCLUDE        ), true); mIncludes       .AddEntry(lEntry);

            // ----- Build --------------------------------------------------

            mBinaries .SetCreator(DI::String::Create);
            mLibraries.SetCreator(DI::String::Create);
            mTests    .SetCreator(DI::String::Create);

            lEntry.Set(&mBinaries , false); AddEntry("Binaries" , lEntry, &MD_BINARIES);
            lEntry.Set(&mLibraries, false); AddEntry("Libraries", lEntry, &MD_LIBRARIES);
            lEntry.Set(&mTests    , false); AddEntry("Tests"    , lEntry, &MD_TESTS);

            lEntry.Set(&mBinaries , false); AddEntry(NAME_OS "Binaries" , lEntry, &MD_OS_BINARIES);
            lEntry.Set(&mLibraries, false); AddEntry(NAME_OS "Libraries", lEntry, &MD_OS_LIBRARIES);
            lEntry.Set(&mTests    , false); AddEntry(NAME_OS "Tests"    , lEntry, &MD_OS_TESTS);

            mF_Binaries  = File::Folder(mF_Product, "Binaries");
            mF_Libraries = File::Folder(mF_Product, "Libraries");
        }

        // ===== CLI::Tool ==================================================

        void Make::DisplayHelp(FILE* aOut)
        {
            assert(nullptr != aOut);

            fprintf(aOut,
                "Clean\n"
                "Depend\n"
                "Make\n");

            CLI::Tool::DisplayHelp(aOut);
        }

        int Make::ExecuteCommand(const char* aC)
        {
            assert(nullptr != aC);

            int lResult;

            if      (0 == strcmp("Clean" , aC)) { lResult = Run_Clean (); }
            else if (0 == strcmp("Depend", aC)) { lResult = Run_Depend(); }
            else if (0 == strcmp("Make"  , aC)) { lResult = Run_Make  (); }
            else
            {
                lResult = CLI::Tool::ExecuteCommand(aC);
            }

            return lResult;
        }

        int Make::Run()
        {
            VerifyConfig();

            char lFolder[PATH_LENGTH];

            sprintf_s(lFolder, "%s_%s", mConfiguration.Get(), mProcessor.Get());

            mF_Bin_Cfg = File::Folder(mF_Binaries , lFolder);
            mF_Lib_Cfg = File::Folder(mF_Libraries, lFolder);

            Prepare();

            return CLI::Tool::Run();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Make::Clean_Binaries()
        {
            for (const auto& lEntry : mBinaries.mInternal)
            {
                auto lB = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lB);

                Clean_Component(lB->Get());
                Clean_Binary   (lB->Get());
            }
        }

        void Make::Clean_Binary(const char* aB)
        {
            if (0 == strcmp(aB, "KMS-Make"))
            {
                return;
            }

            if (mF_Bin_Cfg.DoesFileExist(aB))
            {
                mF_Bin_Cfg.Delete(aB);
            }
        }

        void Make::Clean_Component(const char* aC)
        {
            File::Folder lC(mF_Product, aC);

            for (const auto& lEntry : mCleanExtensions.mInternal)
            {
                auto lCE = dynamic_cast<const DI::String*>(lEntry.Get());

                lC.DeleteFiles(lCE->Get());
            }
        }

        void Make::Clean_Library(const char* aL)
        {
            char lL[FILE_LENGTH];

            sprintf_s(lL, "%s.a", aL);

            if (mF_Lib_Cfg.DoesFileExist(lL))
            {
                mF_Lib_Cfg.Delete(lL);
            }
        }

        void Make::Clean_Libraries()
        {
            for (const auto& lEntry : mLibraries.mInternal)
            {
                auto lL = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lL);

                Clean_Component(lL->Get());
                Clean_Library  (lL->Get());
            }
        }

        void Make::Clean_Tests()
        {
            for (const auto& lEntry : mTests.mInternal)
            {
                auto lT = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lT);

                Clean_Component(lT->Get());
                Clean_Binary   (lT->Get());
            }
        }

        void Make::Depend_Component(const char* aC)
        {
            File::Folder lF_Component(mF_Product, aC);

            Text::File_ASCII lMakeFile;

            lMakeFile.Read(lF_Component, MAKE_FILE_NAME);

            StringSet_ASCII lSources;

            Depend_ParseMakeFile(&lMakeFile, &lSources);
            if (0 < lSources.size())
            {
                Depend lDepend(mIncludes, lF_Component);

                for (const auto& lSource : lSources)
                {
                    Depend_ParseSource(&lDepend, lSource.c_str(), &lMakeFile);
                }

                // Make sure the make file ends with an empty line.
                lMakeFile.mLines.push_back("");

                lF_Component.Backup(MAKE_FILE_NAME);

                lMakeFile.Write(lF_Component, MAKE_FILE_NAME);

                std::cout << lDepend;
                std::cout << std::endl;
            }
        }

        void Make::Depend_Components(const DI::Array& aComponents)
        {
            for (const auto& lEntry : aComponents.mInternal)
            {
                auto lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lC);

                Depend_Component(lC->Get());
            }
        }

        // ASSUMPTION The make file define the SOURCES variable containing
        //            the list of source files.

        // ASSUMPTION The line defining the SOURCES variable in the makefile
        //            also have a source file name in it.

        // ASSUMPTION No comment break the definition of the SOURCES.

        void Make::Depend_ParseMakeFile(Text::File_ASCII* aMF, StringSet_ASCII* aSources)
        {
            assert(nullptr != aMF);
            assert(nullptr != aSources);

            char lSource[PATH_LENGTH];

            for (auto lIt = aMF->mLines.begin(); lIt != aMF->mLines.end(); lIt++)
            {
                if (1 == sscanf_s(lIt->c_str(), "SOURCES = %[^ \\\n\r\r]", lSource SizeInfo(lSource)))
                {
                    aSources->insert(lSource);

                    for (lIt++; lIt != aMF->mLines.end(); lIt++)
                    {
                        if (1 != sscanf_s(lIt->c_str(), " %[^ \\\n\r\t]", lSource SizeInfo(lSource)))
                        {
                            break;
                        }

                        aSources->insert(lSource);
                    }

                    for (lIt++; lIt != aMF->mLines.end(); lIt++)
                    {
                        if (0 == strncmp("# DO NOT DELETE", lIt->c_str(), 15))
                        {
                            aMF->mLines.erase(lIt, aMF->mLines.end());
                            break;
                        }
                    }

                    aMF->mLines.push_back("# DO NOT DELETE - Generated by KMS::Build::Make !");
                    aMF->mLines.push_back("");
                    break;
                }
            }
        }

        // ASSUMPTION The source file listed in the makefile always have an
        //            extension.

        void Make::Depend_ParseSource(Depend* aDepend, const char* aSource, Text::File_ASCII* aMakeFile)
        {
            auto lHeaders = aDepend->ParseFile(aSource);
            if (0 < lHeaders->size())
            {
                char lLongLine[8192];

                strcpy_s(lLongLine, aSource);

                auto lPtr = strrchr(lLongLine, '.');
                assert(nullptr != lPtr);

                strcpy_s(lPtr SizeInfoV(lLongLine + sizeof(lLongLine) - lPtr), ".o:");

                for (const auto& lHeader : *lHeaders)
                {
                    strcat_s(lLongLine, " ");
                    strcat_s(lLongLine, lHeader.c_str());
                }

                aMakeFile->mLines.push_back(lLongLine);
            }
        }

        void Make::Make_Component(const char* aC)
        {
            Proc::Process lP(File::Folder(File::Folder::Id::NONE), mMake.Get());

            lP.SetWorkingDirectory(aC);

            lP.AddArgument(("CONFIG="    + mConfiguration.GetString()).c_str());
            lP.AddArgument(("PROCESSOR=" + mProcessor    .GetString()).c_str());

            lP.Run(MAKE_ALLOWED_TIME_ms);

            if (0 != lP.GetExitCode())
            {
                KMS_EXCEPTION(RESULT_COMPILATION_FAILED, "Cannot make", lP.GetCmdLine());
            }
        }

        void Make::Make_Components(const DI::Array& aComponents)
        {
            for (const auto& lEntry : aComponents.mInternal)
            {
                auto lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(nullptr != lC);

                Make_Component(lC->Get());
            }
        }

        void Make::Prepare()
        {
            if (!mF_Binaries .DoesExist()) { mF_Binaries .Create(); }
            if (!mF_Libraries.DoesExist()) { mF_Libraries.Create(); }

            if (!mF_Bin_Cfg.DoesExist()) { mF_Bin_Cfg.Create(); }
            if (!mF_Lib_Cfg.DoesExist()) { mF_Lib_Cfg.Create(); }
        }

        int Make::Run_Clean()
        {
            switch (mComponentType)
            {
            case ComponentType::BINARY:
            case ComponentType::TEST:
                Clean_Binary(mComponent.Get());
                Clean_Libraries();
                Clean_Component(mComponent.Get());
                break;

            case ComponentType::DRIVER:
                Clean_Libraries();
                Clean_Component(mComponent.Get());
                break;

            case ComponentType::LIBRARY:
                Clean_Library(mComponent.Get());
                Clean_Component(mComponent.Get());
                break;

            case ComponentType::NONE:
                Clean_Binaries();
                Clean_Libraries();
                Clean_Tests();
                break;

            default: assert(false);
            }

            return 0;
        }

        int Make::Run_Depend()
        {
            switch (mComponentType)
            {
            case ComponentType::BINARY:
            case ComponentType::DRIVER:
            case ComponentType::LIBRARY:
            case ComponentType::TEST: Depend_Component(mComponent.Get()); break;

            case ComponentType::NONE:
                Depend_Components(mBinaries);
                Depend_Components(mLibraries);
                Depend_Components(mTests);
                break;

            default: assert(false);
            }

            return 0;
        }

        int Make::Run_Make()
        {
            switch (mComponentType)
            {
            case ComponentType::BINARY:
            case ComponentType::DRIVER:
            case ComponentType::TEST:
                Make_Components(mLibraries);
                Make_Component(mComponent.Get());
                break;

            case ComponentType::LIBRARY:
                Make_Component(mComponent.Get());
                break;

            case ComponentType::NONE:
                Make_Components(mLibraries);
                Make_Components(mTests);
                Make_Components(mBinaries);
                break;

            default: assert(false);
            }

            return 0;
        }

        void Make::VerifyConfig()
        {
            if (0 < mComponent.GetLength())
            {
                auto lComponent = mComponent.Get();

                if      (DoesContain(mBinaries , lComponent)) { mComponentType = ComponentType::BINARY ; }
                else if (DoesContain(mLibraries, lComponent)) { mComponentType = ComponentType::LIBRARY; }
                else if (DoesContain(mTests    , lComponent)) { mComponentType = ComponentType::TEST   ; }
                else
                {
                    KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid component", lComponent);
                }
            }

            KMS_EXCEPTION_ASSERT(0 < mConfiguration.GetLength(), RESULT_INVALID_CONFIG, "Empty configuration", "");
            KMS_EXCEPTION_ASSERT(0 < mProcessor    .GetLength(), RESULT_INVALID_CONFIG, "Empty processor"    , "");
        }

    }
}

using namespace KMS;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

bool DoesContain(const DI::Array& aArray, const char* aStr)
{
    for (const auto& lEntry : aArray.mInternal)
    {
        auto lStr = dynamic_cast<const DI::String*>(lEntry.Get());
        assert(nullptr != lStr);

        if (*lStr == aStr)
        {
            return true;
        }
    }

    return false;
}

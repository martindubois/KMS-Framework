
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Make.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Includes ===========================================================
#include <KMS/Build/Depend.h>
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Proc/Process.h>

#include <KMS/Build/Make.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_CONFIGURATION "Debug"

#define DEFAULT_INCLUDE "Includes"

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MAKE_FILE_NAME "makefile"

static const KMS::Cfg::MetaData MD_BINARIES      ("Binaries += {Name}");
static const KMS::Cfg::MetaData MD_COMPONENT     ("Component = {Name}");
static const KMS::Cfg::MetaData MD_COMPONENT_TYPE("ComponentType = BINARY | LIBRARY | NONE | TEST");
static const KMS::Cfg::MetaData MD_CONFIGURATION ("Configuration = {Name}");
static const KMS::Cfg::MetaData MD_INCLUDES      ("Includes += {Name}");
static const KMS::Cfg::MetaData MD_LIBRARIES     ("Libraries += {Name}");
static const KMS::Cfg::MetaData MD_TESTS         ("Tests += {Name};");

#ifdef _KMS_DARWIN_
    #define NAME_OS "Darwin"

    static const KMS::Cfg::MetaData MD_OS_BINARIES  ("DarwinBinaries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_LIBRARIES ("DarwinLibraries += {Name}");
#endif

#ifdef _KMS_LINUX_
    #define NAME_OS "Linux"

    static const KMS::Cfg::MetaData MD_OS_BINARIES  ("LinuxBinaries += {Name}");
    static const KMS::Cfg::MetaData MD_OS_LIBRARIES ("LinuxLibraries += {Name}");
#endif

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static bool DoesContain(const KMS::DI::Array& aArray, const char* aStr);

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const char* Make::COMPONENT_TYPE_NAMES[] = { "BINARY", "LIBRARY", "NONE", "TEST" };

        int Make::Main(int aCount, const char ** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            try
            {
                KMS::Cfg::Configurator lC;
                KMS::Build::Make       lM;

                lC.AddConfigurable(&lM);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder::CURRENT, "KMS-Build.cfg");

                lC.ParseArguments(aCount - 1, aVector + 1);

                lResult = lM.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Make::Make()
            : mComponentType(ComponentType::NONE)
            , mConfiguration(DEFAULT_CONFIGURATION)
            , mF_Product(File::Folder::Id::CURRENT)
        {
            mBinaries  .SetCreator(DI::String::Create);
            mIncludes  .SetCreator(DI::String_Expand::Create);
            mLibraries .SetCreator(DI::String::Create);
            mTests     .SetCreator(DI::String::Create);

            AddEntry("Binaries"     , &mBinaries     , false, &MD_BINARIES);
            AddEntry("Componentn"   , &mComponent    , false, &MD_COMPONENT);
            AddEntry("ComponentType", &mComponentType, false, &MD_COMPONENT_TYPE);
            AddEntry("Configuration", &mConfiguration, false, &MD_CONFIGURATION);
            AddEntry("Includes"     , &mIncludes     , false, &MD_INCLUDES);
            AddEntry("Libraries"    , &mLibraries    , false, &MD_LIBRARIES);
            AddEntry("Tests"        , &mTests        , false, &MD_TESTS);

            AddEntry(NAME_OS "Binaries" , &mBinaries , false, &MD_OS_BINARIES);
            AddEntry(NAME_OS "Libraries", &mLibraries, false, &MD_OS_LIBRARIES);

            mF_Binaries  = File::Folder(mF_Product, "Binaries");
            mF_Libraries = File::Folder(mF_Product, "Libraries");

            mIncludes.AddEntry(new DI::String(DEFAULT_INCLUDE), true);
        }

        Make::~Make() {}

        void Make::AddBinary   (const char* aB) { mBinaries  .AddEntry(new DI::String(aB), true); }
        void Make::AddInclude  (const char* aI) { mIncludes  .AddEntry(new DI::String_Expand(aI), true); }
        void Make::AddLibrary  (const char* aL) { mLibraries .AddEntry(new DI::String(aL), true); }
        void Make::AddTest     (const char* aT) { mTests     .AddEntry(new DI::String(aT), true); }

        void Make::ResetBinaries  () { mBinaries  .Clear(); }
        void Make::ResetComponent () { mComponent .Clear(); }
        void Make::ResetIncludes  () { mIncludes  .Clear(); }
        void Make::ResetLibraries () { mLibraries .Clear(); }
        void Make::ResetTests     () { mTests     .Clear(); }

        void Make::SetComponent    (const char* aC) { assert(NULL != aC); mComponent     = aC; }
        void Make::SetConfiguration(const char* aC) { assert(NULL != aC); mConfiguration = aC; }

        // ===== CLI::Tool ==================================================

        void Make::DisplayHelp(FILE* aOut)
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "Clean\n"
                "Depend\n"
                "Make\n");

            CLI::Tool::DisplayHelp(aOut);
        }

        void Make::ExecuteCommand(const char* aC)
        {
            assert(NULL != aC);

            if      (0 == strcmp("Clean" , aC)) { Run_Clean (); }
            else if (0 == strcmp("Depend", aC)) { Run_Depend(); }
            else if (0 == strcmp("Make"  , aC)) { Run_Make  (); }
            else
            {
                CLI::Tool::ExecuteCommand(aC);
            }
        }

        int Make::Run()
        {
            VerifyConfig();

            mF_Bin_Cfg = File::Folder(mF_Binaries , mConfiguration.Get());
            mF_Lib_Cfg = File::Folder(mF_Libraries, mConfiguration.Get());

            Prepare();

            return CLI::Tool::Run();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Make::Clean_Binaries()
        {
            for (const DI::Container::Entry& lEntry : mBinaries.mInternal)
            {
                const DI::String* lB = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lB);

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

            lC.DeleteFiles("*.o");
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
            for (const DI::Container::Entry& lEntry : mLibraries.mInternal)
            {
                const DI::String* lL = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lL);

                Clean_Component(lL->Get());
                Clean_Library  (lL->Get());
            }
        }

        void Make::Clean_Tests()
        {
            for (const DI::Container::Entry& lEntry : mTests.mInternal)
            {
                const DI::String* lT = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lT);

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

            Depend lDepend(mIncludes, lF_Component);

            for (const std::string& lSource : lSources)
            {
                Depend_ParseSource(&lDepend, lSource.c_str(), &lMakeFile);
            }

            // Make sure the make file ends with an empty line.
            lMakeFile.mLines.push_back("");

            lF_Component.Backup(MAKE_FILE_NAME);

            lMakeFile.Write(lF_Component, MAKE_FILE_NAME);
        }

        void Make::Depend_Components(const DI::Array& aComponents)
        {
            for (const DI::Container::Entry& lEntry : aComponents.mInternal)
            {
                const DI::String* lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lC);

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
            assert(NULL != aMF);
            assert(NULL != aSources);

            char lSource[PATH_LENGTH];

            Text::File_ASCII::Internal::iterator lIt;

            for (lIt = aMF->mLines.begin(); lIt != aMF->mLines.end(); lIt++)
            {
                if (1 == sscanf(lIt->c_str(), "SOURCES = %[^ \\\n\r\r]", lSource))
                {
                    aSources->insert(lSource);
                    break;
                }
            }

            for (lIt++; lIt != aMF->mLines.end(); lIt++)
            {
                if (1 != sscanf(lIt->c_str(), " %[^ \\\n\r\t]", lSource))
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
        }

        // ASSUMPTION The source file listed in the makefile always have an
        //            extension.

        void Make::Depend_ParseSource(Depend* aDepend, const char* aSource, Text::File_ASCII* aMakeFile)
        {
            printf("%s( , \"%s\",  )\n", __FUNCTION__, aSource);

            KMS::StringSet_ASCII* lHeaders = aDepend->ParseFile(aSource);
            if (0 < lHeaders->size())
            {
                char lLongLine[8192];

                strcpy(lLongLine, aSource);

                char* lPtr = strrchr(lLongLine, '.');
                assert(NULL != lPtr);

                strcpy(lPtr, ".o:");

                for (const std::string& lHeader : *lHeaders)
                {
                    strcat(lLongLine, " ");
                    strcat(lLongLine, lHeader.c_str());
                }

                aMakeFile->mLines.push_back(lLongLine);
            }
        }

        void Make::Make_Component(const char* aC)
        {
            Proc::Process lP(File::Folder(File::Folder::Id::NONE), "make");

            lP.SetWorkingDirectory(aC);

            lP.AddArgument(("CONFIG=" + mConfiguration.mInternal).c_str());

            lP.Run(1000 * 60 * 5);

            if (0 != lP.GetExitCode())
            {
                KMS_EXCEPTION(BUILD_COMPILE_FAILED, "Cannot make", lP.GetCmdLine());
            }
        }

        void Make::Make_Components(const DI::Array& aComponents)
        {
            for (const DI::Container::Entry& lEntry : aComponents.mInternal)
            {
                const DI::String* lC = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lC);

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

        void Make::Run_Clean()
        {
            switch (mComponentType)
            {
            case ComponentType::BINARY:
            case ComponentType::TEST:
                Clean_Binary(mComponent.Get());
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
        }

        void Make::Run_Depend()
        {
            switch (mComponentType)
            {
            case ComponentType::BINARY:
            case ComponentType::LIBRARY:
            case ComponentType::TEST: Depend_Component(mComponent.Get()); break;

            case ComponentType::NONE:
                Depend_Components(mBinaries);
                Depend_Components(mLibraries);
                Depend_Components(mTests);
                break;

            default: assert(false);
            }
        }

        void Make::Run_Make()
        {
            switch (mComponentType)
            {
            case ComponentType::BINARY:
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
        }

        void Make::VerifyConfig()
        {
            if (0 < mComponent.GetLength())
            {
                const char* lComponent = mComponent.Get();

                if      (DoesContain(mBinaries , lComponent)) { mComponentType = ComponentType::BINARY ; }
                else if (DoesContain(mLibraries, lComponent)) { mComponentType = ComponentType::LIBRARY; }
                else if (DoesContain(mTests    , lComponent)) { mComponentType = ComponentType::TEST   ; }
                else
                {
                    KMS_EXCEPTION(BUILD_CONFIG_INVALID, "Invalid component", lComponent);
                }
            }

            KMS_EXCEPTION_ASSERT(0 < mConfiguration.GetLength(), BUILD_CONFIG_INVALID, "Empty configuration", "");
        }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

bool DoesContain(const KMS::DI::Array& aArray, const char* aStr)
{
    for (const KMS::DI::Container::Entry& lEntry : aArray.mInternal)
    {
        const KMS::DI::String* lStr = dynamic_cast<const KMS::DI::String*>(lEntry.Get());
        assert(NULL != lStr);

        if (*lStr == aStr)
        {
            return true;
        }
    }

    return false;
}

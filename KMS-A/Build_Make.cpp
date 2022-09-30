
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
#include <KMS/Proc/Process.h>

#include <KMS/Build/Make.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_CONFIGURATION "Debug"

#define DEFAULT_INCLUDE "Includes"

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MAKE_FILE_NAME "makefile"

namespace KMS
{
    namespace Build
    {

        // Public
        // //////////////////////////////////////////////////////////////////

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

                lM.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT), "KMS-Build.cfg");
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
            mF_Binaries  = File::Folder(mF_Product, "Binaries");
            mF_Libraries = File::Folder(mF_Product, "Libraries");

            mIncludes.insert(DEFAULT_INCLUDE);
        }

        Make::~Make() {}

        void Make::AddBinary   (const char* aB) { assert(NULL != aB); mBinaries  .insert(aB); }
        void Make::AddInclude  (const char* aI) { assert(NULL != aI); mIncludes  .insert(aI); }
        void Make::AddLibrary  (const char* aL) { assert(NULL != aL); mLibraries .insert(aL); }
        void Make::AddOperation(const char* aO) { assert(NULL != aO); mOperations.insert(aO); }
        void Make::AddTest     (const char* aT) { assert(NULL != aT); mTests     .insert(aT); }

        void Make::ResetBinaries  () { mBinaries  .clear(); }
        void Make::ResetComponent () { mComponent .clear(); }
        void Make::ResetIncludes  () { mIncludes  .clear(); }
        void Make::ResetLibraries () { mLibraries .clear(); }
        void Make::ResetOperations() { mOperations.clear(); }
        void Make::ResetTests     () { mTests     .clear(); }

        void Make::SetComponent    (const char* aC) { assert(NULL != aC); mComponent     = aC; }
        void Make::SetConfiguration(const char* aC) { assert(NULL != aC); mConfiguration = aC; }

        int Make::Run()
        {
            VerifyConfig();

            mF_Bin_Cfg = File::Folder(mF_Binaries , mConfiguration.c_str());
            mF_Lib_Cfg = File::Folder(mF_Libraries, mConfiguration.c_str());

            Prepare();

            for (std::string lO : mOperations)
            {
                if      (lO == "Clean" ) { Run_Clean (); }
                else if (lO == "Depend") { Run_Depend(); }
                else if (lO == "Make"  ) { Run_Make  (); }
                else
                {
                    KMS_EXCEPTION_WITH_INFO(CONFIG, "Invalid operation", lO.c_str());
                }
            }

            return 0;
        }

        // ===== Cfg::Configurable ==========================================

        bool Make::AddAttribute(const char* aA, const char* aV)
        {
            CFG_CALL("Binaries"  , AddBinary);
            CFG_CALL("Includes"  , AddInclude);
            CFG_CALL("Libraries" , AddLibrary);
            CFG_CALL("Operations", AddOperation);
            CFG_CALL("Tests"     , AddTest);

            CFG_CALL("LinuxBinaries" , AddBinary);
            CFG_CALL("LinuxLibraries", AddLibrary);
            CFG_CALL("LinuxTests"    , AddTest);

            return Configurable::AddAttribute(aA, aV);
        }

        void Make::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "===== KMS::Build::Make =====\n"
                "Binaries\n"
                "    Reset the list of binary components\n"
                "Binaries += {Value}\n"
                "    Add a binary component to the list\n"
                "Binary = {Value}\n"
                "    Replace the list of binarie components\n"
                "Component\n"
                "    Unselect the selected component\n"
                "Component = {Value}\n"
                "    Select a component\n"
                "Include = {Value}\n"
                "    Replace the list of include folder\n"
                "Includes\n"
                "    Reset the list of include folder\n"
                "Includes += {Value}\n"
                "    Add a folder to the list\n"
                "Libraries\n"
                "    Reset the list of library components\n"
                "Libraries += {Value}\n"
                "    Add a library component to the list\n"
                "LinuxBinaries += {Value}\n"
                "    Add a binary component to the list\n"
                "LinuxLibraries += {Value}\n"
                "    Add a library component to the list\n"
                "LinuxTests += {Value}\n"
                "    Add a test componentn to the list\n"
                "Operation = {Value}\n"
                "    Replace the list of operations\n"
                "Operations\n"
                "    Reset the list of operations\n"
                "Operations += {Value}\n"
                "    Add an operation to the list\n"
                "Test = {Value}\n"
                "    Replace the list of test components\n"
                "Tests\n"
                "    Reset the list of test components\n"
                "Tests += {Value}\n"
                "    Add a test component to the list\n");

            Cfg::Configurable::DisplayHelp(aOut);
        }

        bool Make::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Binaries"  ) { ResetBinaries  (); return true; }
                CFG_IF("Component" ) { ResetComponent (); return true; }
                CFG_IF("Includes"  ) { ResetIncludes  (); return true; }
                CFG_IF("Libraries" ) { ResetLibraries (); return true; }
                CFG_IF("Operations") { ResetOperations(); return true; }
                CFG_IF("Tests"     ) { ResetTests     (); return true; }
            }
            else
            {
                CFG_CALL("Component"    , SetComponent);
                CFG_CALL("Configuration", SetConfiguration);

                CFG_IF("Binary"   ) { ResetBinaries  (); AddBinary   (aV); return true; }
                CFG_IF("Include"  ) { ResetIncludes  (); AddInclude  (aV); return true; }
                CFG_IF("Library"  ) { ResetLibraries (); AddLibrary  (aV); return true; }
                CFG_IF("Operation") { ResetOperations(); AddOperation(aV); return true; }
            }

            return Configurable::SetAttribute(aA, aV);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Make::Clean_Binaries()
        {
            for (std::string lL : mBinaries)
            {
                Clean_Component(lL.c_str());
                Clean_Binary(lL.c_str());
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
                mF_Bin_Cfg.DeleteFile(aB);
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
                mF_Lib_Cfg.DeleteFile(lL);
            }

        }

        void Make::Clean_Libraries()
        {
            for (std::string lL : mLibraries)
            {
                Clean_Component(lL.c_str());
                Clean_Library(lL.c_str());
            }
        }

        void Make::Clean_Tests()
        {
            for (std::string lT : mTests)
            {
                Clean_Component(lT.c_str());
                Clean_Binary(lT.c_str());
            }
        }

        void Make::Depend_Component(const char* aC)
        {
            File::Folder lF_Component(mF_Product, aC);

            Text::TextFile lMakeFile;

            lMakeFile.Read(lF_Component, MAKE_FILE_NAME);

            StringSet lSources;

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

        void Make::Depend_Components(const StringSet& aComponents)
        {
            for (std::string lC : aComponents)
            {
                Depend_Component(lC.c_str());
            }
        }

        // ASSUMPTION The make file define the SOURCES variable containing
        //            the list of source files.

        // ASSUMPTION The line defining the SOURCES variable in the makefile
        //            also have a source file name in it.

        // ASSUMPTION No comment break the definition of the SOURCES.

        void Make::Depend_ParseMakeFile(Text::TextFile* aMF, StringSet* aSources)
        {
            assert(NULL != aMF);
            assert(NULL != aSources);

            char lSource[PATH_LENGTH];

            StringList::iterator lIt;

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

        void Make::Depend_ParseSource(Depend* aDepend, const char* aSource, Text::TextFile* aMakeFile)
        {
            printf("%s( , \"%s\",  )\n", __FUNCTION__, aSource);

            KMS::StringSet* lHeaders = aDepend->ParseFile(aSource);
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

            lP.AddArgument(("CONFIG=" + mConfiguration).c_str());

            lP.Run(1000 * 60 * 5);

            if (0 != lP.GetExitCode())
            {
                KMS_EXCEPTION_WITH_INFO(MAKE_MAKE, "Cannot make", lP.GetCmdLine());
            }
        }

        void Make::Make_Components(const StringSet& aComponents)
        {
            for (std::string lC : aComponents)
            {
                Make_Component(lC.c_str());
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
                Clean_Binary(mComponent.c_str());
                Clean_Libraries();
                Clean_Component(mComponent.c_str());
                break;

            case ComponentType::LIBRARY:
                Clean_Library(mComponent.c_str());
                Clean_Component(mComponent.c_str());
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
            case ComponentType::TEST: Depend_Component(mComponent.c_str()); break;

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
                Make_Component(mComponent.c_str());
                break;

            case ComponentType::LIBRARY:
                Make_Component(mComponent.c_str());
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
            if (0 < mComponent.size())
            {
                if (mBinaries .end() != mBinaries .find(mComponent)) { mComponentType = ComponentType::BINARY ; }
                if (mLibraries.end() != mLibraries.find(mComponent)) { mComponentType = ComponentType::LIBRARY; }
                if (mTests    .end() != mTests    .find(mComponent)) { mComponentType = ComponentType::TEST   ; }
                else
                {
                    KMS_EXCEPTION_WITH_INFO(CONFIG, "Invalid component", mComponent.c_str());
                }
            }

            KMS_EXCEPTION_ASSERT(0 < mConfiguration.size(), CONFIG, "Empty configuration");
        }

    }
}

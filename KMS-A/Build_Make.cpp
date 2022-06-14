
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Make.cpp

#include "Component.h"

// ===== C ==================================================================
#include <unistd.h>

// ===== Includes ===========================================================
#include <KMS/Config/Configurator.h>
#include <KMS/Process.h>

#include <KMS/Build/Make.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_CONFIGURATION "Debug"

#define DEFAULT_OPERATION "Make"

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
                KMS::Build::Make          lM;
                KMS::Config::Configurator lC;

                lM.InitConfigurator(&lC);

                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT), "KMS-Build.cfg");
                lC.ParseArguments(aCount - 1, aVector + 1);

                lC.Init();

                lResult = lM.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Make::Make()
            : mComponentType(ComponentType::NONE)
            , mConfiguration(DEFAULT_CONFIGURATION)
            , mF_Product(File::Folder::Id::CURRENT)
            , mOperation(DEFAULT_OPERATION)
        {
            mF_Binaries  = File::Folder(mF_Product, "Binaries");
            mF_Libraries = File::Folder(mF_Product, "Libraries");
        }

        Make::~Make() {}

        void Make::AddBinary (const char* aB) { assert(NULL != aB); mBinaries .insert(aB); }
        void Make::AddLibrary(const char* aL) { assert(NULL != aL); mLibraries.insert(aL); }
        void Make::AddTest   (const char* aT) { assert(NULL != aT); mTests    .insert(aT); }

        void Make::SetComponent    (const char* aC) { assert(NULL != aC); mComponent     = aC; }
        void Make::SetConfiguration(const char* aC) { assert(NULL != aC); mConfiguration = aC; }
        void Make::SetOperation    (const char* aO) { assert(NULL != aO); mOperation     = aO; }

        int Make::Run()
        {
            VerifyConfig();

            mF_Bin_Cfg = File::Folder(mF_Binaries , mConfiguration.c_str());
            mF_Lib_Cfg = File::Folder(mF_Libraries, mConfiguration.c_str());

            Prepare();

            if      (mOperation == "Clean" ) { Run_Clean(); }
            else if (mOperation == "Depend") { Run_Depend(); }
            else if (mOperation == "Make"  ) { Run_Make(); }
            else
            {
                KMS_EXCEPTION_WITH_INFO(CONFIG, "Invalid operation", mOperation.c_str());
            }

            return 0;
        }

        // ===== Config::Configurable ===================================

        bool Make::AddAttribute(const char* aA, const char* aV)
        {
            CFG_CALL("Binaries" , AddBinary);
            CFG_CALL("Libraries", AddLibrary);
            CFG_CALL("Tests"    , AddTest);

            CFG_CALL("LinuxBinaries" , AddBinary);
            CFG_CALL("LinuxLibraries", AddLibrary);
            CFG_CALL("LinuxTests"    , AddTest);

            return Configurable::AddAttribute(aA, aV);
        }

        bool Make::SetAttribute(const char *aA)
        {
            CFG_IF("Binaries" ) { mBinaries .clear(); return true; }
            CFG_IF("Component") { mComponent.clear(); return true; }
            CFG_IF("Libraries") { mLibraries.clear(); return true; }
            CFG_IF("Tests"    ) { mTests    .clear(); return true; }

            return Configurable::SetAttribute(aA);
        }

        bool Make::SetAttribute(const char* aA, const char* aV)
        {
            CFG_CALL("Component"    , SetComponent);
            CFG_CALL("Configuration", SetConfiguration);
            CFG_CALL("Operation"    , SetOperation);

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
            char lL[1024];

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
            Process lP(File::Folder(File::Folder::Id::NONE), "make");

            lP.SetWorkingDirectory(aC);

            lP.AddArgument("depend");
            lP.AddArgument(("CONFIG=" + mConfiguration).c_str());

            if (0 != lP.Run())
            {
                KMS_EXCEPTION_WITH_INFO(MAKE_DEPEND, "Cannot uptdate file dependencies", lP.GetCmdLine());
            }
        }

        void Make::Depend_Components(const StringSet& aComponents)
        {
            for (std::string lC : aComponents)
            {
                Depend_Component(lC.c_str());
            }
        }

        void Make::Make_Component(const char* aC)
        {
            Process lP(File::Folder(File::Folder::Id::NONE), "make");

            lP.AddArgument(("CONFIG=" + mConfiguration).c_str());

            if (0 != chdir(aC))
            {
                KMS_EXCEPTION_WITH_INFO(FOLDER_ACCESS, "Cannot change current directory", aC);
            }

            int lRet = lP.Run();

            if (0 != chdir(".."))
            {
                KMS_EXCEPTION(FOLDER_ACCESS, "Cannot change current directory");
            }

            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(MAKE_DEPEND, "Cannot uptdate file dependencies", lP.GetCmdLine());
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

            if (0 == mConfiguration.size())
            {
                KMS_EXCEPTION(CONFIG, "Empty configuration");
            }
        }

    }
}

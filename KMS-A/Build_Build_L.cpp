
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Build_Build_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Build/Make.h>
#include <KMS/Cfg/Configurator.h>
#include <KMS/Proc/Process.h>

#include <KMS/Build/Build.h>

namespace KMS
{
    namespace Build
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Build::Compile(const char* aC)
        {
            Cfg::Configurator lC;
            Make lM;

            lC.AddConfigurable(&lM);

            lC.AddConfigurable(&Dbg::gLog);

            lC.ParseFile(File::Folder::CURRENT, "KMS-Build.cfg");

            lM.SetConfiguration(aC);

            lM.AddOperation("Clean");
            lM.AddOperation("Make");

            int lRet = lM.Run();
            if (0 != lRet)
            {
                KMS_EXCEPTION(BUILD_COMPILE, "KMS::Build::Make::Run failed", lRet);
            }
        }

        void Build::Package_Component(const char* aC)
        {
            File::Folder lBinaries(mTempFolder , "Binaries" );
            File::Folder lLibraries(mTempFolder, "Libraries");

            File::Folder lBin(lBinaries , aC);
            File::Folder lLib(lLibraries, aC);

            File::Folder lBin_Src((std::string("Binaries/" ) + aC).c_str());
            File::Folder lLib_Src((std::string("Libraries/") + aC).c_str());

            lBin.Create();
            lLib.Create();

            const DI::Array::Internal& lInternalB = mBinaries.GetInternal();
            for (const DI::Container::Entry& lEntry : lInternalB)
            {
                const DI::String* lB = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lB);

                lBin_Src.Copy(lBin, lB->Get());
            }

            const DI::Array::Internal& lInternalL = mLibraries.GetInternal();
            for (const DI::Container::Entry& lEntry : lInternalL)
            {
                const DI::String* lL = dynamic_cast<const DI::String*>(lEntry.Get());
                assert(NULL != lL);

                lLib_Src.Copy(lLib, (lL->GetInternal() + ".a").c_str());
            }
        }

        void Build::Test(const char* aC)
        {
            const KMS::DI::Array::Internal& lInternal = mTests.GetInternal();
            for (const KMS::DI::Container::Entry& lEntry : lInternal)
            {
                const KMS::DI::String* lT = dynamic_cast<const KMS::DI::String*>(lEntry.Get());
                assert(NULL != lT);

                Proc::Process lP((std::string("Binaries/") + aC).c_str(), lT->Get());

                lP.AddArgument("Groups+=Auto");

                lP.Run(1000 * 60 * 5);

                KMS_EXCEPTION_ASSERT(0 == lP.GetExitCode(), BUILD_TEST, "The test failed", lP.GetCmdLine());
            }
        }

    }
}


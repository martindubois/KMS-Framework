
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Sync.cpp

// CODE COVERAGE 2022-05-28 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Dbg/Log_Cfg.h>
#include <KMS/Dbg/Stats.h>
#include <KMS/Dbg/Stats_Timer.h>
#include <KMS/DI/Array.h>
#include <KMS/File/FileInfoList.h>
#include <KMS/Installer.h>

#include <KMS/File/Sync.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Sync.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_BIDIRECTIONAL ("Bidirectional[{Group}] += {Path}");
static const KMS::Cfg::MetaData MD_UNIDIRECTIONAL("Unidirectional += {Path};{Path}");
static const KMS::Cfg::MetaData MD_VERBOSE       ("Verbose = False | True");

// Static fonction desclarations
// //////////////////////////////////////////////////////////////////////////

static KMS::DI::Object* CreateGroup();

static KMS::File::FileInfoList* ToFileInfoList(const char* aFolder);

namespace KMS
{
    namespace File
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        int Sync::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(nullptr != aVector);
            assert(nullptr != aVector[0]);

            int lResult = __LINE__;

            auto lET = new Dbg::Stats_Timer("Main_ExecutionTime");
            lET->Start();

            try
            {
                Cfg::Configurator lC;
                Installer         lInstaller;
                Dbg::Log_Cfg      lLogCfg(&Dbg::gLog);
                File::Sync        lS;

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lS);

                lC.AddConfigurable(&lLogCfg);
                lC.AddConfigurable(&Dbg::gStats);

                lC.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lC.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lC.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lInstaller.Run();

                lResult = lS.Run();
            }
            KMS_CATCH_RESULT(lResult);

            lET->Stop();

            return lResult;
        }

        Sync::Sync()
        {
            mBidirectional .SetCreator(CreateGroup);
            mUnidirectional.SetCreator(DI::String_Expand::Create);

            AddEntry("Bidirectional" , &mBidirectional , false, &MD_BIDIRECTIONAL);
            AddEntry("Unidirectional", &mUnidirectional, false, &MD_UNIDIRECTIONAL);
            AddEntry("Verbose"       , &mVerbose       , false, &MD_VERBOSE);
        }

        Sync::~Sync() {}

        int Sync::Run()
        {
            VerifyConfig();

            Run_Bidirectional();
            Run_Unidirectional();

            return 0;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Sync::Run_Bidirectional()
        {
            for (const auto& lVT : mBidirectional.mInternal)
            {
                assert(nullptr != lVT.second);

                auto lGroup = dynamic_cast<const DI::Array*>(lVT.second.Get());
                assert(nullptr != lGroup);

                Run_Bidirectional(*lGroup);
            }
        }

        void Sync::Run_Unidirectional()
        {
            for (const DI::Object* lObj : mUnidirectional.mInternal)
            {
                assert(nullptr != lObj);

                auto lStr = dynamic_cast<const DI::String*>(lObj);
                assert(nullptr != lStr);

                char lDstName[PATH_LENGTH];
                char lSrcName[PATH_LENGTH];

                auto lRet = sscanf_s(*lStr, "%[^;];%s", lSrcName SizeInfo(lSrcName), lDstName SizeInfo(lDstName));
                KMS_EXCEPTION_ASSERT(2 == lRet, FILE_CONFIG_INVALID, "Invalid unidirectional entry", lRet);

                auto lDst = ToFileInfoList(lDstName);
                auto lSrc = ToFileInfoList(lSrcName);

                std::cout << *lSrc;

                unsigned int lFlags = FileInfoList::FLAG_IF_DOES_NOT_EXIST | FileInfoList::FLAG_IF_NEWER;

                lFlags |= Folder::FLAG_BACKUP | Folder::FLAG_IGNORE_ERROR;

                if (mVerbose.Get())
                {
                    lFlags |= Folder::FLAG_VERBOSE;
                }

                FileInfoList::Counters lCounters;

                lSrc->Copy(lDst, lFlags, &lCounters);

                delete lDst;
                delete lSrc;

                std::cout << lCounters;
            }
        }

        void Sync::Run_Bidirectional(const DI::Array& aGroup)
        {
            std::list<FileInfoList*> lLists;

            for (const DI::Object* lObj : aGroup.mInternal)
            {
                assert(nullptr != lObj);

                auto lPath = dynamic_cast<const DI::String*>(lObj);
                assert(nullptr != lPath);

                lLists.push_back(ToFileInfoList(*lPath));
            }

            for (auto lA : lLists)
            {
                for (auto lB : lLists)
                {
                    if (lA != lB)
                    {
                        Run_Bidirectional(lA, lB);
                    }
                }
            }

            for (auto lList : lLists)
            {
                delete lList;
            }
        }

        void Sync::Run_Bidirectional(FileInfoList* aA, FileInfoList* aB)
        {
            assert(nullptr != aA);
            assert(nullptr != aB);

            unsigned int lFlags = FileInfoList::FLAG_IF_DOES_NOT_EXIST;

            lFlags |= Folder::FLAG_VERBOSE;

            aA->Copy(aB, lFlags);
            aB->Copy(aA, lFlags);
        }

        void Sync::VerifyConfig() const
        {
            for (const auto& lVT : mBidirectional.mInternal)
            {
                assert(nullptr != lVT.second);

                auto lGroup = dynamic_cast<const DI::Array*>(lVT.second.Get());
                assert(nullptr != lGroup);

                KMS_EXCEPTION_ASSERT(1 < lGroup->GetCount(), FILE_CONFIG_INVALID, "Number of folders cannot be one", lVT.first.c_str());
            }
        }

    }
}

using namespace KMS;

// Static fonctions
// //////////////////////////////////////////////////////////////////////////

DI::Object* CreateGroup()
{
    auto lResult = new DI::Array;

    lResult->SetCreator(DI::String::Create);

    return lResult;
}

File::FileInfoList* ToFileInfoList(const char* aFolder)
{
    return new File::FileInfoList(File::Folder(aFolder), "*", true);
}

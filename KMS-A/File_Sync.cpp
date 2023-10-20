
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Sync.cpp

// CODE COVERAGE 2022-05-28 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/Array.h>
#include <KMS/File/FileInfoList.h>
#include <KMS/Main.h>

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

        const bool Sync::VERBOSE_DEFAULT = false;

        int Sync::Main(int aCount, const char** aVector)
        {
            KMS_MAIN_BEGIN;
            {
                File::Sync lS;

                lConfigurator.AddConfigurable(&lS);

                lConfigurator.ParseFile(File::Folder::EXECUTABLE, CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::HOME      , CONFIG_FILE);
                lConfigurator.ParseFile(File::Folder::CURRENT   , CONFIG_FILE);

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                lResult = lS.Run();
            }
            KMS_MAIN_END;

            KMS_MAIN_RETURN;
        }

        Sync::Sync() : mVerbose(VERBOSE_DEFAULT)
        {
            mBidirectional .SetCreator(CreateGroup);
            mUnidirectional.SetCreator(DI::String_Expand::Create);

            AddEntry("Bidirectional" , &mBidirectional , false, &MD_BIDIRECTIONAL);
            AddEntry("Unidirectional", &mUnidirectional, false, &MD_UNIDIRECTIONAL);
            AddEntry("Verbose"       , &mVerbose       , false, &MD_VERBOSE);
        }

        int Sync::Run()
        {
            Run_Bidirectional();
            Run_Unidirectional();

            return 0;
        }

        // ===== DI::Dictionary =============================================

        Sync::~Sync() {}

        void Sync::Validate() const
        {
            DI::Dictionary::Validate();

            for (const auto& lVT : mBidirectional.mInternal)
            {
                assert(nullptr != lVT.second);

                auto lGroup = dynamic_cast<const DI::Array*>(lVT.second.Get());
                assert(nullptr != lGroup);

                KMS_EXCEPTION_ASSERT(1 < lGroup->GetCount(), RESULT_INVALID_CONFIG, "Number of folders cannot be one", lVT.first.c_str());
            }
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
                KMS_EXCEPTION_ASSERT(2 == lRet, RESULT_INVALID_CONFIG, "Invalid unidirectional entry", lRet);

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


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Sync.cpp

// CODE COVERAGE 2022-05-28 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Config/Configurator.h>
#include <KMS/File/FileInfoList.h>

#include <KMS/File/Sync.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Sync.cfg")

// Static fonction desclarations
// //////////////////////////////////////////////////////////////////////////

static void ClearFolderList(KMS::File::Sync::FolderList* aFolders);

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
            assert(NULL != aVector);
            assert(NULL != aVector[0]);

            int lResult = __LINE__;

            try
            {
                KMS::Config::Configurator lC;
                KMS::File::Sync           lS;

                lS.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::HOME), CONFIG_FILE, false);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT), CONFIG_FILE, false);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lResult = lS.Run();
            }
            KMS_CATCH_RESULT(lResult);

            return lResult;
        }

        Sync::Sync() {}

        Sync::~Sync()
        {
            ClearFolderList(&mDestinations);
            ClearFolderList(&mFolders);
            ClearFolderList(&mSources);
        }

        void Sync::AddDestination(const char* aD) { mDestinations.push_back(ToFileInfoList(aD)); }
        void Sync::AddFolder     (const char* aF) { mFolders     .push_back(ToFileInfoList(aF)); }
        void Sync::AddSource     (const char* aS) { mSources     .push_back(ToFileInfoList(aS)); }

        void Sync::ClearDestinations() { ClearFolderList(&mDestinations); }
        void Sync::ClearFolders     () { ClearFolderList(&mFolders); }
        void Sync::ClearSources     () { ClearFolderList(&mSources); }

        void Sync::SetDestination(const char* aD) { ClearFolderList(&mDestinations); AddDestination(aD); }
        void Sync::SetSource     (const char* aS) { ClearFolderList(&mSources     ); AddSource     (aS); }

        int Sync::Run()
        {
            VerifyConfig();

            Run_Bidirectional();
            Run_Unidirectional();

            return 0;
        }

        // ===== Config::Configurable =======================================

        bool Sync::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            char lE[1024];

            if (0 == strcmp(aA, "Destinations")) { Expand(aV, lE, sizeof(lE)); AddDestination(lE); return true; }
            if (0 == strcmp(aA, "Folders"     )) { Expand(aV, lE, sizeof(lE)); AddFolder     (lE); return true; }
            if (0 == strcmp(aA, "Sources"     )) { Expand(aV, lE, sizeof(lE)); AddSource     (lE); return true; }

            return Configurable::AddAttribute(aA, aV);
        }

        bool Sync::SetAttribute(const char* aA)
        {
            assert(NULL != aA);

            if (0 == strcmp(aA, "Destinations")) { ClearDestinations(); return true; }
            if (0 == strcmp(aA, "Folders"     )) { ClearFolders     (); return true; }
            if (0 == strcmp(aA, "Sources"     )) { ClearSources     (); return true; }

            return Configurable::SetAttribute(aA);
        }

        bool Sync::SetAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            char lE[1024];

            if (0 == strcmp(aA, "Destination")) { Expand(aV, lE, sizeof(lE)); SetDestination(lE); return true; }
            if (0 == strcmp(aA, "Source"     )) { Expand(aV, lE, sizeof(lE)); SetSource     (lE); return true; }

            return Configurable::SetAttribute(aA, aV);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Sync::Run_Bidirectional()
        {
            for (FileInfoList* lA : mFolders)
            {
                for (FileInfoList* lB : mFolders)
                {
                    if (lA != lB)
                    {
                        Run_Bidirectional(lA, lB);
                    }
                }
            }
        }

        void Sync::Run_Bidirectional(FileInfoList* aA, FileInfoList* aB)
        {
            assert(NULL != aA);
            assert(NULL != aB);

            unsigned int lFlags = FileInfoList::FLAG_IF_DOES_NOT_EXIST;

            lFlags |= Folder::FLAG_VERBOSE;

            aA->Copy(aB, lFlags);
            aB->Copy(aA, lFlags);
        }

        void Sync::Run_Unidirectional()
        {
            FolderList::iterator lD = mDestinations.begin();
            FolderList::iterator lS = mSources     .begin();

            while (mDestinations.end() != lD)
            {
                assert(mSources.end() != lS);

                assert(NULL != *lD);
                assert(NULL != *lS);

                unsigned int lFlags = FileInfoList::FLAG_IF_DOES_NOT_EXIST | FileInfoList::FLAG_IF_NEWER;

                lFlags |= Folder::FLAG_BACKUP | Folder::FLAG_IGNORE_ERROR | Folder::FLAG_VERBOSE;

                (*lS)->Copy(*lD, lFlags);

                lD++;
                lS++;
            }
        }

        void Sync::VerifyConfig() const
        {
            if (mDestinations.empty() && mFolders.empty())
            {
                KMS_EXCEPTION(CONFIG, "No folders");
            }

            if (mDestinations.size() != mSources.size())
            {
                KMS_EXCEPTION(CONFIG, "Number of sources does not match the number of destinations");
            }

            if (1 == mFolders.size())
            {
                KMS_EXCEPTION(CONFIG, "Number of folders cannot be one");
            }
        }

    }
}

// Static fonctions
// //////////////////////////////////////////////////////////////////////////

void ClearFolderList(KMS::File::Sync::FolderList* aFolders)
{
    assert(NULL != aFolders);

    for (KMS::File::FileInfoList* lF : *aFolders)
    {
        assert(NULL != lF);

        delete lF;
    }

    aFolders->clear();
}

KMS::File::FileInfoList* ToFileInfoList(const char* aFolder)
{
    KMS::File::FileInfoList* lResult = new KMS::File::FileInfoList(KMS::File::Folder(aFolder), "*", true);
    assert(NULL != lResult);

    return lResult;
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/File_Sync.cpp

// CODE COVERAGE 2022-05-28 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Environment.h>
#include <KMS/File/FileInfoList.h>

#include <KMS/File/Sync.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-Sync.cfg")

#define DEFAULT_GROUP ("Default")

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
                KMS::Cfg::Configurator lC;
                KMS::File::Sync        lS;

                lS.InitConfigurator(&lC);

                Dbg::gLog.InitConfigurator(&lC);

                lC.Init();
                lC.ParseFile(File::Folder(File::Folder::Id::EXECUTABLE), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::HOME      ), CONFIG_FILE);
                lC.ParseFile(File::Folder(File::Folder::Id::CURRENT   ), CONFIG_FILE);
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
            ClearFolderList(&mSources);

            for (GroupMap::value_type lP : mGroups)
            {
                ClearFolderList(lP.second);
                delete lP.second;
            }
        }

        void Sync::AddDestination(const char* aD) { mDestinations.push_back(ToFileInfoList(aD)); }
        void Sync::AddSource     (const char* aS) { mSources     .push_back(ToFileInfoList(aS)); }

        void Sync::AddFolder(const char* aG, const char* aF)
        {
            assert(NULL != aG);

            FolderList* lG;

            GroupMap::iterator lIt = mGroups.find(aG);
            if (mGroups.end() == lIt)
            {
                lG = new FolderList;

                mGroups.insert(GroupMap::value_type(aG, lG));
            }
            else
            {
                lG = lIt->second;
            }
            assert(NULL != lG);

            lG->push_back(ToFileInfoList(aF));
        }

        void Sync::ClearDestinations() { ClearFolderList(&mDestinations); }
        void Sync::ClearSources     () { ClearFolderList(&mSources); }

        void Sync::ClearFolders(const char* aG)
        {
            assert(NULL != aG);

            GroupMap::iterator lIt = mGroups.find(aG);
            if (mGroups.end() != lIt)
            {
                assert(NULL != lIt->second);

                ClearFolderList(lIt->second);
            }
        }

        void Sync::SetDestination(const char* aD) { ClearFolderList(&mDestinations); AddDestination(aD); }
        void Sync::SetSource     (const char* aS) { ClearFolderList(&mSources     ); AddSource     (aS); }

        int Sync::Run()
        {
            VerifyConfig();

            Run_Bidirectional();
            Run_Unidirectional();

            return 0;
        }

        // ===== Cfg::Configurable ==========================================

        bool Sync::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            char lE[LINE_LENGTH];

            CFG_EXPAND("Destinations", AddDestination);
            CFG_EXPAND("Sources"     , AddSource     );

            CFG_IF("Folders") { Env::Expand(aV, lE, sizeof(lE)), AddFolder(DEFAULT_GROUP, lE); return true; }

            return Configurable::AddAttribute(aA, aV);
        }

        bool Sync::AddAttribute_Indexed(const char* aA, const char* aI, const char* aV)
        {
            assert(NULL != aA);

            char lE[LINE_LENGTH];

            CFG_IF("Folders") { Env::Expand(aV, lE, sizeof(lE)); AddFolder(aI, lE); return true; }

            return Configurable::AddAttribute_Indexed(aA, aI, aV);
        }

        bool Sync::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Destinations") { ClearDestinations(); return true; }
                CFG_IF("Sources"     ) { ClearSources     (); return true; }

                CFG_IF("Folders") { ClearFolders(DEFAULT_GROUP); return true; }
            }
            else
            {
                char lE[LINE_LENGTH];

                CFG_EXPAND("Destination", SetDestination);
                CFG_EXPAND("Source"     , SetSource     );
            }

            return Configurable::SetAttribute(aA, aV);
        }

        bool Sync::SetAttribute_Indexed(const char* aA, const char* aI, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Folders") { ClearFolders(aI); return true; }
            }
            else
            {
                CFG_IF("Folders") { ClearFolders(aI); AddFolder(aI, aV); return true; }
            }

            return Configurable::SetAttribute_Indexed(aA, aI, aV);
        }

        void Sync::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "===== KMS::File::Sync =====\n"
                "Destination = {Path}\n"
                "    Set a destination\n"
                "    A matching source must be set\n"
                "Destinations\n"
                "    Clear all destinations\n"
                "Destinations += {Path}\n"
                "    Add a destination\n"
                "    A matching source must be added\n"
                "Folders\n"
                "    Clear folders in the Default group\n"
                "Folders += {Path}\n"
                "    Add a folder in the Default group\n"
                "Folders[{Group}]\n"
                "    Clear folders in the specified group\n"
                "Folders[{Group}] = {Path}\n"
                "    Set a folder in the specified group\n"
                "Folders[{Group}] += {Path}\n"
                "    Add a folder in the specified group\n"
                "Source = {Path}\n"
                "    Set a source\n"
                "    A matching destination must be set\n"
                "Sources\n"
                "    Clear all sources\n"
                "Sources += {Path}\n"
                "    Add a source\n"
                "    A matching destination must be added\n");

            Configurable::DisplayHelp(aOut);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Sync::Run_Bidirectional()
        {
            for (GroupMap::value_type lP : mGroups)
            {
                assert(NULL != lP.second);

                for (FileInfoList* lA : *lP.second)
                {
                    for (FileInfoList* lB : *lP.second)
                    {
                        if (lA != lB)
                        {
                            Run_Bidirectional(lA, lB);
                        }
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
            if (mDestinations.size() != mSources.size())
            {
                KMS_EXCEPTION(CONFIG, "Number of sources does not match the number of destinations");
            }

            for (GroupMap::value_type lP : mGroups)
            {
                if (1 == lP.second->size())
                {
                    KMS_EXCEPTION(CONFIG, "Number of folders cannot be one");
                }
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

    return lResult;
}

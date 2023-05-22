
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Stats.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <fstream>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Dbg/Stats_Entry.h>
#include <KMS/OS.h>

#include <KMS/Dbg/Stats.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DEFAULT_STATS_CONSOLE     (false)
#define DEFAULT_STATS_FOLDER_NAME ("KMS-Framework")

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_STATS_CONSOLE("Stats_Console = false | true");
static const KMS::Cfg::MetaData MD_STATS_FOLDER ("Stats_Folder  = {Path}");

namespace KMS
{
    namespace Dbg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Stats::Stats()
            : mConsole(DEFAULT_STATS_CONSOLE)
            , mFolder (File::Folder(File::Folder::HOME, DEFAULT_STATS_FOLDER_NAME))
        {
            DI::Dictionary::AddEntry("Stats_Console", &mConsole, false, &MD_STATS_CONSOLE);
            DI::Dictionary::AddEntry("Stats_Folder" , &mFolder , false, &MD_STATS_FOLDER );
        }

        Stats::~Stats()
        {
            if (mConsole)
            {
                std::cout << "STATISTICS\n";

                for (Stats_Entry* lEntry : mEntries)
                {
                    assert(NULL != lEntry);

                    std::cout << *lEntry;
                }
            }

            if (mFolder.Get().DoesExist())
            {
                char lFileName[NAME_LENGTH];

                sprintf_s(lFileName, "Stats_%u.txt", OS::GetProcessId());

                char lPath[PATH_LENGTH];

                mFolder.Get().GetPath(lFileName, lPath, sizeof(lPath));

                std::ofstream lStream(lPath);
                if (lStream.is_open())
                {
                    for (auto lEntry : mEntries)
                    {
                        assert(NULL != lEntry);

                        lStream << *lEntry;
                    }
                }
            }

            for (auto lEntry : mEntries)
            {
                assert(NULL != lEntry);

                delete lEntry;
            }
        }

        void Stats::AddEntry(Dbg::Stats_Entry* aEntry) { mEntries.push_back(aEntry); }

        Stats Dbg::gStats;

    }
}

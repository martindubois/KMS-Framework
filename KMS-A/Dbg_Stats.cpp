
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Stats.cpp

// TEST COVERAGE 2023-08-06 Martin Dubois

#include "Component.h"

// ===== C++ ================================================================
#include <fstream>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Dbg/Stats_Entry.h>
#include <KMS/OS.h>

#include <KMS/Dbg/Stats.h>

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

        const bool Stats::CONSOLE_DEFAULT = false;

        Stats::Stats()
            : mConsole(CONSOLE_DEFAULT)
            , mFolder (File::Folder(File::Folder::HOME, "KMS-Framework"))
        {
            Ptr_OF<DI::Object> lEntry;

            lEntry.Set(&mConsole, false); DI::Dictionary::AddEntry("Stats_Console", lEntry, &MD_STATS_CONSOLE);
            lEntry.Set(&mFolder , false); DI::Dictionary::AddEntry("Stats_Folder" , lEntry, &MD_STATS_FOLDER );
        }

        Stats::~Stats()
        {
            if (mConsole)
            {
                std::cout << "STATISTICS\n";

                for (Stats_Entry* lEntry : mEntries)
                {
                    assert(nullptr != lEntry);

                    std::cout << *lEntry;
                }
            }

            if (mFolder.GetFolder().DoesExist())
            {
                char lFileName[NAME_LENGTH];

                sprintf_s(lFileName, "Stats_%u.txt", OS::GetProcessId());

                char lPath[PATH_LENGTH];

                mFolder.GetFolder().GetPath(lFileName, lPath, sizeof(lPath));

                std::ofstream lStream(lPath);
                if (lStream.is_open())
                {
                    for (auto lEntry : mEntries)
                    {
                        assert(nullptr != lEntry);

                        lStream << *lEntry;
                    }
                }
            }

            for (auto lEntry : mEntries)
            {
                assert(nullptr != lEntry);

                delete lEntry;
            }
        }

        void Stats::AddEntry(Stats_Entry* aEntry)
        {
            mEntries.push_back(aEntry);
        }

        Stats gStats;

    }
}

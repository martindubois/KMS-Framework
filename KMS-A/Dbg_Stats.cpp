
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Dbg_Stats.cpp

// TEST COVERAGE  2023-08-06  KMS - Martin Dubois, P. Eng.

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

        #if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
            const char* Stats::FOLDER_DEFAULT = "{$HOME}/KMS-Framework";
        #endif
        #ifdef _KMS_WINDOWS_
            const char* Stats::FOLDER_DEFAULT = "{$USERPROFILE}\\KMS-Framework";
        #endif

        Stats::Stats()
            : mConsole(CONSOLE_DEFAULT)
            , mFolder (File::Folder(File::Folder::HOME, "KMS-Framework"))
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

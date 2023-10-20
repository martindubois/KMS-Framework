
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Stats.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <list>
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Folder.h>
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace Dbg
    {
        class Stats_Entry;

        class Stats final : public DI::Dictionary
        {

        public:

            static const bool  CONSOLE_DEFAULT;
            static const char* FOLDER_DEFAULT;

            DI::Boolean mConsole;
            DI::Folder  mFolder;

            Stats();

            ~Stats();

            void Reset();

        // Internal

            void AddEntry(Stats_Entry* aE);

            void Display(std::ostream& aOut) const;

        private:

            NO_COPY(Stats);

            typedef std::list<Stats_Entry*> EntryList;

            EntryList mEntries;

        };

        extern Stats gStats;

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Dbg::Stats& aIn);

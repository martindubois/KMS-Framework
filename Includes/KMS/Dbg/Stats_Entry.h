
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Stats_Entry.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <ostream>

namespace KMS
{
    namespace Dbg
    {

        class Stats_Entry
        {

        public:

            virtual ~Stats_Entry();

            const char* GetName() const;

            virtual void Reset() = 0;

        // Internal

            virtual void Display(std::ostream& aOut) const = 0;

        protected:

            // aName      Entry's name
            // aRegister  false  Does not register the instance into the
            //                   global statistics list
            //            true   Register the instance into the global
            //                   statistics list
            Stats_Entry(const char* aName, bool aRegister = true);

        private:

            const char* mName;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Dbg::Stats_Entry& aIn);

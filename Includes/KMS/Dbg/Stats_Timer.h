
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Dbg/Stats_Timer.h
// Status    PROD_REDAY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <ostream>

// ===== Includes ===========================================================
#include <KMS/Dbg/Stats_Value.h>

namespace KMS
{
    namespace Dbg
    {

        class Stats_Timer : public Stats_Value<double>
        {

        public:

            Stats_Timer(const char* aName);

            void Start();
            void Stop();

        // Internal

            // ===== Stats_Entry ============================================
            virtual void Display(std::ostream& aOut) const;

        private:

            double mStart_s;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Dbg::Stats_Timer& aIn);

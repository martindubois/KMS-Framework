
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/GUI/MetaData.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

namespace KMS
{
    namespace GUI
    {

        class MetaData : public Cfg::MetaData
        {

        public:

            MetaData(const char* aHelp, const char* aLabel);

            const char* GetLabel() const;

            // ===== DI::MetaData ===========================================
            virtual ~MetaData();

        private:

            const char* mLabel;

        };

    }
}

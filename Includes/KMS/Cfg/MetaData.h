
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Cfg/MetaData.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>

namespace KMS
{
    namespace Cfg
    {

        class MetaData : public DI::MetaData
        {

        public:

            MetaData(const char* aHelp);

            const char* GetHelp() const;

            // ===== DI::MetaData ===========================================
            virtual ~MetaData();

        private:

            const char* mHelp;

        };

    }
}

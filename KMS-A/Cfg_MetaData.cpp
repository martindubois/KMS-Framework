
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Cfg_MetaData.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

namespace KMS
{
    namespace Cfg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        MetaData::MetaData(const char* aHelp) : mHelp(aHelp) {}

        const char* MetaData::GetHelp() const { return mHelp; }

        // ===== DI::MetaData ===============================================
        MetaData::~MetaData() {}

    }
}

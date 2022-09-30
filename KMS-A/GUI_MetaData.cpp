
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/GUI_MetaData.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/GUI/MetaData.h>

namespace KMS
{
    namespace GUI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        MetaData::MetaData(const char* aHelp, const char* aLabel) : Cfg::MetaData(aHelp), mLabel(aLabel) {}

        const char* MetaData::GetLabel() const { return mLabel; }

        // ===== DI::MetaData ===============================================
        MetaData::~MetaData() {}

    }
}

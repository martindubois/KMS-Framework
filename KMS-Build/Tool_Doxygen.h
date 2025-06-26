
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Doxygen.h

// Generate documentation - Only executed if the configuration file exist
// VERIFY        Verify Doxygen is installed (See)
// GENERATE_DOC  Generate DOC

#pragma once

// ===== Local ==============================================================
class ToolList;

namespace Tool_Doxygen
{

    extern void CreateTools(ToolList* aTools, const Config& aCfg);

};

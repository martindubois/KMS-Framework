
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Test.h

// Test executable
// TEST  Verify the file exit
//       Execute the test

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class Config;
class ToolList;

namespace Comp_Test
{

    extern void CreateTools(ToolList* aTools, const Config& aCfg, const KMS::DI::Array& aTests, const KMS::DI::Array& aConfigurations, const KMS::DI::Array& aProcessors);

    extern void CreateTool_OSDep(ToolList* aTools, const Config& aCfg, const char* aTest, const char* aConfiguration, const char* aProcessors);

};

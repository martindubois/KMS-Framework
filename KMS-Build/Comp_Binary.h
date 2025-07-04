
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Binary.h

// Binary executable
// TEST     Verify the file exist (See Comp_File_ToPackage)
// PACKAGE  Copy file to the temporary folder (See Comp_File_ToPackage)

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class CompList;
class Config;

namespace Comp_Binary
{

    extern void CreateComponents(CompList* aComps, const Config& aCfg, const KMS::DI::Array& aBinaries, const KMS::DI::Array& aConfigurations, const KMS::DI::Array& aProcessors);

    extern void CreateComponents_OSDep(CompList* aComps, const Config& aCfg, const char* aBinary, const char* aConfiguration, const char* aProcessor);

}

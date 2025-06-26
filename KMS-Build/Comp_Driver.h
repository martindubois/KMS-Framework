
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Driver.h

// Binary driver
// PACKAGE  Create the .CAB file
// SIGN     Sign the .CAB file

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class CompList;
class Config;
class ToolList;

namespace Comp_Driver
{

    extern void CreateComponentsAndTools(CompList* aComps, ToolList* aTools, const Config& aCfg, const KMS::DI::Array& aDrivers, const KMS::DI::Array& aConfigurations, const KMS::DI::Array& aProcessors);

    extern void CreateComponentsAndTools_OSDep(CompList* aComps, ToolList* aTools, const Config& aCfg, const char* aDriver, const KMS::DI::Array& aConfigurations, const KMS::DI::Array& aProcessors);

};

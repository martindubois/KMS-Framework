
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Installer_W.h

// Software installer
// VERIFY   Verify Inno Setup is installed
// PACKAGE  Create the intaller
// EXPORT   Verify the installer exist
//          Copy the installer to the product folder

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class CompList;
class Config;
class ToolList;

namespace Comp_Installer
{

    extern void CreateComponentsAndTools(CompList* aComps, ToolList* aTools, const Config& aCfg, const KMS::DI::Array& aProcessors);

};

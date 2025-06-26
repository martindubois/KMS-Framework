
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Folder_ToPackage.h

// Folder to copy to the temporary folder to prepare the archive (.zip)
// VERIFY   Verify the folder exist
// PACKAGE  Copy the folder to the temporary folder

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class CompList;
class Config;

namespace Comp_Folder_ToPackage
{

    extern void CreateComponent(CompList* aComps, const Config& aCfg, const char* aFolder);

    extern void CreateComponents(CompList* aComps, const Config& aCfg, const KMS::DI::Array& aFolders);

}

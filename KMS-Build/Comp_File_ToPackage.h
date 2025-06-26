
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_File_ToPackage.h

// File to copy to the temporary folder to prepare the archive (.zip)
// aVerify  Verify the file exist
// PACKAGE  Copy the file to the temporary folder

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/File/Folder.h>

// ===== Local ==============================================================
class CompList;
class Config;
enum class Phase;

namespace Comp_File_ToPackage
{

    // aSrc  This function deletes it
    extern void CreateComponent(CompList* aComps, const Config& aCfg, KMS::File::Folder* aSrc, const char* aDst, const char* aFileName, Phase aVerify);

    extern void CreateComponents(CompList* aComps, const Config& aCfg, const KMS::DI::Array& aFiles);

}

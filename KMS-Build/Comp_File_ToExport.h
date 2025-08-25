
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_File_ToExport.h

// File to export to the product folder
// EXPORT  Verify the file exist
//         Copy the file to the product folder

#pragma once

// ===== Local ==============================================================
class CompList;
class Config;

namespace Comp_File_ToExport
{

    // aFolder  The function deletes it
    extern void CreateComponent(CompList* aComps, const Config& aCfg, KMS::File::Folder* aFolder, const char* aFileName_Src, const char* aFileName_Dst = nullptr);

};

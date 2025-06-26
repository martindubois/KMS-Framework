
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Archive.h

// Archive file (.zip) exported to the product folder.
// VERIFY   Verify the file to export does not already exist in the product
//          folder
// PACKAGE  Copy the files to package into a temporary folder
// EXPORT   Compress the temporary folder into the exported archive

#pragma once

// ===== Local ==============================================================
class CompList;
class Config;
class ToolList;

namespace Comp_Archive
{

    extern void GetBinaryFolder (char* aOut, unsigned int aOutSize_byte, const char* aConfiguration, const char* aProcessor);
    extern void GetLibraryFolder(char* aOut, unsigned int aOutSize_byte, const char* aConfiguration, const char* aProcessor);

    extern void CreateComponentAndTool(CompList* aComps, ToolList* aTools, const Config& aCfg);

}

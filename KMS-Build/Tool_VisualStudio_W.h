
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tools_VisualStudio.h

// COMPILE  Compile using Visual Studio (See Tool_Executable)
//          If not aCfg.GetDoNotCompile()

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class Config;
class ToolList;

namespace Tool_VisualStudio
{

    extern const char* BINARY_OUTPUT_EXTENSIONS         [2];
    extern const char* DRIVER_OUTPUT_EXTENSIONS         [4];
    extern const char* LIBRARY_DYNAMIC_OUTPUT_EXTENSIONS[1];
    extern const char* LIBRARY_STATIC_OUTPUT_EXTENSIONS [2];

    extern KMS::File::Folder* GetDriverOutDir(const char* aConfiguration, const char* aProcessor, const char* aDriver);

    extern KMS::File::Folder* GetOutDir(const char* aConfiguration, const char* aProcessor);

    extern void CreateTools(ToolList* aTools, const Config& aCfg, const KMS::DI::Array& aConfigurations, const KMS::DI::Array& aProcessors);

};

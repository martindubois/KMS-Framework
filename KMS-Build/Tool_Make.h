
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Make.h

// COMPILE  Compile using KMS::Build::Make
//          If not aCfg.GetDoNotCompile()

#pragma once

// ===== Includes ===========================================================
#include <KMS/Build/Make.h>
#include <KMS/Cfg/Configurator.h>
#include <KMS/Dbg/Log_Cfg.h>
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class Config;
class ToolList;

namespace Tool_Make
{

    extern const char* BINARY_OUTPUT_EXTENSIONS         [1];
    extern const char* LIBRARY_DYNAMIC_OUTPUT_EXTENSIONS[1];
    extern const char* LIBRARY_STATIC_OUTPUT_EXTENSIONS [1];

    extern const char* EMBEDDED_BINARY_OUTPUT_EXTENSIONS[1];

    extern KMS::File::Folder* GetOutDir(const char* aConfiguration, const char* aProcessor);

    extern void CreateTools(ToolList* aTools, const Config& aCfg, const KMS::DI::Array& aConfiguration, const KMS::DI::Array& aProcessors);

}

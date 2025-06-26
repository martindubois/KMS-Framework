
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Export.h

// EXPORT  Export files to the product folder
//         If not aCfg.GetDoNotExport

#pragma once

// ===== Local ==============================================================
class CompList;
class Config;
class ToolList;

namespace Tool_Export
{

    extern void CreateTool(ToolList* aTools, const Config& aCfg, CompList* aComps);

}

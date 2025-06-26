
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Editor.h

// Edit a text file
// EDIT  Edit the text file

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
class Config;
enum class Phase;
class ToolList;

namespace Tool_Editor
{

    extern void CreateTools(ToolList* aTools, const Config& aCfg, const KMS::DI::Array& aEditOperations);

}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_Command.h

// Execute a command
// aPhase  Execute the command

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>

// ===== Local ==============================================================
enum class Phase;
class ToolList;

namespace Tool_Command
{

    extern void CreateTools(ToolList* aTools, const KMS::DI::Array& aCommands, Phase aPhase);

    extern void CreateTool(ToolList* aTools, const char* aCommand, Phase aPhase);

};

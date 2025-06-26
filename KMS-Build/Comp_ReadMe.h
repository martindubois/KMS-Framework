
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_ReadMe.h

// Read me file - Add the component only if the file exist
// PACKAGE  Copy the file to the temporary folder

#pragma once

// ===== Local ==============================================================
class CompList;
class Config;

namespace Comp_ReadMe
{

    extern void CreateComponent(CompList* aComps, const Config& aCfg, const char* aComponent);

}

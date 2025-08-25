
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Build_L.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/Proc/Process.h>

// ===== Local ==============================================================
#include "Config.h"
#include "Tool_Make.h"

#include "Build.h"

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_OS_PACKAGES("LinuxPackages += {Name}");

// Public
// //////////////////////////////////////////////////////////////////////////

const char* ::Build::EXPORT_FOLDER_DEFAULT = "{$HOME}/Export";

// Private
// //////////////////////////////////////////////////////////////////////////

void ::Build::Construct_OSDep()
{
    mPackages.SetCreator(DI::String_Expand::Create);

    Ptr_OF<DI::Object> lEntry;

    lEntry.Set(&mPackages, false); AddEntry("LinuxPackages", lEntry, &MD_OS_PACKAGES);

    mExportFolder = File::Folder(File::Folder::Id::HOME, "Export");
}

void ::Build::CreateLists_OSDep()
{
    auto lCfg = CreateConfig();

    Tool_Make::CreateTools(&mTools, *lCfg, mConfigurations, mProcessors);

    delete lCfg;
}

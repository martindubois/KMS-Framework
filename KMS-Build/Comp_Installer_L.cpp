
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Installer_W.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Comp_File_ToExport.h"
#include "Config.h"
#include "Phase.h"
#include "Tool_Command.h"

#include "Comp_Installer.h"

using namespace KMS;

#define FILE_EXT_DEB ".deb"
#define FILE_EXT_SH  ".sh"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define PACKAGES_FOLDER  ("Packages")

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CreateComponents(CompList* aComps, const Config& aCfg, const char* aProcessor);

static void CreateTool(ToolList* aTools, const Config& aCfg, const char* aFileName);

static void DEB_FileName(const char* aPackage, const KMS::Version& aVersion, char* aOut, unsigned int aOutSize_byte);

static bool SH_FileName(const char* aProcessor, char* aOut, unsigned int aOutSize_byte);

namespace Comp_Installer
{
// Functions
// //////////////////////////////////////////////////////////////////////////

    void CreateComponentAndTool_OSDep(CompList* aComps, ToolList* aTools, const Config& aCfg, const char* aProcessor)
    {
        char lFileName[PATH_LENGTH];

        if (SH_FileName(aProcessor, lFileName, sizeof(lFileName)))
        {
            CreateTool(aTools, aCfg, lFileName);

            CreateComponents(aComps, aCfg, aProcessor);
        }
    }

}

// Static functions
// /////////////////////////////////////////////////////////////////////////

void CreateComponents(CompList* aComps, const Config& aCfg, const char* aProcessor)
{
    auto lPackages = new File::Folder(File::Folder::Id::CURRENT, PACKAGES_FOLDER);

    for (auto lPE : aCfg.GetPackages()->mInternal)
    {
        auto lP = dynamic_cast<const DI::String*>(lPE.Get());
        assert(nullptr != lP);

        char lFileName[PATH_LENGTH];

        DEB_FileName(lP->Get(), aCfg.GetVersion(), lFileName, sizeof(lFileName));


        Comp_File_ToExport::CreateComponent(aComps, aCfg, lPackages, lFileName);
    }
}

void CreateTool(ToolList* aTools, const Config& aCfg, const char* aFileName)
{
    if (!aCfg.GetDoNotPackage())
    {
        Tool_Command::CreateTool(aTools, aFileName, Phase::PACKAGE);
    }
}

void DEB_FileName(const char* aPackage, const KMS::Version& aVersion, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aPackage);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    auto lMa = aVersion.GetMajor();
    auto lMi = aVersion.GetMinor();
    auto lBu = aVersion.GetBuild();
    
    sprintf(aOut, "%s_%u.%u-%u" FILE_EXT_DEB, aPackage, lMa, lMi, lBu);
}

bool SH_FileName(const char* aProcessor, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aProcessor);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    sprintf(aOut, "./Product_%s" FILE_EXT_SH, aProcessor);

    return File::Folder::CURRENT.DoesFileExist(aOut);
}

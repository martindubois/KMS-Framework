
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
#include "Tool_Executable.h"

#include "Comp_Installer_W.h"

using namespace KMS;

#define FILE_EXT_EXE ".exe"
#define FILE_EXT_ISS ".iss"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define ALLOWED_TIME_ms (1000 * 60 * 10) // 10 minutes

#define EXECUTABLE ("Compil32")
#define FOLDER     ("Inno Setup 6")

#define INSTALLER_FOLDER  ("Installer")

// Static function declaration
// /////////////////////////////////////////////////////////////////////////

static void CreateComponent(CompList* aComps, const Config& aCfg, const char* aProcessor);

static void CreateTool(ToolList* aTools, const Config& aCfg, const char* aFileName);

static bool ISS_FileName(const char* aProcessor, char* aOut, unsigned int aOutSize_byte);

namespace Comp_Installer
{
// Functions
// //////////////////////////////////////////////////////////////////////////

    void CreateComponentsAndTools(CompList* aComps, ToolList* aTools, const Config& aCfg, const DI::Array& aProcessors)
    {
        for (const auto& lPE : aProcessors.mInternal)
        {
            auto lP = dynamic_cast<const DI::String*>(lPE.Get());
            assert(nullptr != lP);

            char lFileName[PATH_LENGTH];

            if (ISS_FileName(lP->Get(), lFileName, sizeof(lFileName)))
            {
                CreateTool(aTools, aCfg, lFileName);

                CreateComponent(aComps, aCfg, lP->Get());
            }
        }
    }

}

// Static functions
// /////////////////////////////////////////////////////////////////////////

void CreateComponent(CompList* aComps, const Config& aCfg, const char* aProcessor)
{
    assert(nullptr != aProcessor);

    auto lVersion = aCfg.GetVersion();

    auto lMa   = lVersion.GetMajor();
    auto lMi   = lVersion.GetMinor();
    auto lBu   = lVersion.GetBuild();
    auto lType = lVersion.GetType ();

    char lFileName[PATH_LENGTH];

    if (0 == strlen(lType))
    {
        sprintf_s(lFileName, "%s_%u.%u.%u_%s" FILE_EXT_EXE, aCfg.GetProduct(), lMa, lMi, lBu, aProcessor);
    }
    else
    {
        sprintf_s(lFileName, "%s_%u.%u.%u-%s_%s" FILE_EXT_EXE, aCfg.GetProduct(), lMa, lMi, lBu, lType, aProcessor);
    }

    auto lInstaller = new File::Folder(File::Folder::Id::CURRENT, INSTALLER_FOLDER);

    Comp_File_ToExport::CreateComponent(aComps, aCfg, lInstaller, lFileName);
}

void CreateTool(ToolList* aTools, const Config& aCfg, const char* aFileName)
{
    if (!aCfg.GetDoNotPackage())
    {
        auto lFolder = new File::Folder(File::Folder::PROGRAM_FILES_X86, FOLDER);

        auto lTool = Tool_Executable::CreateTool(aTools, aCfg, lFolder, EXECUTABLE, ALLOWED_TIME_ms, Phase::VERIFY, Phase::PACKAGE);

        lTool->AddArgument("/cc");
        lTool->AddArgument(aFileName);
    }
}

bool ISS_FileName(const char* aProcessor, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aProcessor);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    sprintf_s(aOut, aOutSize_byte, "Product_%s" FILE_EXT_ISS, aProcessor);

    return File::Folder::CURRENT.DoesFileExist(aOut);
}

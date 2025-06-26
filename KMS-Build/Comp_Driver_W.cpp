
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Driver_W.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Config.h"
#include "Phase.h"
#include "Tool_Executable.h"

#include "Comp_Driver.h"

using namespace KMS;

#define FILE_EXT_CAB ".cab"
#define FILE_EXT_DDF ".ddf"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define DISK1_FOLDER    ("disk1")
#define MAKECAB         ("makecab")
#define SIGNTOOL        ("signtool")

#define WDK_TOOL_FOLDER_2019 ("Windows Kits\\10\\bin\\10.0.19041.0\\x64")
#define WDK_TOOL_FOLDER_2022 ("Windows Kits\\10\\bin\\10.0.26100.0\\x64")

#define MAKECAB_ALLOWED_TIME_ms   (1000 * 60 *  5) //  5 minutes
#define SIGNTOOL_ALLOWED_TIME_ms  (1000 * 60 * 10) // 10 minutes

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void CAB_FileName(const char* aDriver, const char* aFolder, char* aOut, unsigned int aOutSize_byte);
static bool DDF_FileName(const char* aDriver,                      char* aOut, unsigned int aOutSize_byte);

namespace Comp_Driver
{
    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateComponentsAndTools_OSDep(CompList* aComps, ToolList* aTools, const Config& aCfg, const char* aDriver, const KMS::DI::Array& aConfigurations, const KMS::DI::Array& aProcessors)
    {
        char lDDF[PATH_LENGTH];

        if (DDF_FileName(aDriver, lDDF, sizeof(lDDF)))
        {
            Tool_Executable::Tool* lTool;

            if (!aCfg.GetDoNotPackage())
            {
                lTool = Tool_Executable::CreateTool(aTools, aCfg, new File::Folder(File::Folder::NONE), MAKECAB, MAKECAB_ALLOWED_TIME_ms, Phase::NONE, Phase::PACKAGE);

                lTool->AddArgument("-f");
                lTool->AddArgument(lDDF);
            }

            const char* lToolFolder = WDK_TOOL_FOLDER_2022;

            switch (aCfg.GetVisualStudioVersion())
            {
            case 2019: lToolFolder = WDK_TOOL_FOLDER_2019; break;
            case 2022: lToolFolder = WDK_TOOL_FOLDER_2022; break;

            default: assert(false);
            }

            char lCAB[PATH_LENGTH];

            CAB_FileName(aDriver, DISK1_FOLDER, lCAB, sizeof(lCAB));

            lTool = Tool_Executable::CreateTool(aTools, aCfg, new File::Folder(File::Folder::PROGRAM_FILES_X86, lToolFolder), SIGNTOOL, SIGNTOOL_ALLOWED_TIME_ms, Phase::VERIFY, Phase::SIGN);

            lTool->AddArgument("sign");
            lTool->AddArgument("/fd");
            lTool->AddArgument("sha256");
            lTool->AddArgument("/sha1");
            lTool->AddArgument(aCfg.GetCertificatSHA1());
            lTool->AddArgument(lCAB);
        }
    }

}

// Static funcitons
// //////////////////////////////////////////////////////////////////////////

void CAB_FileName(const char* aDriver, const char* aFolder, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aDriver);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    if (nullptr == aFolder)
    {
        sprintf_s(aOut, aOutSize_byte, "%s" FILE_EXT_CAB, aDriver);
    }
    else
    {
        sprintf_s(aOut, aOutSize_byte, "%s\\%s" FILE_EXT_CAB, aFolder, aDriver);
    }
}

bool DDF_FileName(const char* aDriver, char* aOut, unsigned int aOutSize_byte)
{
    assert(nullptr != aDriver);
    assert(nullptr != aOut);
    assert(0 < aOutSize_byte);

    sprintf_s(aOut, aOutSize_byte, "%s" FILE_EXT_DDF, aDriver);

    return File::Folder::CURRENT.DoesFileExist(aOut);
}

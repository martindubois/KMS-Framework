
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Tool_VisualStudio_W.cpp

#include "Component.h"

// ==== Includes ============================================================
#include <KMS/DI/String.h>

// ==== Local ===============================================================
#include "Config.h"
#include "Phase.h"
#include "Tool_Executable.h"

#include "Tool_VisualStudio_W.h"

using namespace KMS;

#define FILE_EXT_CAT ".cat"
#define FILE_EXT_DLL ".dll"
#define FILE_EXT_EXE ".exe"
#define FILE_EXT_INF ".inf"
#define FILE_EXT_LIB ".lib"
#define FILE_EXT_PDB ".pdb"
#define FILE_EXT_SLN ".sln"
#define FILE_EXT_SYS ".sys"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define ALLOWED_TIME_ms (1000 * 60 * 10) // 10 minutes

#define EXECUTABLE ("MSBuild")

#define FOLDER_2019 ("Microsoft Visual Studio\\2019\\Professional\\Msbuild\\Current\\Bin")
#define FOLDER_2022 ("Microsoft Visual Studio\\2022\\Professional\\Msbuild\\Current\\Bin")

static const char* SLN_FILE_NAME = "Solution" FILE_EXT_SLN;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static File::Folder* GetVisualStudioFolder(uint32_t aVersion);

static void CreateTool(ToolList* aTools, const Config& aCfg, const char* aConfiguration, const char* aProcessor);

namespace Tool_VisualStudio
{

    // Constants
    // //////////////////////////////////////////////////////////////////////

    const char* BINARY_OUTPUT_EXTENSIONS         [2] = { FILE_EXT_EXE, FILE_EXT_PDB };
    const char* DRIVER_OUTPUT_EXTENSIONS         [4] = { FILE_EXT_CAT, FILE_EXT_INF, FILE_EXT_PDB, FILE_EXT_SYS };
    const char* LIBRARY_DYNAMIC_OUTPUT_EXTENSIONS[1] = { FILE_EXT_DLL };
    const char* LIBRARY_STATIC_OUTPUT_EXTENSIONS [2] = { FILE_EXT_LIB, FILE_EXT_PDB };

    // Functions
    // //////////////////////////////////////////////////////////////////////

    KMS::File::Folder* GetDriverOutDir(const char* aConfiguration, const char* aProcessor, const char* aDriver)
    {
        auto lOutDir = GetOutDir(aConfiguration, aProcessor);

        auto lResult = new File::Folder(*lOutDir, aDriver);

        delete lOutDir;

        return lResult;
    }

    File::Folder* GetOutDir(const char* aConfiguration, const char* aProcessor)
    {
        char lPath[PATH_LENGTH];

        if (0 == strcmp("x86", aProcessor))
        {
            sprintf_s(lPath, "%s", aConfiguration);
        }
        else
        {
            sprintf_s(lPath, "%s\\%s", aProcessor, aConfiguration);
        }

        return new File::Folder(File::Folder::CURRENT, lPath);
    }

    void Tool_VisualStudio::CreateTools(ToolList* aTools, const Config& aCfg, const DI::Array& aConfigurations, const DI::Array& aProcessors)
    {
        for (const auto& lCE : aConfigurations.mInternal)
        {
            auto lC = dynamic_cast<const DI::String*>(lCE.Get());
            assert(nullptr != lC);

            for (const auto& lPE : aProcessors.mInternal)
            {
                auto lP = dynamic_cast<const DI::String*>(lPE.Get());
                assert(nullptr != lP);

                CreateTool(aTools, aCfg, lC->Get(), lP->Get());
            }
        }
    }

}

// Static functions
// //////////////////////////////////////////////////////////////////////////

File::Folder* GetVisualStudioFolder(uint32_t aVersion)
{
    File::Folder* lBin;

    switch (aVersion)
    {
    case 2019: lBin = new File::Folder(File::Folder::PROGRAM_FILES_X86, FOLDER_2019); break;
    case 2022: lBin = new File::Folder(File::Folder::PROGRAM_FILES    , FOLDER_2022); break;

    default: KMS_EXCEPTION(RESULT_INVALID_CONFIG, "Invalid Visual Studio version", aVersion);
    }

    return lBin;
}

void CreateTool(ToolList* aTools, const Config& aCfg, const char* aConfiguration, const char* aProcessor)
{
    auto lFolder = GetVisualStudioFolder(aCfg.GetVisualStudioVersion());

    if (!aCfg.GetDoNotCompile())
    {
        auto lTool = Tool_Executable::CreateTool(aTools, aCfg, lFolder, EXECUTABLE, ALLOWED_TIME_ms, Phase::VERIFY, Phase::COMPILE);

        lTool->AddArgument(SLN_FILE_NAME);

        if (aCfg.GetDoNotClean())
        {
            lTool->AddArgument("/target:build");
        }
        else
        {
            lTool->AddArgument("/target:rebuild");
        }

        lTool->AddArgument((std::string("/Property:Configuration=") + aConfiguration).c_str());
        lTool->AddArgument(("/property:Platform=" + std::string(aProcessor)).c_str());
    }
}

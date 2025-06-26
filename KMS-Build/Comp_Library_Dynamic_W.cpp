
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Library_Dynamic_W.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Comp_Archive.h"
#include "Comp_File_ToPackage.h"
#include "Phase.h"
#include "Tool_VisualStudio_W.h"

#include "Comp_Library_Dynamic.h"

using namespace KMS;

namespace Comp_Library_Dynamic
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void Comp_Library_Dynamic::CreateComponents_OSDep(CompList* aComps, const Config& aCfg, const char* aLibrary, const char* aConfiguration, const char* aProcessor)
    {
        assert(nullptr != aLibrary);

        char lDst[PATH_LENGTH];

        Comp_Archive::GetLibraryFolder(lDst, sizeof(lDst), aConfiguration, aProcessor);

        for (unsigned int i = 0; i < sizeof(Tool_VisualStudio::LIBRARY_DYNAMIC_OUTPUT_EXTENSIONS) / sizeof(Tool_VisualStudio::LIBRARY_DYNAMIC_OUTPUT_EXTENSIONS[0]); i++)
        {
            char lFileName[PATH_LENGTH];

            sprintf_s(lFileName, "%s%s", aLibrary, Tool_VisualStudio::LIBRARY_DYNAMIC_OUTPUT_EXTENSIONS[i]);

            auto lSrc = Tool_VisualStudio::GetOutDir(aConfiguration, aProcessor);
            Comp_File_ToPackage::CreateComponent(aComps, aCfg, lSrc, lDst, lFileName, Phase::TEST);
        }
    }

}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Library_L.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Comp_Archive.h"
#include "Comp_File_ToPackage.h"
#include "Phase.h"
#include "Tool_Make.h"

#include "Comp_Library_Static.h"

namespace Comp_Library_Static
{

    // Functions
    // //////////////////////////////////////////////////////////////////////

    void CreateComponents_OSDep(CompList* aComps, const Config& aCfg, const char* aLibrary, const char* aConfiguration, const char* aProcessor)
    {
        char lDst[PATH_LENGTH];

        Comp_Archive::GetLibraryFolder(lDst, sizeof(lDst), aConfiguration, aProcessor);

        unsigned int i = 0;
        while (nullptr != Tool_Make::LIBRARY_STATIC_OUTPUT_EXTENSIONS[i])
        {
            char lFileName[PATH_LENGTH];

            sprintf_s(lFileName, "%s%s", aLibrary, Tool_Make::LIBRARY_STATIC_OUTPUT_EXTENSIONS[i]);

            auto lSrc = Tool_Make::GetLibraryOutDir(aConfiguration, aProcessor);
            Comp_File_ToPackage::CreateComponent(aComps, aCfg, lSrc, lDst, lFileName, Phase::TEST);

            i++;
        }
}

}

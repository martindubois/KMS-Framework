
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_Binary_L.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Comp_Archive.h"
#include "Comp_File_ToPackage.h"
#include "Phase.h"
#include "Tool_Make.h"

#include "Comp_Binary.h"

namespace Comp_Binary
{

    // Function
    // //////////////////////////////////////////////////////////////////////

    void CreateComponents_OSDep(CompList* aComps, const Config& aCfg, const char* aBinary, const char* aConfiguration, const char* aProcessor)
    {
        assert(nullptr != aBinary);
        
        char lDst[PATH_LENGTH];

        Comp_Archive::GetBinaryFolder(lDst, sizeof(lDst), aConfiguration, aProcessor);

        unsigned int i = 0;
        while (nullptr != Tool_Make::BINARY_OUTPUT_EXTENSIONS[i])
        {
            char lFileName[PATH_LENGTH];

            sprintf_s(lFileName, "%s%s", aBinary, Tool_Make::BINARY_OUTPUT_EXTENSIONS[i]);

            auto lSrc = Tool_Make::GetBinaryOutDir(aConfiguration, aProcessor);
            Comp_File_ToPackage::CreateComponent(aComps, aCfg, lSrc, lDst, lFileName, Phase::TEST);

            i++;
        }
    }

}

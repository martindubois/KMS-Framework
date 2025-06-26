
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-Build/Comp_ReadMe.cpp

#include "Component.h"

// ==== Local ===============================================================
#include "Comp_File_ToPackage.h"
#include "Config.h"
#include "Phase.h"

#include "Comp_ReadMe.h"

using namespace KMS;

#define FILE_EXT_TXT ".txt"

namespace Comp_ReadMe
{

    // Function
    // //////////////////////////////////////////////////////////////////////

    void CreateComponent(CompList* aComps, const Config& aCfg, const char* aComponent)
    {
        assert(nullptr != aComponent);

        if (!aCfg.GetDoNotPackage())
        {
            char lFileName[PATH_LENGTH];
            char lFolder  [PATH_LENGTH];

            sprintf_s(lFileName, "%s.%s.ReadMe" FILE_EXT_TXT, aCfg.GetProduct(), aComponent);
            sprintf_s(lFolder  , "%s/_DocUser", aComponent);

            auto lSrc = new File::Folder(File::Folder::CURRENT, lFolder);
            if (lSrc->DoesFileExist(lFileName))
            {
                Comp_File_ToPackage::CreateComponent(aComps, aCfg, lSrc, "", lFileName, Phase::NONE);
            }
            else
            {
                delete lSrc;
            }
        }
    }

}
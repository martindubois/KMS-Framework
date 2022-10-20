
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Installer.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Installer.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_ADD_TO_PATH("AddToPath = False | True");

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    Installer::Installer()
    {
        AddEntry("AddToPath", &mAddToPath, false, &MD_ADD_TO_PATH);
    }

    void Installer::Run()
    {
        if (mAddToPath)
        {
            int lResult = 0;
            try
            {
                std::cout << "Adding the executable folder to the PATH ..." << std::endl;

                AddToPath();

                std::cout << "Added..." << std::endl;
            }
            KMS_CATCH(&lResult);

            exit(lResult);
        }
    }

}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Install_Installer.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>

#include <KMS/Install/Installer.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_ADD_TO_PATH     ("AddToPath = False | True");
static const KMS::Cfg::MetaData MD_REMOVE_FROM_PATH("RemoveFromPath = False | True");
static const KMS::Cfg::MetaData MD_UNINSTALL       ("Uninstall = False | True");

namespace KMS
{
    namespace Install
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const bool Installer::ADD_TO_PATH_DEFAULT      = false;
        const bool Installer::REMOVE_FROM_PATH_DEFAULT = false;
        const bool Installer::UNINSTALL_DEFAULT        = false;

        Installer::Installer() : mAddToPath(ADD_TO_PATH_DEFAULT)
        {
            AddEntry("AddToPath"     , &mAddToPath     , false, &MD_ADD_TO_PATH);
            AddEntry("RemoveFromPath", &mRemoveFromPath, false, &MD_REMOVE_FROM_PATH);
            AddEntry("Uninstall"     , &mUninstall     , false, &MD_UNINSTALL);
        }

        void Installer::Run()
        {
            int  lResult = 0;
            bool lExit   = false;

            try
            {
                if (mUninstall)
                {
                    lExit = true;

                    std::cout << "Uninstalling ..." << std::endl;

                    Uninstall();

                    std::cout << "Uninstalled" << std::endl;
                }
                else if (mRemoveFromPath)
                {
                    lExit = true;

                    std::cout << "Removing the executable folder from the PATH ..." << std::endl;

                    RemoveFromPath();

                    std::cout << "Removed" << std::endl;
                }
                else if (mAddToPath)
                {
                    lExit = true;

                    std::cout << "Adding the executable folder to the PATH ..." << std::endl;

                    AddToPath();

                    std::cout << "Added" << std::endl;
                }
            }
            KMS_CATCH(&lResult);

            if (lExit)
            {
                exit(lResult);
            }
        }

        void Installer::Uninstall()
        {
            RemoveFromPath();
        }

    }
}

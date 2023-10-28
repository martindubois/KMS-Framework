
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Install/Installer.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>

namespace KMS
{
    namespace Install
    {

        class Installer final : public DI::Dictionary
        {

        public:

            static const bool ADD_TO_PATH_DEFAULT;
            static const bool REMOVE_FROM_PATH_DEFAULT;
            static const bool UNINSTALL_DEFAULT;

            DI::Boolean mAddToPath;
            DI::Boolean mRemoveFromPath;
            DI::Boolean mUninstall;

            Installer();

            void AddToPath();

            void RemoveFromPath();

            void Run();

            void Uninstall();

        };

    }
}

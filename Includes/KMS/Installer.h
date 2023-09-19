
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Installer.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>

namespace KMS
{

    class Installer : public DI::Dictionary
    {

    public:

        static const bool ADD_TO_PATH_DEFAULT;

        DI::Boolean mAddToPath;

        Installer();

        void AddToPath();

        void Run();

    };

}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Config_Configurable.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Config/Configurator.h>

#include <KMS/Config/Configurable.h>

namespace KMS
{
    namespace Config
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Configurable::InitConfigurator(Configurator* aC)
        {
            assert(NULL != aC);

            aC->AddConfigurable(this);
        }

        bool Configurable::AddAttribute(const char*, const char*) { return false; }

        bool Configurable::AddAttribute_Indexed(const char*, const char*, const char*) { return false; }

        bool Configurable::SetAttribute(const char*) { return false; }

        bool Configurable::SetAttribute(const char*, const char*) { return false; }

        bool Configurable::SetAttribute_Indexed(const char*, const char*) { return false; }

        bool Configurable::SetAttribute_Indexed(const char*, const char*, const char*) { return false; }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Configurable::Configurable() {}

    }
}

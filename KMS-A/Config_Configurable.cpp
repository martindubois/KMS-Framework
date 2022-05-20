
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

        bool Configurable::AddAttribute(const char* aAttribute, const char* aValue)
        {
            assert(NULL != aAttribute);
            assert(NULL != aValue);

            return false;
        }

        bool Configurable::SetAttribute(const char* aAttribute)
        {
            assert(NULL != aAttribute);

            return false;
        }

        bool Configurable::SetAttribute(const char* aAttribute, const char* aValue)
        {
            assert(NULL != aAttribute);
            assert(NULL != aValue);

            return false;
        }

        bool Configurable::SetAttribute(const char* aAttribute, unsigned int aIndex, const char* aValue)
        {
            assert(NULL != aAttribute);
            assert(NULL != aValue);

            return false;
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        FILE* Configurable::ToFile(const char* aValue, const char* aMode)
        {
            assert(NULL != aValue);
            assert(NULL != aMode);

            if (0 == strcmp(aValue, "stderr")) { return stderr; }
            if (0 == strcmp(aValue, "stdin" )) { return stdin ; }
            if (0 == strcmp(aValue, "stdout")) { return stdout; }

            FILE* lResult;

            int lRet = fopen_s(&lResult, aValue, aMode);
            if (0 != lRet)
            {
                KMS_EXCEPTION_WITH_INFO(FILE_OPEN, "fopen_s failed", aValue);
            }

            assert(NULL != lResult);

            return lResult;
        }

        uint8_t Configurable::ToUInt8(const char* aValue, int aRadix)
        {
            assert(NULL != aValue);

            char * lPtr;

            unsigned long lResult = strtoul(aValue, &lPtr, aRadix);

            if ('\0' != *lPtr)
            {
                KMS_EXCEPTION_WITH_INFO(CONFIG_TYPE, "The value is not of the expected type (uint8_t)", aValue);
            }

            if (256 <= lResult)
            {
                KMS_EXCEPTION_WITH_INFO(CONFIG_TYPE, "The value is to large for the expected type (uint8_t)", aValue);
            }

            return static_cast<uint8_t>(lResult);
        }

    }
}

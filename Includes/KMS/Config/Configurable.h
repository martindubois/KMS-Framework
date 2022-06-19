
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Config/Configurable.h

#pragma once

namespace KMS
{
    namespace Config
    {

        class Configurator;

        class Configurable
        {

        public:

            virtual void InitConfigurator(Configurator * aC);

            virtual bool AddAttribute(const char* aAttribute, const char* aValue);

            virtual bool AddAttribute_Indexed(const char* aAttribute, const char* aIndex, const char* aValue);

            virtual bool SetAttribute(const char* aAttribute, const char* aValue);

            virtual bool SetAttribute_Indexed(const char* aAttribute, const char* aIndex, const char* aValue);

            virtual void DisplayHelp(FILE* aOut) const;

        protected:

            Configurable();

        };

    }
}

#define CFG_CALL(N,F) if (0 == strcmp(aA, N)) { (F)(aV); return true; }

#define CFG_CONVERT(N,F,T) if (0 == strcmp(aA, N)) { (F)(T(aV)); return true; }

#define CFG_EXPAND(N,F) if (0 == strcmp(aA, N)) { KMS::Environment::Expand(aV, lE, sizeof(lE)); (F)(lE); return true; }

#define CFG_IF(N) if (0 == strcmp(aA, N))

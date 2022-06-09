
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

            virtual bool SetAttribute(const char* aAttribute);
            virtual bool SetAttribute(const char* aAttribute, const char* aValue);
            virtual bool SetAttribute(const char* aAttribute, unsigned int aIndex, const char* aValue);

        protected:

            // TODO Find a better place for the Expand function.
            static void Expand(const char* aIn, char* aOut, unsigned int aOutSize_byte);

            Configurable();

        };

    }
}

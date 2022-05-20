
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Config/Configurator.h

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace Config
    {
        class Configurable;

        class Configurator
        {

        public:

            Configurator();

            void AddConfigurable(Configurable * aInterface);

            unsigned int GetIgnoredCount() const;
            
            void Init();

            void ParseArguments(int aCount, const char ** aVector);

            void ParseFile(const File::Folder& aFolder, const char* aFile, bool aMandatory = true);

        private:

            typedef std::list<Configurable *> ConfigurableList;

            Configurator(const Configurator &);

            const Configurator & operator = (const Configurator &);

            void ParseLine(const char * aLine);

            void AddAttribute(const char * aA, const char * aV);

            void SetAttribute(const char * aA);
            void SetAttribute(const char * aA, const char * aV);
            void SetAttribute(const char * aA, unsigned int aI, const char * aV);

            ConfigurableList mConfigurables;

            unsigned int mIgnoredCount;

        };

    }
}

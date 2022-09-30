
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Cfg/Configurator.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace Cfg
    {

        class Configurator
        {

        public:

            Configurator();

            void AddConfigurable(DI::Dictionary* aC);

            void AddConfigFile(const char* aPath);

            void AddOptionalConfigFile(const char* aPath);

            unsigned int GetIgnoredCount() const;

            void Help(FILE* aOut = NULL) const;

            void ParseArguments(int aCount, const char ** aVector);

            void ParseFile(const File::Folder& aFolder, const char* aFile, bool aMandatory = false);

            void Save(const char* aPath);

        private:

            typedef std::list<DI::Dictionary *> ConfigurableList;

            Configurator(const Configurator &);

            const Configurator & operator = (const Configurator &);

            void AddValueToArray(const char* aA, const char* aV);

            void AddValueToArray(const char* aA, const char* aI, const char* aV);

            DI::Object* FindObject(const char* aA);

            void ParseLine(const char * aLine);

            void SetBoolean(const char* aA);

            void SetArrayValue(const char* aA, unsigned int aI, const char* aV);

            void SetDictionaryValue(const char* aA, const char* aI, const char* aV);

            void SetValue(const char* aA, const char* aV);

            ConfigurableList mConfigurables;

            unsigned int mIgnoredCount;

        };

    }
}

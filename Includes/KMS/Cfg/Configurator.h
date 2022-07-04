
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Cfg/Configurator.h

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurable.h>
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace Cfg
    {

        class Configurator : public Configurable
        {

        public:

            Configurator();

            void AddConfigurable(Configurable * aInterface);

            void AddConfigFile(const char* aPath);

            void AddOptionalConfigFile(const char* aPath);

            unsigned int GetIgnoredCount() const;

            void Help(FILE* aOut = NULL) const;

            void Init();

            void ParseArguments(int aCount, const char ** aVector);

            void ParseFile(const File::Folder& aFolder, const char* aFile, bool aMandatory = false);

            // ===== Configurable ===========================================
            virtual bool AddAttribute(const char* aA, const char* aV);
            virtual bool SetAttribute(const char* aA, const char* aV);
            virtual void DisplayHelp(FILE* aOut) const;

        private:

            typedef std::list<Configurable *> ConfigurableList;

            Configurator(const Configurator &);

            const Configurator & operator = (const Configurator &);

            void ParseLine(const char * aLine);

            void CallAddAttribute(const char * aA, const char * aV);

            void CallAddAttribute_Indexed(const char* aA, const char* aI, const char* aV);

            void CallSetAttribute(const char * aA, const char * aV);

            void CallSetAttribute_Indexed(const char * aA, const char* aI, const char * aV);

            ConfigurableList mConfigurables;

            unsigned int mIgnoredCount;

        };

    }
}

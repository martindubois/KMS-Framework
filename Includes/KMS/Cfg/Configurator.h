
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Cfg/Configurator.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/File.h>
#include <KMS/File/Folder.h>

namespace KMS
{
    namespace Cfg
    {

        class Configurator final
        {

        public:

            static const bool  DISPLAY_CONFIG_DEFAULT;
            static const bool  HELP_DEFAULT;
            static const char* SAVE_CONFIG_DEFAULT;

        private:

            DI::Array   mConfigFiles;
            DI::Boolean mDisplayConfig;
            DI::Boolean mHelp;
            DI::Array   mOptionalConfigFiles;
            DI::File    mSaveConfig;

        public:

            Configurator();

            void AddConfigurable(DI::Dictionary* aC);

            void AddConfigFile(const char* aPath);

            void AddOptionalConfigFile(const char* aPath);

            unsigned int GetIgnoredCount() const;

            void SetSilence(const char** aSilence);

            void DisplayConfig();

            void Help();

            void Help(FILE* aOut) const;

            void ParseArguments(int aCount, const char ** aVector);

            void ParseFile(const File::Folder& aFolder, const char* aFile, bool aMandatory = false);

            void SaveConfig(const char* aPath);

            void Validate() const;

        private:

            typedef std::list<DI::Dictionary *> ConfigurableList;

            NO_COPY(Configurator);

            bool IsSilenced(const char* aLine);

            void ParseLine(const char * aLine);

            // ===== Callbacks ==============================================
            const Callback<Configurator> ON_CONFIG_FILES_CHANGED;
            const Callback<Configurator> ON_DISPLAY_CONFIG_CHANGED;
            const Callback<Configurator> ON_HELP_CHANGED;
            const Callback<Configurator> ON_OPTIONAL_CONFIG_FILES_CHANGED;
            const Callback<Configurator> ON_SAVE_CONFIG_CHANGED;
            unsigned int OnConfigFilesChanged        (void* aSender, void* aData);
            unsigned int OnDisplayConfigChanged      (void* aSender, void* aData);
            unsigned int OnHelpChanged               (void* aSender, void* aData);
            unsigned int OnOptionalConfigFilesChanged(void* aSender, void* aData);
            unsigned int OnSaveConfigChanged         (void* aSender, void* aData);

            ConfigurableList mConfigurables;

            DI::Dictionary mDictionary;

            unsigned int mIgnoredCount;

            const char** mSilence;

        };

    }
}

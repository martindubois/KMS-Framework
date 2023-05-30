
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Cfg/Configurator.h
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <list>

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/File.h>
#include <KMS/File/Folder.h>
#include <KMS/Msg/IReceiver.h>

namespace KMS
{
    namespace Cfg
    {

        class Configurator : public Msg::IReceiver
        {

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

            // ===== Msg::IReceived =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        private:

            typedef std::list<DI::Dictionary *> ConfigurableList;

            NO_COPY(Configurator);

            bool IsSilenced(const char* aLine);

            unsigned int OnConfigFilesChanged();
            unsigned int OnDisplayConfigChanged();
            unsigned int OnHelpChanged();
            unsigned int OnOptionalConfigFilesChanged();
            unsigned int OnSaveConfigChanged();

            void ParseLine(const char * aLine);

            ConfigurableList mConfigurables;

            DI::Dictionary mDictionary;

            unsigned int mIgnoredCount;

            const char** mSilence;

            // ===== Configurable attributes ================================
            DI::Array   mConfigFiles;
            DI::Boolean mDisplayConfig;
            DI::Boolean mHelp;
            DI::Array   mOptionalConfigFiles;
            DI::File    mSaveConfig;

        };

    }
}

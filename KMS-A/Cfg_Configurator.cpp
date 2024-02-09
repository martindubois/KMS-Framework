
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Cfg_Configurator.cpp

// TEST COVERAGE 2023-08-06 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/Array.h>
#include <KMS/DI/Array_Sparse.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/Functions.h>
#include <KMS/DI/String.h>
#include <KMS/Text/File_ASCII.h>

#include <KMS/Cfg/Configurator.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_CONFIG_FILES         ("ConfigFiles += {Path}");
static const KMS::Cfg::MetaData MD_DISPLAY_CONFIG       ("DisplayConfig = false | true");
static const KMS::Cfg::MetaData MD_HELP                 ("Help");
static const KMS::Cfg::MetaData MD_OPTIONAL_CONFIG_FILES("OptionalConfigFiles += {Path}");
static const KMS::Cfg::MetaData MD_SAVE_CONFIG          ("SaveConfig = {Path}");

#define FMT_ATT "%[A-Za-z0-9_.]"
#define FMT_IDX "%[A-Za-z0-9_]"
#define FMT_VAL "%[^\n\r\t]"

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Help_Dictionary(FILE* aOut, const char* aName, const KMS::Cfg::MetaData* aMD, const KMS::DI::Dictionary* aD, unsigned int aLevel);
static void Help_Object    (FILE* aOut, const char* aName, const KMS::Cfg::MetaData* aMD,                                unsigned int aLevel);

static void Save_Array     (FILE* aOut, const KMS::DI::Array     * aA, const char* aName);
static void Save_Dictionary(FILE* aOut, const KMS::DI::Dictionary* aD, const char* aName);
static void Save_Object    (FILE* aOut, const KMS::DI::Object    * aO, const char* aName);
static void Save_Value     (FILE* aOut, const KMS::DI::Value     * aV, const char* aName);

namespace KMS
{
    namespace Cfg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const bool  Configurator::DISPLAY_CONFIG_DEFAULT = false;
        const bool  Configurator::HELP_DEFAULT           = false;
        const char* Configurator::SAVE_CONFIG_DEFAULT    = "";

        Configurator::Configurator()
            : mDisplayConfig(DISPLAY_CONFIG_DEFAULT)
            , mHelp         (HELP_DEFAULT)
            , mSaveConfig   (nullptr, SAVE_CONFIG_DEFAULT)
            , mIgnoredCount(0)
            , mSilence(nullptr)
            // ===== Callbacks ==============================================
            , ON_CONFIG_FILES_CHANGED         (this, &Configurator::OnConfigFilesChanged)
            , ON_DISPLAY_CONFIG_CHANGED       (this, &Configurator::OnDisplayConfigChanged)
            , ON_HELP_CHANGED                 (this, &Configurator::OnHelpChanged)
            , ON_OPTIONAL_CONFIG_FILES_CHANGED(this, &Configurator::OnOptionalConfigFilesChanged)
            , ON_SAVE_CONFIG_CHANGED          (this, &Configurator::OnSaveConfigChanged)
        {
            mConfigFiles        .mOnChanged = &ON_CONFIG_FILES_CHANGED;
            mDisplayConfig      .mOnChanged = &ON_DISPLAY_CONFIG_CHANGED;
            mHelp               .mOnChanged = &ON_HELP_CHANGED;
            mOptionalConfigFiles.mOnChanged = &ON_OPTIONAL_CONFIG_FILES_CHANGED;
            mSaveConfig         .mOnChanged = &ON_SAVE_CONFIG_CHANGED;

            mConfigFiles        .SetCreator(DI::String_Expand::Create);
            mOptionalConfigFiles.SetCreator(DI::String_Expand::Create);

            mSaveConfig.SetMode("wb");

            mDictionary.AddEntry("ConfigFiles"        , &mConfigFiles        , false, &MD_CONFIG_FILES);
            mDictionary.AddEntry("DisplayConfig"      , &mDisplayConfig      , false, &MD_DISPLAY_CONFIG);
            mDictionary.AddEntry("Help"               , &mHelp               , false, &MD_HELP);
            mDictionary.AddEntry("OptionalConfigFiles", &mOptionalConfigFiles, false, &MD_OPTIONAL_CONFIG_FILES);
            mDictionary.AddEntry("SaveConfig"         , &mSaveConfig         , false, &MD_SAVE_CONFIG);

            mConfigurables.push_back(&mDictionary);
        }

        void Configurator::AddConfigurable(DI::Dictionary* aD)
        {
            assert(nullptr != aD);

            mConfigurables.push_back(aD);
        }

        void Configurator::AddConfigFile(const char* aPath)
        {
            mConfigFiles.AddEntry(new DI::String_Expand(aPath), true);

            OnConfigFilesChanged(this, nullptr);
        }

        void Configurator::AddOptionalConfigFile(const char* aPath)
        {
            mOptionalConfigFiles.AddEntry(new DI::String_Expand(aPath), true);

            OnOptionalConfigFilesChanged(this, nullptr);
        }

        unsigned int Configurator::GetIgnoredCount() const { return mIgnoredCount; }

        void Configurator::SetSilence(const char** aSilence)
        {
            assert(nullptr != aSilence);

            assert(nullptr == mSilence);

            mSilence = aSilence;
        }

        void Configurator::DisplayConfig()
        {
            mDisplayConfig.Set(true);
            
            OnDisplayConfigChanged(this, nullptr);
        }

        // NOT TESTED
        void Configurator::Help() { mHelp.Set(true); OnHelpChanged(this, nullptr); }

        void Configurator::Help(FILE* aOut) const
        {
            FILE* lOut = (nullptr == aOut) ? mConsole.OutputFile() : aOut;

            for (auto lD : mConfigurables)
            {
                Help_Dictionary(lOut, nullptr, nullptr, lD, 0);
            }
        }

        void Configurator::ParseArguments(int aCount, const char** aVector)
        {
            assert(nullptr != aVector);

            for (int i = 0; i < aCount; i++)
            {
                ParseLine(aVector[i]);
            }
        }

        void Configurator::ParseFile(const File::Folder& aFolder, const char* aFile, bool aMandatory)
        {
            if (aMandatory || aFolder.DoesFileExist(aFile))
            {
                Text::File_ASCII lTF;

                lTF.Read(aFolder, aFile);

                lTF.RemoveEmptyLines();
                lTF.RemoveComments_Script();

                for (const auto& lLine : lTF.mLines)
                {
                    ParseLine(lLine.c_str());
                }
            }
        }

        void Configurator::SaveConfig(const char* aFileName)
        {
            mSaveConfig.Set(aFileName);

            OnSaveConfigChanged(this, nullptr);
        }

        void Configurator::Validate() const
        {
            for (auto lD : mConfigurables)
            {
                lD->Validate();
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Configurator::IsSilenced(const char* aLine)
        {
            if (nullptr != mSilence)
            {
                auto lS = mSilence;

                while (nullptr != (*lS))
                {
                    if (0 == strncmp(aLine, *lS, strlen(*lS)))
                    {
                        return true;
                    }

                    lS++;
                }
            }

            return false;
        }

        void Configurator::ParseLine(const char* aLine)
        {
            assert(nullptr != aLine);

            for (auto lD : mConfigurables)
            {
                if (DI::Execute_Operation(lD, aLine))
                {
                    return;
                }
            }

            if (!IsSilenced(aLine))
            {
                KMS_DBG_LOG_WARNING_F(Dbg::Log::FLAG_USER_REDUNDANT);
                Dbg::gLog.WriteMessage(aLine);
                mIgnoredCount++;
            }
        }

        // ===== Callbacks ==================================================

        unsigned int Configurator::OnConfigFilesChanged(void*, void*)
        {
            auto lCount = mConfigFiles.GetCount();
            if (0 < lCount)
            {
                auto lString = mConfigFiles.GetEntry_R<DI::String>(lCount - 1);
                assert(nullptr != lString);

                ParseFile(File::Folder::NONE, lString->Get(), true);
            }

            return 0;
        }

        unsigned int Configurator::OnDisplayConfigChanged(void*, void*)
        {
            if (mDisplayConfig)
            {
                fprintf(mConsole.OutputFile(),
                    "\n"
                    "===== Configuration =====\n"
                    "\n");

                for (const auto lD : mConfigurables)
                {
                    Save_Dictionary(mConsole.OutputFile(), lD, nullptr);
                }
            }

            return 0;
        }

        // NOT TESTED
        unsigned int Configurator::OnHelpChanged(void*, void*)
        {
            if (mHelp)
            {
                Help(mConsole.OutputFile());

                exit(0);
            }

            return 0;
        }

        unsigned int Configurator::OnOptionalConfigFilesChanged(void*, void*)
        {
            auto lCount = mOptionalConfigFiles.GetCount();
            if (0 < lCount)
            {
                auto lString = mOptionalConfigFiles.GetEntry_R<DI::String>(lCount - 1);
                assert(nullptr != lString);

                ParseFile(File::Folder::NONE, lString->Get(), false);
            }

            return 0;
        }

        unsigned int Configurator::OnSaveConfigChanged(void*, void*)
        {
            if (mSaveConfig.IsOpen())
            {
                fprintf(mSaveConfig,
                    "\n"
                    "# Configuration file generated by the KMS::Cfg::Configurator class\n"
                    "\n");

                for (const auto lD : mConfigurables)
                {
                    Save_Dictionary(mSaveConfig, lD, nullptr);
                }

                mSaveConfig.Close();
            }

            return 0;
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Help_Dictionary(FILE* aOut, const char* aName, const Cfg::MetaData* aMD, const DI::Dictionary* aD, unsigned int aLevel)
{
    assert(nullptr != aOut);
    assert(nullptr != aD);

    Help_Object(aOut, aName, aMD, aLevel);

    for (const auto& lVT : aD->mInternal)
    {
        assert(nullptr != lVT.second);

        auto lMD = dynamic_cast<const Cfg::MetaData *>(lVT.second.mMetaData);
        auto lD  = dynamic_cast<const DI::Dictionary*>(lVT.second.Get());
        if (nullptr == lD)
        {
            Help_Object(aOut, lVT.first.c_str(), lMD, aLevel + 1);
        }
        else
        {
            // NOT TESTED
            Help_Dictionary(aOut, lVT.first.c_str(), lMD, lD, aLevel + 1);
        }
    }
}

void Help_Object(FILE* aOut, const char* aName, const Cfg::MetaData* aMD, unsigned int aLevel)
{
    assert(nullptr != aOut);

    if (nullptr != aMD)
    {
        fprintf(aOut, "%*c%s\n", aLevel * 2, ' ', aMD->GetHelp());
    }
    else if (nullptr != aName)
    {
        // NOT TESTED
        fprintf(aOut, "%*c%s\n", aLevel * 2, ' ', aName);
    }
}

void Save_Array(FILE* aOut, const DI::Array* aA, const char* aName)
{
    assert(nullptr != aOut);
    assert(nullptr != aA);
    assert(nullptr != aName);

    for (const auto& lEntry : aA->mInternal)
    {
        assert(nullptr != lEntry);

        const DI::Value* lV = dynamic_cast<const DI::Value*>(lEntry.Get());
        KMS_EXCEPTION_ASSERT(nullptr != lV, RESULT_INVALID_FORMAT, "Can't save part of the configuration", aName);

        char lData[LINE_LENGTH];

        lV->Get(lData, sizeof(lData));

        fprintf(aOut, "%s += %s\n", aName, lData);
    }
}

void Save_Dictionary(FILE* aOut, const DI::Dictionary* aD, const char* aName)
{
    assert(nullptr != aD);

    for (const auto& lVT : aD->mInternal)
    {
        assert(nullptr != lVT.second);

        if (nullptr == aName)
        {
            Save_Object(aOut, lVT.second.Get(), lVT.first.c_str());
        }
        else
        {
            char lName[NAME_LENGTH];

            sprintf_s(lName, "%s.%s", aName, lVT.first.c_str());

            Save_Object(aOut, lVT.second.Get(), lName);
        }
    }
}

void Save_Object(FILE* aOut, const DI::Object* aO, const char* aName)
{
    assert(nullptr != aO);

    auto lD = dynamic_cast<const DI::Dictionary*>(aO);
    if (nullptr == lD)
    {
        auto lA = dynamic_cast<const DI::Array*>(aO);
        if (nullptr == lA)
        {
            auto lV = dynamic_cast<const DI::Value*>(aO);
            KMS_EXCEPTION_ASSERT(nullptr != lV, RESULT_INVALID_FORMAT, "Can't save part of the configuration", aName);

            Save_Value(aOut, lV, aName);
        }
        else
        {
            Save_Array(aOut, lA, aName);
        }
    }
    else
    {
        Save_Dictionary(aOut, lD, aName);
    }
}

void Save_Value(FILE* aOut, const DI::Value* aV, const char* aName)
{
    assert(nullptr != aV);
    assert(nullptr != aName);

    char lData[LINE_LENGTH];

    aV->Get(lData, sizeof(lData));

    fprintf(aOut, "%s = %s\n", aName, lData);
}

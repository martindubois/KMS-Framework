
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Cfg_Configurator.cpp

// TEST COVERAGE 2022-10-28 KMS - Martin Dubois, P. Eng.

// TODO Do not read the same configuration file twice.

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
static const KMS::Cfg::MetaData MD_HELP                 ("Help");
static const KMS::Cfg::MetaData MD_OPTIONAL_CONFIG_FILES("OptionalConfigFiles += {Path}");
static const KMS::Cfg::MetaData MD_SAVE_CONFIG          ("SaveConfig = {Path}");

#define ON_CONFIG_FILES_CHANGED          (1)
#define ON_HELP_CHANGED                  (2)
#define ON_OPTIONAL_CONFIG_FILES_CHANGED (3)
#define ON_SAVE_CHANGED                  (4)

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

        Configurator::Configurator() : mIgnoredCount(0), mSilence(NULL)
        {
            mConfigFiles        .mOnChanged.Set(this, ON_CONFIG_FILES_CHANGED);
            mHelp               .mOnChanged.Set(this, ON_HELP_CHANGED);
            mOptionalConfigFiles.mOnChanged.Set(this, ON_OPTIONAL_CONFIG_FILES_CHANGED);
            mSaveConfig         .mOnChanged.Set(this, ON_SAVE_CHANGED);

            mConfigFiles        .SetCreator(DI::String_Expand::Create);
            mOptionalConfigFiles.SetCreator(DI::String_Expand::Create);

            mSaveConfig.SetMode("wb");

            mDictionary.AddEntry("ConfigFiles"        , &mConfigFiles        , false, &MD_CONFIG_FILES);
            mDictionary.AddEntry("Help"               , &mHelp               , false, &MD_HELP);
            mDictionary.AddEntry("OptionalConfigFiles", &mOptionalConfigFiles, false, &MD_OPTIONAL_CONFIG_FILES);
            mDictionary.AddEntry("SaveConfig"         , &mSaveConfig            , false, &MD_SAVE_CONFIG);

            mConfigurables.push_back(&mDictionary);
        }

        void Configurator::AddConfigurable(DI::Dictionary* aD) { assert(NULL != aD); mConfigurables.push_back(aD); }

        void Configurator::AddConfigFile(const char* aPath)
        {
            mConfigFiles.AddEntry(new DI::String_Expand(aPath), true);

            OnConfigFilesChanged();
        }

        void Configurator::AddOptionalConfigFile(const char* aPath)
        {
            mOptionalConfigFiles.AddEntry(new DI::String_Expand(aPath), true);

            OnOptionalConfigFilesChanged();
        }

        unsigned int Configurator::GetIgnoredCount() const { return mIgnoredCount; }

        void Configurator::SetSilence(const char** aSilence)
        {
            assert(NULL != aSilence);

            assert(NULL == mSilence);

            mSilence = aSilence;
        }

        void Configurator::Help(FILE* aOut) const
        {
            FILE* lOut = (NULL == aOut) ? stdout : aOut;

            for (DI::Dictionary* lD : mConfigurables)
            {
                Help_Dictionary(lOut, NULL, NULL, lD, 0);
            }
        }

        void Configurator::ParseArguments(int aCount, const char** aVector)
        {
            assert(NULL != aVector);

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

                for (std::string lLine : lTF.mLines)
                {
                    ParseLine(lLine.c_str());
                }
            }
        }

        void Configurator::SaveConfig(const char* aFileName)
        {
            mSaveConfig.Set(aFileName);
        }

        // ===== Msg::IReceived =============================================

        unsigned int Configurator::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

            switch (aCode)
            {
            case ON_CONFIG_FILES_CHANGED         : lResult = OnConfigFilesChanged        (); break;
            case ON_OPTIONAL_CONFIG_FILES_CHANGED: lResult = OnOptionalConfigFilesChanged(); break;
            case ON_SAVE_CHANGED                 : lResult = OnSaveChanged               (); break;

            case ON_HELP_CHANGED: Help(); exit(0);

            default: assert(false);
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        bool Configurator::IsSilenced(const char* aLine)
        {
            if (NULL != mSilence)
            {
                const char** lS = mSilence;

                while (NULL != (*lS))
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

        unsigned int Configurator::OnConfigFilesChanged()
        {
            unsigned int lCount = mConfigFiles.GetCount();
            if (0 < lCount)
            {
                const DI::Object* lObject = mConfigFiles.GetEntry_R(lCount - 1);
                assert(NULL != lObject);

                const DI::String* lString = dynamic_cast<const DI::String*>(lObject);
                assert(NULL != lString);

                ParseFile(File::Folder::NONE, lString->Get(), true);
            }

            return 0;
        }

        unsigned int Configurator::OnOptionalConfigFilesChanged()
        {
            unsigned int lCount = mOptionalConfigFiles.GetCount();
            if (0 < lCount)
            {
                const DI::Object* lObject = mOptionalConfigFiles.GetEntry_R(lCount - 1);
                assert(NULL != lObject);

                const DI::String* lString = dynamic_cast<const DI::String*>(lObject);
                assert(NULL != lString);

                ParseFile(File::Folder::NONE, lString->Get(), false);
            }

            return 0;
        }

        unsigned int Configurator::OnSaveChanged()
        {
            if (mSaveConfig.IsOpen())
            {
                fprintf(mSaveConfig,
                    "\n"
                    "# Configuration file generated by the KMS::Cfg::Configurator class\n"
                    "\n");

                for (DI::Dictionary* lD : mConfigurables)
                {
                    Save_Dictionary(mSaveConfig, lD, NULL);
                }

                mSaveConfig.Close();
            }

            return 0;
        }

        void Configurator::ParseLine(const char* aLine)
        {
            assert(NULL != aLine);

            for (DI::Dictionary* lD : mConfigurables)
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

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Help_Dictionary(FILE* aOut, const char* aName, const Cfg::MetaData* aMD, const DI::Dictionary* aD, unsigned int aLevel)
{
    assert(NULL != aOut);
    assert(NULL != aD);

    Help_Object(aOut, aName, aMD, aLevel);

    for (const DI::Dictionary::Internal::value_type lVT : aD->mInternal)
    {
        assert(NULL != lVT.second);

        const Cfg::MetaData * lMD = dynamic_cast<const Cfg::MetaData *>(lVT.second.mMetaData);
        const DI::Dictionary* lD  = dynamic_cast<const DI::Dictionary*>(lVT.second.Get());
        if (NULL == lD)
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
    assert(NULL != aOut);

    if (NULL != aMD)
    {
        fprintf(aOut, "%*c%s\n", aLevel * 2, ' ', aMD->GetHelp());
    }
    else if (NULL != aName)
    {
        // NOT TESTED
        fprintf(aOut, "%*c%s\n", aLevel * 2, ' ', aName);
    }
}

void Save_Array(FILE* aOut, const DI::Array* aA, const char* aName)
{
    assert(NULL != aOut);
    assert(NULL != aA);
    assert(NULL != aName);

    for (const DI::Container::Entry& lEntry : aA->mInternal)
    {
        assert(NULL != lEntry);

        const DI::Value* lV = dynamic_cast<const DI::Value*>(lEntry.Get());
        KMS_EXCEPTION_ASSERT(NULL != lV, CFG_FORMAT_INVALID, "Can't save part of the configuration", aName);

        char lData[LINE_LENGTH];

        lV->Get(lData, sizeof(lData));

        fprintf(aOut, "%s += %s", aName, lData);
    }
}

void Save_Dictionary(FILE* aOut, const DI::Dictionary* aD, const char* aName)
{
    assert(NULL != aD);

    for (const DI::Dictionary::Internal::value_type lVT : aD->mInternal)
    {
        assert(NULL != lVT.second);

        if (NULL == aName)
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
    assert(NULL != aO);

    const DI::Dictionary* lD = dynamic_cast<const DI::Dictionary*>(aO);
    if (NULL == lD)
    {
        const DI::Array* lA = dynamic_cast<const DI::Array*>(aO);
        if (NULL == lA)
        {
            const DI::Value* lV = dynamic_cast<const DI::Value*>(aO);
            KMS_EXCEPTION_ASSERT(NULL != lV, CFG_FORMAT_INVALID, "Can't save part of the configuration", aName);

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
    assert(NULL != aV);
    assert(NULL != aName);

    char lData[LINE_LENGTH];

    aV->Get(lData, sizeof(lData));

    fprintf(aOut, "%s = %s\n", aName, lData);
}

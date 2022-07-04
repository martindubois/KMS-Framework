
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Cfg_Configurator.cpp

// TODO Do not read the same configuration file twice.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurable.h>
#include <KMS/Environment.h>
#include <KMS/SafeAPI.h>
#include <KMS/Text/TextFile.h>

#include <KMS/Cfg/Configurator.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define FMT_ATT "%[A-Za-z0-9_]"
#define FMT_VAL "%[^\n\r\t]"

namespace KMS
{
    namespace Cfg
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Configurator::Configurator() : mIgnoredCount(0) {}

        void Configurator::AddConfigurable(Configurable* aC) { assert(NULL != aC); mConfigurables.push_back(aC); }

        void Configurator::AddConfigFile(const char* aPath)
        {
            ParseFile(File::Folder(File::Folder::Id::NONE), aPath, true);
        }

        void Configurator::AddOptionalConfigFile(const char* aPath)
        {
            ParseFile(File::Folder(File::Folder::Id::NONE), aPath);
        }

        unsigned int Configurator::GetIgnoredCount() const { return mIgnoredCount; }

        void Configurator::Help(FILE* aOut) const
        {
            FILE* lOut = (NULL == aOut) ? stdout : aOut;

            for (Configurable* lC : mConfigurables)
            {
                lC->DisplayHelp(lOut);
            }
        }

        void Configurator::Init() { mConfigurables.push_back(this); }

        void Configurator::ParseArguments(int aCount, const char ** aVector)
        {
            assert(NULL != aVector);

            for (int i = 0; i < aCount; i++)
            {
                ParseLine(aVector[i]);
            }
        }

        void Configurator::ParseFile(const File::Folder & aFolder, const char* aFile, bool aMandatory)
        {
            if (aMandatory || aFolder.DoesFileExist(aFile))
            {
                Text::TextFile lTF;

                lTF.Read(aFolder, aFile);

                lTF.RemoveEmptyLines();
                lTF.RemoveComments_Script();

                for (std::string lLine : lTF.mLines)
                {
                    ParseLine(lLine.c_str());
                }
            }
        }

        // ===== Configurable ===========================================

        bool Configurator::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            char lE[LINE_LENGTH];

            CFG_EXPAND("ConfigFile"        , AddConfigFile        );
            CFG_EXPAND("OptionalConfigFile", AddOptionalConfigFile);

            return Configurable::AddAttribute(aA, aV);
        }

        bool Configurator::SetAttribute(const char* aA, const char* aV)
        {
            if (NULL == aV)
            {
                CFG_IF("Help") { Help(stdout); exit(0); }
            }

            return Configurable::SetAttribute(aA, aV);
        }

        void Configurator::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "===== KMS::Cfg::Configurator =====\n"
                "ConfigFile += {Path}\n"
                "    Include the specified configuration file\n"
                "Help\n"
                "    Display this help message and exit\n"
                "OptionalConfigFile += {Path}\n"
                "    Include the specified configuraiont file if it exist\n");

            Configurable::DisplayHelp(aOut);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Configurator::ParseLine(const char* aLine)
        {
            assert(NULL != aLine);

            char lA[NAME_LENGTH];
            char lI[NAME_LENGTH];
            char lV[LINE_LENGTH];

            if (3 == sscanf_s(aLine, FMT_ATT "[ " FMT_ATT " ] += " FMT_VAL, lA SizeInfo(lA), lI SizeInfo(lI), lV SizeInfo(lV)))
            {
                CallAddAttribute_Indexed(lA, lI, lV); return;
            }

            if (3 == sscanf_s(aLine, FMT_ATT "[ " FMT_ATT " ] = " FMT_VAL, lA SizeInfo(lA), lI SizeInfo(lI), lV SizeInfo(lV)))
            {
                CallSetAttribute_Indexed(lA, lI, lV); return;
            }

            if (2 == sscanf_s(aLine, FMT_ATT " += " FMT_VAL, lA SizeInfo(lA), lV SizeInfo(lV))) { CallAddAttribute(lA, lV); return; }

            if (2 == sscanf_s(aLine, FMT_ATT " = "  FMT_VAL, lA SizeInfo(lA), lV SizeInfo(lV))) { CallSetAttribute(lA, lV); return; }

            if (2 == sscanf_s(aLine, FMT_ATT "[ " FMT_ATT " ]", lA SizeInfo(lA), lI SizeInfo(lI))) { CallSetAttribute_Indexed(lA, lI, NULL); return; }

            if (1 == sscanf_s(aLine, FMT_ATT, lA SizeInfo(lA))) { CallSetAttribute(lA, NULL); return; }

            KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid configuration format", aLine);
        }

        void Configurator::CallAddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);
            assert(NULL != aV);

            for (Configurable* lC : mConfigurables)
            {
                assert(NULL != lC);

                if (lC->AddAttribute(aA, aV))
                {
                    return;
                }
            }

            mIgnoredCount++;
        }

        void Configurator::CallAddAttribute_Indexed(const char* aA, const char* aI, const char* aV)
        {
            for (Configurable* lC : mConfigurables)
            {
                assert(NULL != lC);

                if (lC->AddAttribute_Indexed(aA, aI, aV))
                {
                    return;
                }
            }

            mIgnoredCount++;
        }

        void Configurator::CallSetAttribute(const char* aA, const char* aV)
        {
            for (Configurable* lC : mConfigurables)
            {
                assert(NULL != lC);

                if (lC->SetAttribute(aA, aV))
                {
                    return;
                }
            }

            mIgnoredCount++;
        }

        void Configurator::CallSetAttribute_Indexed(const char* aA, const char* aI, const char* aV)
        {
            for (Configurable* lC : mConfigurables)
            {
                assert(NULL != lC);

                if (lC->SetAttribute_Indexed(aA, aI, aV))
                {
                    return;
                }
            }

            mIgnoredCount++;
        }

    }
}

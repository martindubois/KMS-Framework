
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Config_Configurator.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/SafeAPI.h>
#include <KMS/Text/TextFile.h>

#include <KMS/Config/Configurator.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define FMT_ATT "%[A-Za-z0-9_]"
#define FMT_VAL "%[^\n\r\t]"

namespace KMS
{
    namespace Config
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Configurator::Configurator() : mIgnoredCount(0) {}

        void Configurator::AddConfigurable(Configurable* aC) { assert(NULL != aC); mConfigurables.push_back(aC); }

        unsigned int Configurator::GetIgnoredCount() const { return mIgnoredCount; }

        // TODO Add the configurator to the list.
        void Configurator::Init() {}

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

        // Private
        // //////////////////////////////////////////////////////////////////

        void Configurator::ParseLine(const char* aLine)
        {
            assert(NULL != aLine);

            char lA[1024];
            char lV[1024];

            unsigned int lI;

            if (3 == sscanf_s(aLine, FMT_ATT "[%u]=" FMT_VAL, lA SizeInfo(lA), &lI, lV SizeInfo(lV))) { SetAttribute(lA, lI, lV); return; }

            if (2 == sscanf_s(aLine, FMT_ATT "+=" FMT_VAL, lA SizeInfo(lA), lV SizeInfo(lV))) { AddAttribute(lA, lV); return; }
            if (2 == sscanf_s(aLine, FMT_ATT "="  FMT_VAL, lA SizeInfo(lA), lV SizeInfo(lV))) { SetAttribute(lA, lV); return; }

            if (1 == sscanf_s(aLine, FMT_ATT, lA SizeInfo(lA))) { SetAttribute(lA); return; }

            KMS_EXCEPTION_WITH_INFO(CONFIG_FORMAT, "Invalid configuration format", aLine);
        }

        void Configurator::AddAttribute(const char* aA, const char* aV)
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

        void Configurator::SetAttribute(const char* aA)
        {
            assert(NULL != aA);

            for (Configurable* lC : mConfigurables)
            {
                assert(NULL != lC);

                if (lC->SetAttribute(aA))
                {
                    return;
                }
            }

            mIgnoredCount++;
        }

        void Configurator::SetAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);
            assert(NULL != aV);

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

        void Configurator::SetAttribute(const char* aA, unsigned int aI, const char* aV)
        {
            assert(NULL != aA);
            assert(NULL != aV);

            for (Configurable* lC : mConfigurables)
            {
                assert(NULL != lC);

                if (lC->SetAttribute(aA, aI, aV))
                {
                    return;
                }
            }

            mIgnoredCount++;
        }

    }
}

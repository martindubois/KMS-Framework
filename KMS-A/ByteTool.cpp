
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/ByteTool.cpp

// TEST COVERAGE 2022-06-14 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Installer.h>

#include <KMS/ByteTool.h>

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-ByteTool.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

#define BYTE_QTY (256)

static const char* LABELS[16] =
{
    "x0_", "x1_", "x2_", "x3_", "x4_", "x5_", "x6_", "x7_", "x8_", "x9_", "xA_", "xB_", "xC_", "xD_", "xE_", "xF_"
};

static const KMS::Cfg::MetaData MD_SOURCE("Source = {FileName}");

// Static function declaration
// //////////////////////////////////////////////////////////////////////////

static void DisplayValue(std::ostream& aOut, unsigned int aValue);

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    int ByteTool::Main(int aCount, const char** aVector)
    {
        assert(1 <= aCount);
        assert(NULL != aVector);
        assert(NULL != aVector[0]);

        int lResult = __LINE__;

        try
        {
            KMS::ByteTool          lBT;
            KMS::Cfg::Configurator lC;
            KMS::Installer         lInstaller;


            lC.AddConfigurable(&lBT);
            lC.AddConfigurable(&lInstaller);

            lC.AddConfigurable(&Dbg::gLog);

            lC.ParseFile(File::Folder::CURRENT, CONFIG_FILE);
            lC.ParseArguments(aCount - 1, aVector + 1);

            lInstaller.Run();

            lResult = lBT.Run();

            std::cerr << lBT;
        }
        KMS_CATCH_RESULT(lResult)

        return lResult;
    }

    ByteTool::ByteTool() : mSource(stdin, "stdin")
    {
        AddEntry("Source", &mSource, false, &MD_SOURCE);

        memset(&mHistogram, 0, sizeof(mHistogram));
    }

    ByteTool::~ByteTool() {}

    const unsigned int* ByteTool::GetHistrogram() const { return mHistogram; }

    int ByteTool::Run()
    {
        uint8_t lByte;

        while (1 == fread(&lByte, sizeof(lByte), 1, mSource))
        {
            mHistogram[lByte] ++;
        }

        return 0;
    }

}

std::ostream& operator << (std::ostream& aOut, const KMS::ByteTool& aBT)
{
    const unsigned int* lH = aBT.GetHistrogram();
    assert(NULL != lH);

    unsigned int lMax = lH[0];
    unsigned int lMin = lH[0];
    unsigned int lSum = lH[0];

    aOut << "      x_0   x_1   x_2   x_3   x_4   x_5   x_6   x_7   x_8   x_9   x_A   x_B   x_C   x_D   x_E   x_F" << "\n";
    aOut << LABELS[0];

    DisplayValue(aOut, lH[0]);

    for (unsigned int i = 1; i < BYTE_QTY; i++)
    {
        if (0 == i % 16)
        {
            aOut << "\n" << LABELS[i / 16];
        }

        if (lMax < lH[i]) { lMax = lH[i]; }
        if (lMin > lH[i]) { lMin = lH[i]; }

        lSum += lH[i];

        DisplayValue(aOut, lH[i]);
    }

    aOut << "\n";
    aOut << "Min : " << lMin << "\n";
    aOut << "Avg : " << lSum / 256.0 << "\n";
    aOut << "Max : " << lMax << "\n";
    aOut << "Sum : " << lSum << std::endl;

    return aOut;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void DisplayValue(std::ostream& aOut, unsigned int aValue)
{
    char lStr[8];

    sprintf_s(lStr, "%6u", aValue);

    aOut << lStr;
}

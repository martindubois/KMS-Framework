
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
#include <KMS/Convert.h>
#include <KMS/Environment.h>

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

            lBT.InitConfigurator(&lC);

            lC.Init();
            lC.ParseFile(File::Folder(File::Folder::Id::CURRENT), CONFIG_FILE);
            lC.ParseArguments(aCount - 1, aVector + 1);

            lResult = lBT.Run();

            std::cerr << lBT;
        }
        KMS_CATCH_RESULT(lResult)

        return lResult;
    }

    ByteTool::ByteTool() : mDestination(NULL), mSource(stdout)
    {
        for (unsigned int i = 0; i < BYTE_QTY; i++)
        {
            mByteTable[i] = static_cast<uint8_t>(i);
            mHistogram[i] = 0;
        }
    }

    ByteTool::~ByteTool()
    {
        CloseFile(mDestination);
        CloseFile(mSource);
    }

    const unsigned int* ByteTool::GetHistrogram() const { return mHistogram; }

    int ByteTool::Run()
    {
        assert(NULL != mSource);

        uint8_t lByte;

        while (1 == fread(&lByte, sizeof(lByte), 1, mSource))
        {
            mHistogram[lByte] ++;

            if (NULL != mDestination)
            {
                lByte = mByteTable[lByte];

                size_t lRet = fwrite(&lByte, sizeof(lByte), 1, mDestination);
                if (1 != lRet)
                {
                    // NOT TESTED
                    KMS_EXCEPTION_WITH_INFO(FILE_WRITE, "fwrite failed", lRet);
                }
            }
        }

        return 0;
    }

    void ByteTool::SetByteTable(unsigned int aIndex, uint8_t aValue)
    {
        if (BYTE_QTY <= aIndex)
        {
            // NOT TESTED
            KMS_EXCEPTION_WITH_INFO(CONFIG_INDEX, "The index into the ByteTable must be between 0 and 255", aIndex);
        }

        mByteTable[aIndex] = aValue;
    }

    void ByteTool::SetDestination(FILE * aFile)
    {
        assert(NULL != aFile);

        CloseFile(mDestination);
        
        mDestination = aFile;
    }

    void ByteTool::SetSource(FILE* aFile)
    {
        assert(NULL != aFile);

        CloseFile(mSource);

        mSource = aFile;
    }

    // ===== Cfg::Configurable ==============================================

    bool ByteTool::SetAttribute(const char* aA, const char* aV)
    {
        if (NULL != aV)
        {
            char lE[LINE_LENGTH];

            CFG_IF("Destination") { Environment::Expand(aV, lE, sizeof(lE)); SetDestination(Convert::ToFile(lE, "wb")); return true; }
            CFG_IF("Source"     ) { Environment::Expand(aV, lE, sizeof(lE)); SetSource     (Convert::ToFile(lE, "rb")); return true; }
        }

        return Configurable::SetAttribute(aA, aV);
    }

    bool ByteTool::SetAttribute_Indexed(const char* aA, const char* aI, const char* aV)
    {
        if (NULL == aV)
        {
            CFG_IF("ByteTable") { SetByteTable(Convert::ToUInt8(aI), Convert::ToUInt8(aI)); return true; }
        }
        else
        {
            CFG_IF("ByteTable") { SetByteTable(Convert::ToUInt8(aI), Convert::ToUInt8(aV)); return true; }
        }

        return Configurable::SetAttribute_Indexed(aA, aI, aV);
    }

    void ByteTool::DisplayHelp(FILE* aOut) const
    {
        fprintf(aOut,
            "===== KMS::ByteTool =====\n"
            "ByteTable[{Index}]\n"
            "    Set an entry into the byte table to the default value\n"
            "    0 <= Index <= 255\n"
            "    Default: The index\n"
            "ByteTable[{Index}] = {Value}\n"
            "    Set an entry into the byte table\n"
            "    0 <= Index <= 255\n"
            "    0 <= Value <= 255\n"
            "Destination = {Path}\n"
            "    Set the destination\n"
            "Source = {Path}\n"
            "    Set the source\n"
            "    Mandatory\n");

        Configurable::DisplayHelp(aOut);
    }

    // Private
    // //////////////////////////////////////////////////////////////////////

    void ByteTool::CloseFile(FILE* aFile)
    {
        if ((NULL != aFile) && (stderr != aFile) && (stdin != aFile) && (stdout != aFile))
        {
            int lRet = fclose(aFile);
            assert(0 == lRet);
        }
    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::ByteTool& aBT)
{
    const unsigned int* lH = aBT.GetHistrogram();
    assert(NULL != lH);

    unsigned int lMax = lH[0];
    unsigned int lMin = lH[0];
    unsigned int lSum = lH[0];

    aOut << "      x_0   x_1   x_2   x_3   x_4   x_5   x_6   x_7   x_8   x_9   x_A   x_B   x_C   x_D   x_E   x_F" << std::endl;
    aOut << LABELS[0];

    DisplayValue(aOut, lH[0]);

    for (unsigned int i = 1; i < BYTE_QTY; i++)
    {
        if (0 == i % 16)
        {
            aOut << std::endl << LABELS[i / 16];
        }

        if (lMax < lH[i]) { lMax = lH[i]; }
        if (lMin > lH[i]) { lMin = lH[i]; }

        lSum += lH[i];

        DisplayValue(aOut, lH[i]);
    }

    aOut << std::endl;
    aOut << "Min : " << lMin << std::endl;
    aOut << "Avg : " << lSum / 256.0 << std::endl;
    aOut << "Max : " << lMax << std::endl;
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

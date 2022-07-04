
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Test_TestManager.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Console/Color.h>

#include <KMS/Test/TestManager.h>

namespace KMS
{
    namespace Test
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        int TestManager::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);

            int lResult = __LINE__;

            try
            {
                KMS::Cfg::Configurator lC;
                KMS::Test::TestManager lTM;

                lTM.InitConfigurator(&lC);

                lC.Init();
                lC.ParseArguments(aCount - 1, aVector + 1);

                lResult = lTM.Run();

                std::cerr << lTM;
            }
            KMS_CATCH_RESULT(lResult);

            return lResult;
        }

        TestManager::TestManager() : mErrorCount(0) {}

        void TestManager::AddGroup(const char * aGroup)
        {
            assert(NULL != aGroup);

            Test::TestList* lTests = Test::GetTests();
            if (NULL != lTests)
            {
                for (Test* lT : *lTests)
                {
                    if (lT->IsGroup(aGroup))
                    {
                        mTests.push_back(lT);
                    }
                }
            }
        }

        void TestManager::AddTest(const char* aTest)
        {
            assert(NULL != aTest);

            Test::TestList* lTests = Test::GetTests();
            if (NULL != lTests)
            {
                for (Test* lT : *lTests)
                {
                    if (lT->IsName(aTest))
                    {
                        mTests.push_back(lT);
                    }
                }
            }
        }

        unsigned int TestManager::GetErrorCount() const { return mErrorCount; }

        int TestManager::Run()
        {
            for (Test* lT : mTests)
            {
                lT->CallRun();

                mErrorCount += lT->GetErrorCount();
            }

            return (0 >= mErrorCount) ? 0 : 98;
        }

        // ===== Cfg::Configurable ==========================================

        bool TestManager::AddAttribute(const char* aA, const char* aV)
        {
            assert(NULL != aA);

            CFG_CALL("Group", AddGroup);
            CFG_CALL("Test" , AddTest );

            return false;
        }

        void TestManager::DisplayHelp(FILE* aOut) const
        {
            fprintf(aOut,
                "===== KMS::Test::TestManager =====\n"
                "Group += {Name}\n"
                "    Add the test of the group to the tests to run\n"
                "Test += {Name}\n"
                "    Add the test to the tests to run\n");

            Configurable::DisplayHelp(aOut);
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Test::TestList* TestManager::GetTests() const { return &mTests; }
    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Test::TestManager& aTM)
{
    const KMS::Test::Test::TestList* lTests = aTM.GetTests();

    unsigned int lIndex = 0;

    for (const KMS::Test::Test* lT : *lTests)
    {
        aOut << "Test " << lIndex << std::endl;
        aOut << *lT;
        aOut << std::endl;

        lIndex++;
    }

    unsigned int lErrorCount = aTM.GetErrorCount();
    if (0 < lErrorCount)
    {
        aOut << KMS::Console::Color::RED;
    }
    else
    {
        aOut << KMS::Console::Color::GREEN;
    }

    aOut << "Test count        : " << lTests->size() << std::endl;
    aOut << "Total error count : " << lErrorCount    << std::endl;

    if (0 < lErrorCount)
    {
        aOut << "Some tests FAILED" << std::endl;
    }
    else
    {
        aOut << "All tests PASSED" << std::endl;
    }

    aOut << KMS::Console::Color::WHITE;

    return aOut;
}

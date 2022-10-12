
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
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/Console/Color.h>

#include <KMS/Test/TestManager.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

static const KMS::Cfg::MetaData MD_GROUPS("Groups+={Name}");
static const KMS::Cfg::MetaData MD_TESTS ("Tests+={Name}" );

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

                lC.AddConfigurable(&lTM);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseArguments(aCount - 1, aVector + 1);

                Dbg::gLog.CloseLogFiles();

                lResult = lTM.Run();

                std::cerr << lTM;
            }
            KMS_CATCH_RESULT(lResult);

            return lResult;
        }

        TestManager::TestManager() : mErrorCount(0)
        {
            mGroups.SetCreator(DI::String::Create);
            mTests .SetCreator(DI::String::Create);

            AddEntry("Groups", &mGroups, false, &MD_GROUPS);
            AddEntry("Tests" , &mTests , false, &MD_TESTS);
        }

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
                        AddTest(lT);
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
                        AddTest(lT);
                    }
                }
            }
        }

        unsigned int TestManager::GetErrorCount() const { return mErrorCount; }

        int TestManager::Run()
        {
            UpdateTestList();

            for (Test* lT : mTestList)
            {
                lT->CallRun();

                mErrorCount += lT->GetErrorCount();
            }

            return (0 >= mErrorCount) ? 0 : 98;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Test::TestList* TestManager::GetTestList() const { return &mTestList; }

        // Private
        // //////////////////////////////////////////////////////////////////

        void TestManager::AddTest(Test* aTest)
        {
            assert(NULL != aTest);

            for (Test* lTest : mTestList)
            {
                assert(NULL != lTest);

                if (aTest == lTest)
                {
                    return;
                }
            }

            mTestList.push_back(aTest);
        }

        void TestManager::UpdateTestList()
        {
            for (const DI::Object* lObj : mGroups.mInternal)
            {
                assert(NULL != lObj);

                const DI::String* lString = dynamic_cast<const DI::String*>(lObj);
                assert(NULL != lString);

                AddGroup(*lString);
            }

            for (const DI::Object* lObj : mTests.mInternal)
            {
                assert(NULL != lObj);

                const DI::String* lString = dynamic_cast<const DI::String*>(lObj);
                assert(NULL != lString);

                AddTest(*lString);
            }
        }

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Test::TestManager& aTM)
{
    const KMS::Test::Test::TestList* lTestList = aTM.GetTestList();

    unsigned int lIndex = 0;

    for (const KMS::Test::Test* lT : *lTestList)
    {
        aOut << "Test " << lIndex << "\n";
        aOut << *lT;
        aOut << "\n";

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

    aOut << "Test count        : " << lTestList->size() << "\n";
    aOut << "Total error count : " << lErrorCount       << "\n";

    if (0 < lErrorCount)
    {
        aOut << "Some tests FAILED";
    }
    else
    {
        aOut << "All tests PASSED";
    }

    aOut << KMS::Console::Color::WHITE << std::endl;

    return aOut;
}

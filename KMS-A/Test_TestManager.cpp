
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Test_TestManager.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/Console/Color.h>
#include <KMS/Main.h>

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
            assert(nullptr != aVector);

            KMS::Dbg::gLog.mConsoleLevel = Dbg::LogFile::Level::LEVEL_NONE;

            KMS_MAIN_BEGIN;
            {
                TestManager lTM;

                lConfigurator.AddConfigurable(&lTM);

                KMS_MAIN_PARSE_ARGS(aCount, aVector);

                KMS_MAIN_VALIDATE;

                lResult = lTM.Run();

                std::cerr << lTM;
            }
            KMS_MAIN_END;

            KMS_MAIN_RETURN;
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
            assert(nullptr != aGroup);

            auto lTests = Test::GetTests();
            if (nullptr != lTests)
            {
                for (auto lT : *lTests)
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
            assert(nullptr != aTest);

            auto lTests = Test::GetTests();
            if (nullptr != lTests)
            {
                for (auto lT : *lTests)
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

            auto lTT = new Dbg::Stats_Timer("TestTime");

            for (auto lT : mTestList)
            {
                lTT->Start();

                lT->mConsole = mConsole;

                lT->CallRun();

                lTT->Stop();

                mErrorCount += lT->GetErrorCount();
            }

            return (0 >= mErrorCount) ? 0 : 98;
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        const Test::TestList* TestManager::GetTestList() const { return &mTestList; }

        void TestManager::Display(std::ostream& aOut) const
        {
            unsigned int lIndex = 0;

            for (auto lT : mTestList)
            {
                aOut << "Test " << lIndex << " - ";
                aOut << *lT;

                lIndex++;
            }

            if (0 < mErrorCount)
            {
                aOut << Console::Color::RED;
            }
            else
            {
                aOut << Console::Color::GREEN;
            }

            aOut << "Test count        : " << mTestList.size() << "\n";
            aOut << "Total error count : " << mErrorCount << "\n";

            if (0 < mErrorCount)
            {
                aOut << "Some tests FAILED";
            }
            else
            {
                aOut << "All tests PASSED";
            }

            aOut << Console::Color::WHITE << std::endl;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void TestManager::AddTest(Test* aTest)
        {
            assert(nullptr != aTest);

            for (auto lTest : mTestList)
            {
                assert(nullptr != lTest);

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
                assert(nullptr != lObj);

                auto lString = dynamic_cast<const DI::String*>(lObj);
                assert(nullptr != lString);

                AddGroup(*lString);
            }

            for (const DI::Object* lObj : mTests.mInternal)
            {
                assert(nullptr != lObj);

                auto lString = dynamic_cast<const DI::String*>(lObj);
                assert(nullptr != lString);

                AddTest(*lString);
            }
        }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const Test::TestManager& aTM)
{
    aTM.Display(aOut);

    return aOut;
}

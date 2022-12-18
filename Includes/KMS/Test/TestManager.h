
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Test/TestManager.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/Test/Test.h>

namespace KMS
{
    namespace Test
    {

        class TestManager : public DI::Dictionary
        {

        public:

            static int Main(int aCount, const char** aVector);

            TestManager();

            void AddGroup(const char* aGroup);
            void AddTest (const char* aTest);

            unsigned int GetErrorCount() const;

            int Run();

        // internal

            const Test::TestList* GetTestList() const;

            void Display(std::ostream& aOut) const;

        private:

            TestManager(const TestManager&);

            void AddTest(Test* aTest);

            void UpdateTestList();

            void operator = (const TestManager&);

            unsigned int mErrorCount;

            Test::TestList mTestList;

            // ===== Configurable attributes ================================
            DI::Array mGroups;
            DI::Array mTests;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Test::TestManager & aTM);

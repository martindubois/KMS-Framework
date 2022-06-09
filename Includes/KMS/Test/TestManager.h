
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Test/TestManager.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Config/Configurable.h>
#include <KMS/Test/Test.h>

namespace KMS
{
    namespace Test
    {

        class TestManager : public Config::Configurable
        {

        public:

            static int Main(int aCount, const char** aVector);

            TestManager();

            void AddGroup(const char* aGroup);
            void AddTest (const char* aTest);

            unsigned int GetErrorCount() const;

            int Run();

            // ===== Config::Configurable ===================================
            bool AddAttribute(const char* aA, const char* aValue);

        // internal

            const Test::TestList* GetTests() const;

        private:

            TestManager(const TestManager&);

            const TestManager& operator = (const TestManager&);

            unsigned int mErrorCount;

            // ===== Configurable attributes ================================
            Test::TestList mTests;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Test::TestManager & aTM);

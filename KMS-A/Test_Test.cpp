
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Test_Test.cpp

#include "Component.h"

// ===== C++ ================================================================
#include <iostream>

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>

#include <KMS/Test/Test.h>

// Static variables
// //////////////////////////////////////////////////////////////////////////

static KMS::Test::Test::TestList* sTests = NULL;

namespace KMS
{
    namespace Test
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        bool Test::Assert(bool aCondition, const char* aFileName, unsigned int aLine)
        {
            assert(NULL != aFileName);
            assert(0 < aLine);

            if (!aCondition)
            {
                DisplayTestName();
                std::cerr << "Assert failure at line " << aLine << " of " << aFileName << Console::Color::WHITE << std::endl;
                mErrorCount++;
            }

            return aCondition;
        }

        void Test::Cleanup() {};

        #define COMPARE(T)                                                                             \
            bool Test::Compare(T aValue, T aExpected, const char* aFileName, unsigned int aLine)       \
            {                                                                                          \
                bool lResult = Assert(aValue == aExpected, aFileName, aLine);                          \
                if (!lResult)                                                                          \
                {                                                                                      \
                    std::cerr << Console::Color::RED;                                                  \
                    std::cerr << "    Value    : " << aValue    << "\n";                               \
                    std::cerr << "    Expected : " << aExpected << Console::Color::WHITE << std::endl; \
                }                                                                                      \
                return lResult;                                                                        \
            }

        COMPARE(Exception::Code);
        COMPARE(         int   );
        COMPARE(unsigned int   );

        unsigned int Test::GetErrorCount() const { return mErrorCount; }
        const char * Test::GetName      () const { return mName.c_str(); }

        bool Test::IsGroup(const char* aGroup) const { assert(NULL != aGroup); return mGroup == aGroup; }
        bool Test::IsName (const char* aName ) const { assert(NULL != aName ); return mName  == aName ; }

        // Internal
        // //////////////////////////////////////////////////////////////////

        Test::TestList* Test::GetTests() { return sTests; }

        void Test::CallRun()
        {
            try
            {
                std::cout << "Executing " << mName << " ..." << std::endl;
                Run();
                Cleanup();
            }
            catch (Exception eE)
            {
                DisplayTestName();
                std::cerr << "EXCEPTION" << "\n";
                std::cerr << eE;
                std::cerr << Console::Color::WHITE << std::endl;

                mErrorCount++;
            }
            catch (std::exception eE)
            {
                DisplayTestName();
                std::cerr << "EXCEPTION" << "\n";
                std::cerr << eE.what() << "\n";
                std::cerr << Console::Color::WHITE << std::endl;

                mErrorCount++;
            }
            catch (...)
            {
                DisplayTestName();
                std::cerr << "UNKNOWN EXCEPTION" << "\n";
                std::cerr << Console::Color::WHITE << std::endl;

                mErrorCount++;
            }
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Test::Test(const char* aName, const char* aGroup) : mErrorCount(0), mGroup(aGroup), mName(aName)
        {
            assert(NULL != aName);
            assert(NULL != aGroup);

            if (NULL == sTests)
            {
                sTests = new TestList;
            }

            sTests->push_back(this);
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Test::DisplayTestName() { std::cerr << Console::Color::RED << "Test : " << mName << std::endl; }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const Test::Test& aT)
{
    unsigned int lErrorCount = aT.GetErrorCount();
    if (0 < lErrorCount)
    {
        aOut << Console::Color::RED;
    }
    else
    {
        aOut << Console::Color::GREEN;
    }

    aOut << "    " << aT.GetName() << " - " << lErrorCount << " error(s)" << "\n";
    
    if (0 < lErrorCount)
    {
        aOut << "    FAILED";
    }
    else
    {
        aOut << "    PASSED";
    }

    aOut << Console::Color::WHITE << std::endl;

    return aOut;
}

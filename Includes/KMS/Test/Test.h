
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Test/Test.h

#pragma once

// ===== C++ ================================================================
#include <list>
#include <string>

// ===== Includes ===========================================================
#include <KMS/Console/Color.h>
#include <KMS/Console/Console.h>

namespace KMS
{
    namespace Test
    {

        class Test
        {

        public:

            static const char* GROUP_AUTO;

            bool Assert(bool aCondition, const char* aFile, unsigned int aLine);

            bool Compare(Exception::Code aValue, Exception::Code aExpected, const char* aFile, unsigned int aLine);
            bool Compare(         int    aValue,          int    aExpected, const char* aFile, unsigned int aLine);
            bool Compare(unsigned int    aValue, unsigned int    aExpected, const char* aFile, unsigned int aLine);

            virtual void Cleanup();

            unsigned int GetErrorCount() const;
            const char * GetName() const;

            bool IsGroup(const char* aGroup) const;
            bool IsName (const char* aName ) const;

            virtual void Run() = 0;

        // internal

            typedef std::list<Test*> TestList;

            static TestList* GetTests();

            void CallRun();

            void Display(std::ostream& aOut) const;

            Console::Console mConsole;

        protected:

            Test(const char* aName, const char* aGroup);

            void DisplayTestName();

        private:

            NO_COPY(Test);

            unsigned int mErrorCount;
            std::string  mGroup;
            std::string  mName;

        };

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::Test::Test& aT);

#define KMS_TEST(C,N,G,V)            \
    class C : public KMS::Test::Test \
    {                                \
    public:                          \
        C();                         \
        virtual void Run();          \
    };                               \
    static C V;                      \
    C::C() : Test(N, G) {};          \
    void C::Run()

#define KMS_TEST_ASSERT(C)        Assert(C, __FILE__, __LINE__)
#define KMS_TEST_ASSERT_BREAK(C)  if (!Assert(C, __FILE__, __LINE__)) { break; }
#define KMS_TEST_ASSERT_RETURN(C) if (!Assert(C, __FILE__, __LINE__)) { return; }

#define KMS_TEST_COMPARE(V, E)        Compare(V, E, __FILE__, __LINE__)
#define KMS_TEST_COMPARE_BREAK(V, E)  if (!Compare(V, E, __FILE__, __LINE__)) { break; }
#define KMS_TEST_COMPARE_RETURN(V, E) if (!Compare(V, E, __FILE__, __LINE__)) { return; }

#define KMS_TEST_CATCH(C)                                                    \
    catch(KMS::Exception eE)                                                 \
    {                                                                        \
        Compare(eE.GetCode(), KMS::Exception::Code::C, __FILE__, __LINE__ ); \
    }

#define KMS_TEST_CATCH_BREAK(C)                                                              \
    catch(KMS::Exception eE)                                                                 \
    {                                                                                        \
        if (!Compare(eE.GetCode(), KMS::Exception::Code::C, __FILE__, __LINE__ )) { break; } \
    }

#define KMS_TEST_CATCH_RETURN(C)                                                              \
    catch(KMS::Exception eE)                                                                  \
    {                                                                                         \
        if (!Compare(eE.GetCode(), KMS::Exception::Code::C, __FILE__, __LINE__ )) { return; } \
    }

#define KMS_TEST_OUTPUT_BEGIN()                                        \
    mConsole.OutputStream() << KMS::Console::Color::BLUE;              \
    mConsole.OutputStream() << "TEST OUTPUT BEGIN";                    \
    mConsole.OutputStream() << KMS::Console::Color::WHITE << std::endl

#define KMS_TEST_OUTPUT_END()                                          \
    mConsole.OutputStream() << KMS::Console::Color::BLUE;              \
    mConsole.OutputStream() << "TEST OUTPUT END";                      \
    mConsole.OutputStream() << KMS::Console::Color::WHITE << std::endl

#define KMS_TEST_CATCH_OUTPUT_END(C)                                         \
    catch(KMS::Exception eE)                                                 \
    {                                                                        \
        mConsole.OutputStream() << KMS::Console::Color::BLUE;                \
        mConsole.OutputStream() << "TEST OUTPUT END";                        \
        mConsole.OutputStream() << KMS::Console::Color::WHITE << std::endl;  \
        Compare(eE.GetCode(), KMS::Exception::Code::C, __FILE__, __LINE__ ); \
    }

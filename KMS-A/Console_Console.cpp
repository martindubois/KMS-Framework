
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Console_Console.cpp

// TEST COVERAGE 2023-08-01 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Console.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#if defined( _KMS_DARWIN_ ) || defined( _KMS_LINUX_ )
    #define NULL_DEV "/dev/null"
#endif

#ifdef _KMS_WINDOWS_
    #define NULL_DEV "NUL:"
#endif

namespace KMS
{
    namespace Console
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        const unsigned int Console::FLAG_ERROR  = 0x00000001;
        const unsigned int Console::FLAG_OUTPUT = 0x00000002;

        const unsigned int Console::FLAG_FILE   = 0x00000100;
        const unsigned int Console::FLAG_STREAM = 0x00000200;

        Console::Console() : mFile(nullptr) { Set_Default(); }

        Console::~Console() { CloseFile(); }

        void Console::operator = (Console& aInteract)
        {
            CloseFile();

            mErrorFile    = aInteract.mErrorFile;
            mErrorStream  = aInteract.mErrorStream;
            mOutputFile   = aInteract.mOutputFile;
            mOutputStream = aInteract.mOutputStream;
        }

        void Console::Set(const char* aFileName, unsigned int aFlags)
        {
            assert(nullptr != aFileName);

            CloseFile();

            if (0 != (aFlags & FLAG_FILE))
            {
                auto lRet = fopen_s(&mFile, aFileName, "wb");
                assert(0 == lRet);

                if (0 != (aFlags & FLAG_ERROR))
                {
                    mErrorFile = mFile;
                }
                if (0 != (aFlags & FLAG_OUTPUT))
                {
                    mOutputFile = mFile;
                }
            }

            if (0 != (aFlags & FLAG_STREAM))
            {
                mStream = std::ofstream(aFileName);
                if (0 != (aFlags & FLAG_ERROR))
                {
                    mErrorStream = &mStream;
                }
                if (0 != (aFlags & FLAG_OUTPUT))
                {
                    mOutputStream = &mStream;
                }
            }
        }

        void Console::Set_Default()
        {
            CloseFile();

            mErrorFile   = stderr;
            mErrorStream = &std::cerr;

            mOutputFile   = stdout;
            mOutputStream = &std::cout;
        }

        void Console::Set_Null()
        {
            Set(NULL_DEV, FLAG_ERROR | FLAG_OUTPUT | FLAG_FILE | FLAG_STREAM);
        }

        FILE        * Console::ErrorFile   () const { return  mErrorFile; }
        std::ostream& Console::ErrorStream () const { return *mErrorStream; }

        FILE        * Console::OutputFile  () const { return  mOutputFile; }
        std::ostream& Console::OutputStream() const { return *mOutputStream; }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Console::CloseFile()
        {
            if (nullptr != mFile)
            {
                auto lRet = fclose(mFile);
                assert(0 == lRet);
            }
        }

    }
}


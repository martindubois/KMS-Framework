
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Console/Console.h
// Status    PROD_READY
// Library   KMS-A

#pragma once

// ===== C++ ================================================================
#include <fstream>

namespace KMS
{
    namespace Console
    {

        class Console
        {

        public:

            static const unsigned int FLAG_ERROR;
            static const unsigned int FLAG_OUTPUT;

            static const unsigned int FLAG_FILE;
            static const unsigned int FLAG_STREAM;

            Console();

            ~Console();

            void operator = (Console& aConsole);

            // aFlags  FLAG_...
            void Set(const char* aFileName, unsigned int aFlags);

            void Set_Default();

            void Set_Null();

            std::ostream& ErrorStream() const;
            FILE        * ErrorFile  () const;

            std::ostream& OutputStream() const;
            FILE        * OutputFile  () const;

        private:

            void CloseFile();

            FILE        * mErrorFile;
            std::ostream* mErrorStream;

            FILE* mFile;

            FILE        * mOutputFile;
            std::ostream* mOutputStream;

            std::ofstream mStream;

        };

    }
}

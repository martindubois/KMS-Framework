
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/CLI/CommandLine.h
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Types.h>

namespace KMS
{
    namespace CLI
    {

        class CommandLine
        {

        public:

            CommandLine(const char* aLine);

            CommandLine(int aCount, const char** aVector);

            const char* GetCurrent() const;

            void GetRemaining(char* aOut, unsigned int aOutSize_byte);

            bool IsAtEnd() const;

            void Next();

        private:

            StringList_ASCII::iterator mCurrent;

            StringList_ASCII mParts;

        };

    }
}

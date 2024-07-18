
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/CLI_CommandLine.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/CLI/CommandLine.h>

namespace KMS
{
    namespace CLI
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        CommandLine::CommandLine(const char* aLine)
        {
            assert(nullptr != aLine);

            // --> INIT <==+----+
            //     | |     |    |
            //     | +--> PART  |
            //     |       |    |
            //     +-------+==> QUOTED
            typedef enum
            {
                STATE_INIT,
                STATE_PART,
                STATE_QUOTED,
            }
            State;

            unsigned int lIndex;
            char lPart[LINE_LENGTH];
            auto lPtr = aLine;
            auto lState = STATE_INIT;

            while ('\0' != *lPtr)
            {
                switch (lState)
                {
                case STATE_INIT:
                    switch (*lPtr)
                    {
                    case ' ':
                    case '\t': break;

                    case '"':
                        lIndex = 0;
                        lState = STATE_QUOTED;
                        break;

                    default:
                        lIndex = 1;
                        lPart[0] = *lPtr;
                        lState = STATE_PART;
                    }
                    break;

                case STATE_PART:
                    switch (*lPtr)
                    {
                    case ' ':
                    case '\t':
                        lPart[lIndex] = '\0';
                        lState = STATE_INIT;
                        mParts.push_back(lPart);
                        break;

                    case '"':
                        lState = STATE_QUOTED;
                        break;

                    default:
                        lPart[lIndex] = *lPtr;
                        lIndex++;
                    }
                    break;

                case STATE_QUOTED:
                    switch (*lPtr)
                    {
                    case '"':
                        lPart[lIndex] = '\0';
                        lState = STATE_INIT;
                        mParts.push_back(lPart);
                        break;

                    default:
                        lPart[lIndex] = *lPtr;
                        lIndex++;
                    }
                    break;

                default: assert(false);
                }

                lPtr++;
            }

            switch (lState)
            {
            case STATE_INIT: break;

            case STATE_PART:
            case STATE_QUOTED:
                lPart[lIndex] = '\0';
                mParts.push_back(lPart);
                break;

            default: assert(false);
            }

            Reset();
        }

        CommandLine::CommandLine(int aCount, const char** aVector)
        {
            assert(0 < aCount);
            assert(nullptr != aVector);

            for (int i = 0; i < aCount; i++)
            {
                assert(nullptr != aVector[i]);

                mParts.push_back(aVector[i]);
            }

            Reset();

            Next(); // Skip the executable name
        }

        const char* CommandLine::GetCurrent() const
        {
            KMS_EXCEPTION_ASSERT(!IsAtEnd(), RESULT_INVALID_COMMAND, "Incomplete command", "");

            return mCurrent->c_str();
        }

        void CommandLine::GetRemaining(char* aOut, unsigned int aOutSize_byte)
        {
            assert(nullptr != aOut);

            auto lOut = aOut;
            auto lOutSize_byte = aOutSize_byte;

            while (!IsAtEnd())
            {
                auto lPart = GetCurrent();

                strcpy_s(lOut SizeInfoV(lOutSize_byte), lPart);

                auto lSize_byte = static_cast<unsigned int>(strlen(lPart));

                lOut += lSize_byte;
                lOutSize_byte -= lSize_byte;

                Next();
            }
        }

        bool CommandLine::IsAtEnd() const { return mParts.end() == mCurrent; }

        void CommandLine::Next() { mCurrent++; }

        void CommandLine::Reset() { mCurrent = mParts.begin(); }

    }
}

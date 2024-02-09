
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Environment.cpp

// TEST COVERAGE 2023-08-01 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Environment.h>

namespace KMS
{
    namespace Env
    {

        // Functions
        // //////////////////////////////////////////////////////////////////////

        #define APPEND_CHAR(C) *lOut = (C); lOut++; lOutSize_byte--

        #define APPEND_VAR()                                        \
            lVarName[lVarIndex] = '\0';                             \
            lVarIndex = 0;                                          \
            lLen = GetVariableValue(lVarName, lOut, lOutSize_byte); \
            lOut += lLen; lOutSize_byte -= lLen

        void Expand(const char* aIn, char* aOut, unsigned int aOutSize_byte)
        {
            // --> TEXT <==+-----+
            //      |      |     |
            //      +--> MARK    |
            //            |      |
            //            +--> NAME
            static const unsigned int STATE_MARK = 0;
            static const unsigned int STATE_NAME = 1;
            static const unsigned int STATE_TEXT = 2;

            auto         lIn = aIn;
            unsigned int lLen;
            auto         lOut = aOut;
            auto         lOutSize_byte = aOutSize_byte;
            unsigned int lState = STATE_TEXT;
            char         lVarName[NAME_LENGTH];
            unsigned int lVarIndex = 0;

            while ('\0' != *lIn)
            {
                switch (lState)
                {
                case STATE_MARK:
                    switch (*lIn)
                    {
                    case '$': lState = STATE_NAME; break;

                    default:
                        APPEND_CHAR('{');
                        APPEND_CHAR(*lIn);
                        lState = STATE_TEXT;
                    }
                    break;

                case STATE_NAME:
                    switch (*lIn)
                    {
                    case '}':
                        APPEND_VAR();
                        lState = STATE_TEXT;
                        break;

                    default: lVarName[lVarIndex] = *lIn; lVarIndex++;
                    }
                    break;

                case STATE_TEXT:
                    switch (*lIn)
                    {
                    case '{': lState = STATE_MARK; break;

                    default: APPEND_CHAR(*lIn);
                    }
                    break;

                default: assert(false);
                }

                KMS_EXCEPTION_ASSERT(1 < lOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aIn);

                lIn++;
            }

            switch (lState)
            {
            case STATE_MARK: APPEND_CHAR('{'); break;
            case STATE_NAME: APPEND_VAR(); break;
            case STATE_TEXT: break;

            default: assert(false);
            }

            KMS_EXCEPTION_ASSERT(1 < lOutSize_byte, RESULT_OUTPUT_TOO_SHORT, "The output buffer is too short", aIn);

            *lOut = '\0';
        }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Environment.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Environment.h>

namespace KMS
{
    namespace Env
    {

        // Functions
        // //////////////////////////////////////////////////////////////////////

        void Expand(const char* aIn, char* aOut, unsigned int aOutSize_byte)
        {
            // --> TEXT <==+-----+
            //      |      |     |
            //      +--> MARK    |
            //            |      |
            //            +--> NAME
            const unsigned int STATE_MARK = 0;
            const unsigned int STATE_NAME = 1;
            const unsigned int STATE_TEXT = 2;

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
                        *lOut = '{' ; lOut++; lOutSize_byte--;
                        *lOut = *lIn; lOut++; lOutSize_byte--;
                        lState = STATE_TEXT;
                    }
                    break;

                case STATE_NAME:
                    switch (*lIn)
                    {
                    case '}':
                        lVarName[lVarIndex] = '\0';
                        lVarIndex = 0;

                        lLen = GetVariableValue(lVarName, lOut, lOutSize_byte);

                        lOut += lLen; lOutSize_byte -= lLen;
                        lState = STATE_TEXT;
                        break;

                    default: lVarName[lVarIndex] = *lIn; lVarIndex++;
                    }
                    break;

                case STATE_TEXT:
                    switch (*lIn)
                    {
                    case '{': lState = STATE_MARK; break;

                    default: *lOut = *lIn; lOut++; lOutSize_byte--;
                    }
                    break;

                }

                KMS_EXCEPTION_ASSERT(1 < lOutSize_byte, ENV_OUTPUT_TOO_SHORT, "The output buffer is too short", aIn);

                lIn++;
            }

            *lOut = '\0';
        }

    }
}

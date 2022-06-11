
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Config_Configurable.cpp

#include "Component.h"

// ===== Windows ============================================================
#include <Windows.h>

// ===== Includes ===========================================================
#include <KMS/Config/Configurator.h>

#include <KMS/Config/Configurable.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static unsigned int GetVariableValue(const char* aName, char* aOut, unsigned int aOutSize_byte);

namespace KMS
{
    namespace Config
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Configurable::InitConfigurator(Configurator* aC)
        {
            assert(NULL != aC);

            aC->AddConfigurable(this);
        }

        bool Configurable::AddAttribute(const char*, const char*) { return false; }

        bool Configurable::AddAttribute_Indexed(const char*, const char*, const char*) { return false; }

        bool Configurable::SetAttribute(const char*) { return false; }

        bool Configurable::SetAttribute(const char*, const char*) { return false; }

        bool Configurable::SetAttribute_Indexed(const char*, const char*) { return false; }

        bool Configurable::SetAttribute_Indexed(const char*, const char*, const char*) { return false; }

        // Protected
        // //////////////////////////////////////////////////////////////////

        void Configurable::Expand(const char* aIn, char* aOut, unsigned int aOutSize_byte)
        {
            // --> TEXT <==+-----+
            //      |      |     |
            //      +--> MARK    |
            //            |      |
            //            +--> NAME
            const unsigned int STATE_MARK = 0;
            const unsigned int STATE_NAME = 1;
            const unsigned int STATE_TEXT = 2;

            const char* lIn = aIn;
            unsigned int lLen;
            char* lOut = aOut;
            unsigned int lOutSize_byte = aOutSize_byte;
            unsigned int lState = STATE_TEXT;
            char lVarName[1024];
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

                if (1 >= lOutSize_byte)
                {
                    KMS_EXCEPTION(OUTPUT_TOO_SHORT, "The output buffer is too short");
                }

                lIn++;
            }

            *lOut = '\0';
        }

        Configurable::Configurable() {}

    }
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

unsigned int GetVariableValue(const char* aName, char* aOut, unsigned int aOutSize_byte)
{
    assert(NULL != aName);
    assert(NULL != aOut);

    unsigned int lResult = GetEnvironmentVariable(aName, aOut, aOutSize_byte);
    if ((0 >= lResult) || (aOutSize_byte <= lResult))
    {
        KMS_EXCEPTION_WITH_INFO(CONFIG_EXPAND, "GetEnvironmentVariable failed", aName);
    }

    return lResult;
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/Array.h>
#include <KMS/JSON/Dictionary.h>
#include <KMS/JSON/Value.h>

#include <KMS/JSON/JSON.h>

namespace KMS
{
    namespace JSON
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        unsigned int CreateFromHTTP(const char* aIn, unsigned int aInSize_byte, Base** aOut)
        {
            assert(NULL != aIn);

            Base* lBase = NULL;

            unsigned int lResult_byte = 0;
            while (aInSize_byte > lResult_byte)
            {
                switch (aIn[lResult_byte])
                {
                case ' ':
                case '\n':
                case '\r':
                case '\t': break;

                default: lBase = new Value(); assert(NULL != lBase); break;
                }

                if (NULL != lBase)
                {
                    break;
                }

                lResult_byte++;
            }

            if (NULL == lBase)
            {
                KMS_EXCEPTION(HTTP_FORMAT, "Invalid HTTP format");
            }

            lResult_byte += lBase->HTTP_Set(aIn + lResult_byte, aInSize_byte - lResult_byte);

            *aOut = lBase;

            return lResult_byte;
        }

        unsigned int CreateFromJSON(const char* aIn, unsigned int aInSize_byte, Base** aOut)
        {
            assert(NULL != aIn);

            Base* lBase = NULL;

            unsigned int lResult_byte = 0;
            while (aInSize_byte < lResult_byte)
            {
                switch (aIn[lResult_byte])
                {
                case ' ':
                case '\n':
                case '\r':
                case '\t': break;

                case '[': lBase = new Array     (); assert(NULL != lBase); break;
                case '{': lBase = new Dictionary(); assert(NULL != lBase); break;
                default : lBase = new Value     (); assert(NULL != lBase); break;
                }

                if (NULL != lBase)
                {
                    break;
                }

                lResult_byte++;
            }

            if (NULL == lBase)
            {
                KMS_EXCEPTION(JSON_FORMAT, "Invalid JSON format");
            }

            lResult_byte += lBase->JSON_Set(aIn + lResult_byte, aInSize_byte - lResult_byte);

            *aOut = lBase;

            return lResult_byte;
        }

    }
}

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

        unsigned int CreateFromHTTP(const char* aIn, unsigned int aInSize_byte, Object** aOut)
        {
            assert(NULL != aIn);

            Object* lObj = NULL;

            unsigned int lResult_byte = 0;
            while (aInSize_byte > lResult_byte)
            {
                switch (aIn[lResult_byte])
                {
                case ' ':
                case '\n':
                case '\r':
                case '\t': break;

                default: lObj = new Value(); break;
                }

                if (NULL != lObj)
                {
                    break;
                }

                lResult_byte++;
            }

            if (NULL == lObj)
            {
                KMS_EXCEPTION(HTTP_FORMAT, "Invalid HTTP format");
            }

            lResult_byte += lObj->HTTP_Set(aIn + lResult_byte, aInSize_byte - lResult_byte);

            *aOut = lObj;

            return lResult_byte;
        }

        unsigned int CreateFromJSON(const char* aIn, unsigned int aInSize_byte, Object** aOut)
        {
            assert(NULL != aIn);

            Object* lObj = NULL;

            unsigned int lResult_byte = 0;
            while (aInSize_byte < lResult_byte)
            {
                switch (aIn[lResult_byte])
                {
                case ' ':
                case '\n':
                case '\r':
                case '\t': break;

                case '[': lObj = new Array     (); break;
                case '{': lObj = new Dictionary(); break;
                default : lObj = new Value     (); break;
                }

                if (NULL != lObj)
                {
                    break;
                }

                lResult_byte++;
            }

            if (NULL == lObj)
            {
                KMS_EXCEPTION(JSON_FORMAT, "Invalid JSON format");
            }

            lResult_byte += lObj->JSON_Set(aIn + lResult_byte, aInSize_byte - lResult_byte);

            *aOut = lObj;

            return lResult_byte;
        }

    }
}
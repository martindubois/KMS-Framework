
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON_String.cpp

// TEST COVERAGE

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/String.h>

namespace KMS
{
    namespace JSON
    {

        // Public
        // //////////////////////////////////////////////////////////////////////

        String::String() {}

        String::String(const String& aV) { Set(aV.Get()); }

        String::String(const char* aValue) { Set(aValue); }

        const char* String::Get() const { return mValue.c_str(); }

        void String::Set(const char* aValue) { assert(NULL != aValue); mValue = aValue; }

        // ===== Object =====================================================

        String::~String() {}

        void String::Clear() { mValue.clear(); }

        bool String::IsEmpty() const { return mValue.empty(); }

        unsigned int String::HTTP_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = static_cast<unsigned int>(mValue.size());
            if (aOutSize_byte < lResult_byte + 2)
            {
                KMS_EXCEPTION_WITH_INFO(OUTPUT_TOO_SHORT, "The output buffer is too small", lResult_byte);
            }

            strcpy_s(aOut SizeInfoV(aOutSize_byte), mValue.c_str());

            return lResult_byte;
        }

        unsigned int String::HTTP_Set(const char* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            unsigned int lResult_byte = 0;
            while (aInSize_byte > lResult_byte)
            {
                char lValue[4096];

                switch (aIn[lResult_byte])
                {
                case ' ':
                case '\n':
                case '\r':
                case '\t': break;

                case '"':
                    if (1 != sscanf_s(aIn + lResult_byte + 1, "%[^\"\n\r\t]\"", lValue SizeInfo(lValue)))
                    {
                        KMS_EXCEPTION(HTTP_FORMAT, "Invalid HTTP format");
                    }

                    mValue = lValue;

                    lResult_byte += static_cast<unsigned int>(strlen(lValue));
                    return lResult_byte;

                default:
                    int lRet = sscanf_s(aIn + lResult_byte, "%[^ \n\r\t]", lValue SizeInfo(lValue));
                    assert(1 == lRet);

                    mValue = lValue;

                    lResult_byte += static_cast<unsigned int>(strlen(lValue));
                    return lResult_byte;
                }

                lResult_byte++;
            }

            KMS_EXCEPTION(HTTP_FORMAT, "Invalid HTTP format");
        }

        unsigned int String::JSON_Get(char* aOut, unsigned int aOutSize_byte) const
        {
            assert(NULL != aOut);

            unsigned int lResult_byte = static_cast<unsigned int>(mValue.size());
            if (aOutSize_byte < lResult_byte + 4)
            {
                KMS_EXCEPTION_WITH_INFO(OUTPUT_TOO_SHORT, "The output buffer is too small", lResult_byte);
            }

            sprintf_s(aOut SizeInfoV(aOutSize_byte), "\"%s\"", mValue.c_str());

            return lResult_byte;
        }

        unsigned int String::JSON_Set(const char* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            // TODO
            mValue = aIn;
            return static_cast<unsigned int>(mValue.size());
        }

    }
}

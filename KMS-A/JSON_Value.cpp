
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON_Value.cpp

// TEST COVERAGE 2022-08-23 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/JSON/Value.h>

namespace KMS
{
    namespace JSON
    {

        // Public
        // //////////////////////////////////////////////////////////////////////

        Value::Value() {}

        Value::Value(const Value& aV) { Set(aV.Get()); }

        Value::Value(const char* aValue) { Set(aValue); }

        Value::Value(double aValue) { Set(aValue); }

        Value::Value( int32_t aValue, Radix aRadix) { Set(aValue, aRadix); }
        Value::Value(uint32_t aValue, Radix aRadix) { Set(aValue, aRadix); }

        const char* Value::Get() const { return mValue.c_str(); }

        void Value::Set(const char* aValue) { assert(NULL != aValue); mValue = aValue; }

        void Value::Set(double aValue)
        {
            char lValue[32];

            sprintf_s(lValue, "%f", aValue);

            Set(lValue);
        }

        void Value::Set(int32_t aValue, Radix aRadix)
        {
            char lValue[16];

            switch (aRadix)
            {
            case Radix::DECIMAL    : sprintf_s(lValue, "%d", aValue); break;
            case Radix::HEXADECIMAL: sprintf_s(lValue, "%x", aValue); break;

            default: assert(false);
            }

            Set(lValue);
        }

        void Value::Set(uint32_t aValue, Radix aRadix)
        {
            char lValue[16];

            switch (aRadix)
            {
            case Radix::DECIMAL    : sprintf_s(lValue, "%u", aValue); break;
            case Radix::HEXADECIMAL: sprintf_s(lValue, "%x", aValue); break;

            default: assert(false);
            }

            Set(lValue);
        }

        // ===== Object =====================================================

        Value::~Value() {}

        void Value::Clear() { mValue.clear(); }

        bool Value::IsEmpty() const { return mValue.empty(); }

        unsigned int Value::HTTP_Get(char* aOut, unsigned int aOutSize_byte) const
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

        unsigned int Value::HTTP_Set(const char* aIn, unsigned int aInSize_byte)
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

                    mValue = "\"";
                    mValue += lValue;
                    mValue += "\"";

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

        unsigned int Value::JSON_Get(char* aOut, unsigned int aOutSize_byte) const
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

        unsigned int Value::JSON_Set(const char* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            // TODO
            mValue = aIn;
            return static_cast<unsigned int>(mValue.size());
        }

    }
}

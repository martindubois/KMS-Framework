
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Array.h>
#include <KMS/DI/Boolean.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/Text/ReadPtr.h>
#include <KMS/Text/WritePtr.h>

#include <KMS/JSON/JSON.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Decode(KMS::DI::Object * aObject, KMS::Text::ReadPtr* aPtr);
static void Decode(KMS::DI::Object** aObject, KMS::Text::ReadPtr* aPtr);

static void Decode_Array     (KMS::DI::Array     * aArray     , KMS::Text::ReadPtr* aPtr);
static void Decode_Dictionary(KMS::DI::Dictionary* aDictionary, KMS::Text::ReadPtr* aPtr);

static void Decode_String(KMS::DI::String* aString, KMS::Text::ReadPtr* aPtr);
static void Decode_Value (KMS::DI::Value * aString, KMS::Text::ReadPtr* aPtr);

static void Encode(const KMS::DI::Object* aObject, KMS::Text::WritePtr* aPtr);

static void Encode_Array     (const KMS::DI::Array     * aArray     , KMS::Text::WritePtr* aPtr);
static void Encode_Dictionary(const KMS::DI::Dictionary* aDictionary, KMS::Text::WritePtr* aPtr);
static void Encode_String    (const KMS::DI::String    * aString    , KMS::Text::WritePtr* aPtr);

namespace KMS
{
    namespace JSON
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        unsigned int Decode(DI::Object* aObject, const char* aIn, unsigned int aInSize_byte)
        {
            Text::ReadPtr lPtr(aIn, aInSize_byte);

            ::Decode(aObject, &lPtr);

            return lPtr.GetIndex();
        }

        unsigned int Decode(DI::Object** aObject, const char* aIn, unsigned int aInSize_byte)
        {
            Text::ReadPtr lPtr(aIn, aInSize_byte);

            ::Decode(aObject, &lPtr);

            return lPtr.GetIndex();
        }

        unsigned int Encode(const DI::Object* aObject, char* aOut, unsigned int aOutSize_byte)
        {
            Text::WritePtr lPtr(aOut, aOutSize_byte);

            ::Encode(aObject, &lPtr);

            return lPtr.GetIndex();
        }

        unsigned int Encode_Dictionary(const DI::Dictionary* aDictionary, char* aOut, unsigned int aOutSize_byte)
        {
            Text::WritePtr lPtr(aOut, aOutSize_byte);

            ::Encode_Dictionary(aDictionary, &lPtr);

            return lPtr.GetIndex();
        }

    }
}

using namespace KMS;

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Decode(DI::Object* aObject, Text::ReadPtr* aPtr)
{
    assert(nullptr != aObject);

    // Dictionary must be tested before Array because Dictionary is an Array
    auto lDictionary = dynamic_cast<DI::Dictionary*>(aObject);
    if (nullptr != lDictionary)
    {
        Decode_Dictionary(lDictionary, aPtr);
    }
    else
    {
        auto lArray = dynamic_cast<DI::Array*>(aObject);
        if (nullptr != lArray)
        {
            Decode_Array(lArray, aPtr);
        }
        else
        {
            // String must be tested before Value because String is a Value
            auto lString = dynamic_cast<DI::String*>(aObject);
            if (nullptr != lString)
            {
                Decode_String(lString, aPtr);
            }
            else
            {
                auto lValue = dynamic_cast<DI::Value*>(aObject);
                KMS_EXCEPTION_ASSERT(nullptr != lValue, RESULT_NOT_IMPLEMENTED, "JSON does not suppoert this data type", "");

                Decode_Value(lValue, aPtr);
            }
        }
    }
}

void Decode(DI::Object** aObject, Text::ReadPtr* aPtr)
{
    assert(nullptr != aObject);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    DI::Array         * lArray;
    DI::Boolean       * lBoolean;
    DI::Dictionary    * lDictionary;
    DI::String        * lString;
    DI::UInt<uint32_t>* lUInt32;

    switch (*lPtr)
    {
    case '[':
        lArray = new DI::Array;
        Decode_Array(lArray, &lPtr);
        *aObject = lArray;
        break;

    case '{':
        lDictionary = new DI::Dictionary;
        Decode_Dictionary(lDictionary, &lPtr);
        *aObject = lDictionary;
        break;

    case '"':
        lString = new DI::String;
        Decode_String(lString, &lPtr);
        *aObject = lString;
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        lUInt32 = new DI::UInt<uint32_t>;
        Decode_Value(lUInt32, &lPtr);
        *aObject = lUInt32;
        break;

    case 'f':
    case 't':
        lBoolean = new DI::Boolean;
        Decode_Value(lBoolean, &lPtr);
        *aObject = lBoolean;
        break;

    default: KMS_EXCEPTION(RESULT_INVALID_FORMAT, "Invalid JSON format", "");
    }

    *aPtr = lPtr;
}

void Decode_Array(DI::Array* aArray, Text::ReadPtr* aPtr)
{
    assert(nullptr != aArray);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    lPtr.Test('[');

    lPtr.SkipBlank();

    if (']' != *lPtr)
    {
        unsigned int lIndex = 0;

        for (;;)
        {
            auto lObject = aArray->GetEntry_RW(lIndex);
            if (nullptr != lObject)
            {
                Decode(lObject, &lPtr);
            }
            else
            {
                Decode(&lObject, &lPtr);
                assert(nullptr != lObject);

                aArray->AddEntry(lObject, true);
            }

            lIndex++;

            lPtr.SkipBlank();
            if (']' == *lPtr)
            {
                break;
            }

            lPtr.Test(',');

            lPtr.SkipBlank();
        }
    }

    lPtr.Test(']');

    *aPtr = lPtr;
}

void Decode_Dictionary(DI::Dictionary* aDictionary, Text::ReadPtr* aPtr)
{
    assert(nullptr != aDictionary);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    lPtr.Test('{');

    lPtr.SkipBlank();

    if ('}' != *lPtr)
    {
        char lName[NAME_LENGTH];

        for (;;)
        {
            lPtr.Test('"');
            lPtr.ExtractUntil('"', lName, sizeof(lName));
            lPtr.Test('"');

            lPtr.SkipBlank();

            lPtr.Test(':');

            lPtr.SkipBlank();

            auto lObject = aDictionary->GetEntry_RW(lName);
            if (nullptr != lObject)
            {
                Decode(lObject, &lPtr);
            }
            else
            {
                Decode(&lObject, &lPtr);
                assert(nullptr != lObject);

                aDictionary->AddEntry(lName, lObject, true);
            }

            lPtr.SkipBlank();
            if ('}' == *lPtr)
            {
                break;
            }

            lPtr.Test(',');

            lPtr.SkipBlank();
        }
    }

    lPtr.Test('}');

    *aPtr = lPtr;
}

void Decode_String(DI::String* aString, Text::ReadPtr* aPtr)
{
    assert(nullptr != aString);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    char lStr[LINE_LENGTH];

    lPtr.Test('"');
    lPtr.ExtractUntil('"', lStr, sizeof(lStr));
    lPtr.Test('"');

    aString->Set(lStr);

    *aPtr = lPtr;
}

void Decode_Value(DI::Value* aValue, Text::ReadPtr* aPtr)
{
    assert(nullptr != aValue);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    char lStr[LINE_LENGTH];

    lPtr.ExtractUntil(" \n\r\t,]}", lStr, sizeof(lStr));

    aValue->Set(lStr);

    *aPtr = lPtr;
}

void Encode(const DI::Object* aObject, Text::WritePtr* aPtr)
{
    assert(nullptr != aObject);
    assert(nullptr != aPtr);

    Text::WritePtr lPtr(*aPtr);

    // Dictionary must be tested before Array because Dictionary is an Array
    auto lDictionary = dynamic_cast<const DI::Dictionary*>(aObject);
    if (nullptr != lDictionary)
    {
        Encode_Dictionary(lDictionary, &lPtr);
    }
    else
    {
        auto lArray = dynamic_cast<const DI::Array*>(aObject);
        if (nullptr != lArray)
        {
            Encode_Array(lArray, &lPtr);
        }
        else
        {
            // String must be tested before Value because String is a Value
            auto lString = dynamic_cast<const DI::String*>(aObject);
            if (nullptr != lString)
            {
                Encode_String(lString, &lPtr);
            }
            else
            {
                auto lValue = dynamic_cast<const DI::Value*>(aObject);
                KMS_EXCEPTION_ASSERT(nullptr != lValue, RESULT_NOT_IMPLEMENTED, "JSON do not support this data type", "");

                lPtr += lValue->Get(lPtr, lPtr.GetRemainingSize());
            }
        }
    }

    *aPtr = lPtr;
}

void Encode_Array(const DI::Array* aArray, Text::WritePtr* aPtr)
{
    assert(nullptr != aArray);
    assert(nullptr != aPtr);

    Text::WritePtr lPtr(*aPtr);

    lPtr.Write('[');

    auto lFirst = true;

    for (const DI::Object* lObj : aArray->mInternal)
    {
        assert(nullptr != lObj);

        if (lFirst)
        {
            lFirst = false;
        }
        else
        {
            lPtr.Write(',');
        }

        Encode(lObj, &lPtr);
    }

    lPtr.Write(']');

    *aPtr = lPtr;
}

void Encode_Dictionary(const DI::Dictionary* aDictionary, Text::WritePtr* aPtr)
{
    assert(nullptr != aDictionary);
    assert(nullptr != aPtr);

    Text::WritePtr lPtr(*aPtr);

    lPtr.Write('{');

    auto lFirst = true;

    for (const auto& lVT : aDictionary->mInternal)
    {
        assert(nullptr != lVT.second);

        if (lFirst)
        {
            lFirst = false;
        }
        else
        {
            lPtr.Write(',');
        }

        lPtr.Write('"');
        lPtr.Write(lVT.first.c_str(), static_cast<unsigned int>(lVT.first.size()));
        lPtr.Write('"');

        lPtr.Write(':');

        ::Encode(lVT.second, &lPtr);
    }

    lPtr.Write('}');

    *aPtr = lPtr;
}

void Encode_String(const DI::String* aString, Text::WritePtr* aPtr)
{
    assert(nullptr != aString);
    assert(nullptr != aPtr);

    Text::WritePtr lPtr(*aPtr);

    lPtr.Write('"');
    lPtr += aString->Get(lPtr, lPtr.GetRemainingSize());
    lPtr.Write('"');

    *aPtr = lPtr;
}

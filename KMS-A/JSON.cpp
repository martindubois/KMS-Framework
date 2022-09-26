
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt32.h>
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

// Static functions
// //////////////////////////////////////////////////////////////////////////

void Decode(KMS::DI::Object* aObject, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aObject);

    // Dictionary must be tested before Array because Dictionary is an Array
    KMS::DI::Dictionary* lDictionary = dynamic_cast<KMS::DI::Dictionary*>(aObject);
    if (NULL != lDictionary)
    {
        Decode_Dictionary(lDictionary, aPtr);
    }
    else
    {
        KMS::DI::Array* lArray = dynamic_cast<KMS::DI::Array*>(aObject);
        if (NULL != lArray)
        {
            Decode_Array(lArray, aPtr);
        }
        else
        {
            // String must be tested before Value because String is a Value
            KMS::DI::String* lString = dynamic_cast<KMS::DI::String*>(aObject);
            if (NULL != lString)
            {
                Decode_String(lString, aPtr);
            }
            else
            {
                KMS::DI::Value* lValue = dynamic_cast<KMS::DI::Value*>(aObject);
                if (NULL != lValue)
                {
                    Decode_Value(lValue, aPtr);
                }
                else
                {
                    KMS_EXCEPTION(NOT_IMPLEMENTED, "JSON do not suppoert this data type");
                }
            }
        }
    }
}

void Decode(KMS::DI::Object** aObject, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aObject);
    assert(NULL != aPtr);

    KMS::Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    KMS::DI::Array     * lArray;
    KMS::DI::Dictionary* lDictionary;
    KMS::DI::String    * lString;
    KMS::DI::UInt32    * lUInt32;

    switch (*lPtr)
    {
    case '[':
        lArray = new KMS::DI::Array(&KMS::DI::META_DATA_DELETE_OBJECT);
        Decode_Array(lArray, &lPtr);
        *aObject = lArray;
        break;

    case '{':
        lDictionary = new KMS::DI::Dictionary(&KMS::DI::META_DATA_DELETE_OBJECT);
        Decode_Dictionary(lDictionary, &lPtr);
        *aObject = lDictionary;
        break;

    case '"':
        lString = new KMS::DI::String("", &KMS::DI::META_DATA_DELETE_OBJECT);
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
        lUInt32 = new KMS::DI::UInt32(0, &KMS::DI::META_DATA_DELETE_OBJECT);
        Decode_Value(lUInt32, &lPtr);
        *aObject = lUInt32;
        break;

    default: KMS_EXCEPTION(JSON_FORMAT, "Invalid JSON format");
    }

    *aPtr = lPtr;
}

void Decode_Array(KMS::DI::Array* aArray, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aArray);
    assert(NULL != aPtr);

    KMS::Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    lPtr.Test('[');

    lPtr.SkipBlank();

    if (']' != *lPtr)
    {
        unsigned int lIndex = 0;

        for (;;)
        {
            KMS::DI::Object* lObject = (*aArray)[lIndex];
            if (NULL != lObject)
            {
                Decode(lObject, &lPtr);
            }
            else
            {
                Decode(&lObject, &lPtr);
                assert(NULL != lObject);

                aArray->AddEntry(lObject);
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

void Decode_Dictionary(KMS::DI::Dictionary* aDictionary, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aDictionary);
    assert(NULL != aPtr);

    KMS::Text::ReadPtr lPtr(*aPtr);

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

            KMS::DI::Object* lObject = (*aDictionary)[lName];
            if (NULL != lObject)
            {
                Decode(lObject, &lPtr);
            }
            else
            {
                Decode(&lObject, &lPtr);
                assert(NULL != lObject);

                unsigned int lFlags = KMS::DI::MetaData::FLAG_DELETE_OBJECT | KMS::DI::MetaData::FLAG_COPY_NAME | KMS::DI::MetaData::FLAG_DELETE_META_DATA;
                lObject->SetMetaData(new KMS::DI::MetaData(lName, NULL, lFlags));
                aDictionary->AddEntry(lObject);
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

void Decode_String(KMS::DI::String* aString, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aString);
    assert(NULL != aPtr);

    KMS::Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    char lStr[LINE_LENGTH];

    lPtr.Test('"');
    lPtr.ExtractUntil('"', lStr, sizeof(lStr));
    lPtr.Test('"');

    aString->Set(lStr);

    *aPtr = lPtr;
}

void Decode_Value(KMS::DI::Value* aValue, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aValue);
    assert(NULL != aPtr);

    KMS::Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    char lStr[LINE_LENGTH];

    lPtr.ExtractUntil(" \n\r\t,]}", lStr, sizeof(lStr));

    aValue->Set(lStr);

    *aPtr = lPtr;
}

void Encode(const KMS::DI::Object* aObject, KMS::Text::WritePtr* aPtr)
{
    assert(NULL != aObject);
    assert(NULL != aPtr);

    KMS::Text::WritePtr lPtr(*aPtr);

    // Dictionary must be tested before Array because Dictionary is an Array
    const KMS::DI::Dictionary* lDictionary = dynamic_cast<const KMS::DI::Dictionary*>(aObject);
    if (NULL != lDictionary)
    {
        Encode_Dictionary(lDictionary, &lPtr);
    }
    else
    {
        const KMS::DI::Array* lArray = dynamic_cast<const KMS::DI::Array*>(aObject);
        if (NULL != lArray)
        {
            Encode_Array(lArray, &lPtr);
        }
        else
        {
            // String must be tested before Value because String is a Value
            const KMS::DI::String* lString = dynamic_cast<const KMS::DI::String*>(aObject);
            if (NULL != lString)
            {
                Encode_String(lString, &lPtr);
            }
            else
            {
                const KMS::DI::Value* lValue = dynamic_cast<const KMS::DI::Value*>(aObject);
                if (NULL != lValue)
                {
                    lPtr += lValue->Get(lPtr, lPtr.GetRemainingSize());
                }
                else
                {
                    KMS_EXCEPTION(NOT_IMPLEMENTED, "JSON do not support this data type");
                }
            }
        }
    }

    *aPtr = lPtr;
}

void Encode_Array(const KMS::DI::Array* aArray, KMS::Text::WritePtr* aPtr)
{
    assert(NULL != aArray);
    assert(NULL != aPtr);

    KMS::Text::WritePtr lPtr(*aPtr);

    lPtr.Write('[');

    bool lFirst = true;

    const KMS::DI::Array::Internal& lInternal = aArray->GetInternal();
    for (const KMS::DI::Object* lObj : lInternal)
    {
        assert(NULL != lObj);

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

void Encode_Dictionary(const KMS::DI::Dictionary* aDictionary, KMS::Text::WritePtr* aPtr)
{
    assert(NULL != aDictionary);
    assert(NULL != aPtr);

    KMS::Text::WritePtr lPtr(*aPtr);

    lPtr.Write('{');

    bool lFirst = true;

    const KMS::DI::Dictionary::Internal& lInternal = aDictionary->GetInternal();
    for (const KMS::DI::Object* lObj : lInternal)
    {
        assert(NULL != lObj);

        if (lFirst)
        {
            lFirst = false;
        }
        else
        {
            lPtr.Write(',');
        }

        lPtr.Write('"');
        lPtr += lObj->GetName(lPtr, lPtr.GetRemainingSize());
        lPtr.Write('"');

        lPtr.Write(':');

        ::Encode(lObj, &lPtr);
    }

    lPtr.Write('}');

    *aPtr = lPtr;
}

void Encode_String(const KMS::DI::String* aString, KMS::Text::WritePtr* aPtr)
{
    assert(NULL != aString);
    assert(NULL != aPtr);

    KMS::Text::WritePtr lPtr(*aPtr);

    lPtr.Write('"');
    lPtr += aString->Get(lPtr, lPtr.GetRemainingSize());
    lPtr.Write('"');

    *aPtr = lPtr;
}

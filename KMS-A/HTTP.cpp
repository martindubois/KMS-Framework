
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/Container.h>
#include <KMS/DI/MetaData.h>
#include <KMS/DI/String.h>
#include <KMS/DI/UInt32.h>
#include <KMS/Text/ReadPtr.h>
#include <KMS/Text/WritePtr.h>

#include <KMS/HTTP/HTTP.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Decode(KMS::DI::Object * aObject, KMS::Text::ReadPtr* aPtr);
static void Decode(KMS::DI::Object** aObject, KMS::Text::ReadPtr* aPtr);

static void Decode_Dictionary(KMS::DI::Dictionary* aDictionary, KMS::Text::ReadPtr* aPtr);
static void Decode_Value     (KMS::DI::Value     * aValue     , KMS::Text::ReadPtr* aPtr);

static void Encode(const KMS::DI::Object* aObject, KMS::Text::WritePtr* aPtr);

static void Encode_Dictionary(const KMS::DI::Dictionary* aDictionary, KMS::Text::WritePtr* aPtr);

namespace KMS
{
    namespace HTTP
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        unsigned int Decode_Dictionary(DI::Dictionary* aDictionary, const char* aIn, unsigned int aInSize_byte)
        {
            Text::ReadPtr lPtr(aIn, aInSize_byte);

            ::Decode_Dictionary(aDictionary, &lPtr);

            return lPtr.GetIndex();
        }

        unsigned int Encode_Dictionary(const KMS::DI::Dictionary* aDictionary, char* aOut, unsigned int aOutSize_byte)
        {
            Text::WritePtr lPtr(aOut, aOutSize_byte);

            ::Encode_Dictionary(aDictionary, &lPtr);

            return lPtr.GetIndex();
        }

    }
}

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

void Decode(KMS::DI::Object* aObject, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aObject);

    KMS::DI::Dictionary* lDictionary = dynamic_cast<KMS::DI::Dictionary*>(aObject);
    if (NULL != lDictionary)
    {
        Decode_Dictionary(lDictionary, aPtr);
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

void Decode(KMS::DI::Object** aObject, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aObject);
    assert(NULL != aPtr);

    KMS::Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    KMS::DI::String* lString;
    KMS::DI::UInt32* lUInt32;

    switch (*lPtr)
    {
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
        lUInt32 = new KMS::DI::UInt32();
        lUInt32->SetMetaData(&KMS::DI::META_DATA_DELETE_OBJECT);
        Decode_Value(lUInt32, &lPtr);
        *aObject = lUInt32;
        break;

    default:
        lString = new KMS::DI::String();
        lString->SetMetaData(&KMS::DI::META_DATA_DELETE_OBJECT);
        Decode_Value(lString, &lPtr);
        *aObject = lString;
    }

    *aPtr = lPtr;
}

void Decode_Dictionary(KMS::DI::Dictionary* aDictionary, KMS::Text::ReadPtr* aPtr)
{
    assert(NULL != aDictionary);
    assert(NULL != aPtr);

    KMS::Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    while (!lPtr.IsAtEnd())
    {
        char lName[NAME_LENGTH];

        lPtr.ExtractUntil(": \n\r\t", lName, sizeof(lName));
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

            if (aDictionary->TestFlag(KMS::DI::MetaData::FLAG_DYNAMIC))
            {
                unsigned int lFlags = KMS::DI::MetaData::FLAG_DELETE_OBJECT | KMS::DI::MetaData::FLAG_DYNAMIC | KMS::DI::MetaData::FLAG_COPY_NAME | KMS::DI::MetaData::FLAG_DELETE_META_DATA;
                lObject->SetMetaData(new KMS::DI::MetaData(lName, NULL, lFlags));
                (*aDictionary) += lObject;
            }
            else
            {
                delete lObject;
            }
        }

        lPtr.SkipBlank();
    }

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

    const KMS::DI::Dictionary* lDictionary = dynamic_cast<const KMS::DI::Dictionary*>(aObject);
    if (NULL != lDictionary)
    {
        Encode_Dictionary(lDictionary, aPtr);
    }
    else
    {
        const KMS::DI::Value* lValue = dynamic_cast<const KMS::DI::Value*>(aObject);
        if (NULL != lValue)
        {
            (*aPtr) += lValue->Get(*aPtr, aPtr->GetRemainingSize());
        }
        else
        {
            KMS_EXCEPTION(NOT_IMPLEMENTED, "HTTP do not support this data type");
        }
    }
}

void Encode_Dictionary(const KMS::DI::Dictionary* aDictionary, KMS::Text::WritePtr* aPtr)
{
    assert(NULL != aDictionary);
    assert(NULL != aPtr);

    KMS::Text::WritePtr lPtr(*aPtr);

    unsigned int lCount = aDictionary->GetCount();
    for (unsigned int i = 0; i < lCount; i++)
    {
        const KMS::DI::Object* lO = (*aDictionary)[i];
        assert(NULL != lO);

        if (0 < i)
        {
            lPtr.Write("\r\n", 2);
        }

        Encode(lO, &lPtr);
    }

    *aPtr = lPtr;
}

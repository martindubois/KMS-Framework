
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/JSON.cpp

#include "Component.h"

// ===== Includes ===========================================================
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
            KMS_EXCEPTION(NOT_IMPLEMENTED, "HTTP do not suppoert this data type");
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
        Decode_Value(lUInt32, &lPtr);
        *aObject = lUInt32;
        break;

    default:
        lString = new KMS::DI::String();
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

        lPtr.ExtractUntil(":", lName, sizeof(lName));
        lPtr.Test(':');

        lPtr.SkipBlank();

        KMS::DI::Object* lObject = aDictionary->GetEntry_RW(lName);
        if (NULL != lObject)
        {
            Decode(lObject, &lPtr);
        }
        else
        {
            Decode(&lObject, &lPtr);
            assert(NULL != lObject);

            aDictionary->AddEntry(lName, lObject);
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

    lPtr.ExtractUntil("\n\r", lStr, sizeof(lStr));

    aValue->Set(lStr);

    lPtr.SkipBlank();

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

    const KMS::DI::Dictionary::Internal& lInternal = aDictionary->GetInternal();
    for (const KMS::DI::Dictionary::Internal::value_type lVT : lInternal)
    {
        assert(NULL != lVT.second);

        lPtr.Write(lVT.first.c_str(), static_cast<unsigned int>(lVT.first.size()));

        lPtr.Write(": ", 2);

        Encode(lVT.second, &lPtr);

        lPtr.Write("\r\n", 2);
    }

    lPtr.Write("\r\n", 2);

    *aPtr = lPtr;
}

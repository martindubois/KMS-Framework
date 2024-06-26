
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/HTTP.cpp

// TEST COVERAGE 2023-12-11 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DI/String.h>
#include <KMS/DI/UInt.h>
#include <KMS/Text/ReadPtr.h>
#include <KMS/Text/WritePtr.h>

#include <KMS/HTTP/HTTP.h>

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void Decode(KMS::DI::Object * aObject, KMS::Text::ReadPtr* aPtr);
static void Decode(KMS::DI::Object** aObject, KMS::Text::ReadPtr* aPtr);

static void Decode_Dictionary(KMS::DI::Dictionary* aDictionary, KMS::Text::ReadPtr* aPtr);
static void Decode_Value     (KMS::DI::Value     * aValue     , KMS::Text::ReadPtr* aPtr);
static bool Decode_Value_Try (KMS::DI::Value     * aValue     , KMS::Text::ReadPtr* aPtr);

static void Encode(const KMS::DI::Object* aObject, KMS::Text::WritePtr* aPtr);

static void Encode_Dictionary(const KMS::DI::Dictionary* aDictionary, KMS::Text::WritePtr* aPtr);

namespace KMS
{
    namespace HTTP
    {

        const char* FIELD_NAME_CONTENT_LENGTH = "Content-Length";
        const char* FIELD_NAME_CONTENT_TYPE   = "Content-Type";

        const DI::String FIELD_VALUE_CONTENT_TYPE_APPLICATION_JAVASCRIPT("application/javascript");
        const DI::String FIELD_VALUE_CONTENT_TYPE_APPLICATION_JSON      ("application/json");
        const DI::String FIELD_VALUE_CONTENT_TYPE_IMAGE_X_ICON          ("image/x-icon");
        const DI::String FIELD_VALUE_CONTENT_TYPE_TEXT_CSS              ("text/css");
        const DI::String FIELD_VALUE_CONTENT_TYPE_TEXT_HTML             ("text/html; charset=utf-8");
        const DI::String FIELD_VALUE_CONTENT_TYPE_TEXT_PLAIN            ("text/plain; charset=utf-8");

        namespace Request
        {

            const char* FIELD_NAME_ACCEPT          = "Accept";
            const char* FIELD_NAME_ACCEPT_ENCODING = "Accept-Encoding";
            const char* FIELD_NAME_CONNECTION      = "Connection";
            const char* FIELD_NAME_HOST            = "Host";
            const char* FIELD_NAME_USER_AGENT      = "User-Agent";

            const DI::String FIELD_VALUE_ACCEPT_TEXT_HTML("text/html");

            const DI::String FIELD_VALUE_ACCEPT_ENCODING_DEFLATE("deflate");

            const DI::String FIELD_VALUE_CONNECTION("keep-alive");

            const DI::String FIELD_VALUE_USER_AGENT("KMS-Framework");

        }

        namespace Response
        {

            const char* FIELD_NAME_ACCESS_CONTROL_ALLOW_HEADERS = "Access-Control-Allow-Headers";
            const char* FIELD_NAME_ACCESS_CONTROL_ALLOW_ORIGIN  = "Access-Control-Allow-Origin";
            const char* FIELD_NAME_CONTENT_DISPOSITION          = "Content-Disposition";
            const char* FIELD_NAME_DATE                         = "Date";
            const char* FIELD_NAME_LOCATION                     = "Location";
            const char* FIELD_NAME_SERVER                       = "Server";

            const DI::String FIELD_VALUE_ACCESS_CONTROL_ALLOW_HEADERS_DEFAULT("Origin, X-Requested-With, Content-Type, Accept");

            const DI::String FIELD_VALUE_ACCESS_CONTROL_ALLOW_ORIGIN_ALL("*");

            const DI::String FIELD_VALUE_CONTENT_DISPOSITION_INLINE("inline");

            const DI::String FIELD_VALUE_SERVER("KMS-Framework");

        }

        // Functions
        // //////////////////////////////////////////////////////////////////

        unsigned int Decode_Dictionary(DI::Dictionary* aDictionary, const char* aIn, unsigned int aInSize_byte)
        {
            Text::ReadPtr lPtr(aIn, aInSize_byte);

            ::Decode_Dictionary(aDictionary, &lPtr);

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

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

// NOT TESTED
void Decode(DI::Object* aObject, Text::ReadPtr* aPtr)
{
    assert(nullptr != aObject);

    auto lDictionary = dynamic_cast<DI::Dictionary*>(aObject);
    if (nullptr != lDictionary)
    {
        Decode_Dictionary(lDictionary, aPtr);
    }
    else
    {
        auto lValue = dynamic_cast<DI::Value*>(aObject);
        KMS_EXCEPTION_ASSERT(nullptr != lValue, RESULT_NOT_IMPLEMENTED, "HTTP do not suppoert this data type", "");

        Decode_Value(lValue, aPtr);
    }
}

void Decode(DI::Object** aObject, Text::ReadPtr* aPtr)
{
    assert(nullptr != aObject);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    DI::String        * lString;
    DI::UInt<uint32_t>* lUInt32;

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
        lUInt32 = new DI::UInt<uint32_t>;

        if (Decode_Value_Try(lUInt32, &lPtr))
        {
            *aObject = lUInt32;
        }
        else
        {
            delete lUInt32;

            lString = new DI::String;
            Decode_Value(lString, &lPtr);
            *aObject = lString;
        }
        break;

    default:
        lString = new DI::String;
        Decode_Value(lString, &lPtr);
        *aObject = lString;
    }

    *aPtr = lPtr;
}

void Decode_Dictionary(DI::Dictionary* aDictionary, Text::ReadPtr* aPtr)
{
    assert(nullptr != aDictionary);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    while (!lPtr.IsAtEnd())
    {
        char lName[NAME_LENGTH];

        lPtr.ExtractUntil(":", lName, sizeof(lName));
        lPtr.Test(':');

        lPtr.SkipBlank();

        auto lObject = aDictionary->GetEntry_RW(lName);
        if (nullptr != lObject)
        {
            // NOT TESTED
            Decode(lObject, &lPtr);
        }
        else
        {
            Decode(&lObject, &lPtr);
            assert(nullptr != lObject);

            Ptr_OF<DI::Object> lEntry(lObject, true);

            aDictionary->AddEntry(lName, lEntry);
        }

        lPtr.SkipBlank();
    }

    *aPtr = lPtr;
}

void Decode_Value(DI::Value* aValue, Text::ReadPtr* aPtr)
{
    assert(nullptr != aValue);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    char lStr[4096];

    lPtr.ExtractUntil(HTTP_EOL, lStr, sizeof(lStr));

    aValue->Set(lStr);

    lPtr.SkipBlank();

    *aPtr = lPtr;
}

bool Decode_Value_Try(KMS::DI::Value* aValue, KMS::Text::ReadPtr* aPtr)
{
    assert(nullptr != aValue);
    assert(nullptr != aPtr);

    Text::ReadPtr lPtr(*aPtr);

    lPtr.SkipBlank();

    char lStr[4096];

    lPtr.ExtractUntil(HTTP_EOL, lStr, sizeof(lStr));

    auto lResult = aValue->Set_Try(lStr);
    if (lResult)
    {
        lPtr.SkipBlank();

        *aPtr = lPtr;
    }

    return lResult;
}

void Encode(const DI::Object* aObject, Text::WritePtr* aPtr)
{
    assert(nullptr != aObject);
    assert(nullptr != aPtr);

    auto lDictionary = dynamic_cast<const DI::Dictionary*>(aObject);
    if (nullptr != lDictionary)
    {
        // NOT TESTED
        Encode_Dictionary(lDictionary, aPtr);
    }
    else
    {
        auto lValue = dynamic_cast<const DI::Value*>(aObject);
        if (nullptr != lValue)
        {
            (*aPtr) += lValue->Get(*aPtr, aPtr->GetRemainingSize());
        }
        else
        {
            KMS_EXCEPTION(RESULT_NOT_IMPLEMENTED, "HTTP do not support this data type (NOT TESTED)", "");
        }
    }
}

void Encode_Dictionary(const DI::Dictionary* aDictionary, Text::WritePtr* aPtr)
{
    assert(nullptr != aDictionary);
    assert(nullptr != aPtr);

    Text::WritePtr lPtr(*aPtr);

    for (const auto& lVT : aDictionary->mInternal)
    {
        assert(nullptr != lVT.second);

        lPtr.Write(lVT.first.c_str(), static_cast<unsigned int>(lVT.first.size()));

        lPtr.Write(": ", 2);

        Encode(lVT.second, &lPtr);

        lPtr.Write(HTTP_EOL, 2);
    }

    lPtr.Write(HTTP_EOL, 2);

    *aPtr = lPtr;
}

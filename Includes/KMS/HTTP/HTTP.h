
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2026 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/HTTP.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/String.h>

#define HTTP_EOH        "\r\n\r\n"
#define HTTP_EOH_LENGTH (4)
#define HTTP_EOL        "\r\n"
#define HTTP_EOL_LENGTH (2)

namespace KMS
{
    namespace HTTP
    {

        extern const char* FIELD_NAME_CONTENT_LENGTH;
        extern const char* FIELD_NAME_CONTENT_TYPE;

        extern const DI::String FIELD_VALUE_CONTENT_TYPE_APPLICATION_JAVASCRIPT;
        extern const DI::String FIELD_VALUE_CONTENT_TYPE_APPLICATION_JSON;
        extern const DI::String FIELD_VALUE_CONTENT_TYPE_IMAGE_X_ICON;
        extern const DI::String FIELD_VALUE_CONTENT_TYPE_TEXT_CSS;
        extern const DI::String FIELD_VALUE_CONTENT_TYPE_TEXT_HTML;
        extern const DI::String FIELD_VALUE_CONTENT_TYPE_TEXT_PLAIN;

        /// \brief TODO Remove use of DI namespace
        namespace Request
        {

            extern const char* FIELD_NAME_ACCEPT;
            extern const char* FIELD_NAME_ACCEPT_ENCODING;
            extern const char* FIELD_NAME_CONNECTION;
            extern const char* FIELD_NAME_HOST;
            extern const char* FIELD_NAME_USER_AGENT;

            extern const DI::String FIELD_VALUE_ACCEPT_TEXT_HTML;

            extern const DI::String FIELD_VALUE_ACCEPT_ENCODING_DEFLATE;

            extern const DI::String FIELD_VALUE_CONNECTION;

            extern const DI::String FIELD_VALUE_USER_AGENT;

        }

        /// \brief TODO Remove use of DI namespace
        namespace Response
        {

            extern const char* FIELD_NAME_ACCESS_CONTROL_ALLOW_HEADERS;
            extern const char* FIELD_NAME_ACCESS_CONTROL_ALLOW_ORIGIN;
            extern const char* FIELD_NAME_CONTENT_DISPOSITION;
            extern const char* FIELD_NAME_CONTENT_LENGTH;
            extern const char* FIELD_NAME_CONTENT_TYPE;
            extern const char* FIELD_NAME_DATE;
            extern const char* FIELD_NAME_LOCATION;
            extern const char* FIELD_NAME_SERVER;

            extern const DI::String FIELD_VALUE_ACCESS_CONTROL_ALLOW_HEADERS_DEFAULT;

            extern const DI::String FIELD_VALUE_ACCESS_CONTROL_ALLOW_ORIGIN_ALL;

            extern const DI::String FIELD_VALUE_CONTENT_DISPOSITION_INLINE;

            extern const DI::String FIELD_VALUE_SERVER;

        }

        /// \brief TODO Remove use of DI namespace
        extern unsigned int Decode_Dictionary(DI::Dictionary* aDictionary, const char* aIn, unsigned int aInSize_byte);

        /// \brief TODO Remove use of DI namespace
        extern unsigned int Encode_Dictionary(const DI::Dictionary* aDictionary, char* aOut, unsigned int aOutSize_byte);

    }
}

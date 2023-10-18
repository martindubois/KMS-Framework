
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Result.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/HTTP/Result.h>

namespace KMS
{
    namespace HTTP
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        const char* GetResultName(Result aR)
        {
            const char* lResult;

            switch (aR)
            {
            case Result::ACCEPTED             : lResult = "ACCEPTED"             ; break;
            case Result::BAD_REQUEST          : lResult = "BAD_REQUEST"          ; break;
            case Result::CREATED              : lResult = "CREATED"              ; break;
            case Result::FORBIDDEN            : lResult = "FORBIDDEN"            ; break;
            case Result::FOUND                : lResult = "FOUND"                ; break;
            case Result::INTERNAL_SERVER_ERROR: lResult = "INTERNAL_SERVER_ERROR"; break;
            case Result::METHOD_NOT_ALLOWED   : lResult = "METHOD_NOT_ALLOWED"   ; break;
            case Result::MOVED_PERMANENTLY    : lResult = "MOVED_PERMANENTLY"    ; break;
            case Result::NOT_ACCEPTABLE       : lResult = "NOT_ACCEPTABLE"       ; break;
            case Result::NOT_FOUND            : lResult = "NOT_FOUND"            ; break;
            case Result::NO_CONTENT           : lResult = "NO_CONTENT"           ; break;
            case Result::OK                   : lResult = "OK"                   ; break;
            case Result::REQUEST_TIMEOUT      : lResult = "REQUEST_TIMEOUT"      ; break;
            case Result::RESET_CONTENT        : lResult = "RESET_CONTENT"        ; break;

            default: lResult = "Invalid HTTP result";
            }

            return lResult;
        }

        const char* GetResultName(unsigned int aR)
        {
            return GetResultName(static_cast<Result>(aR));
        }

    }
}

using namespace KMS;

std::ostream& operator << (std::ostream& aOut, const HTTP::Result aR)
{
    aOut << HTTP::GetResultName(aR);

    return aOut;
}

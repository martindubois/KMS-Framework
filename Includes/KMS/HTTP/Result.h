
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/HTTP/Result.h
// Library   KMS-B

#pragma once

// ===== C++ ================================================================
#include <ostream>

namespace KMS
{
    namespace HTTP
    {

        enum class Result
        {
            OK                    = 200,
            CREATED               = 201,
            ACCEPTED              = 202,
            NO_CONTENT            = 204,
            RESET_CONTENT         = 205,
            MOVED_PERMANENTLY     = 301,
            BAD_REQUEST           = 400,
            FORBIDDEN             = 403,
            NOT_FOUND             = 404,
            METHOD_NOT_ALLOWED    = 405,
            NOT_ACCEPTABLE        = 406,
            REQUEST_TIMEOUT       = 408,
            INTERNAL_SERVER_ERROR = 500,
        };

        // Functions
        // //////////////////////////////////////////////////////////////////

        const char* GetResultName(Result aR);

        const char* GetResultName(unsigned int aR);

    }
}

std::ostream& operator << (std::ostream& aOut, const KMS::HTTP::Result& aR);

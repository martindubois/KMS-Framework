
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Request_W.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/HTTP/Request.h>

namespace KMS
{
    namespace HTTP
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Request::RetrieveTime(char* aOut, unsigned int aOutSize_byte)
        {
            static const char* DAY_NAMES[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
            static const char* MONTH_NAMES[13] = { nullptr, "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dev" };

            SYSTEMTIME lST;

            GetSystemTime(&lST);

            sprintf_s(aOut, aOutSize_byte, "%s, %u %s %u %u:%02u:%02u GMT",
                DAY_NAMES[lST.wDayOfWeek], lST.wDay, MONTH_NAMES[lST.wMonth], lST.wYear, lST.wHour, lST.wMinute, lST.wSecond);
        }

    }
}

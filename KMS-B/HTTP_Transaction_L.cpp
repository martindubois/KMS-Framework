
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/HTTP_Transaction_L.cpp

#include "Component.h"

// ===== C ==================================================================
#include <time.h>

// ===== Includes ===========================================================
#include <KMS/HTTP/Transaction.h>

namespace KMS
{
    namespace HTTP
    {

        // Private
        // //////////////////////////////////////////////////////////////////

        void Transaction::RetrieveTime(char* aOut, unsigned int aOutSize_byte)
        {
            static const char* DAY_NAMES[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
            static const char* MONTH_NAMES[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dev" };

            time_t lTime;

            time(&lTime);

            struct tm lTM;

            gmtime_r(&lTime, &lTM);

            sprintf(aOut, "%s, %u %s %u %u:%02u:%02u GMT",
                DAY_NAMES[lTM.tm_wday], lTM.tm_mday, MONTH_NAMES[lTM.tm_mon], 1900 + lTM.tm_year, lTM.tm_hour, lTM.tm_min, lTM.tm_sec);
        }

    }
}

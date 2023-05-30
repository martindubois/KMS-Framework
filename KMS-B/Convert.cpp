
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B/DI_NetAddress.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Address.h>
#include <KMS/Net/AddressRange.h>

namespace KMS
{
    namespace Convert
    {

        // Functions
        // //////////////////////////////////////////////////////////////////

        Net::Address ToNetAddress(const char* aASCII)
        {
            Net::Address lResult;

            auto lASCII = aASCII;

            if      (0 == _stricmp(lASCII, "IPv4")) { lResult.SetType(Net::Address::Type::IPv4); }
            else if (0 == _stricmp(lASCII, "IPv6")) { lResult.SetType(Net::Address::Type::IPv6); }
            else
            {

                if      (0 == _strnicmp(lASCII, "IPv4:", 5)) { lResult.SetType(Net::Address::Type::IPv4); lASCII += 5; }
                else if (0 == _strnicmp(lASCII, "IPv6:", 5)) { lResult.SetType(Net::Address::Type::IPv6); lASCII += 5; }

                char         lN[LINE_LENGTH];
                unsigned int lP;

                if (2 == sscanf_s(lASCII, "%[^:]:%u", lN SizeInfo(lN), &lP))
                {
                    lResult.SetAddress(lN); lResult.SetPortNumber(lP);
                }
                else
                {
                    lResult.SetAddress(lASCII);
                }
            }

            return lResult;
        }

        Net::AddressRange ToNetAddressRange(const char* aASCII)
        {
            Net::AddressRange lResult;

            assert(NULL != aASCII);

            char         lA[LINE_LENGTH];
            unsigned int lM;

            if (2 == sscanf_s(aASCII, "%[^/]/%u", lA SizeInfo(lA), &lM))
            {
                lResult = Convert::ToNetAddress(lA);
                lResult.SetMask(lM);
            }
            else
            {
                lResult = Convert::ToNetAddress(aASCII);
            }

            return lResult;
        }

    }
}

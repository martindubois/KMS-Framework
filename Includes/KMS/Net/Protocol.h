
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Net/Protocol.h
// Library   KMS-B

#pragma once

namespace KMS
{
    namespace Net
    {

        enum class Protocol
        {
            UNKNOWN = 0,

            ARPv4,
            ARPv6,
            FTP,
            HTTP,
            HTTPS,
            ICMP,
            IGMP,
            IPv4,
            IPv6,
            UDP,
            TCP,
        };

    }
}

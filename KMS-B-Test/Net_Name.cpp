
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/NET_Name.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Name.h>

using namespace KMS;

static const uint8_t ADDRESS_IPv4[]   = { 127, 0, 0, 1 };
static const char  * ADDRESS_IPv4_STR = "127.0.0.1";

KMS_TEST(NET_Name_Base, "Auto", sTest_Base)
{
    Net::Name lN0;

    // GetAddress
    KMS_TEST_ASSERT(nullptr == lN0.GetAddress());

    // GetAddressLength
    KMS_TEST_ASSERT(0 == lN0.GetAddressLength());

    // GetIPv4
    KMS_TEST_ASSERT(nullptr == lN0.GetIPv4());

    // GetIPv6
    KMS_TEST_ASSERT(nullptr == lN0.GetIPv6());

    // GetPortNumber
    KMS_TEST_ASSERT(0 == lN0.GetPortNumber());

    // GetProtocol
    KMS_TEST_ASSERT(Net::Protocol::UNKNOWN == lN0.GetProtocol());

    // GetURL

    // IsAddressValid
    KMS_TEST_ASSERT(!lN0.IsAddressValid());

    // IsIPv4Valid
    KMS_TEST_ASSERT(!lN0.IsIPv4Valid());

    // IsIPv6Valid
    KMS_TEST_ASSERT(!lN0.IsIPv6Valid());

    // IsPortNumberValid
    KMS_TEST_ASSERT(!lN0.IsPortNumberValid());

    // IsProtocolValid
    KMS_TEST_ASSERT(!lN0.IsProtocolValid());

    // SetIPv4
    lN0.SetIPv4(ADDRESS_IPv4);
    lN0.SetIPv4(ADDRESS_IPv4_STR);

    // SetIPv6
    // TODO

    // SetName
    lN0.SetName("www.kms-quebec.com");

    // SetPortNumber
    lN0.SetPortNumber(80);

    // SetProtocol
    lN0.SetProtocol(Net::Protocol::UDP);

    // SetURL
    lN0.SetURL("http://www.kms-quebec.com");

    // Resolve
    lN0.Resolve();
}

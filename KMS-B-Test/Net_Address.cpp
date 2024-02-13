
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-B-Test/NET_Address.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Net/Address.h>

using namespace KMS;

KMS_TEST(NET_Address_Base, "Auto", sTest_Base)
{
    // Constructor
    Net::Address lA0;

    // operator ==

    // Clear
    
    // GetName

    // GetPortNumber

    // GetType

    // IsBroadcast

    // IsMulticast

    // SetBroadcast_IPv4

    // SetBroadcast_IPv6

    // SetLocal_IPv4

    // SetLocal_IPv6

    // SetMulticast_IPv4

    // SetMulticast_IPv6

    // SetPortNumber

    // SetType
    lA0.SetType(Net::Address::Type::UNKNOWN);

    // ===== Internal =======================================================

    // GetBinary

    // GetInternalFamily

    // GetInternalSize

    // SetAddress

    // SetBinary

    // SetInternal

    // SetInternalSize

    // SetPortNumber

    // Display

    // operator <<

}

KMS_TEST(NET_Address_Exception, "Auto", sTest_Exception)
{
    Net::Address lA0;

    // ===== Internal =======================================================

    // SetAddress
    try
    {
        lA0.SetAddress("192.168.0.256");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_INVALID_ADDRESS");

    try
    {
        lA0.SetAddress("doesnotexist.kms-quebec.com");
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_ADDRESS_RESOLUTION_FAILED");

    // SetPortNumber
    try
    {
        lA0.SetPortNumber(static_cast<unsigned int>(0x10000));
        KMS_TEST_ASSERT(false);
    }
    KMS_TEST_CATCH_N("RESULT_INVALID_PORT");

}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Stats_Histogram.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Console/Redirection.h>
#include <KMS/Stats/Histogram.h>

using namespace KMS;

KMS_TEST(Stats_Histogram_Base, "Auto", sTest_Base)
{
    // Constructor
    Stats::Histogram lH0(0.0, 10.0, 10);

    // GetAboveMaxCount
    KMS_TEST_COMPARE(lH0.GetAboveMaxCount(), 0U);
    lH0.AddData(10.5);
    KMS_TEST_COMPARE(lH0.GetAboveMaxCount(), 1U);

    // GetBelowMinCount
    KMS_TEST_COMPARE(lH0.GetBelowMinCount(), 0U);
    lH0.AddData(-0.5);
    KMS_TEST_COMPARE(lH0.GetBelowMinCount(), 1U);

    // GetZoonCount
    KMS_TEST_COMPARE(lH0.GetZoneCount(0), 0U);
    lH0.AddData(0.5);
    KMS_TEST_COMPARE(lH0.GetZoneCount(0), 1U);

    // GetZoneMax
    KMS_TEST_COMPARE(lH0.GetZoneMax(9), 10.0);
    KMS_TEST_COMPARE(lH0.GetZoneMax(8),  9.0);

    // GetZoneMin
    KMS_TEST_COMPARE(lH0.GetZoneMin(0), 0.0);
    KMS_TEST_COMPARE(lH0.GetZoneMin(1), 1.0);

    // GetZoneQty
    KMS_TEST_COMPARE(lH0.GetZoneQty(), 10U);

    // GetZoneWidth
    KMS_TEST_COMPARE(lH0.GetZoneWidth(), 1.0);

    // GetZones
    KMS_TEST_ASSERT(nullptr != lH0.GetZones());

    // ===== Stats ==========================================================

    // AddData
    lH0.AddData(1.5);
    lH0.AddData(11.5);

    // GetDataAverage
    KMS_TEST_COMPARE(lH0.GetDataAverage(), 4.7000000000000002);

    // GetDataCount
    KMS_TEST_COMPARE(lH0.GetDataCount(), 5U);

    // GetDataLast
    KMS_TEST_COMPARE(lH0.GetDataLast(), 11.5);

    // GetDataMax
    KMS_TEST_COMPARE(lH0.GetDataMax(), 11.5);

    // GetDataMin
    KMS_TEST_COMPARE(lH0.GetDataMin(), -0.5);

    // GetDataStdDev
    KMS_TEST_COMPARE(lH0.GetDataStdDev(), 5.8051701094799970);

    // GetDataVariance
    KMS_TEST_COMPARE(lH0.GetDataVariance(), 33.700000000000003);

    // Reset
    lH0.Reset();
    KMS_TEST_COMPARE(lH0.GetAboveMaxCount(), 0U);
    KMS_TEST_COMPARE(lH0.GetBelowMinCount(), 0U);
    KMS_TEST_COMPARE(lH0.GetZoneCount(0), 0U);
    KMS_TEST_COMPARE(lH0.GetDataAverage(), 0.0);
    KMS_TEST_COMPARE(lH0.GetDataCount(), 0U);
    KMS_TEST_COMPARE(lH0.GetDataLast(), 0.0);
    KMS_TEST_COMPARE(lH0.GetDataMax(), 0.0);
    KMS_TEST_COMPARE(lH0.GetDataMin(), 0.0);
    KMS_TEST_COMPARE(lH0.GetDataStdDev(), 0.0);
    KMS_TEST_COMPARE(lH0.GetDataVariance(), 0.0);

    // operator <<
    Console::Redirection lR(Console::Redirection::What::WHAT_STDOUT);
    {
        std::cout << lH0;
    }
    lR.Restore();
}

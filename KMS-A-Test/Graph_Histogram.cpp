
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A-Test/Graph_Histogram.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Graph/Histogram.h>

using namespace KMS;

KMS_TEST(Graph_Histogram_Base, "Auto", sTest_Base)
{
    // Constructor
    Graph::Histogram lH0(0.0, 1.0, 4);

    // Prepare
    lH0.mBitmap.Init(16, 16);
    lH0.Prepare();

    // ===== Stats::Histrogram ==============================================

    // AddData
    for (unsigned int i = 0; i < 17; i++)
    {
        lH0.AddData(0.1);
    }

    // Reset
    lH0.Reset();
}

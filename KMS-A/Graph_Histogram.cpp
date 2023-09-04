
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Graph_Histogram.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Graph/Histogram.h>

namespace KMS
{
    namespace Graph
    {

        Histogram::Histogram(double aMin, double aMax, unsigned int aZoneQty)
            : KMS::Stats::Histogram(aMin, aMax, aZoneQty)
            , mDivY(0)
        {}

        void Histogram::Prepare()
        {
            auto lSizeX_px = mBitmap.GetSizeX_px();
            auto lZoneQty = KMS::Stats::Histogram::GetZoneQty();

            mStepX_px = lSizeX_px / lZoneQty;
            assert(0 < mStepX_px);

            mX0_px = (lSizeX_px - mStepX_px * lZoneQty) / 2;
            mX1_px = mX0_px + mStepX_px - 1 - mStepX_px / 4;

            Clear();
        }

        // ===== Stats::Histogram ===========================================

        void Histogram::AddData(double aData)
        {
            KMS::Stats::Histogram::AddData(aData);

            auto lSizeY_px = mBitmap.GetSizeY_px();
            auto lZoneQty  = KMS::Stats::Histogram::GetZoneQty();

            auto lZones = KMS::Stats::Histogram::GetZones();

            for (unsigned int i = 0; i < lZoneQty; i++)
            {
                auto y = lZones[i] / mDivY;
                if (lSizeY_px <= y)
                {
                    Clear();
                    Draw();
                    break;
                }

                auto x0 = mX0_px + mStepX_px * i;
                auto x1 = mX1_px + mStepX_px * i;

                mBitmap.SetLine(Point(x0, y), Point(x1, y), Color::WHITE);
            }
        }

        void Histogram::Reset()
        {
            KMS::Stats::Histogram::Reset();

            mDivY = 0;

            Clear();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Histogram::Clear()
        {
            mBitmap.SetBox(Point::ORIGIN, mBitmap.GetUpperRight(), Color::BLACK);
        }

        void Histogram::Draw()
        {
            auto lZoneQty = KMS::Stats::Histogram::GetZoneQty();

            auto lZones = KMS::Stats::Histogram::GetZones();

            for (unsigned int i = 0; i < lZoneQty; i++)
            {
                auto y = lZones[i] / mDivY;

                auto x0 = mX0_px + mStepX_px * i;
                auto x1 = mX1_px + mStepX_px * i;

                mBitmap.SetLine(Point(x0, 0), Point(x1, y), Color::WHITE);
            }
        }

    }
}

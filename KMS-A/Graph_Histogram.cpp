
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Graph_Histogram.cpp

// TEST COVERAGE 2023-09-06 Martin Dubois

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Graph/Histogram.h>

namespace KMS
{
    namespace Graph
    {

        Histogram::Histogram(double aMin, double aMax, unsigned int aZoneQty)
            : KMS::Stats::Histogram(aMin, aMax, aZoneQty)
            , mDivY(1)
            , mStepX_px(0)
            , mX0_px(0)
            , mX1_px(0)
        {}

        void Histogram::Prepare()
        {
            auto lSizeX_px = mBitmap.GetSizeX_px();
            auto lZoneQty = KMS::Stats::Histogram::GetZoneQty();
            assert(0 < lSizeX_px);
            assert(0 < lZoneQty);

            mStepX_px = lSizeX_px / lZoneQty;
            assert(0 < mStepX_px);

            mX0_px = (lSizeX_px - mStepX_px * lZoneQty) / 2;
            mX1_px = mX0_px + mStepX_px - 1 - mStepX_px / 4;
            assert(mX0_px <= mX1_px);

            Clear();
        }

        // ===== Stats::Histogram ===========================================

        void Histogram::AddData(double aData)
        {
            assert(0 < mDivY);

            KMS::Stats::Histogram::AddData(aData);

            auto lSizeY_px = mBitmap.GetSizeY_px();
            auto lZoneQty  = KMS::Stats::Histogram::GetZoneQty();
            assert(0 < lSizeY_px);
            assert(0 < lZoneQty);

            auto lZones = KMS::Stats::Histogram::GetZones();
            assert(nullptr != lZones);

            for (unsigned int i = 0; i < lZoneQty; i++)
            {
                if (0 < lZones[i])
                {
                    auto lY_px = lZones[i] / mDivY;
                    if (lSizeY_px <= lY_px)
                    {
                        mDivY++;
                        Clear();
                        Draw();
                        break;
                    }

                    auto lX0_px = mX0_px + mStepX_px * i;
                    auto lX1_px = mX1_px + mStepX_px * i;
                    assert(lX0_px <= lX1_px);

                    mBitmap.SetLine(Point(lX0_px, lY_px), Point(lX1_px, lY_px), Color::WHITE);
                }
            }
        }

        void Histogram::Reset()
        {
            KMS::Stats::Histogram::Reset();

            mDivY = 1;

            Clear();
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Histogram::Clear()
        {
            auto lUR = mBitmap.GetUpperRight();

            mBitmap.SetBox(Point::ORIGIN, lUR, Color::BLACK);

            char lStr[16];

            sprintf_s(lStr, "%8u", (lUR.mY_px + 1) * mDivY);

            mBitmap.SetString(lStr, Point(lUR.mX_px - 70, lUR.mY_px - 15), 0x808080);
        }

        void Histogram::Draw()
        {
            assert(0 < mDivY);

            auto lZoneQty = KMS::Stats::Histogram::GetZoneQty();

            auto lZones = KMS::Stats::Histogram::GetZones();
            assert(nullptr != lZones);

            for (unsigned int i = 0; i < lZoneQty; i++)
            {
                if (0 < lZones[i])
                {
                    auto lY_px = lZones[i] / mDivY;

                    auto lX0_px = mX0_px + mStepX_px * i;
                    auto lX1_px = mX1_px + mStepX_px * i;

                    mBitmap.SetBox(Point(lX0_px, 0), Point(lX1_px, lY_px), 0xf0f0f0);
                }
            }
        }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/ScaleAndOffset.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/ScaleAndOffset.h>

namespace KMS
{

    // Public
    // //////////////////////////////////////////////////////////////////////

    ScaleAndOffset::ScaleAndOffset(double aScale, double aOffset) : mOffset(aOffset), mScale(aScale)
    {
        assert(0.0 != aScale);
    }

    double ScaleAndOffset::Revert(double aIn) const
    {
        assert(0.0 != mScale);

        return (aIn - mOffset) / mScale;
    }

    double ScaleAndOffset::Scale(double aIn) const
    {
        assert(0.0 != mScale);

        return aIn * mScale + mOffset;
    }

}

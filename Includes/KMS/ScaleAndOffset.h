
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/ScaleAndOffset.h

#pragma once

namespace KMS
{

    class ScaleAndOffset
    {

    public:

        ScaleAndOffset(double aScale, double aOffset = 0.0);

        double Revert(double aIn) const;
        double Scale (double aIn) const;

    private:

        double mOffset;
        double mScale;

    };

}

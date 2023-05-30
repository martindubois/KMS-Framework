
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/DAC_AnalogConverter.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/DAQ/AnalogConverter.h>

namespace KMS
{
    namespace DAQ
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void AnalogConverter::AddScaleAndOffset(Id aId, double aScale, double aOffset)
        {
            mScales.insert(ScaleMap::value_type(aId, ScaleAndOffset(aScale, aOffset)));
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        double AnalogConverter::Revert(Id aId, double aIn) const
        {
            auto lIt = mScales.find(aId);
            assert(mScales.end() != lIt);

            return lIt->second.Revert(aIn);
        }

        double AnalogConverter::Scale(Id aId, double aIn) const
        {
            auto lIt = mScales.find(aId);
            assert(mScales.end() != lIt);

            return lIt->second.Scale(aIn);
        }

    }
}

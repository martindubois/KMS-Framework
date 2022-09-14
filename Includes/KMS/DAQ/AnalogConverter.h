
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/DAQ/AnalogConverter.h

#pragma once

// ===== C++ ================================================================
#include <map>

// ===== Includes ===========================================================
#include <KMS/DAQ/DAQ.h>
#include <KMS/ScaleAndOffset.h>

namespace KMS
{
    namespace DAQ
    {

        class AnalogConverter
        {

        public:

            void AddScaleAndOffset(Id aId, double aScale, double aOffset = 0.0);

        protected:

            double Scale (Id aId, double aIn) const;
            double Revert(Id aId, double aIn) const;

        private:

            typedef std::map<Id, ScaleAndOffset> ScaleMap;

            ScaleMap mScales;

        };

    }
}

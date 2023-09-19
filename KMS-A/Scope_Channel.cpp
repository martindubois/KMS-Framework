

// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/Scope_Channel.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Scope/Channel.h>

namespace KMS
{
    namespace Scope
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        // ===== Configurable attribute =================================
        const Graph::Color Channel::COLOR_DEFAULT      = Graph::Color::RED;
        const double       Channel::SCALE_Y_DEFAULT    = 1.0;
        const double       Channel::TRIG_LEVEL_DEFAULT = 1.0;
        const uint16_t     Channel::Y_DEFAULT_px       =  128;
        const uint16_t     Channel::Y_MAX_px           = 2048;

        // Internal
        // //////////////////////////////////////////////////////////////////

        void Channel::Sample(unsigned int* aY_px, bool* aTrig)
        {
            assert(nullptr != aY_px);
            assert(nullptr != aTrig);

            auto lLast = mStats.GetDataLast();

            *aY_px = mY_px + static_cast<unsigned int>(lLast * mScaleY);

            // TODO  Implement rasing/falling trigger
            if ((lLast < mTrigLevel) && (mPrevious >= mTrigLevel))
            {
                *aTrig = true;
            }
            else if ((lLast > mTrigLevel) && (mPrevious <= mTrigLevel))
            {
                *aTrig = true;
            }
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Channel::Channel()
            : mPrevious(0.0)
            // ===== Configurable attributes ================================
            , mColor    (COLOR_DEFAULT)
            , mScaleY   (SCALE_Y_DEFAULT)
            , mTrigLevel(TRIG_LEVEL_DEFAULT)
            , mY_px     (Y_DEFAULT_px)
        {}

        void Channel::SetSample(double aValue)
        {
            mPrevious = mStats.GetDataLast();

            mStats.AddData(aValue);
        }

    }
}

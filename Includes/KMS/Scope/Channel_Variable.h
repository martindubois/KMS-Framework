
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Scope/Channel_Variable.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Scope/Channel.h>

namespace KMS
{
    namespace Scope
    {

        template <typename T>
        class Channel_Variable : public Channel
        {

        public:

            Channel_Variable(T* aPtr);

        // Internal

            // ===== Channel ================================================
            virtual void Sample(unsigned int* aY_px, bool* aTrig);

        private:

            T* mPtr;

        };

        // Public
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        Channel_Variable<T>::Channel_Variable(T* aPtr) : mPtr(aPtr)
        {
            assert(nullptr != aPtr);
        }

        // Internal
        // //////////////////////////////////////////////////////////////////

        template <typename T>
        void Channel_Variable<T>::Sample(unsigned int* aY_px, bool* aTrig)
        {
            assert(nullptr != mPtr);

            SetSample(static_cast<double>(*mPtr));

            Channel::Sample(aY_px, aTrig)
        }

    }
}

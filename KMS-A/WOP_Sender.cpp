
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_Sender.cpp

// TEST COVERAGE 2022-11-10 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/Sender.h>

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Sender::Sender(System* aSystem, Object** aInstances, uint8_t aCount)
            : mInstance(0)
            , mInstances(aInstances)
            , mInstanceCount(aCount)
            , mSystem(aSystem)
        {
            // assert(NULL != aInstances);
            // assert(NULL != aSystem);
        }

        void Sender::SetInstanceCount(uint8_t aCount) { mInstanceCount = aCount; }

        const FrameBuffer* Sender::PrepareFrame()
        {
            if (mSystem->PrepareFrame(&mFrameBuffer, KMS_WOP_INSTANCE_SYSTEM))
            {
                mFrameBuffer.Seal();
                return &mFrameBuffer;
            }

            for (unsigned int i = 0; i < mInstanceCount; i++)
            {
                if (mInstanceCount <= mInstance)
                {
                    mInstance = 0;
                }

                // assert(NULL != mInstances[mInstance]);

                if (mInstances[mInstance]->PrepareFrame(&mFrameBuffer, mInstance))
                {
                    mInstance++;

                    mFrameBuffer.Seal();
                    return &mFrameBuffer;
                }

                mInstance++;
            }

            return NULL;
        }

    }
}

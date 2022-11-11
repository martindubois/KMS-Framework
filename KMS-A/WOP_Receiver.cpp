
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_Receiver.cpp

// TEST COVERAGE 2022-11-10 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/Receiver.h>

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Receiver::Receiver(System* aSystem, Object** aInstances, uint8_t aCount)
            : mInstances(aInstances)
            , mInstanceCount(aCount)
            , mSystem(aSystem)
        {
            assert(NULL != aInstances);
            assert(NULL != aSystem);
        }

        void Receiver::AddReceivedByte(uint8_t aIn)
        {
            assert(NULL != mSystem);

            uint8_t lRet = mFrameBuffer.AddReceivedByte(aIn);
            switch (lRet)
            {
            case KMS_WOP_RESULT_OK: WriteData(); break;

            case KMS_WOP_RESULT_PARTIAL: break;

            default:
                mFrameBuffer.Clear();
                mSystem->SetResult(lRet);
            }
        }

        void Receiver::AddReceivedBytes(const uint8_t* aIn, unsigned int aInSize_byte)
        {
            assert(NULL != aIn);

            for (unsigned int i = 0; i < aInSize_byte; i++)
            {
                AddReceivedByte(aIn[i]);
            }
        }

        void Receiver::SetInstanceCount(uint8_t aCount) { mInstanceCount = aCount; }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Receiver::WriteData()
        {
            assert(NULL != mInstances);
            assert(NULL != mSystem);

            uint8_t lRet = KMS_WOP_RESULT_INVALID_INSTANCE;

            uint8_t lInstance = mFrameBuffer.GetInstance();
            if (KMS_WOP_INSTANCE_SYSTEM == lInstance)
            {
                lRet = mSystem->WriteData(&mFrameBuffer);
            }
            else if (mInstanceCount > lInstance)
            {
                assert(NULL != mInstances[lInstance]);

                lRet = mInstances[lInstance]->WriteData(&mFrameBuffer);
            }

            if (KMS_WOP_RESULT_OK != lRet)
            {
                // NOT TESTED
                mSystem->SetResult(lRet);
            }

            mFrameBuffer.Clear();
        }

    }
}

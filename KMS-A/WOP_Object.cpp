
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_Object.cpp

// TEST COVERAGE 2022-11-20 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/WOP.h>

#include <KMS/WOP/Object.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define BIT_REQUEST (1 << KMS_WOP_DATA_TYPE_REQUEST)

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        void Object::AddRequest  (uint8_t aMask) { mRequest_In  |=  aMask; }
        void Object::ClearRequest(uint8_t aMask) { mRequest_In  &= ~aMask; }

        void Object::SendRequest(uint8_t aMask)
        {
            mRequest_Out |= aMask;

            if (0 != mRequest_Out)
            {
                AddRequest(BIT_REQUEST);
            }
        }

        // Protected
        // //////////////////////////////////////////////////////////////////

        Object::Object() : mRequest_In(0), mRequest_Out(0) {}

        uint8_t Object::PopRequest(uint8_t aMask)
        {
            for (unsigned int i = 0; i < 7; i++)
            {
                uint8_t lBit = 1 << i;

                if ((0 != (mRequest_In & lBit)) && (0 != (aMask & lBit)))
                {
                    ClearRequest(lBit);

                    return lBit;
                }
            }

            return 0;
        }

        void Object::PrepareFrame_REQUEST(FrameBuffer* aOut, uint8_t aInstance)
        {
            // assert(nullptr != aOut);

            aOut->Prepare(aInstance, KMS_WOP_DATA_TYPE_REQUEST);

            aOut->AddDataByte(mRequest_Out);

            mRequest_Out = 0;
        }

        uint8_t Object::WriteData_REQUEST(const FrameBuffer* aIn)
        {
            // assert(nullptr != aIn);

            uint8_t lMask = 0;

            uint8_t lResult = aIn->GetData(&lMask, sizeof(lMask));
            if (KMS_WOP_RESULT_OK == lResult)
            {
                AddRequest(lMask);
            }

            return lResult;
        }

    }
}

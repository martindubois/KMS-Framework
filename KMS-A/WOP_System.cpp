
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_System.cpp

// TEST COVERAGE 2022-11-10 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/WOP.h>

#include <KMS/WOP/System.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define DATA_TYPE_RESULT (1)

#define BIT_RESULT (1 << DATA_TYPE_RESULT)

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        System::System() : mResult_In(KMS_WOP_RESULT_OK), mResult_Out(KMS_WOP_RESULT_OK) {}

        uint8_t System::GetResult()
        {
            uint8_t lResult = mResult_In;

            mResult_In = KMS_WOP_RESULT_OK;

            return lResult;
        }

        void System::SetResult(uint8_t aIn)
        {
            mResult_Out = aIn;

            if (KMS_WOP_RESULT_OK != mResult_Out)
            {
                AddRequest(BIT_RESULT);
            }
        }

        // ===== Object =====================================================

        bool System::PrepareFrame(FrameBuffer* aOut, uint8_t aInstance)
        {
            assert(NULL != aOut);

            bool lResult = false;

            switch (PopRequest(KMS_WOP_BIT_REQUEST | BIT_RESULT))
            {
            case KMS_WOP_BIT_REQUEST: PrepareFrame_REQUEST(aOut, aInstance); lResult = true; break;

            case BIT_RESULT:
                aOut->Prepare(aInstance, DATA_TYPE_RESULT);
                aOut->AddDataByte(mResult_Out);
                mResult_Out = KMS_WOP_RESULT_OK;
                lResult = true;
                break;
            }

            return lResult;
        }

        uint8_t System::WriteData(const FrameBuffer* aIn)
        {
            assert(NULL != aIn);

            uint8_t lResult = KMS_WOP_RESULT_INVALID_DATA_TYPE;

            switch (aIn->GetDataType())
            {
            case KMS_WOP_DATA_TYPE_REQUEST: lResult = WriteData_REQUEST(aIn); break;

            case DATA_TYPE_RESULT: lResult = aIn->GetData(&mResult_In, sizeof(mResult_In)); break;
            }

            return lResult;
        }

    }
}

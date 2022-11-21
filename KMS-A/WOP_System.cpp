
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

#define DATA_TYPE_RESULT   (1)
#define DATA_TYPE_PROTOCOL (2)
#define DATA_TYPE_TRACE    (3)
#define DATA_TYPE_VERSION  (4)

#define BIT_RESULT   (1 << DATA_TYPE_RESULT)
#define BIT_PROTOCOL (1 << DATA_TYPE_PROTOCOL)
#define BIT_TRACE    (1 << DATA_TYPE_TRACE)
#define BIT_VERSION  (1 << DATA_TYPE_VERSION)

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        System::System(const Version& aVersion)
            : mResult_In(KMS_WOP_RESULT_OK)
            , mResult_Out(KMS_WOP_RESULT_OK)
            , mVersion_Out(aVersion)
        {}

        uint8_t System::GetResult()
        {
            uint8_t lResult = mResult_In;

            mResult_In = KMS_WOP_RESULT_OK;

            return lResult;
        }

        const Version& System::GetVersion() const { return mVersion_In; }

        void System::SetResult(uint8_t aIn)
        {
            mResult_Out = aIn;

            if (KMS_WOP_RESULT_OK != mResult_Out)
            {
                AddRequest(BIT_RESULT);
            }
        }

        #ifdef _KMS_EMBEDDED_

            void System::AddTrace(const char* aMsg, uint8_t aLen)
            {
                uint8_t lSpace = sizeof(mTrace) - mTraceLen;
                if (aLen > lSpace)
                {
                    mTrace[0] = '*';
                    mTraceLen = 1;
                }

                strcpy(mTrace + mTraceLen, aMsg, aLen);
                mTraceLen += aLen;
            }

        #else

            void System::Dump()
            {
                std::cout << "System:\n";
                std::cout << "  Protocol In : " << static_cast<unsigned int>(mProtocol_In) << "\n";
                std::cout << "  Result In   : " << static_cast<unsigned int>(mResult_In) << "\n";
                std::cout << "  Result Out  : " << static_cast<unsigned int>(mResult_Out) << "\n";
                std::cout << "  Version In  : " << mVersion_In << std::endl;
            }

        #endif

        // ===== Object =====================================================

        bool System::PrepareFrame(FrameBuffer* aOut, uint8_t aInstance)
        {
            // assert(NULL != aOut);

            bool lResult = false;

            switch (PopRequest(KMS_WOP_BIT_REQUEST | BIT_RESULT))
            {
            case KMS_WOP_BIT_REQUEST: PrepareFrame_REQUEST(aOut, aInstance); lResult = true; break;

            case BIT_PROTOCOL:
                aOut->Prepare(aInstance, DATA_TYPE_PROTOCOL);
                aOut->AddDataByte(0);
                lResult = true;
                break;
            case BIT_RESULT:
                aOut->Prepare(aInstance, DATA_TYPE_RESULT);
                aOut->AddDataByte(mResult_Out);
                mResult_Out = KMS_WOP_RESULT_OK;
                lResult = true;
                break;
            case BIT_VERSION:
                aOut->Prepare(aInstance, DATA_TYPE_VERSION);
                aOut->AddDataBytes(&mVersion_Out, sizeof(mVersion_Out));
                lResult = true;
                break;

            #ifdef _KMS_EMBEDDED_
                case BIT_TRACE:
                    aOut->Prepare(aInstance, DATA_TYPE_RESULT);
                    aOut->AddDataTypes(mTrace, mTraceLen);
                    mTraceLen = 0;
                    lResult = true;
                    break;
            #endif
            }

            return lResult;
        }

        uint8_t System::WriteData(const FrameBuffer* aIn)
        {
            // assert(NULL != aIn);

            uint8_t lResult = KMS_WOP_RESULT_INVALID_DATA_TYPE;

            switch (aIn->GetDataType())
            {
            case KMS_WOP_DATA_TYPE_REQUEST: lResult = WriteData_REQUEST(aIn); break;

            case DATA_TYPE_PROTOCOL: lResult = aIn->GetData(&mProtocol_In, sizeof(mProtocol_In)); break;
            case DATA_TYPE_RESULT  : lResult = aIn->GetData(&mResult_In  , sizeof(mResult_In  )); break;
            case DATA_TYPE_VERSION : lResult = aIn->GetData(&mVersion_In , sizeof(mVersion_In )); break;

            case DATA_TYPE_TRACE:
                #ifndef _KMS_EMBEDDED_
                    char lData[256];
                    memset(&lData, 0, sizeof(lData));
                    aIn->GetData(lData, sizeof(lData) - 1);
                    std::cout << "WOP TRACE " << lData << std::endl;
                #endif
                lResult = KMS_WOP_RESULT_OK;
                break;
            }

            return lResult;
        }

    }
}

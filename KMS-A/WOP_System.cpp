
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-A/WOP_System.cpp

// TEST COVERAGE 2022-11-22 KMS - Martin Dubois, P. Eng.

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/WOP.h>

#include <KMS/WOP/System.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define DATA_TYPE_PROTOCOL (1)
#define DATA_TYPE_RESULT   (2)
#define DATA_TYPE_TRACE    (3)
#define DATA_TYPE_VERSION  (4)

#define BIT_PROTOCOL (1 << DATA_TYPE_PROTOCOL)
#define BIT_RESULT   (1 << DATA_TYPE_RESULT)
#define BIT_TRACE    (1 << DATA_TYPE_TRACE)
#define BIT_VERSION  (1 << DATA_TYPE_VERSION)

#define INSTANCE_COUNT_UNKNOWN (0xff)

#define MAGIC_UNKNOWN          (0xffffffff)

#define VERSION_SYSTEM         (0x01)

#define VERSION_UNKNOWN        (0xff)

namespace KMS
{
    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        System::System(const Version& aVersion, uint32_t aMagic, uint8_t aProtocolVersion)
            : mInstances(NULL)
            , mTx_Instance(0)
            , mProtocol_Out(aMagic, aProtocolVersion)
            , mResult_In(KMS_WOP_RESULT_OK)
            , mResult_Out(KMS_WOP_RESULT_OK)
            , mTraceLen(0)
            , mVersion_Out(aVersion)
        {}

        uint8_t System::GetResult()
        {
            uint8_t lResult = mResult_In;

            mResult_In = KMS_WOP_RESULT_OK;

            return lResult;
        }

        const Version& System::GetVersion() const { return mVersion_In; }

        void System::SetInstances(Object** aVector, uint8_t aCount)
        {
            mInstances                   = aVector;
            mProtocol_Out.mInstanceCount = aCount;
        }

        void System::SetResult(uint8_t aIn)
        {
            mResult_Out = aIn;

            if (KMS_WOP_RESULT_OK != mResult_Out)
            {
                AddRequest(BIT_RESULT);
            }
        }

        void System::AddReceivedByte(uint8_t aIn)
        {
            uint8_t lRet = mRx_FrameBuffer.AddReceivedByte(aIn);
            switch (lRet)
            {
            case KMS_WOP_RESULT_OK: WriteData(); break;

            case KMS_WOP_RESULT_PARTIAL: break;

            default:
                mRx_FrameBuffer.Clear();
                SetResult(lRet);
            }
        }

        void System::AddReceivedBytes(const uint8_t* aIn, unsigned int aInSize_byte)
        {
            // assert(NULL != aIn);

            for (unsigned int i = 0; i < aInSize_byte; i++)
            {
                AddReceivedByte(aIn[i]);
            }
        }

        void System::AddTrace(const char* aMsg, uint8_t aLen)
        {
            uint8_t lSpace = sizeof(mTrace) - mTraceLen;
            if (aLen > lSpace)
            {
                mTrace[0] = '*';
                mTraceLen = 1;
            }

            memcpy(mTrace + mTraceLen, aMsg, aLen);
            mTraceLen += aLen;

            AddRequest(BIT_TRACE);
        }

        const FrameBuffer* System::PrepareFrame()
        {
            if (PrepareFrame(&mTx_FrameBuffer, KMS_WOP_INSTANCE_SYSTEM))
            {
                mTx_FrameBuffer.Seal();
                return &mTx_FrameBuffer;
            }

            for (unsigned int i = 0; i < mProtocol_Out.mInstanceCount; i++)
            {
                if (mProtocol_Out.mInstanceCount <= mTx_Instance)
                {
                    mTx_Instance = 0;
                }

                // assert(NULL != mInstances[mInstance]);

                if (mInstances[mTx_Instance]->PrepareFrame(&mTx_FrameBuffer, mTx_Instance))
                {
                    mTx_Instance++;

                    mTx_FrameBuffer.Seal();
                    return &mTx_FrameBuffer;
                }

                mTx_Instance++;
            }

            return NULL;
        }

        #ifndef _KMS_EMBEDDED_

            void System::Dump()
            {
                std::cout << "System:\n";
                std::cout << "  Result In  : " << static_cast<unsigned int>(mResult_In ) << "\n";
                std::cout << "  Result Out : " << static_cast<unsigned int>(mResult_Out) << "\n";
                std::cout << "  Version In : " << mVersion_In << std::endl;
            }

        #endif

        // ===== Object =====================================================

        bool System::PrepareFrame(FrameBuffer* aOut, uint8_t aInstance)
        {
            // assert(NULL != aOut);

            bool lResult = false;

            switch (PopRequest(KMS_WOP_BIT_REQUEST | BIT_PROTOCOL | BIT_RESULT | BIT_VERSION | BIT_TRACE))
            {
            case KMS_WOP_BIT_REQUEST: PrepareFrame_REQUEST(aOut, aInstance); lResult = true; break;

            case BIT_PROTOCOL:
                aOut->Prepare(aInstance, DATA_TYPE_PROTOCOL);
                aOut->AddDataBytes(&mProtocol_Out, sizeof(mProtocol_Out));
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

            case BIT_TRACE:
                aOut->Prepare(aInstance, DATA_TYPE_TRACE);
                aOut->AddDataBytes(mTrace, mTraceLen);
                mTraceLen = 0;
                lResult = true;
                break;
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
                    // NOT TESTED
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

        // Private
        // //////////////////////////////////////////////////////////////////

        System::ProtocolInfo::ProtocolInfo()
            : mMagic(MAGIC_UNKNOWN)
            , mInstanceCount(INSTANCE_COUNT_UNKNOWN)
            , mVersion_Base(VERSION_UNKNOWN)
            , mVersion_System(VERSION_UNKNOWN)
            , mVersion_Application(VERSION_UNKNOWN)
        {}

        System::ProtocolInfo::ProtocolInfo(uint32_t aMagic, uint8_t aProtcolVersion)
            : mMagic(aMagic)
            , mInstanceCount(0)
            , mVersion_Base(KMS_WOP_VERSION_BASE)
            , mVersion_System(VERSION_SYSTEM)
            , mVersion_Application(aProtcolVersion)
        {}

        void System::WriteData()
        {
            uint8_t lRet = KMS_WOP_RESULT_INVALID_INSTANCE;

            uint8_t lInstance = mRx_FrameBuffer.GetInstance();
            if (KMS_WOP_INSTANCE_SYSTEM == lInstance)
            {
                lRet = WriteData(&mRx_FrameBuffer);
            }
            else if (mProtocol_Out.mInstanceCount > lInstance)
            {
                // assert(NULL != mInstances[lInstance]);

                lRet = mInstances[lInstance]->WriteData(&mRx_FrameBuffer);
            }

            if (KMS_WOP_RESULT_OK != lRet)
            {
                // NOT TESTED
                SetResult(lRet);
            }

            mRx_FrameBuffer.Clear();
        }

    }
}

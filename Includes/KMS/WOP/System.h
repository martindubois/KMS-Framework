
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2024 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/System.h
// Status
// Library   KMS-A

#pragma once

// ===== Includes ===========================================================
#include <KMS/Version.h>
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/Object.h>

namespace KMS
{
    namespace WOP
    {

        // TODO  Add a Callback for error, trace, version and protocol

        class System : public Object
        {

        public:

            System(const Version& aVersion, uint32_t aMagic, uint8_t aProtocolVersion);

            uint8_t GetResult();

            const Version& GetVersion() const;

            void SetInstances(Object** aVector, uint8_t aCount);

            void SetResult(uint8_t aIn);

            void AddReceivedByte(uint8_t aIn);

            void AddReceivedBytes(const uint8_t* aIn, unsigned int aInSize_byte);

            void AddTrace(const char* aMsg, uint8_t aLen);

            void AddTrace(uint16_t aMsg);

            const FrameBuffer* PrepareFrame();

            #ifndef _KMS_EMBEDDED_
                void Dump();
            #endif

            // ===== Object =================================================
            virtual bool    PrepareFrame(FrameBuffer* aOut, uint8_t aInstance);
            virtual uint8_t WriteData(const FrameBuffer* aIn);

        private:

            class ProtocolInfo
            {

            public:

                ProtocolInfo();

                ProtocolInfo(uint32_t aMagic, uint8_t aProtocolVersion);

                uint32_t mMagic;

                uint8_t mInstanceCount;
                uint8_t mVersion_Base;
                uint8_t mVersion_System;
                uint8_t mVersion_Application;

            };

            void WriteData();

            Object** mInstances;

            FrameBuffer mRx_FrameBuffer;

            FrameBuffer mTx_FrameBuffer;
            uint8_t     mTx_Instance;

            // ===== Data ===================================================
            ProtocolInfo mProtocol_In;
            ProtocolInfo mProtocol_Out;

            uint8_t mResult_In;
            uint8_t mResult_Out;

            char    mTrace[32];
            uint8_t mTraceLen;

            Version mVersion_In;
            Version mVersion_Out;

        };

    }
}

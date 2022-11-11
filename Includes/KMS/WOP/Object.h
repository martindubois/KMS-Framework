
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Object.h

#pragma once

namespace KMS
{
    namespace WOP
    {

        class FrameBuffer;

        class Object
        {

        public:

            void AddRequest  (uint8_t aMask);
            void ClearRequest(uint8_t aMask);
            void SendRequest (uint8_t aMask);

            virtual bool PrepareFrame(FrameBuffer* aOut, uint8_t aInstance) = 0;

            virtual uint8_t WriteData(const FrameBuffer* aIn) = 0;

        protected:

            Object();

            uint8_t PopRequest(uint8_t aMask);

            void PrepareFrame_REQUEST(FrameBuffer* aOut, uint8_t aInstance);

            uint8_t WriteData_REQUEST(const FrameBuffer* aIn);

        private:

            uint8_t mRequest_In;
            uint8_t mRequest_Out;

        };

    }
}

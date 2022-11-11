
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Receiver.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/System.h>

namespace KMS
{
    namespace WOP
    {

        class Receiver
        {
        
        public:

            Receiver(System* aSystem, Object** aInstances, uint8_t aCount);

            void AddReceivedByte(uint8_t aIn);

            void AddReceivedBytes(const uint8_t* aIn, unsigned int aInSize_byte);

            void SetInstanceCount(uint8_t aCount);

        private:

            Receiver(const Receiver&);

            const Receiver& operator = (const Receiver&);

            void WriteData();

            FrameBuffer  mFrameBuffer;
            Object    ** mInstances;
            unsigned int mInstanceCount;
            System     * mSystem;

        };

    }
}
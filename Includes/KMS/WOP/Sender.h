
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Sender.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/WOP/FrameBuffer.h>
#include <KMS/WOP/System.h>

namespace KMS
{
    namespace WOP
    {

        class Sender
        {
        
        public:

            Sender(System* aSystem, Object** aInstances, uint8_t aCount);

            void SetInstanceCount(uint8_t aCount);

            const FrameBuffer* PrepareFrame();

        private:

            Sender(const Sender&);

            const Sender& operator = (const Sender&);

            FrameBuffer  mFrameBuffer;
            uint8_t      mInstance;
            Object    ** mInstances;
            unsigned int mInstanceCount;
            System     * mSystem;

        };

    }
}
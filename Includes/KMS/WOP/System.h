
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/System.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/WOP/Object.h>

namespace KMS
{
    namespace WOP
    {

        class System : public Object
        {

        public:

            System();

            uint8_t GetResult();

            void SetResult(uint8_t aIn);

            // ===== Object =================================================
            virtual bool    PrepareFrame(FrameBuffer* aOut, uint8_t aInstance);
            virtual uint8_t WriteData(const FrameBuffer* aIn);

        private:

            uint8_t mResult_In;
            uint8_t mResult_Out;

        };

    }
}

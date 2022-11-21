
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Object_Dynamic.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/WOP/Object.h>

namespace KMS
{
    namespace WOP
    {

        class Object_Dynamic : public Object
        {

        public:

            void Dump() const;

            // ===== Object =================================================
            virtual bool PrepareFrame(FrameBuffer* aOut, uint8_t aInstance);
            virtual uint8_t WriteData(const FrameBuffer* aIn);

        private:

            class Buffer
            {

            public:

                Buffer();

                void Dump() const;

                void PrepareFrame(FrameBuffer* aOut);

                uint8_t WriteData(const FrameBuffer* aIn);

            private:

                uint8_t mData[256];
                uint8_t mSize_byte;

            };

            Buffer mBuffers[7];

        };

    }
}

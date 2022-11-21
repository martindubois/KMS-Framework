
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/System.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Version.h>
#include <KMS/WOP/Object.h>

namespace KMS
{
    namespace WOP
    {

        class System : public Object
        {

        public:

            System(const Version& aVersion);

            uint8_t GetResult();

            const Version& GetVersion() const;

            void SetResult(uint8_t aIn);

            #ifdef _KMS_EMBEDED_
                void AddTrace(const char* aMsg, uint8_t aLen);
            #else
                void Dump();
            #endif

            // ===== Object =================================================
            virtual bool    PrepareFrame(FrameBuffer* aOut, uint8_t aInstance);
            virtual uint8_t WriteData(const FrameBuffer* aIn);

        private:

            uint8_t mProtocol_In;

            uint8_t mResult_In;
            uint8_t mResult_Out;

            Version mVersion_In;
            Version mVersion_Out;

            #ifdef _KMS_EMBEDDED_
                char    mTrace[32];
                uint8_t mTraceLen;
            #endif

        };

    }
}

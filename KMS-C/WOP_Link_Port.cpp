
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/WOP_Link_Port.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/System.h>

#include <KMS/WOP/Link_Port.h>

// Constants
// //////////////////////////////////////////////////////////////////////////

#define MSG_ITERATE (1)

namespace KMS
{

    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Link_Port::Link_Port(System* aSystem, Com::Port* aPort)
            : mPort(aPort)
            , mSystem(aSystem)
        {
            assert(NULL != aSystem);
            assert(NULL != aPort);

            mThread.mOnIterate.Set(this, MSG_ITERATE);
        }

        void Link_Port::Start() { mThread.Start(); }
        void Link_Port::Stop () { mThread.StopAndWait(3000); } // 3 s

        // ===== Msg::IReceiver =============================================

        unsigned int Link_Port::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

            switch (aCode)
            {
            case MSG_ITERATE: lResult = OnIterate(); break;
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        // TODO Use OVERLAPPED IO or other thread to be able to read and
        //      write at the same time.

        unsigned int Link_Port::OnIterate()
        {
            assert(NULL != mPort);
            assert(NULL != mSystem);

            uint8_t lBuffer[262];

            unsigned int lSize_byte = mPort->Read(lBuffer, sizeof(lBuffer));
            if (0 < lSize_byte)
            {
                mSystem->AddReceivedBytes(lBuffer, lSize_byte);
            }

            const FrameBuffer* lFB = mSystem->PrepareFrame();
            if (NULL != lFB)
            {
                mPort->Write(lFB->GetRawFrame(), lFB->GetFrameSize_byte());
            }

            return 0;
        }

    }
}

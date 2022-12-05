
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

#define MSG_ITERATE_RX (1)
#define MSG_ITERATE_TX (2)

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

            mThread_Rx.mOnIterate.Set(this, MSG_ITERATE_RX);
            mThread_Tx.mOnIterate.Set(this, MSG_ITERATE_TX);
        }

        void Link_Port::Start()
        {
            mThread_Rx.Start();
            mThread_Tx.Start();
        }

        void Link_Port::Stop()
        {
            mThread_Rx.Stop();
            mThread_Tx.Stop();

            mThread_Tx.Wait(2000); // 2 s
            mThread_Rx.Wait(2000); // 2 s
        }

        // ===== Msg::IReceiver =============================================

        unsigned int Link_Port::Receive(void* aSender, unsigned int aCode, void* aData)
        {
            unsigned int lResult = Msg::IReceiver::MSG_IGNORED;

            switch (aCode)
            {
            case MSG_ITERATE_RX: lResult = OnIterate_Rx(); break;
            case MSG_ITERATE_TX: lResult = OnIterate_Tx(); break;
            }

            return lResult;
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        unsigned int Link_Port::OnIterate_Rx()
        {
            assert(NULL != mPort);
            assert(NULL != mSystem);

            uint8_t lBuffer[262];

            unsigned int lSize_byte = mPort->Read(lBuffer, sizeof(lBuffer));
            if (0 < lSize_byte)
            {
                mSystem->AddReceivedBytes(lBuffer, lSize_byte);
            }

            return 0;
        }

        unsigned int Link_Port::OnIterate_Tx()
        {
            assert(NULL != mPort);
            assert(NULL != mSystem);

            const FrameBuffer* lFB = mSystem->PrepareFrame();
            if (NULL != lFB)
            {
                mPort->Write(lFB->GetRawFrame(), lFB->GetFrameSize_byte());
            }
            else
            {
                Sleep(20); // ms
            }

            return 0;
        }

    }
}

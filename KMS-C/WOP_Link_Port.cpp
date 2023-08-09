
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/WOP_Link_Port.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/WOP/System.h>

#include <KMS/WOP/Link_Port.h>

namespace KMS
{

    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        Link_Port::Link_Port(System* aSystem, Com::Port* aPort)
            : mPort(aPort)
            , mSystem(aSystem)
            // ===== Callbacks ==============================================
            , ON_ITERATE_RX(this, &Link_Port::OnIterate_Rx)
            , ON_ITERATE_TX(this, &Link_Port::OnIterate_Tx)
        {
            assert(nullptr != aSystem);
            assert(nullptr != aPort);

            mThread_Rx.mOnIterate = &ON_ITERATE_RX;
            mThread_Tx.mOnIterate = &ON_ITERATE_TX;
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

        // Private
        // //////////////////////////////////////////////////////////////////

        // ===== Callbacks ==================================================

        unsigned int Link_Port::OnIterate_Rx(void*, void*)
        {
            assert(nullptr != mPort);
            assert(nullptr != mSystem);

            uint8_t lBuffer[262];

            auto lSize_byte = mPort->Read(lBuffer, sizeof(lBuffer));
            if (0 < lSize_byte)
            {
                mSystem->AddReceivedBytes(lBuffer, lSize_byte);
            }

            return 0;
        }

        unsigned int Link_Port::OnIterate_Tx(void*, void*)
        {
            assert(nullptr != mPort);
            assert(nullptr != mSystem);

            auto lFB = mSystem->PrepareFrame();
            if (nullptr != lFB)
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

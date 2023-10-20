
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Link_Port.h
// Status

#pragma once

// ===== Includes ===========================================================
#include <KMS/Callback.h>
#include <KMS/Com/Port.h>
#include <KMS/Thread/Thread.h>

namespace KMS
{
    namespace WOP
    {

        class System;

        class Link_Port final
        {

        public:

            Link_Port(System* aSystem, Com::Port* aPort);

            void Start();
            void Stop ();

        private:

            // ===== Callbacks ==============================================
            const Callback<Link_Port> ON_ITERATE_RX;
            const Callback<Link_Port> ON_ITERATE_TX;
            unsigned int OnIterate_Rx(void* aSender, void* aData);
            unsigned int OnIterate_Tx(void* aSender, void* aData);

            Com::Port    * mPort;
            System       * mSystem;
            Thread::Thread mThread_Rx;
            Thread::Thread mThread_Tx;

        };

    }
}

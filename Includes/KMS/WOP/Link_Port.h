
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Link_Port.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Com/Port.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/Thread/Thread.h>

namespace KMS
{
    namespace WOP
    {

        class System;

        class Link_Port : public Msg::IReceiver
        {

        public:

            Link_Port(System* aSystem, Com::Port* aPort);

            Com::Port* GetPort();

            void Start();
            void Stop ();

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

        private:

            unsigned int OnIterate();

            Com::Port    * mPort;
            System       * mSystem;
            Thread::Thread mThread;

        };

    }
}

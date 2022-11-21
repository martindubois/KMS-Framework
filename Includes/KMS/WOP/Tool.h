
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Tool.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>
#include <KMS/Com/Port.h>
#include <KMS/Msg/IReceiver.h>
#include <KMS/Thread/Thread.h>
#include <KMS/WOP/Object_Dynamic.h>
#include <KMS/WOP/Receiver.h>
#include <KMS/WOP/Sender.h>
#include <KMS/WOP/System.h>

namespace KMS
{
    namespace WOP
    {

        class Tool : public CLI::Tool, public Msg::IReceiver
        {

        public:

            static int Main(int aCount, const char** aVector);

            Tool();

            DI::UInt<uint8_t> mInstanceCount;
            Com::Port         mPort;

            // ===== Msg::IReceiver =========================================
            virtual unsigned int Receive(void* aSender, unsigned int aCode, void* aData);

            // ===== CLI::Tool ==============================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual void ExecuteCommand(const char* aC);

        private:

            void Dump();
            void Dump(unsigned int aInstance);

            unsigned int OnIterate();

            void SendRequest(unsigned int aInstance, unsigned int aMask);

            void Receiver_Start();
            void Receiver_Stop ();

            WOP::Object       * mInstances[255];
            WOP::Object_Dynamic mDynamics[255];
            Receiver            mReceiver;
            Sender              mSender;
            System              mSystem;
            Thread::Thread      mThread;

        };

    }
}

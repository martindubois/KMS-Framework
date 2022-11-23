
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/WOP/Tool.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/CLI/Tool.h>
#include <KMS/Com/Port.h>
#include <KMS/WOP/Link_Port.h>
#include <KMS/WOP/Object_Dynamic.h>
#include <KMS/WOP/System.h>

namespace KMS
{
    namespace WOP
    {

        class Tool : public CLI::Tool
        {

        public:

            static int Main(int aCount, const char** aVector);

            Tool();

            Com::Port mPort;

            // ===== CLI::Tool ==============================================
            virtual void DisplayHelp(FILE* aOut) const;
            virtual void ExecuteCommand(const char* aC);

        private:

            void Dump();
            void Dump(unsigned int aInstance);

            void SendRequest(unsigned int aInstance, unsigned int aMask);

            WOP::Link_Port      mLink;
            WOP::Object       * mInstances[255];
            WOP::Object_Dynamic mDynamics[255];
            System              mSystem;

        };

    }
}

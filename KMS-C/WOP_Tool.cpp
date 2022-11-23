
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/WOP_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Installer.h>

#include <KMS/WOP/Tool.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("KMS-WOP.cfg")

namespace KMS
{

    namespace WOP
    {

        // Public
        // //////////////////////////////////////////////////////////////////

        int Tool::Main(int aCount, const char** aVector)
        {
            assert(1 <= aCount);
            assert(NULL != aVector);

            int lResult = __LINE__;

            try
            {
                Cfg::Configurator lC;
                Installer         lInstaller;
                WOP::Tool         lT;

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lT);

                lC.AddConfigurable(&Dbg::gLog);

                lC.ParseFile(File::Folder::CURRENT, CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lInstaller.Run();

                lResult = lT.Run();
            }
            KMS_CATCH_RESULT(lResult)

            return lResult;
        }

        Tool::Tool()
            : mLink(&mSystem, &mPort)
            , mSystem(VERSION, 0xffffffff, 0xff)
        {
            for (unsigned int i = 0; i < 255; i++)
            {
                mInstances[i] = mDynamics + i;
            }

            mSystem.SetInstances(mInstances, 255);

            AddEntry("Port", &mPort, false);
        }

        // ===== CLI::Tool ==================================================

        void Tool::DisplayHelp(FILE* aOut) const
        {
            assert(NULL != aOut);

            fprintf(aOut,
                "Connect\n"
                "Disconnect\n"
                "Dump [Instance]\n"
                "SendRequest {Instance} {Mask_Hex}\n"
                "StartReceiver\n"
                "StopReceivr\n");

            CLI::Tool::DisplayHelp(aOut);
        }

        void Tool::ExecuteCommand(const char* aC)
        {
            unsigned int lInstance;
            unsigned int lMask;

            if      (0 == strcmp(aC, "Connect"      )) { mPort.Connect(Dev::Device::FLAG_READ_ACCESS | Dev::Device::FLAG_WRITE_ACCESS); }
            else if (0 == strcmp(aC, "Disconnect"   )) { mPort.Disconnect(); }
            else if (0 == strcmp(aC, "Dump"         )) { Dump(); }
            else if (0 == strcmp(aC, "StartReceiver")) { mLink.Start(); }
            else if (0 == strcmp(aC, "StopReceiver" )) { mLink.Stop (); }
            else if (1 == sscanf_s(aC, "Dump %u", &lInstance)) { Dump(lInstance); }
            else if (2 == sscanf_s(aC, "SendRequest %u %x", &lInstance, &lMask)) { SendRequest(lInstance, lMask); }
            else
            {
                CLI::Tool::ExecuteCommand(aC);
            }
        }

        // Private
        // //////////////////////////////////////////////////////////////////

        void Tool::Dump()
        {
            for (unsigned int i = 0; i < 255; i++)
            {
                Dump(i);
            }

            mSystem.Dump();
        }

        void Tool::Dump(unsigned int aInstance)
        {
            if (0 < mDynamics[aInstance].GetDataSize_byte())
            {
                std::cout << "Object_Dynamic " << aInstance << "\n";

                mDynamics[aInstance].Dump();
            }
        }

        void Tool::SendRequest(unsigned int aInstance, unsigned int aMask)
        {
            mInstances[aInstance]->SendRequest(aMask);
        }

    }
}


// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2022-2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      KMS-C/WOP_Tool.cpp

#include "Component.h"

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/Dbg/Log_Cfg.h>
#include <KMS/Dbg/Stats.h>
#include <KMS/Dbg/Stats_Timer.h>
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

            auto lET = new Dbg::Stats_Timer("Main_ExecutionTime");
            lET->Start();

            try
            {
                Cfg::Configurator lC;
                Installer         lInstaller;
                Dbg::Log_Cfg      lLogCfg(&Dbg::gLog);
                WOP::Tool         lT;

                lC.AddConfigurable(&lInstaller);
                lC.AddConfigurable(&lT);

                lC.AddConfigurable(&lLogCfg);
                lC.AddConfigurable(&Dbg::gStats);

                lC.ParseFile(File::Folder::CURRENT, CONFIG_FILE);
                lC.ParseArguments(aCount - 1, aVector + 1);

                lInstaller.Run();

                lResult = lT.Run();
            }
            KMS_CATCH_RESULT(lResult)

            lET->Stop();

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
                "Start\n"
                "Stop\n");

            CLI::Tool::DisplayHelp(aOut);
        }

        void Tool::ExecuteCommand(const char* aC)
        {
            unsigned int lInstance;
            unsigned int lMask;

            if      (0 == strcmp(aC, "Connect"   ))
            {
                if (!mPort.Connect(Dev::Device::FLAG_ACCESS_READ | Dev::Device::FLAG_ACCESS_WRITE))
                {
                    KMS_EXCEPTION(DEV_CONNECT_FAILED, "Connexion failed", "");
                }
            }
            else if (0 == strcmp(aC, "Disconnect")) { mPort.Disconnect(); }
            else if (0 == strcmp(aC, "Dump"      )) { Dump(); }
            else if (0 == strcmp(aC, "Start"     )) { mLink.Start(); }
            else if (0 == strcmp(aC, "Stop"      )) { mLink.Stop (); }
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
            if (0xff <= aInstance)
            {
                mSystem.SendRequest(aMask);
            }
            else
            {
                mInstances[aInstance]->SendRequest(aMask);
            }
        }

    }
}

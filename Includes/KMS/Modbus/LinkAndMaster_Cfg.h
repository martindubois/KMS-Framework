
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Framework
// File      Includes/KMS/Modbus/LinkAndMaster_Cfg.h

#pragma once

// ===== Includes ===========================================================
#include <KMS/Cfg/Configurator.h>
#include <KMS/CLI/CommandLine.h>
#include <KMS/Com/Port.h>
#include <KMS/Modbus/Master_CFG.h>
#include <KMS/Modbus/Master_IDevice.h>
#include <KMS/Modbus/Master_TCP.h>
#include <KMS/Stream/Stream_Cfg.h>

namespace KMS
{
    namespace Modbus
    {

        class LinkAndMaster_Cfg : public Stream::Stream_Cfg
        {

        public:

            LinkAndMaster_Cfg(Cfg::Configurator* aConfigurator, Stream::StreamType aDefault);

            ~LinkAndMaster_Cfg();

            Master* GetMaster();

            void ParseArguments(CLI::CommandLine* aCmdLine);

        private:

            Master    * mMaster;
            Master_Cfg* mM_Cfg;

        };

    }
}
